//  Copyright (c) 2015-2015 The KID Authors. All rights reserved.
//  Created on: 2017/1/12 Author: zjc

#ifndef SRC_PUB_LOGIC_USER_ENGINE_H_
#define SRC_PUB_LOGIC_USER_ENGINE_H_

#include "macros.h"
#include "user_info.h"

namespace strade_user {

class UserEngine {
 public:
  ~UserEngine();
  static UserEngine* GetUserEngine() {
    static UserEngine engine;
    return &engine;
  }
 public:
  bool Init();
  void OnCloseMarket();
  bool exist(UserId id) const { return user_id_map_.count(id); }
  UserInfo* GetUser(UserId id);
  UserIdMap GetUserMap() const { return user_id_map_; }
 private:
  UserEngine();
  DISALLOW_COPY_AND_ASSIGN(UserEngine);
 private:
  bool initialized_;
  UserIdMap user_id_map_;
};

} /* namespace strade_user */

#endif /* SRC_PUB_LOGIC_USER_ENGINE_H_ */
