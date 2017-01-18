//  Copyright (c) 2015-2015 The KID Authors. All rights reserved.
//  Created on: 2017/1/11 Author: zjc

#include "order_filter.h"
#include "order_info.h"

namespace strade_user {

bool OrderOperationFilter::filter(const OrderInfo& order) {
  if (order.operation() == op_) {
    return false;
  }
  return true;
}

bool OrderStatusFilter::filter(const OrderInfo& order) {
  if (order.status() == status_) {
    return false;
  }
  return true;
}

bool OrderCreateTimeFilter::filter(const OrderInfo& order) {
  if (order.craete_time() >= begin_time_
      && order.craete_time() <= end_time_) {
    return false;
  }
  return true;
}

bool OrderDealTimeFilter::filter(const OrderInfo& order) {
  if (order.deal_time() >= begin_time_
      && order.deal_time() <= end_time_) {
    return false;
  }
  return true;
}

} /* namespace strade_user */
