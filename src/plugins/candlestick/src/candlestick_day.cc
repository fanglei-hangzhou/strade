/*
 * candlestick_day.cc
 *
 *  Created on: 2017年1月10日
 *      Author: pangh
 */
#include "src/candlestick_day.h"
#include "stdio.h"
namespace candlestick {
StockDealCandleStickDay::StockDealCandleStickDay() {
}

StockDealCandleStickDay::~StockDealCandleStickDay() {
}

std::string StockDealCandleStickDay::GetStoreKey(const StockDate &date) {
  char buf[64];
  snprintf(buf, sizeof(buf), "%04d:%02d:%02d", date.year, date.month, date.day);
  return buf;
}

void StockDealCandleStickDay::UpdateSingleStockInfo(
    const StockDate &date, const SingleStockInfo &deal_info) {
  std::string key = GetStoreKey(date);
  UpdateSingleStockInfoByKey(key, deal_info);
}

void StockDealCandleStickDay::UpdateCompositeindexInfo(const StockDate &date, const CompositeindexInfo &deal_info) {
  std::string key = GetStoreKey(date);
  UpdateCompositeindexInfoByKey(key, deal_info);
}

}  // namespace candlestick
