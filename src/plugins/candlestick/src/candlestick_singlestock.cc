/*
 * candlestick_singlestock.cc
 *
 *  Created on: 2017年1月11日
 *      Author: pangh
 */
#include "src/candlestick_singlestock.h"
#include "stdio.h"
namespace candlestick {
SingleStockCandleStickData *SingleStockCandleStickData::instance_ = NULL;
SingleStockCandleStickData *SingleStockCandleStickData::GetInstance() {
  if (!instance_) {
    instance_ = new SingleStockCandleStickData();
  }
  return instance_;
}

SingleStockCandleStickData::SingleStockCandleStickData() {
}

SingleStockCandleStickData::~SingleStockCandleStickData() {
}

StockDealCandleStickHistory *SingleStockCandleStickData::CreateHistoryDataControl(
    std::string code, StockHistoryType type) {
  StockDealCandleStickHistory *history =
      StockRealInfoFactory::CreateStockDealCandleStickHistory(type);
  if (!history)
    return NULL;
  std::map<std::string, StockDealCandleStickHistory*> &his_map =
      stock_deal_info_his_[type];
  his_map[code] = history;
  return history;
}

void SingleStockCandleStickData::Reap(StockHistoryType type) {
  time_t now = time(NULL);
  for (int type = 0; type < 3; type++) {
    std::map<std::string, StockDealCandleStickHistory*> &his_map =
        stock_deal_info_his_[type];
    std::map<std::string, StockDealCandleStickHistory*>::iterator it = his_map
        .begin();
    while (it != his_map.end()) {
      StockDealCandleStickHistory *history = it->second;
      if (history->IsOutofDate(now)) {
        history->ResetData();
      }
      it++;
    }
  }
}

StockDealCandleStickHistory *SingleStockCandleStickData::GetHistoryDataControl(
    std::string stock_code, StockHistoryType type) {
  std::map<std::string, StockDealCandleStickHistory*> &his_map =
      stock_deal_info_his_[type];
  std::map<std::string, StockDealCandleStickHistory*>::iterator it = his_map
      .find(stock_code);
  StockDealCandleStickHistory *history = NULL;
  if (it != his_map.end()) {
    history = it->second;
    return history;
  }
  return history;
}

bool SingleStockCandleStickData::GetSingleStockHisData(
    std::string code, StockHistoryType type,
    std::list<SingleStockInfo> *index_data) {
  if (code.length() == 0)
    return false;
  StockDealCandleStickHistory *history = GetHistoryDataControl(code, type);
  if (history == NULL || history->IsOutofDate(time(NULL))) {
    printf("The history is %x", history);
    LoadSingleStockHisData(code, type);
  }
  history = GetHistoryDataControl(code, type);
  if (!history)
    return false;
  history->GetSingleStockInfo(index_data);
  return true;
}

bool SingleStockCandleStickData::ClearSingleStockHisData(
    std::string stock_code, StockHistoryType type) {

  std::map<std::string, StockDealCandleStickHistory*> &his_map =
      stock_deal_info_his_[type];
  std::map<std::string, StockDealCandleStickHistory*>::iterator it = his_map
      .find(stock_code);
  if (it != his_map.end()) {
    delete it->second;
    his_map.erase(it);
  }
  return true;
}

void SingleStockCandleStickData::GetStockDeal(SingleStockInfo *deal,
                                              const StockHistoryInfo &info) {
  GetStockDealInfo(deal, info);
  deal->set_high_price(info.high_);
  deal->set_low_price(info.low_);
  deal->set_open_price(info.open_);
  deal->set_close_price(info.close_);
}

bool SingleStockCandleStickData::LoadSingleStockHisData(
    std::string stock_code, const STOCK_HISTORY_MAP &info) {
  bool r = true;
  int types[] = { STOCK_HISTORY_MONTH, STOCK_HISTORY_WEEK, STOCK_HISTORY_DAY };
  for (int i = 0; i < 3; i++) {
    if (LoadSingleStockHisData(stock_code,
                               static_cast<StockHistoryType>(types[i]), info)
        == false)
      r = false;
  }
  return r;
}

bool SingleStockCandleStickData::LoadSingleStockHisData(std::string stock_code,
                                                        StockHistoryType type) {
  STOCK_HISTORY_MAP info;  //GetStradeShareEngine()
  //  ->GetStockHistMap(stock_code);
  return LoadSingleStockHisData(stock_code, type, info);
}
bool SingleStockCandleStickData::LoadSingleStockHisData(
    std::string stock_code, StockHistoryType type,
    const STOCK_HISTORY_MAP &info) {
  //STOCK_HISTORY_MAP info ;//GetStradeShareEngine()
  //  ->GetStockHistMap(stock_code);
  StockDealCandleStickHistory *history = GetHistoryDataControl(stock_code,
                                                               type);
  if (history) {
    history->ResetData();
  } else {
    history = CreateHistoryDataControl(stock_code, type);
    if (!history) {
      return false;
    }

  }
  STOCK_HISTORY_MAP::const_iterator mit = info.begin();
  while (mit != info.end()) {
    const StockHistoryInfo &his_info = mit->second;
    StockDate date;
    if (ParseDate(mit->first, &date)) {
      SingleStockInfo deal_info;
      GetStockDeal(&deal_info, his_info);
      history->UpdateSingleStockInfo(date, deal_info);
    }
    mit++;
  }
  return true;
}

}  // namespace candlestock

