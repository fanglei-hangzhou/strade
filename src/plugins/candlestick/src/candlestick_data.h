/*
 * candlestick_data.h
 *
 *  Created on: 2017年1月10日
 *      Author: Administrator
 */

#ifndef CANDLESTICK_SRC_CANDLESTICK_DATA_H_
#define CANDLESTICK_SRC_CANDLESTICK_DATA_H_
#include <map>
#include <string>
#include "src/candlestick_week.h"
namespace candlestick {
class StockHistoryInfo {
 public:
  StockHistoryInfo() {
  }
  StockHistoryInfo(std::string date, double high, double low, double open,
                   double close, int amount) {
    date_ = date;
    high_ = high;
    low_ = low;
    close_ = close;
    open_ = open;
    amount_ = amount;
  }
  std::string date_;
  double high_;
  double low_;
  double close_;
  double open_;
  int amount_;
};

typedef std::map<std::string, StockHistoryInfo> STOCK_HISTORY_MAP;

class StockCandleStickData {
 public:
  virtual ~StockCandleStickData();
  bool ParseDate(std::string date, StockDate *stock_date);
  void GetStockDealInfo(StockDealInfo *deal, const StockHistoryInfo &info);
};
}  // namespace candlestick

#endif /* CANDLESTICK_SRC_CANDLESTICK_DATA_H_ */

