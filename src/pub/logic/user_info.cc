//  Copyright (c) 2015-2015 The KID Authors. All rights reserved.
//  Created on: 2017/1/10 Author: zjc

#include "user_info.h"
#include "logic/logic_comm.h"
#include "message.h"

namespace strade_user {

UserInfo::UserInfo() {
  data_ = new Data();
}

REFCOUNT_DEFINE(UserInfo)

GroupId UserInfo::CreateGroup(const std::string& name,
                    const StockCodeList& code_list) {
  for (size_t i = 0; i < data_->stock_group_list_.size(); ++i) {
    if (name == data_->stock_group_list_[i].name()) {
      LOG_ERROR2("user:%s create group error: group_name:%s exist",
                 data_->name_.data(), name.data());
      return INVALID_GROUPID;
    }
  }

  // TODO: insert into mysql and get id and update cache
}

StockGroup* UserInfo::GetGroup(GroupId group_id) {
  for (size_t i = 0; i < data_->stock_group_list_.size(); ++i) {
    if (group_id == data_->stock_group_list_[i].id()) {
      return &data_->stock_group_list_[i];
    }
  }
  return NULL;
}

bool UserInfo::AddStock(GroupId group_id, StockCodeList& code_list) {
  StockGroup *g = GetGroup(group_id);
  if (NULL == g) {
    LOG_ERROR2("user:%s add stock error: group_id:%d not exist",
               data_->name_.data(), group_id);
    return false;
  }

  g->AddStocks(code_list);
  // TODO: update mysql and cache
}

bool UserInfo::DelStock(GroupId group_id, StockCodeList& code_list) {
  StockGroup *g = GetGroup(group_id);
  if (NULL == g) {
    LOG_ERROR2("user:%s del stock error: group_id:%d not exist",
               data_->name_.data(), group_id);
    return false;
  }

  g->DelStocks(code_list);
  // TODO: update mysql and cache
}

bool UserInfo::GetGroupStock(GroupId group_id, StockCodeList& stocks) {
  StockGroup *g = GetGroup(group_id);
  if (NULL == g) {
    LOG_ERROR2("user:%s get group stock error: group_id:%d not exist",
               data_->name_.data(), group_id);
    return false;;
  }

  stocks = g->stocks();
  return !stocks.empty();
}

GroupStockPositionList UserInfo::GetHoldingStocks() {
  return data_->stock_position_list_;
}

OrderList UserInfo::FindOrders(const OrderFilterList& filters) {
  OrderList orders;
  for (size_t i = 0; i < data_->order_list_.size(); ++i) {
    bool filter = false;
    for (size_t j = 0; j < filters.size(); ++j) {
      if (filters[j]->filter(data_->order_list_[i])) {
        filter = true;
        break;
      }
    }
    if (!filter) {
      orders.push_back(data_->order_list_[i]);
    }
  }
  return orders;
}

GroupStockPosition* UserInfo::GetGroupStockPosition(GroupId group_id,
                                          const std::string& code) {
  StockGroup* g = GetGroup(group_id);
  if (NULL == g) {
    LOG_ERROR2("user:%s get stock position error: group_id:%d not exist",
               data_->name_.data(), group_id);
    return NULL;
  }

  for (size_t i = 0; i < data_->stock_position_list_.size(); ++i) {
    if (data_->stock_position_list_[i].group_id() == group_id) {
      return &data_->stock_position_list_[i];
    }
  }
  return NULL;
}

bool UserInfo::SubmitOrder(SubmitOrderReq& req) {
  StockGroup* g = GetGroup(req.group_id);
  if (NULL == g) {
    LOG_ERROR2("user:%s submit order error: group_id:%d not exist",
               data_->name_.data(), req.group_id);
    return false;
  }

  if (g->exist_stock(req.code)) {
    LOG_ERROR2("user:%s submit order error: stock:%s not in group:%s",
               data_->name_.data(), req.code.data(), g->name().data());
    return false;
  }

  if (BUY == req.op) {
  // TODO: check available capital
  }

  if (SELL == req.op) {
    GroupStockPosition* p = GetGroupStockPosition(req.group_id, req.code);
    if (NULL == p) {
      LOG_ERROR2("user:%s submit order error: no stock:%s position",
                 data_->name_.data(), req.code.data());
      return false;
    }
    if (req.order_nums > p->count()) {
      LOG_ERROR2("user:%s submit order error: "
          "current stock:%s count:%d less than order count:%d",
          data_->name_.data(), req.code.data(), p->count(), req.order_nums);
      req.order_nums = p->count();
    }
  }

  // TODO: insert into mysql

  data_->order_list_.push_back(OrderInfo(req));

  // TODO: register callback

  return true;
}

} /* namespace strade_user */
