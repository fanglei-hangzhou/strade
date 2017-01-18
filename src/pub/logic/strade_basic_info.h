//
// Created by Harvey on 2017/1/7.
//

#ifndef STRADE_STRADE_BASIC_INFO_H
#define STRADE_STRADE_BASIC_INFO_H

#include "logic/base_values.h"
#include "logic/logic_comm.h"
#include "storage/mysql_engine.h"

namespace strade_logic {

class StockHistInfo;
class StockRealInfo;

typedef std::map<std::string, StockHistInfo> STOCK_HIST_MAP;
typedef std::map<time_t, StockRealInfo> STOCK_REAL_MAP;

// 实时数据
class StockRealInfo {
 public:
  StockRealInfo();
  StockRealInfo(const StockRealInfo& rhs);
  StockRealInfo& operator=(const StockRealInfo& rhs);
  ~StockRealInfo();

  time_t GetTradeTime() const {
    return data_->trade_time_;
  }

  const std::string& GetStockCode() const {
    return data_->stock_code_;
  }

 private:
  class Data {
   public:
    Data()
        : trade_time_(0),
          refcount_(1) {

    }
    void AddRef() {
      __sync_fetch_and_add(&refcount_, 1);
    }
    void Release() {
      __sync_fetch_and_sub(&refcount_, 1);
      if (!refcount_)
        delete this;
    }

   public:
    std::string stock_code_;
    int32 trade_time_;
    double change_percent_;
    double trade_;
    double open_;
    double high_;
    double low_;
    double settlement_;
    double volume_;
    double yield_;
    //相对于上一分钟的价格变化方向，涨则为1，跌则为-1，平则为0
    int price_change_direction_;
    //相对于上一分钟的价格变化量
    double price_change_;
    int visit_count_;

   private:
    int refcount_;
  };

 private:
  Data* data_;
};

// 历史数据
class StockHistInfo : public base_logic::AbstractDao{
 public:
  StockHistInfo();
  StockHistInfo(const StockHistInfo& rhs);
  StockHistInfo& operator=(const StockHistInfo& rhs);
  ~StockHistInfo();

  const std::string& GetHistDate() const {
    return data_->date_;
  }

 private:
  virtual void Deserialize();

 private:
  class Data {
   public:
    Data()
        : date_(""),
          refcount_(1) {

    }

    void AddRef() {
      __sync_fetch_and_add(&refcount_, 1);
    }
    void Release() {
      __sync_fetch_and_sub(&refcount_, 1);
      if (!refcount_)
        delete this;
    }
   public:
    std::string date_;
    double open_;
    double high_;
    double close_;
    double low_;
    double support_pos_;
    double pressure_pos_;
    double mid_price_;
    double qfq_close_;
    double day_yield_;
    double adjusted_day_yield_;
    double month_init_price_;
    //(最高价-最低价)/昨日收盘价
    double day_volatility_;
    double adjusted_day_volatility_;
    int visit_num_;

   private:
    int refcount_;
  };
 private:
  Data* data_;
};

class StockTotalInfo : public base_logic::AbstractDao {
 public:
  StockTotalInfo();
  StockTotalInfo(const StockTotalInfo& rhs);
  StockTotalInfo& operator=(const StockTotalInfo& rhs);
  ~StockTotalInfo();

  void ClearRealMap();

  STOCK_HIST_MAP GetStockHistMap() const;

  STOCK_REAL_MAP GetStockRealMap() const;

  bool AddStockHistInfoByDate(
      const std::string& date, const StockHistInfo& stock_hist_info);

  bool AddStockHistVec(std::vector<StockHistInfo>& stock_hist_vec);

  bool GetStockHistInfoByDate(
      const std::string& date, StockHistInfo& stock_hist_info);

  bool ReplaceStockHistInfo(
      const std::string& date, const StockHistInfo& stock_hist_info);

  bool AddStockRealInfoByTime(
      const time_t trade_time, const StockRealInfo& stock_real_info);

  bool GetStockRealInfoByTradeTime(
      const time_t trade_time, StockRealInfo& stock_real_info);

  bool ReplaceStockRealInfo(
      const time_t trade_time, const StockRealInfo& stock_real_info);

  bool GetCurrRealMarketInfo(StockRealInfo& stock_real_info);

 public:
  const std::string& GetStockCode() const {
    return data_->code_;
  }

 private:
  virtual void Deserialize();

 private:
  class Data {
   public:
    Data()
        : code_(""),
          refcount_(1) {
    }

    void AddRef() {
      __sync_fetch_and_add(&refcount_, 1);
    }
    void Release() {
      __sync_fetch_and_sub(&refcount_, 1);
      if (!refcount_)
        delete this;
    }

   public:
    std::string code_;                                    //股票代码
    std::string name_;                                    //股票名称
    std::string industry_;                                //行业
    double volume_;                                       //成交量
    double chengjiaoe_;                                   //成交额
    double change_percent_;                               //涨跌幅,收益率
    std::string area_;                                    //地区
    double pe_;                                           //市盈率
    double outstanding_;                                  //流通股本
    double totals_;                                       //总股本
    double totalAssets_;                                  //总资产(万)
    double liquidAssets_;                                 //流动资产
    double fixedAssets_;                                  //固定资产
    double reserved_;                                     //公积金
    double reservedPerShare_;                             //每股公积金
    double eps_;                                          //每股收益
    double bvps_;                                         //每股净资
    double pb_;                                           //市净率
    double market_value_;                                 //总市值
    double liquidMarketValue_;                            //流通市值
    double liquidScale_;                                  //流通比例
    std::string timeToMarket_;                            //上市日期
    double amplitude_;                                    //振幅
    double turnoverratio_;                                //换手率
    double current_trade_;                                //股价

    STOCK_HIST_MAP stock_hist_map_;
    STOCK_REAL_MAP stock_real_map_;

   private:
    int refcount_;
  };

 private:
  Data* data_;
};

} /* namespace strade_logic */

#endif //STRADE_STRADE_BASIC_INFO_H
