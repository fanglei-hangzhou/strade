/*
 * candlestick_compositeindex.cc
 *
 *  Created on: 2017年1月11日
 *      Author: pangh
 */
#include "src/candlestick_compositeindex.h"
namespace candlestick {
CompositeindexCandleStickData::CompositeindexCandleStickData() {
}

CompositeindexCandleStickData::~CompositeindexCandleStickData() {
}

StockDealCandleStickHistory *CompositeindexCandleStickData::CreateHistoryDataControl(
    StockHistoryType type) {
  StockDealCandleStickHistory *history =
      StockRealInfoFactory::CreateStockDealCandleStickHistory(type);
  if (!history)
    return NULL;
  index_info_[type] = history;
  return history;
}

void CompositeindexCandleStickData::GetStockDeal(
    CompositeindexInfo *deal, const StockHistoryInfo &info) {
  GetStockDealInfo(deal, info);
  deal->set_high_index(info.high_);
  deal->set_low_index(info.low_);
  deal->set_open_index(info.open_);
  deal->set_close_index(info.close_);
}

StockDealCandleStickHistory *CompositeindexCandleStickData::GetHistoryDataControl(
    StockHistoryType type) {
  return index_info_[type];
}

bool CompositeindexCandleStickData::ClearCompositeindexHisData(StockHistoryType type) {
  delete index_info_[type];
  index_info_[type] = NULL;
  return true;
}

bool CompositeindexCandleStickData::LoadCompositeindexHisData(
    StockHistoryType type, const STOCK_HISTORY_MAP &info) {
  StockDealCandleStickHistory *history = GetHistoryDataControl(type);
  if (history) {
    history->ResetData();
  } else {
    history = CreateHistoryDataControl(type);
    if (!history) {
      return false;
    }
  }

  std::map<std::string, StockHistoryInfo>::const_iterator mit = info.begin();
  while (mit != info.end()) {
    const StockHistoryInfo &his_info = mit->second;
    StockDate date;
    if (ParseDate(mit->first, &date)) {
      CompositeindexInfo deal_info;
      GetStockDeal(&deal_info, his_info);
      history->UpdateCompositeindexInfo(date, deal_info);
    }
    mit++;
  }
  return true;
}

bool CompositeindexCandleStickData::GetCompositeindexHisData(
    StockHistoryType type, std::list<CompositeindexInfo> *index_data) {
  if (index_info_[type] == NULL || index_info_[type]->IsOutofDate(time(NULL))) {
    LoadCompositeindexHisData(type);
  }
  if (index_info_[type]) {
    index_info_[type]->GetCompositeindexInfo(index_data);
    return true;
  }
  return false;
}

}  // namespace candlestick
