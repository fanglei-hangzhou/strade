//
// Created by Harvey on 2017/1/4.
//

#ifndef STRADE_STRADE_SHARE_DB_H
#define STRADE_STRADE_SHARE_DB_H

#include "logic/strade_basic_info.h"
#include "storage/mysql_engine.h"
#include "dao/abstract_dao.h"

namespace strade_share {

class StradeShareDB {
 public:
  StradeShareDB(config::FileConfig* config);
  ~StradeShareDB();

 public:
  // 载入股票基本数据
  bool FetchAllStockList(std::vector<strade_logic::StockTotalInfo>& stock_vec);

  bool FetchStockHistList(const std::string& stock_code,
                          std::vector<strade_logic::StockHistInfo>& stock_hist_vec);

  template<typename T>
  bool ReadData(const std::string& sql,
                std::vector<T>& result) {
    return mysql_engine_->ReadData<T>(sql, result);
  }

  bool ReadDataRows(const std::string& sql,
                    std::vector<MYSQL_ROW>& rows_vec);

  bool WriteData(const std::string& sql);

  // 执行存储过程
  bool ExcuteStorage(const std::string& sql,
                     std::vector<MYSQL_ROW>& rows_vec) {
    return mysql_engine_->ExcuteStorage(sql, rows_vec);
  }

 private:
  base_logic::MysqlEngine* mysql_engine_;
};

} /* namespace strade_share */

#endif //STRADE_STRADE_SHARE_DB_H
