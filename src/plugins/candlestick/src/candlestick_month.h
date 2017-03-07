/*
 * candlestick_month.h
 *
 *  Created on: 2017年1月10日
 *      Author: pangh
 */

#ifndef CANDLESTICK_SRC_CANDLESTICK_MONTH_H_
#define CANDLESTICK_SRC_CANDLESTICK_MONTH_H_
#include <map>
#include <string>
#include "src/candlestick_history.h"
namespace candlestick {
class StockDealCandleStickMonth : public StockDealCandleStickHistory {
 public:
  StockDealCandleStickMonth();

  virtual ~StockDealCandleStickMonth();

  virtual std::string GetStoreKey(const StockDate &date);

  virtual void UpdateSingleStockInfo(const StockDate &date,
                                     const SingleStockInfo &deal_info);

  virtual void UpdateCompositeindexInfo(const StockDate &date,
                                        const CompositeindexInfo &deal_info);
};

}  // namespace candlestick
#endif /* CANDLESTICK_SRC_CANDLESTICK_MONTH_H_ */
