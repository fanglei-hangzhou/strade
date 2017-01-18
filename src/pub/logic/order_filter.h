//  Copyright (c) 2015-2015 The KID Authors. All rights reserved.
//  Created on: 2017/1/11 Author: zjc

#ifndef SRC_PUB_LOGIC_ORDER_FILTER_H_
#define SRC_PUB_LOGIC_ORDER_FILTER_H_

#include <vector>

#include "user_defined_types.h"

namespace strade_user {

class OrderInfo;
class OrderFilter;

typedef std::vector<OrderFilter*> OrderFilterList;

class OrderFilter {
 public:
  OrderFilter() {}
  virtual ~OrderFilter() {}
  virtual bool filter(const OrderInfo& order) { return false; }
};

class OrderOperationFilter : public OrderFilter {
 public:
  OrderOperationFilter(OrderOperation op)
      : op_(op) {}
  ~OrderOperationFilter() {}
  bool filter(const OrderInfo& order);
 private:
  OrderOperation op_;
};

class OrderStatusFilter : public OrderFilter {
 public:
  OrderStatusFilter(OrderStatus status)
      : status_(status) {}
  ~OrderStatusFilter() {}
  bool filter(const OrderInfo& order);
 private:
  OrderStatus status_;
};

class OrderCreateTimeFilter : public OrderFilter {
 public:
  OrderCreateTimeFilter(time_t begin_time, time_t end_time)
      : begin_time_(begin_time),
        end_time_(end_time) {}
  ~OrderCreateTimeFilter() {}
  bool filter(const OrderInfo& order);
 private:
  time_t begin_time_;
  time_t end_time_;
};

class OrderDealTimeFilter : public OrderFilter {
 public:
  OrderDealTimeFilter(time_t begin_time, time_t end_time)
      : begin_time_(begin_time),
        end_time_(end_time) {}
  ~OrderDealTimeFilter() {}
  bool filter(const OrderInfo& order);
 private:
  time_t begin_time_;
  time_t end_time_;
};

} /* namespace strade_user */

#endif /* SRC_PUB_LOGIC_ORDER_FILTER_H_ */
