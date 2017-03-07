//  Copyright (c) 2015-2015 The KID Authors. All rights reserved.
//  Created on: 2016年10月13日 Author: zjc

#include "realtime_code_info.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <sstream>

#include "logic/logic_comm.h"

namespace stock_logic {

#define TO_STRING(x)  # x

#define SET_ORDER(x)                                        \
  do {                                                      \
    orders[x-1].buy = atof(data[BUY##x].c_str());             \
    orders[x-1].buy_vol = atof(data[BUY##x##VOL].c_str());   \
    orders[x-1].sale = atof(data[SAL##x].c_str());            \
    orders[x-1].sale_vol = atof(data[SAL##x##VOL].c_str());  \
  } while (0)

#define PRINT(x)\
  oss << #x << ':' << x << ',';

#define PRINT_ORDER(x)\
    do {\
      oss << TO_STRING(buy ## x) << ':' << orders[x].buy << ',';\
      oss << TO_STRING(buy_vol ## x) << ':' << orders[x].buy_vol << ',';\
      oss << TO_STRING(sal ## x) << ':' << orders[x].sale << ',';\
      oss << TO_STRING(sal_vol ## x) << ':' << orders[x].sale_vol << ',';\
    } while (0)

bool StockRealInfo::Valid() {
  if (price < MIN_PRICE) {
    return false;
  }
  if (high < MIN_PRICE) {
    return false;
  }
  if (low < MIN_PRICE) {
    return false;
  }
  if (close < MIN_PRICE) {
    return false;
  }

  double change_percent = 0.0;
  if (0.0 != close) {
    change_percent = price / close - 1.0;
    change_percent *= 100;
  }

  if (!(change_percent >= MIN_PRICE_RISE
      && change_percent <= MAX_PRICE_RISE)) {
    return false;
  }

  return true;
}

bool StockRealInfo::Delist() {
//  if (price >= 0.1) {
//    return false;
//  }
//  if (open >= 0.1) {
//    return false;
//  }
//  if (high >= 0.1) {
//    return false;
//  }
//  if (low >= 0.1) {
//    return false;
//  }
//  if (amount >= 0.1) {
//    return false;
//  }
//
//  for (size_t i = 0; i < 10; ++i) {
//    if (orders[i].buy >= 0.1)
//      return false;
//
//    if (orders[i].sale >= 0.1)
//      return false;
//  }

  if (open < 0.1) {
    return true;
  }

  return false;
}

bool StockRealInfo::Deserialize(const StrVec& data) {
  if (data.size() < MAX_FIELD) {
    return false;
  }

  code = data[CODE];
  name = data[NAME];
  price = atof(data[PRICE].c_str());
  open = atof(data[OPEN].c_str());
  high = atof(data[HIGH].c_str());
  low = atof(data[LOW].c_str());
  close = atof(data[CLOSE].c_str());
  amount = atof(data[AMOUNT].c_str());
  vol = atof(data[VOL].c_str());

//  orders[0].buy = atof(data[BUY1].c_str());
//  orders[0].buy_vol = atoll(data[BUY1VOL].c_str());
//  orders[0].sale = atof(data[SAL1].c_str());
//  orders[0].sale_vol = atoll(data[SAL1VOL].c_str());
  SET_ORDER(1);
  SET_ORDER(2);
  SET_ORDER(3);
  SET_ORDER(4);
  SET_ORDER(5);
  SET_ORDER(6);
  SET_ORDER(7);
  SET_ORDER(8);
  SET_ORDER(9);
  SET_ORDER(10);

  oi = atoi(data[OI].c_str());
  poi = atoi(data[POI].c_str());
  settle = atof(data[SETTLE].c_str());
  psettle = atof(data[PSETTLE].c_str());
  upper = atof(data[UPPER].c_str());
  lower = atof(data[LOWER].c_str());
  tradetime = atoi(data[TRADETIME].c_str());

  return true;
}

std::string StockRealInfo::Serialized() {
  std::ostringstream oss;
  double change_percent = 0.0;
  if (0.0 != close) {
    change_percent = price / close - 1.0;
    change_percent *= 100;
  }

  PRINT(code);
//  PRINT(name);
  PRINT(price);
  PRINT(close);
  PRINT(change_percent);
  PRINT(open);
  PRINT(high);
  PRINT(low);
  PRINT(amount);
  PRINT(vol);

  PRINT_ORDER(0);
  PRINT_ORDER(1);
  PRINT_ORDER(2);
  PRINT_ORDER(3);
  PRINT_ORDER(4);
  PRINT_ORDER(5);
  PRINT_ORDER(6);
  PRINT_ORDER(7);
  PRINT_ORDER(8);
  PRINT_ORDER(9);

  PRINT(oi);
  PRINT(poi);
  PRINT(settle);
  PRINT(upper);
  PRINT(lower);
  PRINT(tradetime);
  return oss.str();
}

DbCodeInfo StockRealInfo::CreateDbCodeInfo() {
  DbCodeInfo d;
  d.code = code;
  d.name = name;
  d.price_change = price - open;
  if (close > 0.0) {
    d.change_percent = price / close - 1.0;
    d.change_percent *= 100;
  }
  d.settlement = close;
  if (Delist()) {
    d.change_percent = 0.0;
    return d;
  }

  d.price = price;
  d.open = open;
  d.high = high;
  d.close = close;
  d.low = low;
  d.volume = vol;
  d.amount = amount;
  return d;
}

} /* namespace stock_logic */
