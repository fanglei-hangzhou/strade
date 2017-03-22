//  Copyright (c) 2015-2015 The KID Authors. All rights reserved.
//  Created on: 2016/10/13 Author: zjc

#include "../realtime/stock_init.h"

#include "../realtime/stock_logic.h"
#include "core/common.h"
#include "core/plugins.h"

struct stockplugin {
  char* id;
  char* name;
  char* version;
  char* provider;
};

static void *OnStockStart() {
  signal(SIGPIPE, SIG_IGN);
  struct stockplugin* stock =
      (struct stockplugin*) calloc(1, sizeof(struct stockplugin));
  stock->id = "stock";
  stock->name = "stock";
  stock->version = "1.0.0";
  stock->provider = "zjc";
  if (!stock_logic::StockLogic::GetInstance())
    assert(0);
  return stock;
}

static handler_t OnStockShutdown(struct server* srv, void* pd) {
  stock_logic::StockLogic::FreeInstance();

  return HANDLER_GO_ON;
}

static handler_t OnStockConnect(struct server *srv, int fd, void *data,
                                int len) {
  stock_logic::StockLogic::GetInstance()->OnStockConnect(srv, fd);
  return HANDLER_GO_ON;
}

static handler_t OnStockMessage(struct server *srv, int fd, void *data,
                                int len) {
  stock_logic::StockLogic::GetInstance()->OnStockMessage(srv, fd, data, len);
  return HANDLER_GO_ON;
}

static handler_t OnStockClose(struct server *srv, int fd) {
  stock_logic::StockLogic::GetInstance()->OnStockClose(srv, fd);
  return HANDLER_GO_ON;
}

static handler_t OnUnknow(struct server *srv, int fd, void *data, int len) {
  return HANDLER_GO_ON;
}

static handler_t OnBroadcastConnect(struct server* srv, int fd, void *data,
                                    int len) {
  stock_logic::StockLogic::GetInstance()->OnBroadcastConnect(srv, fd, data,
                                                             len);
  return HANDLER_GO_ON;
}

static handler_t OnBroadcastClose(struct server* srv, int fd) {
  stock_logic::StockLogic::GetInstance()->OnBroadcastClose(srv, fd);
  return HANDLER_GO_ON;
}

static handler_t OnBroadcastMessage(struct server* srv, int fd, void *data,
                                    int len) {
  stock_logic::StockLogic::GetInstance()->OnBroadcastMessage(srv, fd, data,
                                                             len);
  return HANDLER_GO_ON;
}

static handler_t OnIniTimer(struct server* srv) {
  stock_logic::StockLogic::GetInstance()->OnIniTimer(srv);
  return HANDLER_GO_ON;
}

static handler_t OnTimeOut(struct server* srv, char* id, int opcode, int time) {
  stock_logic::StockLogic::GetInstance()->OnTimeout(srv, id, opcode, time);
  return HANDLER_GO_ON;
}

int stock_plugin_init(struct plugin *pl) {
  pl->init = OnStockStart;
  pl->clean_up = OnStockShutdown;
  pl->connection = OnStockConnect;
  pl->connection_close = OnStockClose;
  pl->connection_close_srv = OnBroadcastClose;
  pl->connection_srv = OnBroadcastConnect;
  pl->handler_init_time = OnIniTimer;
  pl->handler_read = OnStockMessage;
  pl->handler_read_srv = OnBroadcastMessage;
  pl->handler_read_other = OnUnknow;
  pl->time_msg = OnTimeOut;
  pl->data = NULL;
  return 0;
}


