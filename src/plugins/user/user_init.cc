//  Copyright (c) 2015-2015 The KID Authors. All rights reserved.
//  Created on: 2017/1/9/ Author: zjc

#include "user_init.h"

#include "user_logic.h"

#include "core/common.h"
#include "core/plugins.h"

struct userplugin {
  char* id;
  char* name;
  char* version;
  char* provider;
};

static void *OnUserStart() {
  signal(SIGPIPE, SIG_IGN);
  struct userplugin* user =
      (struct userplugin*) calloc(1, sizeof(struct userplugin));
  user->id = "user";
  user->name = "user";
  user->version = "1.0.0";
  user->provider = "zjc";
  if (!strade_user::UserLogic::GetInstance())
    assert(0);
  return user;
}

static handler_t OnUserShutdown(struct server* srv, void* pd) {
  strade_user::UserLogic::FreeInstance();

  return HANDLER_GO_ON;
}

static handler_t OnUserConnect(struct server *srv, int fd, void *data,
                                int len) {
  strade_user::UserLogic::GetInstance()->OnUserConnect(srv, fd);
  return HANDLER_GO_ON;
}

static handler_t OnUserMessage(struct server *srv, int fd, void *data,
                                int len) {
  strade_user::UserLogic::GetInstance()->OnUserMessage(srv, fd, data, len);
  return HANDLER_GO_ON;
}

static handler_t OnUserClose(struct server *srv, int fd) {
  strade_user::UserLogic::GetInstance()->OnUserClose(srv, fd);
  return HANDLER_GO_ON;
}

static handler_t OnUnknow(struct server *srv, int fd, void *data, int len) {
  return HANDLER_GO_ON;
}

static handler_t OnBroadcastConnect(struct server* srv, int fd, void *data,
                                    int len) {
  strade_user::UserLogic::GetInstance()->OnBroadcastConnect(srv, fd, data,
                                                             len);
  return HANDLER_GO_ON;
}

static handler_t OnBroadcastClose(struct server* srv, int fd) {
  strade_user::UserLogic::GetInstance()->OnBroadcastClose(srv, fd);
  return HANDLER_GO_ON;
}

static handler_t OnBroadcastMessage(struct server* srv, int fd, void *data,
                                    int len) {
  strade_user::UserLogic::GetInstance()->OnBroadcastMessage(srv, fd, data,
                                                             len);
  return HANDLER_GO_ON;
}

static handler_t OnIniTimer(struct server* srv) {
  strade_user::UserLogic::GetInstance()->OnIniTimer(srv);
  return HANDLER_GO_ON;
}

static handler_t OnTimeOut(struct server* srv, char* id, int opcode, int time) {
  strade_user::UserLogic::GetInstance()->OnTimeout(srv, id, opcode, time);
  return HANDLER_GO_ON;
}

int user_plugin_init(struct plugin *pl) {
  pl->init = OnUserStart;
  pl->clean_up = OnUserShutdown;
  pl->connection = OnUserConnect;
  pl->connection_close = OnUserClose;
  pl->connection_close_srv = OnBroadcastClose;
  pl->connection_srv = OnBroadcastConnect;
  pl->handler_init_time = OnIniTimer;
  pl->handler_read = OnUserMessage;
  pl->handler_read_srv = OnBroadcastMessage;
  pl->handler_read_other = OnUnknow;
  pl->time_msg = OnTimeOut;
  pl->data = NULL;
  return 0;
}


