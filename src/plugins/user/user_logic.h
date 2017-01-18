//  Copyright (c) 2015-2015 The KID Authors. All rights reserved.
//  Created on: 2017/1/9/ Author: zjc

#ifndef SRC_PLUGINS_USER_USER_LOGIC_H_
#define SRC_PLUGINS_USER_USER_LOGIC_H_

#include "core/common.h"

namespace base_logic {
class Value;
class ListValue;
class DictionaryValue;
}

using base_logic::Value;
using base_logic::ListValue;
using base_logic::DictionaryValue;

namespace strade_user {

class UserLogic {
 public:
  UserLogic();
  virtual ~UserLogic();

 private:
  static UserLogic *instance_;

 public:
  static UserLogic *GetInstance();
  static void FreeInstance();

 public:

  bool OnUserConnect(struct server *srv, const int socket);

  bool OnUserMessage(struct server *srv, const int socket, const void *msg,
                      const int len);

  bool OnUserClose(struct server *srv, const int socket);

  bool OnBroadcastConnect(struct server *srv, const int socket,
                          const void *data, const int len);

  bool OnBroadcastMessage(struct server *srv, const int socket, const void *msg,
                          const int len);

  bool OnBroadcastClose(struct server *srv, const int socket);

  bool OnIniTimer(struct server *srv);

  bool OnTimeout(struct server *srv, char* id, int opcode, int time);
 private:
  DictionaryValue* HttpDeserialize(const char* msg, int len);

  void OnCreateGroup(DictionaryValue& dict);
  void OnAddStock(DictionaryValue& dict);
  void OnDelStock(DictionaryValue& dict);
  void OnQueryGroup(DictionaryValue& dict);
  void OnQueryStock(DictionaryValue& dict);
  void OnQueryHoldingStock(DictionaryValue& dict);
  void OnQueryTodayOrder(DictionaryValue& dict);
  void OnQueryTodayFinishedOrder(DictionaryValue& dict);
  void OnQueryHistoryFinishedOrder(DictionaryValue& dict);
  void OnQueryStatement(DictionaryValue& dict);
  void OnSubmitOrder(DictionaryValue& dict);
};

} /* namespace strade_user */

#endif /* SRC_PLUGINS_USER_USER_LOGIC_H_ */
