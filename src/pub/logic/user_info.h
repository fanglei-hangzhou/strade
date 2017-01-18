//  Copyright (c) 2015-2015 The KID Authors. All rights reserved.
//  Created on: 2017/1/10 Author: zjc

#ifndef SRC_PUB_LOGIC_USER_INFO_H_
#define SRC_PUB_LOGIC_USER_INFO_H_

#include "macros.h"
#include "user_defined_types.h"
#include "stock_group.h"
#include "order_filter.h"
#include "order_info.h"
#include "stock_position.h"

namespace strade_user {

class UserInfo {
 public:
  UserInfo();
  REFCOUNT_DECLARE(UserInfo);
 public:
  GroupId CreateGroup(const std::string& name,
                      const StockCodeList& code_list);
  StockGroup* GetGroup(GroupId group_id);
  GroupStockPosition* GetGroupStockPosition(GroupId group_id, const std::string& code);

  bool AddStock(GroupId group_id, StockCodeList& code_list);
  bool DelStock(GroupId group_id, StockCodeList& code_list);
  StockGroupList GetAllGroups() const { return data_->stock_group_list_; }
  bool GetGroupStock(GroupId group_id, StockCodeList& stocks);
  GroupStockPositionList GetHoldingStocks();

  OrderList FindOrders(const OrderFilterList& filters);
  bool SubmitOrder(SubmitOrderReq& req);

 public:
  UserId id() const { return data_->id_; }
  void set_id(UserId id) { data_->id_ = id; }

  std::string name() const { return data_->name_; }
  void set_name(const std::string& name) { data_->name_ = name; }

  std::string phone() const { return data_->phone_; }
  void set_phone(const std::string& phone) { data_->phone_ = phone; }

  double total_assets() const { return data_->total_assets_; }
  void set_total_assets(double assets) { data_->total_assets_ = assets; }

  double available_capital() const { return data_->available_capital_; }
  void set_available_capital(double available_capital) { data_->available_capital_ = available_capital; }

 private:
  class Data {
   public:
    Data()
        : refcount_(1),
          id_(0),
          total_assets_(0.0),
          available_capital_(0.0) {
    }

   public:
    UserId id_;
    std::string name_;
    std::string phone_;

    double total_assets_;       // 总资产
    double available_capital_;  // 可用资金

    StockGroupList stock_group_list_;         // 股票组合
    GroupStockPositionList stock_position_list_;   // 当前持仓
    OrderList order_list_;                    // 委托

    void AddRef() {
      __sync_fetch_and_add(&refcount_, 1);
    }
    void Release() {
      __sync_fetch_and_sub(&refcount_, 1);
      if (!refcount_)
        delete this;
    }

   private:
    int refcount_;
  };
 private:
  Data* data_;
};

} /* namespace strade_user */

#endif /* SRC_PUB_LOGIC_USERINFO_H_ */
