//
// Created by Harvey on 2016/12/29.
//

#ifndef STRADE_SRC_PUB_STORAGE_MYSQL_THREAD_POOL_H
#define STRADE_SRC_PUB_STORAGE_MYSQL_THREAD_POOL_H

#include "config/config.h"
#include "storage/storage.h"
#include "thread/base_thread_lock.h"
#include "logic/logic_comm.h"
#include "logic/base_values.h"
#include "net/comm_head.h"
#include "dao/abstract_dao.h"

#include <list>
#include <queue>
#include <semaphore.h>
#include <mysql/mysql.h>
#include <sstream>

namespace base_logic {

enum MYSQL_JOB_TYPE {
  MYSQL_READ = 1,
  MYSQL_WRITE = 2,
  MYSQL_STORAGE = 3,
};

typedef std::list<base_storage::DBStorageEngine*> MYSQL_ENGINE_POOL;

namespace {

struct MysqlEngineSharedInfo {
  MysqlEngineSharedInfo() {
    pthread_mutex_init(&mutex_, NULL);
  }

  ~MysqlEngineSharedInfo() {
    pthread_mutex_destroy(&mutex_);
    if (!db_read_pool_.empty()) {
      base_storage::DBStorageEngine* engine = db_read_pool_.front();
      db_read_pool_.pop_front();
      if (engine) {
        engine->Release();
        delete engine;
        engine = NULL;
      }
    }
    if (!db_write_pool_.empty()) {
      base_storage::DBStorageEngine* engine = db_write_pool_.front();
      db_write_pool_.pop_front();
      if (engine) {
        engine->Release();
        delete engine;
        engine = NULL;
      }
    }
  }

  base_storage::DBStorageEngine* popEngine(MYSQL_JOB_TYPE engine_type) {
    pthread_mutex_lock(&mutex_);
    base_storage::DBStorageEngine* engine = NULL;
    if (MYSQL_WRITE == engine_type || MYSQL_STORAGE == engine_type) {
      engine = db_write_pool_.front();
      db_write_pool_.pop_front();
    } else if (MYSQL_READ == engine_type) {
      engine = db_read_pool_.front();
      if (NULL != engine) {
        db_read_pool_.pop_front();
      }
    } else {
      LOG_ERROR("mysql job type illegal");
    }
    pthread_mutex_unlock(&mutex_);
    return engine;
  }

  void pushEngine(MYSQL_JOB_TYPE engine_type,
                  base_storage::DBStorageEngine* engine) {
    assert(NULL != engine);
    pthread_mutex_lock(&mutex_);
    if (MYSQL_WRITE == engine_type || MYSQL_STORAGE == engine_type) {
      db_write_pool_.push_back(engine);
    } else if (MYSQL_READ == engine_type) {
      db_read_pool_.push_back(engine);
    } else {
      LOG_ERROR("mysql job type illegal");
    }
    pthread_mutex_unlock(&mutex_);
  }

  pthread_mutex_t mutex_;
  MYSQL_ENGINE_POOL db_read_pool_;
  MYSQL_ENGINE_POOL db_write_pool_;
};

struct MySqlJobAdapter {
  MySqlJobAdapter(MYSQL_JOB_TYPE type,
                  const std::string& sql)
      : type_(type),
        sql_(sql) {
  }

  template<class T>
  bool ReadData(MysqlEngineSharedInfo* shared_info,
                std::vector<T>& result) {
    bool r = false;
    base_storage::DBStorageEngine* engine =
        shared_info->popEngine(type_);
    if (NULL == engine) {
      LOG_DEBUG2("excute sql=%s error, engine NULL",
                 sql_.c_str());
      return false;
    }
    do {
      r = engine->SQLExec(sql_.c_str());
      if (!r) {
        r = false;
        LOG_ERROR("exec sql error");
        break;
      }
      MYSQL_ROW rows;
      int32 num = engine->RecordCount();
      if (engine->RecordCount() > 0) {
        while ((rows = (*(MYSQL_ROW*) (engine->FetchRows())->proc))) {
          T obj;
          obj(rows);
          result.push_back(obj);
        }
      }
      r = true;
    } while (0);
    // 归还链接
    shared_info->pushEngine(type_, engine);
    return r;
  }

  bool ReadDataRows(MysqlEngineSharedInfo* shared_info,
                    std::vector<MYSQL_ROW>& rows_vec) {
    bool r = false;
    base_storage::DBStorageEngine* engine =
        shared_info->popEngine(type_);
    if (NULL == engine) {
      LOG_DEBUG2("excute sql=%s error, engine NULL",
                 sql_.c_str());
      return false;
    }
    do {
      r = engine->SQLExec(sql_.c_str());
      if (!r) {
        r = false;
        LOG_ERROR("exec sql error");
        break;
      }
      MYSQL_ROW rows;
      int32 num = engine->RecordCount();
      if (engine->RecordCount() > 0) {
        while ((rows = (*(MYSQL_ROW*) (engine->FetchRows())->proc))) {
          rows_vec.push_back(rows);
        }
      }
      r = true;
    } while (0);
    // 归还链接
    shared_info->pushEngine(type_, engine);
    return r;
  }

  bool WriteData(MysqlEngineSharedInfo* shared_info) {
    bool r = false;
    base_storage::DBStorageEngine* engine =
        shared_info->popEngine(type_);
    if (NULL == engine) {
      LOG_DEBUG2("excute sql=%s error, engine NULL",
                 sql_.c_str());
      return false;
    }
    do {
      r = engine->SQLExec(sql_.c_str());
      if (!r) {
        r = false;
        LOG_ERROR("exec sql error");
        break;
      }
      r = true;
    } while (0);
    // 归还链接
    shared_info->pushEngine(type_, engine);
    return r;
  }

  std::string sql_;
  MYSQL_JOB_TYPE type_;
};

} /* namespace */


class MysqlEngine {
 public:
  MysqlEngine(base::ConnAddr& read_addr,
              base::ConnAddr& write_addr);
  ~MysqlEngine();

 private:
  void Initialize();
  bool CreateMysqlReadEnginePool(int32 pool_num = 3);
  bool CreateMysqlWriteEnginePool(int32 pool_num = 1);

 public:
  // 获取对象
  template<class T>
  bool ReadData(const std::string& sql,
                std::vector<T>& result) {
    MySqlJobAdapter mysql_job(base_logic::MYSQL_READ, sql);
    return mysql_job.ReadData<T>(&shared_info_, result);
  }

  // 获取原始 MYSQL_ROW， 用于特殊处理
  bool ReadDataRows(const std::string& sql,
                        std::vector<MYSQL_ROW>& rows_vec);

  // 更新数据
  bool WriteData(const std::string& sql);

  // 执行存储过程
  bool ExcuteStorage(const std::string& sql,
                     std::vector<MYSQL_ROW>& rows_vec);

 private:
  base::ConnAddr read_addr_;
  base::ConnAddr write_addr_;
  MysqlEngineSharedInfo shared_info_;
};

} /* namespace base_logic */

#endif //STRADE_SRC_PUB_STORAGE_MYSQL_THREAD_POOL_H
