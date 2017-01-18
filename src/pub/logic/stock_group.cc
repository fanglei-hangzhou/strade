//  Copyright (c) 2015-2015 The KID Authors. All rights reserved.
//  Created on: 2017/1/10 Author: zjc

#include "stock_group.h"
#include "logic/logic_comm.h"

namespace strade_user {

StockGroup::StockGroup() {
  data_ = new Data();
}

REFCOUNT_DEFINE(StockGroup)

bool StockGroup::AddStocks(StockCodeList& stocks) {
  size_t n = stocks.size();
  StockCodeList s;
  for (size_t i = 0; i < stocks.size(); ++i) {
    if (data_->stock_set_.count(stocks[i])) {
      LOG_ERROR2("stock:%s exist in group:%s",
                 stocks[i].data(), data_->name_.data());
      continue;
    }
    data_->stock_set_.insert(stocks[i]);
    data_->stock_list_.push_back(stocks[i]);
    s.push_back(stocks[i]);
  }
  stocks.swap(s);
  return stocks.size() == n;
}


bool StockGroup::DelStocks(StockCodeList& stocks) {
  size_t n = stocks.size();
  StockCodeList s;
  for (size_t i = 0; i < stocks.size(); ++i) {
    if (!data_->stock_set_.count(stocks[i])) {
      LOG_ERROR2("stock:%s not exist in group:%s",
                 stocks[i].data(), data_->name_.data());
      continue;
    }
    data_->stock_set_.erase(stocks[i]);
    s.push_back(stocks[i]);
  }

  StockCodeList t;
  for (size_t i = 0; i < data_->stock_list_.size(); ++i) {
    bool find = false;
    for (size_t j = 0; j < s.size(); ++j) {
      if (s[j] == data_->stock_list_[i]) {
        find = true;
      }
    }
    if (!find) {
      t.push_back(data_->stock_list_[i]);
    }
  }

  stocks.swap(s);
  data_->stock_list_.swap(t);
  return stocks.size() == n;
}

} /* namespace strade_user */
