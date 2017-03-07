/*
 * candlestick_week.cc
 *
 *  Created on: 2017年1月9日
 *      Author: panghao
 */
#include "src/candlestick_week.h"
#include "stdio.h"
namespace candlestick {
StockDealCandleStickWeek::StockDealCandleStickWeek() {
}

StockDealCandleStickWeek::~StockDealCandleStickWeek() {
}

std::string StockDealCandleStickWeek::GetStoreKey(const StockDate &date) {
  char buf[64];
  snprintf(buf, sizeof(buf), "%04d:%02d", date.year, date.week_index);
  return buf;
}

void StockDealCandleStickWeek::UpdateSingleStockInfo(
    const StockDate &date, const SingleStockInfo &deal_info) {
  std::string key = GetStoreKey(date);
  UpdateSingleStockInfoByKey(key, deal_info);
}

void StockDealCandleStickWeek::UpdateCompositeindexInfo(
    const StockDate &date, const CompositeindexInfo &deal_info) {
  std::string key = GetStoreKey(date);
  UpdateCompositeindexInfoByKey(key, deal_info);
}

}  // namespace candlestick
