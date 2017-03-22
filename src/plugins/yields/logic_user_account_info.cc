//Copyright (c) 2016-2017 The strade Authors. All rights reserved.
//Created on: 2017/01/09 Author: tangtao

#include "yields/yields_logic.h"

#include <sstream> 

#include "core/common.h"
#include "base/logic/logic_comm.h"
#include "base/logic/base_values.h"
#include "public/config/config.h"
#include "logic/logic_unit.h"
#include "net/http_data.h"
#include "yields/yields_pack.h"
#include "yields/yields_info.h"
#include "yields/db_mysql.h"

namespace yields {

bool YieldsLogic::OnRequestGroupAccountInfo(struct server *srv,  \
                                           const int socket,    \
                                           NetBase *netbase,    \
                                           const void *msg,     \
                                           const int len) {
  //get user id
  RecvLoginBase recv_pack(netbase);
  uint32 user_id = recv_pack.user_id();

  //get group id
  std::string temp;
  bool r = netbase->GetString(L"group_id", &temp);
  std::stringstream ss;
  ss << temp;
  uint32 group_id;
  ss >> group_id;
  if (!r || 0 == group_id) {
    LOG_ERROR("OnRequestGroupAccountInfo error, can not get group_id");
    return false;
  }

  GroupAccountInfo group_account_info;
  
  r = group_account_info.fetch_group_account_info(ss_engine_, user_id, group_id);
  if (!r) {
    return false;
  }

  SendYieldsPack send_packet;
  send_packet.GroupAccountInfoToSendPack(&group_account_info);
  std::string json;
  send_packet.GetJsonSerialize(&json);
  send_message_by_size(socket, json);

  return true;
}

}     // namespace yields
