//
// Created by Harvey on 2017/1/19.
//

#include "strade_share_logic.h"
#include "strade_share_head.h"

using namespace strade_logic;

namespace strade_share_logic {

StradeShareLogic::StradeShareLogic() {
  timer_handle_ = StradeShareTimer::GetInstance();
}

StradeShareLogic* StradeShareLogic::GetInstance() {
  static StradeShareLogic instance;
  return &instance;
}

void StradeShareLogic::SavePluginPtr(struct plugin* plugin) {
  plugin_ = plugin;
}

bool StradeShareLogic::OnIniTimer(struct server* srv) {
  // 载入股票当前访问量
  srv->add_time_task(srv, "stradeshare_logic", ONTIME_LOAD_STOCK_VISIT_TIME, 5, -1);
  srv->add_time_task(srv, "stradeshare_logic", ONTIME_TEST_TIMER, 1, -1);
  return true;
}

bool StradeShareLogic::OnTimeout(
    struct server* srv, char* id, int opcode, int time) {
  switch (opcode) {
    case ONTIME_LOAD_STOCK_VISIT_TIME: {
      timer_handle_->OnTimeLoadStockVisit();
      break;
    }
    case ONTIME_TEST_TIMER:{
      timer_handle_->OnTimeTest();
      break;
    }
    default: {
      break;
    }
  }
  return true;
}


} /* namespace strade_share_logic */