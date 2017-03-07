//Copyright (c) 2016-2017 The strade Authors. All rights reserved.
//Created on: 2017/01/09 Author: tangtao

#ifndef _STRADE_SRC_PLUGINS_YIELDS_YIELDS_PACK_H_
#define _STRADE_SRC_PLUGINS_YIELDS_YIELDS_PACK_H_

#include <string>
#include <vector>
#include <bitset>

#include "net/http_data.h"
#include "yields/yields_info.h"

namespace yields {

class SendYieldsPack : public SendPacketBase {
 public:
  SendYieldsPack();
  virtual ~SendYieldsPack() {}
  virtual NetBase* release();
  void AddYieldsInfo(NetBase *nb);
  void GroupAccountInfoToSendPack(GroupAccountInfo* group_account_info_pt);
  void YieldsHistoryToSendPack(std::vector<YieldsHistoryInfo*> *vec_yields_history_pt);

 protected:
  void Init();

 private:
   scoped_ptr<base_logic::ListValue> yields_list_pt_;
};

}  //namespace yields


#endif //_STRADE_SRC_PLUGINS_YIELDS_YIELDS_PACK_H_
