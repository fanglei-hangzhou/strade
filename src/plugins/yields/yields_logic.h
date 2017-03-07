//Copyright (c) 2016-2017 The strade Authors. All rights reserved.
//Created on: 2017/01/09 Author: tangtao

#ifndef _STRADE_SRC_PLUGINS_YIELDS_YIELDS_LOGIC_H_
#define _STRADE_SRC_PLUGINS_YIELDS_YIELDS_LOGIC_H_

#include "net/http_data.h"
#include "net/packet_process.h"
#include "core/common.h"

#include "yields/db_mysql.h"

#define DEFAULT_CONFIG_PATH     "./plugins/yields/yields_config.xml"

namespace yields {

class YieldsLogic {
 public:
  virtual ~YieldsLogic();

 private:
  YieldsLogic();

  class crelease {
    ~crelease() {
      if (instance_ != NULL) {
        delete instance_;
        instance_ = NULL;
      }
    }
};

  static crelease release_;
  static YieldsLogic *instance_;

 public:
  static YieldsLogic *GetInstance() {
    if (NULL == instance_) {
      instance_ = new YieldsLogic();
    }
    return instance_;
  }

 public:
  bool OnYieldsConnect(struct server *srv, const int socket);

  bool OnYieldsMessage(struct server *srv,  \
                          const int socket, \
                           const void *msg, \
                            const int len);

  bool OnYieldsClose(struct server *srv, const int socket);

  bool OnBroadcastConnect(struct server *srv, \
                            const int socket, \
                            const void *data, \
                              const int len);

  bool OnBroadcastMessage(struct server *srv, \
                            const int socket, \
                             const void *msg, \
                              const int len);

  bool OnBroadcastClose(struct server *srv, const int socket);

  bool OnIniTimer(struct server *srv);

  bool OnTimeout(struct server *srv, char *id, int opcode, int time);

  bool OnRequestGroupAccountInfo(struct server *srv,     \
                                 const int socket,       \
                                 NetBase *netbase,       \
                                 const void *msg = NULL, \
                                 const int len = 0);

  bool OnRequestYieldsHistory(struct server *srv,     \
                              const int socket,       \
                              NetBase *netbase,       \
                              const void *msg = NULL, \
                              const int len = 0);

  bool OnUpdateYields();

 private:
  bool Init();
  //DbMysql* db_mysql_;
  strade_share::SSEngine* ss_engine_;
};

}     //namespace yields

#endif   //_STRADE_SRC_PLUGINS_YIELDS_YIELDS_LOGIC_H_
