/*
 * candlestick_history.h
 *
 *  Created on: 2017年1月10日
 *      Author: pangh
 */

#ifndef CANDLESTICK_SRC_CANDLESTICK_HISTORY_H_
#define CANDLESTICK_SRC_CANDLESTICK_HISTORY_H_
#include <map>
#include <list>
#include <string>
#include "src/candlestick_info.h"
namespace candlestick {
class SingleStockInfoHistory {
 public:
  SingleStockInfoHistory();
  explicit SingleStockInfoHistory(const SingleStockInfo &deal_info);

  void StatSingleStockInfo(const SingleStockInfo &info);

  SingleStockInfo get_single_stock_info() {
    return deal_info_;
  }

 private:
  SingleStockInfo deal_info_;
};

class CompositeindexInfoHistory {
 public:
  CompositeindexInfoHistory();
  explicit CompositeindexInfoHistory(const CompositeindexInfo &deal_info);

  void StatCompositeindexInfo(const CompositeindexInfo &info);
  CompositeindexInfo get_compositeindex_info() {
    return deal_info_;
  }

 private:
  CompositeindexInfo deal_info_;
};

class StockDealCandleStickHistory {
 public:
  StockDealCandleStickHistory();
  virtual ~StockDealCandleStickHistory();
  virtual void UpdateSingleStockInfo(const StockDate &date,
                                     const SingleStockInfo &deal_info) = 0;
  virtual void UpdateCompositeindexInfo(const StockDate &date,
                                        const CompositeindexInfo &deal_info) = 0;
  // single stock
  void GetSingleStockInfo(std::list<SingleStockInfo> *stock_info);
  // compositeindex
  void GetCompositeindexInfo(std::list<CompositeindexInfo> *stock_info);
  // outofdate
  bool IsOutofDate(time_t compare_stamp);

  void ResetData() {
    single_stock_map_.clear();
    index_map_.clear();
  }
  virtual std::string GetStoreKey(const StockDate &date) = 0;

 protected:
  void UpdateSingleStockInfoByKey(const std::string &key,
                                  const SingleStockInfo &deal_info);
  void UpdateCompositeindexInfoByKey(const std::string &key,
                                     const CompositeindexInfo &deal_info);

 private:
  // key: year:month   2017:0
  std::map<std::string, SingleStockInfoHistory> single_stock_map_;
  // key: year:month   2017:0
  std::map<std::string, CompositeindexInfoHistory> index_map_;
  time_t cache_seconds_;
  time_t store_stamp_;
};

class StockRealInfoFactory {
 public:
  static StockDealCandleStickHistory *CreateStockDealCandleStickHistory(StockHistoryType type);
};
}  // namespace candlestick
#endif /* CANDLESTICK_SRC_CANDLESTICK_HISTORY_H_ */
