//  Copyright (c) 2015-2015 The KID Authors. All rights reserved.
//  Created on: 2017/1/9 Author: zjc

#include "user_logic.h"

#include <sstream>

#include "basic/scoped_ptr.h"
#include "logic/base_values.h"
#include "logic/logic_comm.h"
#include "logic/message.h"


#define DEFAULT_CONFIG_PATH     "./plugins/user/user_config.xml"

using base_logic::ValueSerializer;

namespace strade_user {

UserLogic* UserLogic::GetInstance() {
  if (NULL == instance_)
    instance_ = new UserLogic();
  return instance_;
}

void UserLogic::FreeInstance() {
  delete instance_;
  instance_ = NULL;
}

UserLogic::UserLogic() {
}

UserLogic::~UserLogic() {
}

bool UserLogic::OnUserConnect(struct server *srv, const int socket) {
  return true;
}

DictionaryValue* UserLogic::HttpDeserialize(const char* msg, int len) {
  if (NULL == msg || len <= 0) {
    return NULL;
  }

  if ('\n' != msg[len-1]) {
    LOG_ERROR("error msg");
    return NULL;
  }

  int error_code = 0;
  std::string error_str;
  std::string data(msg, len-1);

  scoped_ptr<ValueSerializer> engine(
      ValueSerializer::Create(base_logic::IMPL_HTTP, &data));

  Value* value = engine->Deserialize(&error_code, &error_str);
  if (0 != error_code || NULL == value) {
    LOG_ERROR2("deserialize error, err_code: %d, err_msg: %s",
               error_code, error_str.c_str());
    return NULL;
  }

  return (DictionaryValue*) value;
}

bool UserLogic::OnUserMessage(struct server *srv, const int socket,
                                const void *msg, const int len) {
  std::string data((const char*)msg, len);
  LOG_DEBUG2("recv msg:\n%s", data.c_str());

  scoped_ptr<DictionaryValue> dict(HttpDeserialize((const char*)msg, len));
  if (dict == NULL) {
    return false;
  }

  Head head;
  if (!head.Deserialize(*dict)) {
    return false;
  }

  switch (head.opcode) {
    case CreateGroupReq::ID:
      OnCreateGroup(*dict);
      break;
    case AddStockReq::ID:
      OnAddStock(*dict);
      break;
    case DelStockReq::ID:
      OnDelStock(*dict);
      break;
    case QueryGroupReq::ID:
      OnQueryGroup(*dict);
      break;
    case QueryStocksReq::ID:
      OnQueryStock(*dict);
      break;
    case QueryHoldingStocksReq::ID:
      OnQueryHoldingStock(*dict);
      break;
    case QueryTodayOrdersReq::ID:
      OnQueryTodayOrder(*dict);
      break;
    case QueryTodayFinishedOrdersReq::ID:
      OnQueryTodayFinishedOrder(*dict);
      break;
    case QueryHistoryFinishedOrdersReq::ID:
      OnQueryHistoryFinishedOrder(*dict);
      break;
    case QueryStatementReq::ID:
      OnQueryStatement(*dict);
      break;
    case SubmitOrderReq::ID:
      OnSubmitOrder(*dict);
      break;
    default:
      LOG_ERROR2("UNKNOWN opcode: %d", head.opcode);
      break;
  }
  return true;
}

bool UserLogic::OnUserClose(struct server *srv, const int socket) {
  return true;
}

bool UserLogic::OnBroadcastConnect(struct server *srv, const int socket,
                                    const void *msg, const int len) {
  return true;
}

bool UserLogic::OnBroadcastMessage(struct server *srv, const int socket,
                                    const void *msg, const int len) {
  return true;
}

bool UserLogic::OnBroadcastClose(struct server *srv, const int socket) {
  return true;
}

bool UserLogic::OnIniTimer(struct server *srv) {
  return true;
}

bool UserLogic::OnTimeout(struct server *srv, char *id, int opcode, int time) {
  return true;
}

void UserLogic::OnCreateGroup(DictionaryValue& dict) {
  CreateGroupReq msg;
  if (!msg.StartDeserialize(dict)) {
    return ;
  }

  std::ostringstream oss;
  msg.StartDump(oss);
  LOG_DEBUG2("%s", oss.str().data());


}

void UserLogic::OnAddStock(DictionaryValue& dict) {
  AddStockReq msg;
  if (!msg.StartDeserialize(dict)) {
    return ;
  }

  std::ostringstream oss;
  msg.StartDump(oss);
  LOG_DEBUG2("%s", oss.str().data());
}
void UserLogic::OnDelStock(DictionaryValue& dict) {
  DelStockReq msg;
  if (!msg.StartDeserialize(dict)) {
    return ;
  }

  std::ostringstream oss;
  msg.StartDump(oss);
  LOG_DEBUG2("%s", oss.str().data());
}
void UserLogic::OnQueryGroup(DictionaryValue& dict) {
  QueryGroupReq msg;
  if (!msg.StartDeserialize(dict)) {
    return ;
  }

  std::ostringstream oss;
  msg.StartDump(oss);
  LOG_DEBUG2("%s", oss.str().data());
}
void UserLogic::OnQueryStock(DictionaryValue& dict) {
  QueryStocksReq msg;
  if (!msg.StartDeserialize(dict)) {
    return ;
  }

  std::ostringstream oss;
  msg.StartDump(oss);
  LOG_DEBUG2("%s", oss.str().data());
}
void UserLogic::OnQueryHoldingStock(DictionaryValue& dict) {
  QueryHoldingStocksReq msg;
  if (!msg.StartDeserialize(dict)) {
    return ;
  }

  std::ostringstream oss;
  msg.StartDump(oss);
  LOG_DEBUG2("%s", oss.str().data());
}
void UserLogic::OnQueryTodayOrder(DictionaryValue& dict) {
  QueryTodayOrdersReq msg;
  if (!msg.StartDeserialize(dict)) {
    return ;
  }

  std::ostringstream oss;
  msg.StartDump(oss);
  LOG_DEBUG2("%s", oss.str().data());
}
void UserLogic::OnQueryTodayFinishedOrder(DictionaryValue& dict) {
  QueryTodayFinishedOrdersReq msg;
  if (!msg.StartDeserialize(dict)) {
    return ;
  }

  std::ostringstream oss;
  msg.StartDump(oss);
  LOG_DEBUG2("%s", oss.str().data());
}
void UserLogic::OnQueryHistoryFinishedOrder(DictionaryValue& dict) {
  QueryHistoryFinishedOrdersReq msg;
  if (!msg.StartDeserialize(dict)) {
    return ;
  }

  std::ostringstream oss;
  msg.StartDump(oss);
  LOG_DEBUG2("%s", oss.str().data());
}
void UserLogic::OnQueryStatement(DictionaryValue& dict) {
  QueryStatementReq msg;
  if (!msg.StartDeserialize(dict)) {
    return ;
  }

  std::ostringstream oss;
  msg.StartDump(oss);
  LOG_DEBUG2("%s", oss.str().data());
}
void UserLogic::OnSubmitOrder(DictionaryValue& dict) {
  SubmitOrderReq msg;
  if (!msg.StartDeserialize(dict)) {
    return ;
  }

  std::ostringstream oss;
  msg.StartDump(oss);
  LOG_DEBUG2("%s", oss.str().data());
}

} /* namespace strade_user */
