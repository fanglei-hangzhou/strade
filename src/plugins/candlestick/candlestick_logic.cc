// author: pangh
// date: Mon Jun 20 16:50:31 2016

#include "candlestick_logic.h"
#include "core/common.h"
#include "base/logic/logic_comm.h"
#include "base/logic/base_values.h"
#include "public/config/config.h"

#include "logic/logic_unit.h"
#include "net/operator_code.h"
#include "candlestick/candlestick_pro.h"
#include "candlestick/src/candlestick_singlestock.h"
#include "candlestick/src/candlestick_compositeindex.h"
#include "strade_share/strade_share_engine.h"
#include <sstream>

#define DEFAULT_CONFIG_PATH                    "./plugins/candlestick/candlestick_config.xml"
#define TIME_CANDLESTICK_UPDATE_ALL                          10000

namespace candlestick {

CandleStickLogic* CandleStickLogic::instance_ = NULL;

CandleStickLogic::CandleStickLogic() {
  if (!Init())
    assert(0);
}

CandleStickLogic::~CandleStickLogic() {
}

bool CandleStickLogic::Init() {
  return true;
}

CandleStickLogic* CandleStickLogic::GetInstance() {
  if (instance_ == NULL)
    instance_ = new CandleStickLogic();
  return instance_;
}

void CandleStickLogic::FreeInstance() {
  delete instance_;
  instance_ = NULL;
}

bool CandleStickLogic::OnCandleStickConnect(struct server *srv,
                                            const int socket) {
  return true;
}

bool CandleStickLogic::OnCandleStickMessage(struct server *srv,
                                            const int socket, const void *msg,
                                            const int len) {
  bool r = true;
  do {
    const char* packet = reinterpret_cast<const char*>(msg);
    if (NULL == packet) {
      r = false;
      break;
    }
    std::string http_str(packet, len);
    std::string error_str;
    int error_code = 0;
    scoped_ptr < base_logic::ValueSerializer
        > serializer(
            base_logic::ValueSerializer::Create(base_logic::IMPL_HTTP,
                                                &http_str));
    NetBase* value = (NetBase*) (serializer.get()->Deserialize(&error_code,
                                                               &error_str));
    LOG_DEBUG2("http_str:%s", http_str.c_str());
    if (NULL == value) {
      error_code = STRUCT_ERROR;
      // send_error(error_code, socket);
      r = true;
      break;
    }
    scoped_ptr < RecvPacketBase > head_packet(new RecvPacketBase(value));
    int32 type = head_packet->GetType();
    switch (type) {
      case CANDLESTICK_HISTORY: {
        r = OnSingleStockRecords(srv, socket, value, msg, len);
        break;
      }
      default: {
        r = false;
        break;
      }
    }
  } while (0);
  return r;
}

bool CandleStickLogic::OnCandleStickClose(struct server *srv,
                                          const int socket) {
  return true;
}

bool CandleStickLogic::OnBroadcastConnect(struct server *srv, const int socket,
                                          const void *msg, const int len) {
  return true;
}

bool CandleStickLogic::OnBroadcastMessage(struct server *srv, const int socket,
                                          const void *msg, const int len) {
  return true;
}

bool CandleStickLogic::OnBroadcastClose(struct server *srv, const int socket) {
  return true;
}

bool CandleStickLogic::OnIniTimer(struct server *srv) {
  if (srv->add_time_task != NULL) {
    srv->add_time_task(srv, "candlestick", TIME_CANDLESTICK_UPDATE_ALL, 600,
                       -1);
  }
  return true;
}

bool CandleStickLogic::OnTimeout(struct server *srv, char *id, int opcode,
                                 int time) {
  if (opcode == TIME_CANDLESTICK_UPDATE_ALL) {
    LOG_MSG("Update candlestick");
  }
  return true;
}

bool CandleStickLogic::GetStockData(
    const strade_share::STOCK_HIST_MAP &share_map,
    STOCK_HISTORY_MAP *history_map) {
  return true;
}

bool CandleStickLogic::OnSingleStockRecords(struct server *srv,
                                            const int socket, NetBase* netbase,
                                            const void* msg, const int len) {
  bool r = true;
  int err = 0;
  std::string stock_code;
  int64 record_type;
  r = netbase->GetString(L"stock_code", &stock_code);
  if (!r) {
    err = -1;
    LOG_ERROR("OnSingleCandle error");
    // send_error(err, socket, 0, "");
  }
  stock_code = stock_code.substr(0, 6);
  LOG_MSG2("stockcodes %s\n", stock_code.c_str());
  r = netbase->GetBigInteger(L"record_type", &record_type);
  if (!r || record_type > 2 || record_type < 0) {
    err = -1;
    LOG_ERROR("OnSingleCandle error");
    // send_error(err, socket, 0, "");
  }
  SendCandleStickHistoryProtocol candle_pro;
  std::list<SingleStockInfo> ll;
  STOCK_HISTORY_MAP history_map;
  strade_share::STOCK_HIST_MAP hist_map = GetStradeShareEngine()
      ->GetStockHistMapByCodeCopy(stock_code);
  GetStockData(hist_map, &history_map);
  StockHistoryType type = static_cast<StockHistoryType>(record_type);
  SingleStockCandleStickData::GetInstance()->LoadSingleStockHisData(
      stock_code, type, history_map);
  SingleStockCandleStickData::GetInstance()->GetSingleStockHisData(stock_code,
                                                                   type, &ll);
  candle_pro.set_history_info(ll);
  std::string json = candle_pro.GetJson();
  base_logic::LogicComm::SendFull(socket, json.c_str(), json.length());
  return r;
}

}  // candlestick

