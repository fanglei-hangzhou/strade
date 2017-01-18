//
// Created by Harvey on 2017/1/7.
//

#include "test_share_logic.h"
#include "logic/logic_unit.h"

void TestShareLogic::Init() {
  strade_share::SSEngine* (* ss_engine)(void);
  std::string strade_share_library = "./strade_share/strade_share.so";
  std::string strade_share_func = "GetStradeShareEngine";
  ss_engine = (strade_share::SSEngine* (*)(void))
      base_logic::SomeUtils::GetLibraryFunction(
          strade_share_library, strade_share_func);
  ssEngine_ = (*ss_engine)();
  if (ssEngine_ == NULL)
    assert(0);
  ssEngine_->Init();
}

bool TestShareLogic::OnIniTimer(struct server* srv) {
  return false;
}

bool TestShareLogic::OnTimeout(struct server* srv, char* id, int opcode, int time) {
  return false;
}
