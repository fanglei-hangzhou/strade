/*
 * candlestick_singlestock.h
 *
 *  Created on: 2017年1月11日
 *      Author: pangh
 */

#ifndef CANDLESTICK_SRC_CANDLESTICK_SINGLESTOCK_H_
#define CANDLESTICK_SRC_CANDLESTICK_SINGLESTOCK_H_
#include "src/candlestick_data.h"
namespace candlestick {
class SingleStockCandleStickData : public StockCandleStickData {
 public:
  SingleStockCandleStickData();
  ~SingleStockCandleStickData();
  // from sharedate
  bool LoadSingleStockHisData(std::string stock_code, StockHistoryType type, const STOCK_HISTORY_MAP &info);

  // from sharedate
  bool LoadSingleStockHisData(std::string stock_code, const STOCK_HISTORY_MAP &info);

  bool LoadSingleStockHisData(std::string stock_code, StockHistoryType type);
  //clear buffer for week, month, day
  bool ClearSingleStockHisData(std::string stock_code, StockHistoryType type);
  // from sharedate
  bool GetSingleStockHisData(std::string code, StockHistoryType type,
                             std::list<SingleStockInfo> *index_data);

  static SingleStockCandleStickData *instance_;

  static SingleStockCandleStickData *GetInstance();

 private:

  void Reap(StockHistoryType type);

  void GetStockDeal(SingleStockInfo *deal, const StockHistoryInfo &info);

  StockDealCandleStickHistory *GetHistoryDataControl(std::string code, StockHistoryType type);
  StockDealCandleStickHistory *CreateHistoryDataControl(std::string code, StockHistoryType type);
  // code:dealinfo  0-month  1-week 2-day
  std::map<std::string, StockDealCandleStickHistory*> stock_deal_info_his_[3];
};

}  // namespace candlestick

#endif /* CANDLESTICK_SRC_CANDLESTICK_SINGLESTOCK_H_ */
