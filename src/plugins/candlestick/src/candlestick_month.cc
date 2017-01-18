/*
 * candlestick_month.cc
 *
 *  Created on: 2017年1月10日
 *      Author: pangh
 */
#include "src/candlestick_month.h"
#include "stdio.h"
namespace candlestick {
StockDealCandleStickMonth::StockDealCandleStickMonth() {
}

StockDealCandleStickMonth::~StockDealCandleStickMonth() {
}

std::string StockDealCandleStickMonth::GetStoreKey(const StockDate &date) {
  char buf[64];
  snprintf(buf, sizeof(buf), "%04d:%02d", date.year, date.month);
  return buf;
}

void StockDealCandleStickMonth::UpdateSingleStockInfo(
    const StockDate &date, const SingleStockInfo &deal_info) {
  std::string key = GetStoreKey(date);
  UpdateSingleStockInfoByKey(key, deal_info);
  printf("%s %d\n", __FILE__, __LINE__);
}

void StockDealCandleStickMonth::UpdateCompositeindexInfo(
    const StockDate &date, const CompositeindexInfo &deal_info) {
  std::string key = GetStoreKey(date);
  UpdateCompositeindexInfoByKey(key, deal_info);
}

}  // namespace candlestick
