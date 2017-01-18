/*
 * candlestick_history.cc
 *
 *  Created on: 2017年1月10日
 *      Author: pangh
 */

#include "src/candlestick_history.h"
#include <stdio.h>
#include "src/candlestick_month.h"
#include "src/candlestick_week.h"
#include "src/candlestick_day.h"
namespace candlestick {
SingleStockInfoHistory::SingleStockInfoHistory() {
}
// dealinfo
SingleStockInfoHistory::SingleStockInfoHistory(const SingleStockInfo &deal_info) {
  deal_info_ = deal_info;
}

void SingleStockInfoHistory::StatSingleStockInfo(
    const SingleStockInfo &deal_info) {
  deal_info_.StatSingleStockInfo(deal_info);
}

CompositeindexInfoHistory::CompositeindexInfoHistory() {
}

CompositeindexInfoHistory::CompositeindexInfoHistory(const CompositeindexInfo &deal_info) {
  deal_info_ = deal_info;
}

void CompositeindexInfoHistory::StatCompositeindexInfo(
    const CompositeindexInfo &deal_info) {
  deal_info_.StatCompositeindexInfo(deal_info);
}

// control
StockDealCandleStickHistory::StockDealCandleStickHistory()
    : cache_seconds_(60),
      store_stamp_(0) {
}

StockDealCandleStickHistory::~StockDealCandleStickHistory() {
}

bool StockDealCandleStickHistory::IsOutofDate(time_t compare_stamp) {
  return store_stamp_ + cache_seconds_ < compare_stamp;
}

void StockDealCandleStickHistory::UpdateSingleStockInfoByKey(
    const std::string &key, const SingleStockInfo &deal_info) {
  std::map<std::string, SingleStockInfoHistory>::iterator it =
      single_stock_map_.find(key);
  if (it != single_stock_map_.end()) {
    SingleStockInfoHistory &info = it->second;
    info.StatSingleStockInfo(deal_info);
  } else {
    printf("updating %s\n", key.c_str());
    single_stock_map_[key] = SingleStockInfoHistory(deal_info);
  }
  store_stamp_ = time(NULL);
}

void StockDealCandleStickHistory::UpdateCompositeindexInfoByKey(
    const std::string &key, const CompositeindexInfo &deal_info) {
  std::map<std::string, CompositeindexInfoHistory>::iterator it =
      index_map_.find(key);
  if (it != index_map_.end()) {
    CompositeindexInfoHistory &info = it->second;
    info.StatCompositeindexInfo(deal_info);
  } else {
    index_map_[key] = CompositeindexInfoHistory(deal_info);
  }
  store_stamp_ = time(NULL);
}

void StockDealCandleStickHistory::GetSingleStockInfo(std::list<SingleStockInfo> *stock_info) {
  std::map<std::string, SingleStockInfoHistory>::iterator it = single_stock_map_
      .begin();
  while (it != single_stock_map_.end()) {
    SingleStockInfoHistory &info = it->second;
    stock_info->push_back(info.get_single_stock_info());
    it++;
  }
}

void StockDealCandleStickHistory::GetCompositeindexInfo(std::list<CompositeindexInfo> *stock_info) {
  std::map<std::string, CompositeindexInfoHistory>::iterator it = index_map_
      .begin();
  while (it != index_map_.end()) {
    CompositeindexInfoHistory &info = it->second;
    stock_info->push_back(info.get_compositeindex_info());
    it++;
  }
}

StockDealCandleStickHistory *StockRealInfoFactory::CreateStockDealCandleStickHistory(
    StockHistoryType type) {
  switch (type) {
    case STOCK_HISTORY_MONTH:
      return new StockDealCandleStickMonth();
    case STOCK_HISTORY_WEEK:
      return new StockDealCandleStickWeek();
    case STOCK_HISTORY_DAY:
      return new StockDealCandleStickDay();
  }
  return NULL;
}
} // candlestick
