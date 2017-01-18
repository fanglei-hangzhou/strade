//  Copyright (c) 2015-2015 The KID Authors. All rights reserved.
//  Created on: 2017/1/10 Author: zjc

#ifndef SRC_PUB_LOGIC_ORDER_INFO_H_
#define SRC_PUB_LOGIC_ORDER_INFO_H_

#include <time.h>

#include <string>
#include <vector>

#include "macros.h"
#include "user_defined_types.h"
#include "basic/basictypes.h"


namespace strade_user {

struct SubmitOrderReq;

class OrderInfo {
 public:
  OrderInfo(const SubmitOrderReq& req);
  REFCOUNT_DECLARE(OrderInfo);
 public:
  void set_operation(OrderOperation op) { data_->op_ = op; }
  OrderOperation operation() const { return data_->op_; }

  void set_status(OrderStatus status) { data_->status_ = status; }
  OrderStatus status() const { return data_->status_; }

  time_t craete_time() const { return data_->create_time_; }
  time_t deal_time() const { return data_->deal_time_; }

 private:
  class Data {
   public:
    Data()
        : refcount_(1),
          group_id_(INVALID_GROUPID),
          op_(BUY),
          status_(PENDING),
          create_time_(time(NULL)),
          order_price_(0.0),
          order_num_(0),
          deal_time_(-1),
          deal_price_(0.0),
          deal_num_(0),
          commission_(0.0),
          transfer_fee_(0.0),
          amount_(0.0) {
    }

   public:
    GroupId group_id_;
    std::string code_;
    OrderOperation op_;
    OrderStatus status_;

    time_t create_time_;
    double order_price_;
    uint32 order_num_;

    time_t deal_time_;
    double deal_price_;
    double deal_num_;
    double commission_;
    double transfer_fee_;
    double amount_;

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

typedef std::vector<OrderInfo> OrderList;
} /* namespace strade_user */

#endif /* SRC_PUB_LOGIC_ORDER_INFO_H_ */
