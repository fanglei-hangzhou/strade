//  Copyright (c) 2015-2015 The KID Authors. All rights reserved.
//  Created on: 2016年10月13日 Author: zjc

#ifndef SRC_PLUGINS_REALTIME_STOCK_DB_H_
#define SRC_PLUGINS_REALTIME_STOCK_DB_H_

#include <string>
#include <vector>

#include "logic/base_values.h"
#include "basic/basictypes.h"
#include "logic/base_values.h"
#include "basic/scoped_ptr.h"
#include "storage/data_engine.h"

namespace stock_logic {

class StockDB {
 public:
  struct CodeRealtimeInfo {
    std::string code;
    std::string name;
    std::string date;
    double price_change;
    double change_percent;
    double price;
    double open;
    double high;
    double close;
    double low;
    double settlement;
    int type;
    time_t market_time;
    double volume;
    double amount;
    CodeRealtimeInfo()
        : price_change(0.0),
          change_percent(0.0),
          price(0.0),
          open(0.0),
          high(0.0),
          close(0.0),
          low(0.0),
          settlement(0.0),
          type(0),
          market_time(0.0),
          volume(0.0),
          amount(0.0) {}

    std::string Serialized() const;
  };
  typedef std::vector<CodeRealtimeInfo> CodeInfoArray;

  StockDB(config::FileConfig* config);
  virtual ~StockDB();
 public:

  // clear algo_today_stock_all_info table
  bool ClearTodayStockAllInfo();

  // del from algo_today_stock_all_info
  bool DelPreTradeData();

  // del algo_today_stock
  bool DelOldRealtimeData(time_t mtime);

  bool UpdateTodayStockAllInfo(const CodeInfoArray& data);

  bool UpdateTodayStock(const CodeInfoArray& data);

  bool UpdateYieldRate(const CodeRealtimeInfo& info);

  static void CallBackBasicInfo(void* param, base_logic::Value* value);
 private:
  base_logic::DataEngine* mysql_engine_;
};

} /* namespace stock_logic */

#endif /* SRC_PLUGINS_REALTIME_STOCK_DB_H_ */
