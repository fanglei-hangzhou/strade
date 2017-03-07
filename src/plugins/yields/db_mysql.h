//Copyright (c) 2016-2017 The strade Authors. All rights reserved.
//Created on: 2017/01/09 Author: tangtao

#ifndef _STRADE_SRC_PLUGINS_YIELDS_DB_MYSQL_H_
#define _STRADE_SRC_PLUGINS_YIELDS_DB_MYSQL_H_

#include <list>
#include <string>
#include <vector>
#include <map>

#include "public/basic/basic_info.h"
#include "base/db/base_db_mysql_auto.h"
//#include "baseconfig/base_config.h" 
//#include "sqldbpool/mysql_db_pool.h"
#include "strade_share/strade_share_engine.h"

#include "yields/yields_info.h"

namespace yields {

class DbMysql {
public:
  DbMysql();
  ~DbMysql();

  void Init(strade_share::SSEngine* ss_engine, std::list<base::ConnAddr>* const addrlist);
  void Dest();

  bool RequestYieldsHistory(const uint32 user_id, strade_share::SSEngine* ss_engine, std::vector<YieldsHistoryInfo*> *vec_yields_history_info_pt);

  //request_id is user_id or group_id, 0 on behalf of user and 1 on behalf of group
  bool RequestTodayDealRecord(const uint32 group_id, strade_share::SSEngine* ss_engine, double& profit_or_loss_for_deal, double& stock_cost_for_deal);

 private:
  base_storage::DBStorageEngine* GetEngine();

private:
  int error_info_;
  base_storage::DBStorageEngine *engine_;
  //strade_share::SSEngine* ss_engine_;
};

}    // namespace yields

#endif    // _STRADE_SRC_PLUGINS_YIELDS_DB_MYSQL_H_
