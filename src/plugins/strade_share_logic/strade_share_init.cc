//
// Created by Harvey on 2017/1/19.
//

#include "strade_share_init.h"

#include "core/common.h"
#include "core/plugins.h"
#include "logic/logic_comm.h"

#include "strade_share_logic/strade_share_logic.h"

#include <signal.h>

using strade_share_logic::StradeShareLogic;

typedef struct strade_share_plugin {
  char* id;
  char* name;
  char* version;
  char* provider;
} StradeSharePlugin;

static void* OnTestShareInit() {
  signal(SIGPIPE, SIG_IGN);
  StradeSharePlugin* strade_share_plugin =
      (StradeSharePlugin*) calloc(1, sizeof(StradeSharePlugin));
  if (!strade_share_plugin) {
    LOG_ERROR2("OnHostSuggestStart::calloc error, errno = %s", strerror(errno));
    assert(0);
  }
  strade_share_plugin->id = "stradeshare_logic";
  strade_share_plugin->name = "stradeshare_logic";
  strade_share_plugin->version = "1.0.0";
  strade_share_plugin->provider = "harvey";
  if (!StradeShareLogic::GetInstance()) {
    assert(0);
  }
  return strade_share_plugin;
}

static handler_t OnIniTimer(struct server* srv) {
  StradeShareLogic::GetInstance()->OnIniTimer(srv);
  return HANDLER_GO_ON;
}

static handler_t OnTimeOut(struct server* srv, char* id, int opcode, int time) {
  StradeShareLogic::GetInstance()->OnTimeout(srv, id, opcode, time);
  return HANDLER_GO_ON;
}

static handler_t ThreadFun(struct server* srv, int fd, void* data) {

  return HANDLER_GO_ON;
}

int stradeshare_logic_plugin_init(struct plugin* pl) {
  pl->init = OnTestShareInit;
  pl->clean_up = NULL;
  pl->connection = NULL;
  pl->connection_close = NULL;
  pl->connection_close_srv = NULL;
  pl->connection_srv = NULL;
  pl->handler_init_time = OnIniTimer;
  pl->handler_read = NULL;
  pl->handler_read_srv = NULL;
  pl->handler_read_other = NULL;
  pl->time_msg = OnTimeOut;
  pl->thread_func = ThreadFun;
  pl->data = NULL;
  StradeShareLogic::GetInstance()->SavePluginPtr(pl);
  return 0;
}