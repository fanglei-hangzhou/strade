// author: panghao
// date: Mon Jun 20 16:50:31 2016

#ifndef __PLUGIN__CANDLESTICK_LOGIC___
#define __PLUGIN__CANDLESTICK_LOGIC___
#include <string>
#include "core/common.h"
#include "net/http_data.h"
#include "candlestick/src/candlestick_data.h"
#include "strade_share/strade_share_engine.h"
#include "logic/strade_basic_info.h"
namespace candlestick {
class CandleStickLogic {
 public:
  CandleStickLogic();
  virtual ~CandleStickLogic();
 private:
  static CandleStickLogic *instance_;
 public:
  static CandleStickLogic *GetInstance();
  static void FreeInstance();
 public:
  bool OnCandleStickConnect(struct server *srv, const int socket);
  bool OnCandleStickMessage(struct server *srv, const int socket,
                            const void *msg, const int len);
  bool OnCandleStickClose(struct server *srv, const int socket);
  bool OnBroadcastConnect(struct server *srv, const int socket,
                          const void *data, const int len);
  bool OnBroadcastMessage(struct server *srv, const int socket, const void *msg,
                          const int len);
  bool OnBroadcastClose(struct server *srv, const int socket);
  bool OnIniTimer(struct server *srv);
  bool OnTimeout(struct server *srv, char* id, int opcode, int time);
 protected:
  bool OnRequestCandleStick(NetBase *kv, struct server *srv, const int socket,
                            const void *msg, const int len);
 private:
  bool Init();
  bool OnSingleStockRecords(struct server *srv, const int socket,
                            NetBase* netbase, const void* msg, const int len);
  bool GetStockData(const strade_share::STOCK_HIST_MAP &share_map, STOCK_HISTORY_MAP *history_map);

  bool CheckMonthBegin(std::string date);

  bool CheckWeekBegin(std::string date);
  
  void MonthBeginData(STOCK_HISTORY_MAP *history_map);
  
  void WeekBeginData(STOCK_HISTORY_MAP *history_map);
};

}  // namespace candlestock

#endif // __PLUGIN__CANDLESTICK_LOGIC___

