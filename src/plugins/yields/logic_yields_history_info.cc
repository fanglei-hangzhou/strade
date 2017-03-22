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

bool YieldsLogic::OnRequestYieldsHistory(struct server *srv,  \
                                         const int socket,    \
                                         NetBase *netbase,    \
                                         const void *msg,     \
                                         const int len) {
  RecvLoginBase recv_pack(netbase);
  uint32 user_id = recv_pack.user_id();

  std::vector<YieldsHistoryInfo*> vec_yields_history_info;
  DbMysql mysql;
  
  bool r = mysql.RequestYieldsHistory(user_id, ss_engine_, &vec_yields_history_info);
  if (!r) {
    return false;
  }
  
  SendYieldsPack send_packet;
  send_packet.YieldsHistoryToSendPack(&vec_yields_history_info);
  std::string json;
  send_packet.GetJsonSerialize(&json);
  send_message_by_size(socket, json);

  return true;
}

}     // namespace yields
