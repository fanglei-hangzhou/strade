/*
 * candlestick_week.h
 *
 *  Created on: 2017年1月9日
 *      Author: panghao
 */

#ifndef CANDLESTICK_SRC_CANDLESTICK_WEEK_H_
#define CANDLESTICK_SRC_CANDLESTICK_WEEK_H_
#include <map>
#include <string>
#include "src/candlestick_history.h"
namespace candlestick {
class StockDealCandleStickWeek : public StockDealCandleStickHistory {
 public:

  StockDealCandleStickWeek();

  virtual ~StockDealCandleStickWeek();

  virtual std::string GetStoreKey(const StockDate &date);

  virtual void UpdateSingleStockInfo(const StockDate &date,
                                     const SingleStockInfo &deal_info);

  virtual void UpdateCompositeindexInfo(const StockDate &date,
                                        const CompositeindexInfo &deal_info);
};
}  // namespace candlestck
#endif /* CANDLESTICK_SRC_CANDLESTICK_WEEK_H_ */
