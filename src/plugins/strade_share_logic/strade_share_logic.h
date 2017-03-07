//
// Created by Harvey on 2017/1/19.
//

#ifndef STRADE_STRADE_SHARE_LOGIC_H
#define STRADE_STRADE_SHARE_LOGIC_H

#include "core/common.h"

#include "strade_share_timer.h"

namespace strade_share_logic {

class StradeShareLogic {
 private:
  StradeShareLogic();

 public:
  static StradeShareLogic* GetInstance();
  void SavePluginPtr(struct plugin* plugin);

  bool OnIniTimer(struct server* srv);
  bool OnTimeout(struct server* srv, char* id, int opcode, int time);

 private:
  struct plugin* plugin_;
  StradeShareTimer* timer_handle_;

};

} /* namespace strade_share_logic */

#endif //STRADE_STRADE_SHARE_LOGIC_H
