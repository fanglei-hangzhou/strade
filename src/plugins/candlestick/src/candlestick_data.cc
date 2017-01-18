/*
 * candlestick_data.cc
 *
 *  Created on: 2017年1月10日
 *      Author: panghao
 */
#include "src/candlestick_data.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "time.h"
namespace candlestick {
StockCandleStickData::~StockCandleStickData() {
}

bool StockCandleStickData::ParseDate(std::string date, StockDate *stock_date) {
  if (date.length() != strlen("1900-01-01")) {
    return false;
  }
  char buf[8] = { 0 };
  strncpy(buf, date.c_str(), 4);
  int myear = atoi(buf);
  memset(buf, 0, sizeof(buf));
  strncpy(buf, date.c_str() + 5, 2);
  int mon = atoi(buf);
  memset(buf, 0, sizeof(buf));
  strncpy(buf, date.c_str() + 8, 2);
  int day = atoi(buf);
  struct tm day_tm;
  memset(&day_tm, 0, sizeof(day_tm));
  day_tm.tm_yday = myear - 1900;
  day_tm.tm_mon = mon - 1;
  day_tm.tm_mday = day;
  mktime(&day_tm);
  if (strftime(buf, sizeof(buf), "%W", &day_tm) == 0)
    return false;
  stock_date->week_index = atoi(buf);
  stock_date->year = myear;
  stock_date->month = mon;
  stock_date->day = day;
  printf("%d\n", mon);
  return true;
}

void StockCandleStickData::GetStockDealInfo(StockDealInfo *deal,
                                            const StockHistoryInfo &info) {
  deal->set_date(info.date_);
  deal->set_amount(info.amount_);
}

}  // namespace candlestick

