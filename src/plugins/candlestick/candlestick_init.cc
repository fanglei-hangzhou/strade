// author: tianyiheng
// date: Mon Jun 20 16:50:31 2016

#include "candlestick_init.h"
#include <signal.h>
#include <stdlib.h>
#include "core/common.h"
#include "core/plugins.h"
#include "candlestick/candlestick_logic.h"

struct Candleplugin {
  char* id;
  char* name;
  char* version;
  char* provider;
};

static void *OnCandleStickStart() {
  signal(SIGPIPE, SIG_IGN);
  struct Candleplugin* candlestick = (struct Candleplugin*) calloc(
      1, sizeof(struct Candleplugin));
  candlestick->id = "candlestick";
  candlestick->name = "candlestick";
  candlestick->version = "1.0.0";
  candlestick->provider = "pangh";

  candlestick::CandleStickLogic::GetInstance();
  return candlestick;
}

static handler_t OnCandleStickShutdown(struct server* srv, void* pd) {
  return HANDLER_GO_ON;
}

static handler_t OnCandleStickConnect(struct server *srv, int fd, void *data,
                                      int len) {
  return HANDLER_GO_ON;
}

static handler_t OnCandleStickMessage(struct server *srv, int fd, void *data,
                                      int len) {
  if (candlestick::CandleStickLogic::GetInstance()->OnCandleStickMessage(srv,
                                                                         fd,
                                                                         data,
                                                                         len)) {
    return HANDLER_FINISHED;
  }
  return HANDLER_GO_ON;
}

static handler_t OnCandleStickClose(struct server *srv, int fd) {

  return HANDLER_GO_ON;
}

static handler_t OnUnknow(struct server *srv, int fd, void *data, int len) {
  return HANDLER_GO_ON;
}

static handler_t OnBroadcastConnect(struct server* srv, int fd, void *data,
                                    int len) {

  return HANDLER_GO_ON;
}

static handler_t OnBroadcastClose(struct server* srv, int fd) {

  return HANDLER_GO_ON;
}

static handler_t OnBroadcastMessage(struct server* srv, int fd, void *data,
                                    int len) {

  return HANDLER_GO_ON;
}

static handler_t OnIniTimer(struct server* srv) {
  candlestick::CandleStickLogic::GetInstance()->OnIniTimer(srv);
  return HANDLER_GO_ON;
}

static handler_t OnTimeOut(struct server* srv, char* id, int opcode, int time) {
  candlestick::CandleStickLogic::GetInstance()->OnTimeout(srv, id, opcode,
                                                          time);

  return HANDLER_GO_ON;
}

int candlestick_plugin_init(struct plugin *pl) {
  pl->init = OnCandleStickStart;
  pl->clean_up = OnCandleStickShutdown;
  pl->connection = OnCandleStickConnect;
  pl->connection_close = OnCandleStickClose;
  pl->connection_close_srv = OnBroadcastClose;
  pl->connection_srv = OnBroadcastConnect;
  pl->handler_init_time = OnIniTimer;
  pl->handler_read = OnCandleStickMessage;
  pl->handler_read_srv = OnBroadcastMessage;
  pl->handler_read_other = OnUnknow;
  pl->time_msg = OnTimeOut;
  pl->data = NULL;
  return 0;
}

