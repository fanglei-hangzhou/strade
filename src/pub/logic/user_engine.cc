//  Copyright (c) 2015-2015 The KID Authors. All rights reserved.
//  Created on: 2017/1/12 Author: zjc

#include "user_engine.h"

#include <mysql.h>

#include "logic/logic_comm.h"
#include "strade_share/strade_share_engine.h"

using strade_share::SSEngine;

namespace strade_user {

UserEngine::UserEngine()
    : initialized_(false) {
}

UserEngine::~UserEngine() {
}

bool UserEngine::Init() {
  if (initialized_) {
    return true;
  }
  initialized_ = true;
  SSEngine* engine = GetStradeShareEngine();
  std::vector<UserInfo> users;
  if (!engine->ReadData(UserInfo::kGetAllUserInfoSql, users)) {
    LOG_ERROR("init user info error");
    return false;
  }
  for (size_t i = 0; i < users.size(); ++i) {
    if (!users[i].Init()) {
      continue;
    }
    user_id_map_.insert(UserIdMap::value_type(users[i].id(), users[i]));
  }
  LOG_MSG2("init %d user info", users.size());
  return true;
}

UserInfo* UserEngine::GetUser(UserId id) {
  UserIdMap::iterator it = user_id_map_.find(id);
  if (user_id_map_.end() == it) {
    return NULL;
  }
  return &it->second;
}

void UserEngine::OnCloseMarket() {
  for (UserIdMap::iterator it = user_id_map_.begin();
      user_id_map_.end() != it; ++it) {
    it->second.OnCloseMarket();
  }
}

} /* namespace strade_user */
