/*
 * candlestick_compositeindex.h
 *
 *  Created on: 2017年1月11日
 *      Author: pangh
 */
#ifndef CANDLESTICK_SRC_CANDLESTICK_COMPOSITEINDEX_H_
#define CANDLESTICK_SRC_CANDLESTICK_COMPOSITEINDEX_H_
#include "src/candlestick_data.h"
#include "src/candlestick_info.h"

namespace candlestick {
class CompositeindexCandleStickData : public StockCandleStickData {
 public:
  CompositeindexCandleStickData();
  virtual ~CompositeindexCandleStickData();
  // from sharedate
  bool LoadCompositeindexHisData(StockHistoryType type);
  bool LoadCompositeindexHisData(StockHistoryType type,
                                 const STOCK_HISTORY_MAP &info);
  bool GetCompositeindexHisData(StockHistoryType type,
                                std::list<CompositeindexInfo> *index_data);
  // from sharedate
  bool ClearCompositeindexHisData(StockHistoryType type);
  static CompositeindexCandleStickData *instance_;
  static CompositeindexCandleStickData *GetInstance();

 private:
  void GetStockDeal(CompositeindexInfo *deal, const StockHistoryInfo &info);
  StockDealCandleStickHistory *GetHistoryDataControl(StockHistoryType type);
  StockDealCandleStickHistory *CreateHistoryDataControl(StockHistoryType type);
  // shanghai compositeindex
  StockDealCandleStickHistory *index_info_[3];
};

}  // namespace candlestick

#endif  /* CANDLESTICK_SRC_CANDLESTICK_COMPOSITEINDEX_H_ */
