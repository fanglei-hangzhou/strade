/*
 * candlestick_day.h
 *
 *  Created on: 2017年1月10日
 *      Author: Administrator
 */

#ifndef CANDLESTICK_SRC_CANDLESTICK_DAY_H_
#define CANDLESTICK_SRC_CANDLESTICK_DAY_H_
#include "src/candlestick_history.h"
namespace candlestick {
class StockDealCandleStickDay : public StockDealCandleStickHistory {
 public:

  StockDealCandleStickDay();

  virtual ~StockDealCandleStickDay();

  virtual std::string GetStoreKey(const StockDate &date);

  virtual void UpdateSingleStockInfo(const StockDate &date,
                                     const SingleStockInfo &deal_info);

  virtual void UpdateCompositeindexInfo(const StockDate &date,
                                        const CompositeindexInfo &deal_info);
};
}  // namespace candlestick
#endif /* CANDLESTICK_SRC_CANDLESTICK_DAY_H_ */
