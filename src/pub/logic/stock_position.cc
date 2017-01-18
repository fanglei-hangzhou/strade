//  Copyright (c) 2015-2015 The KID Authors. All rights reserved.
//  Created on: 2017/1/10 Author: zjc

#include "stock_position.h"

namespace strade_user {

FakeStockPosition::FakeStockPosition(StockPositionId id, uint32 count) {
  data_ = new Data();
  data_->id_ = id;
  data_->count_ = count;
}

REFCOUNT_DEFINE(FakeStockPosition)

///////////////////////////////////////////////////////////////////////////////

GroupStockPosition::GroupStockPosition(GroupId id, const std::string& code, uint32 count) {
  data_ = new Data();
  data_->group_id_ = id;
  data_->code_ = code;
  data_->count_ = count;
}

REFCOUNT_DEFINE(GroupStockPosition)
///////////////////////////////////////////////////////////////////////////////

StockPosition::StockPosition(const std::string& code, uint32 count) {
  data_ = new Data();
  data_->code_ = code;
  data_->count_ = count;
}

REFCOUNT_DEFINE(StockPosition)

} /* namespace strade_user */
