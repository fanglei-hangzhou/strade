//  Copyright (c) 2015-2015 The KID Authors. All rights reserved.
//  Created on: 2017/1/10 Author: zjc

#include "order_info.h"
#include "message.h"

namespace strade_user {

OrderInfo::OrderInfo(const SubmitOrderReq& req) {
  data_ = new Data();
  data_->group_id_ = req.group_id;
  data_->code_ = req.code;
  data_->op_ = req.op;
  data_->order_price_ = req.order_price;
  data_->order_num_ = req.order_nums;
}

REFCOUNT_DEFINE(OrderInfo)

} /* namespace strade_user */
