//
// Created by Harvey on 2017/1/4.
//

#ifndef STRADE_STRADE_SHARE_ENGINE_H
#define STRADE_STRADE_SHARE_ENGINE_H

#include "thread/base_thread_lock.h"
#include "config/config.h"
#include "strade_share_db.h"

#include "logic/strade_basic_info.h"
#include "logic/observer.h"
#include "logic/subject.h"
#include "logic/comm_head.h"

#include <vector>

namespace strade_share {

// 所有股票数据 key: 股票代码
typedef std::map<std::string, strade_logic::StockTotalInfo> STOCKS_MAP;

// 某只股票所有历史数据集合 key: 历史交易日期
typedef std::map<std::string, strade_logic::StockHistInfo> STOCK_HIST_MAP;

// 某只股票当天所有实时行情 key: 交易时间unix时间戳
typedef std::map<time_t, strade_logic::StockRealInfo> STOCK_REAL_MAP;

typedef std::vector<strade_logic::StockRealInfo> REAL_MARKET_DATA_VEC;
typedef std::vector<strade_logic::StockHistInfo> STOCK_HIST_DATA_VEC;

class SSEngine {
 public:
  virtual bool Init() = 0;

  // 注册观察者
  virtual void AttachObserver(
      strade_logic::Observer* observer) = 0;

  // 更新实时行情数据
  virtual void UpdateStockRealMarketData(
      REAL_MARKET_DATA_VEC& stocks_market_data) = 0;

  // 更新股票当天历史数据
  virtual bool UpdateStockHistInfoByDate(
      const std::string& stock_code,
      const std::string& date,
      strade_logic::StockHistInfo& stock_hist_info) = 0;

  // 清空股票当天实时数据（股票实时数据只保存一天）
  virtual bool ClearOldRealTradeMap() = 0;

  // 更新股票历史数据
  virtual bool UpdateStockHistDataVec(
      const std::string& stock_code,
      STOCK_HIST_DATA_VEC& stock_vec) = 0;

  virtual STOCKS_MAP GetAllStockTotalMapCopy() = 0;

  virtual STOCK_HIST_MAP GetStockHistMapByCodeCopy(
      const std::string& stock_code) = 0;

  virtual STOCK_REAL_MAP GetStockRealInfoMapCopy(
      const std::string& stock_code) = 0;

  // 获取某只股票所有数据
  virtual bool GetStockTotalInfoByCode(
      const std::string& stock_code,
      strade_logic::StockTotalInfo& stock_total_info) = 0;

  // 获取某只股票，某个历史日期数据
  virtual bool GetStockHistInfoByDate(
      const std::string& stock_code,
      const std::string& date,
      strade_logic::StockHistInfo& stock_hist_info) = 0;

  // 获取某只股票，当天某个交易时间的实时数据
  virtual bool GetStockRealMarketDataByTime(
      const std::string& stock_code,
      const time_t& time,
      strade_logic::StockRealInfo& stock_real_info) = 0;

  // 获取某只股票当前最新的实时数据
  virtual bool GetStockCurrRealMarketInfo(
      const std::string& stock_code,
      strade_logic::StockRealInfo& stock_real_info) = 0;

  // 获取mysql结果集对象， T 类型必须继承自 pub/dao/AbstractDao 类
  template<typename T>
  bool ReadData(const std::string& sql,
                std::vector<T>& result) {
    return false;
  }

  // 获取mysql 结果集， 用于自定义 MYSQL_ROW 的转化
  virtual bool ReadDataRows(
      const std::string& sql, std::vector<MYSQL_ROW>& rows_vec) = 0;

  // 更新数据
  virtual bool WriteData(const std::string& sql) = 0;

  // 执行存储过程
  virtual bool ExcuteStorage(
      const std::string& sql, std::vector<MYSQL_ROW>& rows_vec) = 0;

};

struct StradeShareCache {
  STOCKS_MAP stocks_map_;
};

class SSEngineImpl : public SSEngine, public strade_logic::Subject {
 public:
  SSEngineImpl();
  virtual ~SSEngineImpl();

  static SSEngineImpl* GetInstance();

  virtual bool Init();

  virtual void AttachObserver(
      strade_logic::Observer* observer);

  void LoadAllStockBasicInfo();

  virtual void UpdateStockRealMarketData(
      REAL_MARKET_DATA_VEC& stocks_market_data);

  virtual bool UpdateStockHistInfoByDate(
      const std::string& stock_code,
      const std::string& date,
      strade_logic::StockHistInfo& stock_hist_info);

  virtual bool ClearOldRealTradeMap();

  virtual bool UpdateStockHistDataVec(
      const std::string& stock_code,
      STOCK_HIST_DATA_VEC& stock_vec);

  virtual STOCKS_MAP GetAllStockTotalMapCopy();

  virtual STOCK_HIST_MAP GetStockHistMapByCodeCopy(
      const std::string& stock_code);

  virtual STOCK_REAL_MAP GetStockRealInfoMapCopy(
      const std::string& stock_code);

  virtual bool GetStockTotalInfoByCode(
      const std::string& stock_code,
      strade_logic::StockTotalInfo& stock_total_info);

  virtual bool GetStockHistInfoByDate(
      const std::string& stock_code,
      const std::string& date,
      strade_logic::StockHistInfo& stock_hist_info);

  virtual bool GetStockRealMarketDataByTime(
      const std::string& stock_code,
      const time_t& time,
      strade_logic::StockRealInfo& stock_real_info);

  virtual bool GetStockCurrRealMarketInfo(
      const std::string& stock_code,
      strade_logic::StockRealInfo& stock_real_info);

 public:
  bool AddStockTotalInfoNonblock(
      const strade_logic::StockTotalInfo& stock_total_info);

  bool AddStockTotalInfoBlock(
      const strade_logic::StockTotalInfo& stock_total_info);

  template<typename T>
  bool ReadData(const std::string& sql,
                std::vector<T>& result) {
    base_logic::WLockGd lk(lock_);
    return mysql_engine_->ReadData<T>(sql, result);
  }

  virtual bool ReadDataRows(
      const std::string& sql, std::vector<MYSQL_ROW>& rows_vec);

  virtual bool WriteData(const std::string& sql);

  virtual bool ExcuteStorage(const std::string& sql, std::vector<MYSQL_ROW>& rows_vec);

 private:
  bool GetStockTotalNonBlock(const std::string stock_code,
                             strade_logic::StockTotalInfo& stock_total_info) {
    STOCKS_MAP::iterator iter(share_cache_.stocks_map_.find(stock_code));
    if (iter != share_cache_.stocks_map_.end()) {
      stock_total_info = iter->second;
      return true;
    }
    return false;
  }

  bool InitParam();

 private:
  threadrw_t* lock_;
  StradeShareCache share_cache_;
  StradeShareDB* mysql_engine_;
  static SSEngineImpl* instance_;
};

} /* namespace strade_share */

#ifdef __cplusplus
extern "C" {
#endif
strade_share::SSEngine* GetStradeShareEngine(void);
#ifdef __cplusplus
}
#endif

#endif //STRADE_STRADE_SHARE_ENGINE_H
