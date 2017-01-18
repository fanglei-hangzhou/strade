//  Copyright (c) 2015-2015 The KID Authors. All rights reserved.
//  Created on: 2017/1/9 Author: zjc

#include "message.h"

#include <sstream>

#include "logic/base_values.h"
#include "logic/logic_comm.h"

#define OSS_WRITE(x)        \
  oss << "\t\t" << #x << " = " << x << std::endl

namespace strade_user {

bool Head::StartDeserialize(DictionaryValue& dict) {
  return Head::Deserialize(dict) && Deserialize(dict);
}

bool Head::Deserialize(DictionaryValue& dict) {
  int64 t;
  if (!dict.GetBigInteger(L"opcode", &t)) {
    LOG_ERROR("NOT FIND opcode");
    return false;
  }
  opcode = t;

  if (!dict.GetBigInteger(L"user_id", &t)) {
    LOG_ERROR("NOT FIND user_id");
    return false;
  }
  user_id = t;

  if (!dict.GetString(L"token", &token)) {
    LOG_ERROR("NOT FIND token");
    return false;
  }
  return true;
}

void Head::StartDump(std::ostringstream& oss) {
  Head::Dump(oss);
  Dump(oss);
}

void Head::Dump(std::ostringstream& oss) {
  oss << "\t\t--------- Head ---------" << std::endl;
  OSS_WRITE(opcode);
  OSS_WRITE(user_id);
  OSS_WRITE(token);
}

///////////////////////////////////////////////////////////////////////////////
bool CreateGroupReq::Deserialize(DictionaryValue& dict) {
  if (!dict.GetString(L"group_name", &group_name)) {
    LOG_ERROR("NOT FIND group_name");
    return false;
  }

  std::string str;
  if (!dict.GetString(L"code_list", &str)) {
    LOG_ERROR("NOT FIND code_list");
    return false;
  }

  std::string code;
  std::istringstream iss(str);
  while (std::getline(iss, code, ',')) {
    if (!code.empty()) {
      code_list.push_back(code);
    }
  }
  return true;
}

void CreateGroupReq::Dump(std::ostringstream& oss) {
  oss << "\t\t--------- CreateGroupReq ---------" << std::endl;
  OSS_WRITE(group_name);
  oss << "\t\tcode_list = ";
  for (size_t i = 0; i < code_list.size(); ++i) {
    oss << code_list[i] << ",";
  }
}

bool CreateGroupRes::Serialize(DictionaryValue& dict) {
  dict.SetBigInteger(L"group_id", group_id);
  return true;
}

///////////////////////////////////////////////////////////////////////////////

bool AddStockReq::Deserialize(DictionaryValue& dict) {
  int64 t;
  if (!dict.GetBigInteger(L"group_id", &t)) {
    LOG_ERROR("NOT FIND group_id");
    return false;
  }
  group_id = t;

  std::string str;
  if (!dict.GetString(L"code_list", &str)) {
    LOG_ERROR("NOT FIND code_list");
    return false;
  }

  std::string code;
  std::istringstream iss(str);
  while (std::getline(iss, code, ',')) {
    if (!code.empty()) {
      code_list.push_back(code);
    }
  }
  return true;
}

void AddStockReq::Dump(std::ostringstream& oss) {
  oss << "\t\t--------- AddStockReq ---------" << std::endl;
  OSS_WRITE(group_id);
  oss << "\t\tcode_list = ";
  for (size_t i = 0; i < code_list.size(); ++i) {
    oss << code_list[i] << ",";
  }
}
///////////////////////////////////////////////////////////////////////////////
bool DelStockReq::Deserialize(DictionaryValue& dict) {
  int64 t;
  if (!dict.GetBigInteger(L"group_id", &t)) {
    LOG_ERROR("NOT FIND group_id");
    return false;
  }
  group_id = t;

  std::string str;
  if (!dict.GetString(L"code_list", &str)) {
    LOG_ERROR("NOT FIND code_list");
    return false;
  }

  std::string code;
  std::istringstream iss(str);
  while (std::getline(iss, code, ',')) {
    if (!code.empty()) {
      code_list.push_back(code);
    }
  }
  return true;
}

void DelStockReq::Dump(std::ostringstream& oss) {
  oss << "\t\t--------- DelStockReq ---------" << std::endl;
  OSS_WRITE(group_id);
  oss << "\t\tcode_list = ";
  for (size_t i = 0; i < code_list.size(); ++i) {
    oss << code_list[i] << ",";
  }
}
///////////////////////////////////////////////////////////////////////////////
bool QueryGroupReq::Deserialize(DictionaryValue& dict) {
  return true;
}

void QueryGroupReq::Dump(std::ostringstream& oss) {
  oss << "\t\t--------- QueryGroupReq ---------" << std::endl;
}

bool QueryGroupsRes::GroupInfo::Serialize(DictionaryValue& dict) {
  dict.SetBigInteger(L"id", id);
  dict.SetString(L"name", name);
  return true;
}

bool QueryGroupsRes::Serialize(DictionaryValue& dict) {
  ListValue* unit_list = new ListValue();
  for (size_t i = 0; i < group_list.size(); ++i) {
    DictionaryValue* unit = new DictionaryValue();
    group_list[i].Serialize(*unit);
    unit_list->Append(unit);
  }
  dict.Set(L"group_list", unit_list);
  return true;
}
///////////////////////////////////////////////////////////////////////////////
bool QueryStocksReq::Deserialize(DictionaryValue& dict) {
  int64 t;
  if (!dict.GetBigInteger(L"group_id", &t)) {
    LOG_ERROR("NOT FIND group_id");
    return false;
  }
  group_id = t;
  return true;
}

void QueryStocksReq::Dump(std::ostringstream& oss) {
  oss << "\t\t--------- QueryStocksReq ---------" << std::endl;
  OSS_WRITE(group_id);
}

bool QueryStocksRes::StockInfo::Serialize(DictionaryValue& dict) {
  dict.SetString(L"code", code);
  dict.SetString(L"name", name);
  dict.SetBigInteger(L"visit_heat", visit_heat);
  dict.SetReal(L"price", price);
  dict.SetReal(L"change", change);
  dict.SetBigInteger(L"volume", volume);
  dict.SetString(L"industry", industry);
  return true;
}

bool QueryStocksRes::Serialize(DictionaryValue& dict) {
  ListValue* unit_list = new ListValue();
  for (size_t i = 0; i < stock_list.size(); ++i) {
    DictionaryValue* unit = new DictionaryValue();
    stock_list[i].Serialize(*unit);
    unit_list->Append(unit);
  }
  dict.Set(L"stock_list", unit_list);
  return true;
}
///////////////////////////////////////////////////////////////////////////////
bool QueryHoldingStocksReq::Deserialize(DictionaryValue& dict) {
  return true;
}

void QueryHoldingStocksReq::Dump(std::ostringstream& oss) {
  oss << "\t\t--------- QueryHoldingStocksReq ---------" << std::endl;
}

bool QueryHoldingStocksRes::StockInfo::Serialize(DictionaryValue& dict) {
  dict.SetString(L"code", code);
  dict.SetBigInteger(L"holding", holding);
  dict.SetBigInteger(L"available", available);
  dict.SetReal(L"cost", cost);
  dict.SetReal(L"price", price);
  dict.SetReal(L"market_value", market_value);
  dict.SetReal(L"profit", profit);
  dict.SetReal(L"profit_ratio", profit_ratio);
  dict.SetReal(L"position", position);
  return true;
}

bool QueryHoldingStocksRes::Serialize(DictionaryValue& dict) {
  ListValue* unit_list = new ListValue();
  for (size_t i = 0; i < stock_list.size(); ++i) {
    DictionaryValue* unit = new DictionaryValue();
    stock_list[i].Serialize(*unit);
    unit_list->Append(unit);
  }
  dict.Set(L"stock_list", unit_list);
  return true;
}

///////////////////////////////////////////////////////////////////////////////
bool QueryTodayOrdersReq::Deserialize(DictionaryValue& dict) {
  return true;
}

void QueryTodayOrdersReq::Dump(std::ostringstream& oss) {
  oss << "\t\t--------- QueryTodayOrdersReq ---------" << std::endl;
}

bool QueryTodayOrdersRes::OrderInfo::Serialize(DictionaryValue& dict) {
  dict.SetString(L"code", code);
  dict.SetBigInteger(L"order_operation", op);
  dict.SetReal(L"order_price", order_price);
  dict.SetBigInteger(L"order_nums", order_nums);
  dict.SetBigInteger(L"order_time", order_time);
  dict.SetBigInteger(L"status", status);
  return true;
}

bool QueryTodayOrdersRes::Serialize(DictionaryValue& dict) {
  ListValue* unit_list = new ListValue();
  for (size_t i = 0; i < order_list.size(); ++i) {
    DictionaryValue* unit = new DictionaryValue();
    order_list[i].Serialize(*unit);
    unit_list->Append(unit);
  }
  dict.Set(L"order_list", unit_list);
  return true;
}
///////////////////////////////////////////////////////////////////////////////
bool QueryTodayFinishedOrdersReq::Deserialize(DictionaryValue& dict) {
  return true;
}

void QueryTodayFinishedOrdersReq::Dump(std::ostringstream& oss) {
  oss << "\t\t--------- QueryTodayFinishedOrdersReq ---------" << std::endl;
}

bool QueryTodayFinishedOrdersRes::OrderInfo::Serialize(DictionaryValue& dict) {
  dict.SetString(L"code", code);
  dict.SetBigInteger(L"order_operation", op);
  dict.SetReal(L"order_price", order_price);
  dict.SetBigInteger(L"order_nums", order_nums);
  dict.SetReal(L"amount", amount);
  dict.SetBigInteger(L"order_time", order_time);
  return true;
}

bool QueryTodayFinishedOrdersRes::Serialize(DictionaryValue& dict) {
  ListValue* unit_list = new ListValue();
  for (size_t i = 0; i < order_list.size(); ++i) {
    DictionaryValue* unit = new DictionaryValue();
    order_list[i].Serialize(*unit);
    unit_list->Append(unit);
  }
  dict.Set(L"order_list", unit_list);
  return true;
}
///////////////////////////////////////////////////////////////////////////////
bool QueryHistoryFinishedOrdersReq::Deserialize(DictionaryValue& dict) {
  return true;
}

void QueryHistoryFinishedOrdersReq::Dump(std::ostringstream& oss) {
  oss << "\t\t--------- QueryHistoryFinishedOrdersReq ---------" << std::endl;
}

bool QueryHistoryFinishedOrdersRes::OrderInfo::Serialize(DictionaryValue& dict) {
  dict.SetString(L"code", code);
  dict.SetBigInteger(L"order_operation", op);
  dict.SetReal(L"order_price", order_price);
  dict.SetBigInteger(L"order_nums", order_nums);
  dict.SetReal(L"amount", amount);
  dict.SetBigInteger(L"order_time", order_time);
  return true;
}

bool QueryHistoryFinishedOrdersRes::Serialize(DictionaryValue& dict) {
  ListValue* unit_list = new ListValue();
  for (size_t i = 0; i < order_list.size(); ++i) {
    DictionaryValue* unit = new DictionaryValue();
    order_list[i].Serialize(*unit);
    unit_list->Append(unit);
  }
  dict.Set(L"order_list", unit_list);
  return true;
}
///////////////////////////////////////////////////////////////////////////////
bool QueryStatementReq::Deserialize(DictionaryValue& dict) {
  if (!dict.GetString(L"begin_time", &begin_time)) {
    LOG_ERROR("NOT FIND begin_time");
    return false;
  }

  if (!dict.GetString(L"end_time", &end_time)) {
    LOG_ERROR("NOT FIND end_time");
    return false;
  }

  return true;
}

void QueryStatementReq::Dump(std::ostringstream& oss) {
  oss << "\t\t--------- QueryStatementReq ---------" << std::endl;
  OSS_WRITE(begin_time);
  OSS_WRITE(end_time);
}

bool QueryStatementRes::StatementRecord::Serialize(DictionaryValue& dict) {
  dict.SetString(L"code", code);
  dict.SetBigInteger(L"order_operation", op);
  dict.SetReal(L"order_price", order_price);
  dict.SetBigInteger(L"order_nums", order_nums);
  dict.SetReal(L"commission", commission);
  dict.SetReal(L"stamp_duty", stamp_duty);
  dict.SetReal(L"transfer_fee", transfer_fee);
  dict.SetReal(L"amount", amount);
  dict.SetReal(L"available_capital", available_capital);
  return true;
}

bool QueryStatementRes::Serialize(DictionaryValue& dict) {
  ListValue* unit_list = new ListValue();
  for (size_t i = 0; i < statement_list.size(); ++i) {
    DictionaryValue* unit = new DictionaryValue();
    statement_list[i].Serialize(*unit);
    unit_list->Append(unit);
  }
  dict.Set(L"statement_list", unit_list);
  return true;
}

///////////////////////////////////////////////////////////////////////////////
bool SubmitOrderReq::Deserialize(DictionaryValue& dict) {
  int64 t;
  if (!dict.GetBigInteger(L"group_id", &t)) {
    LOG_ERROR("NOT FIND group_id");
    return false;
  }
  group_id = t;

  if (!dict.GetString(L"code", &code)) {
    LOG_ERROR("NOT FIND code");
    return false;
  }

  if (!dict.GetReal(L"order_price", &order_price)) {
    LOG_ERROR("NOT FIND order_price");
    return false;
  }

  if (!dict.GetBigInteger(L"order_nums", &t)) {
    LOG_ERROR("NOT FIND order_nums");
    return false;
  }
  order_nums = t;

  if (!dict.GetBigInteger(L"order_operation", &t)) {
    LOG_ERROR("NOT FIND order_operation");
    return false;
  }
  op = (OrderOperation)t;
  return true;
}

void SubmitOrderReq::Dump(std::ostringstream& oss) {
  oss << "\t\t--------- SubmitOrderReq ---------" << std::endl;
  OSS_WRITE(group_id);
  OSS_WRITE(code);
  OSS_WRITE(order_price);
  OSS_WRITE(order_nums);
  OSS_WRITE((int)op);
}
}

