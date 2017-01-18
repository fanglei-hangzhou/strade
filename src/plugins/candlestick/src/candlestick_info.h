/*
 * CandleStickInfo.h
 *
 *  Created on: 2017年1月9日
 *      Author: Administrator
 */

#ifndef CANDLESTICK_SRC_CANDLESTICK_INFO_H_
#define CANDLESTICK_SRC_CANDLESTICK_INFO_H_
#include <string>
namespace candlestick {
enum HistoryDataType {
  SINGLE_STOCK,
  COMPOSITEINDEX,
};

enum  StockHistoryType {
  STOCK_HISTORY_MONTH,
  STOCK_HISTORY_WEEK,
  STOCK_HISTORY_DAY
};

struct StockDate {
  int year;
  int month;
  int day;
  int week_index;
};

class StockDealInfo {
 public:
  StockDealInfo();
  virtual ~StockDealInfo();
  virtual void StatStockDealInfo(const StockDealInfo &info);
  HistoryDataType get_type() {
    return type_;
  }
  void set_date(std::string date) { date_ = date;}
  void set_amount(long amount) {amount_ = amount;}
  std::string get_date() const {return date_;}
  long get_amount() const {return amount_;}
 protected:
  long amount_;
  std::string date_;
  HistoryDataType type_;
};
class SingleStockInfo : public StockDealInfo{
 public:
  SingleStockInfo();
  virtual ~SingleStockInfo();
  virtual void StatSingleStockInfo(const SingleStockInfo &info);
  void set_high_price(double price) { high_price_ = price; }
  void set_low_price(double price) { low_price_ = price; }
  void set_open_price(double price) { open_price_ = price; }
  void set_close_price(double price) { close_price_ = price; }
  double get_high_price() const {return high_price_;}
  double get_low_price() const {return low_price_;}
  double get_open_price() const {return open_price_;}
  double get_close_price() const {return close_price_;}

 private:
  double high_price_;
  double low_price_;
  double open_price_;
  double close_price_;
};

class CompositeindexInfo : public StockDealInfo {
 public:
  CompositeindexInfo();
  virtual ~CompositeindexInfo();
  virtual void StatCompositeindexInfo(const CompositeindexInfo &info);
  void set_high_index(double index) { high_index_ = index; }
  void set_low_index(double index) { low_index_ = index; }
  void set_open_index(double index) { open_index_ = index; }
  void set_close_index(double index) { close_index_ = index; }
  double get_high_index() {return high_index_;}
  double get_low_index() {return low_index_;}
  double get_open_index() {return open_index_;}
  double get_close_index() {return close_index_;}
 private:
  double high_index_;
  double low_index_;
  double open_index_;
  double close_index_;
};
}  // namespace candlestick
#endif /* CANDLESTICK_SRC_CANDLESTICK_INFO_H_ */
