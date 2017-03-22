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

#define SEND_HTTP_ERROR(b) \  
do {\
  std::string response = b;\
  base_logic::LogicUnit::SendMessageBySize(socket, response);\
}while(0)

#define SEND_ERROR_INFO(c, i) \  
do {\
  SendCandleStickHistoryProtocol error_pro;\
  error_pro.SetErrorState(c, i);\
  std::string response = error_pro.GetJson();\
  base_logic::LogicUnit::SendMessageBySize(socket, response);\
}while(0)

#define SEND_HTTP_INFO(b) \  
do {\
  std::string response = b;\
  base_logic::LogicUnit::SendMessageBySize(socket, response);\
}while(0)

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
      SEND_ERROR_INFO(error_code, "struture error");
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
  strade_share::STOCK_HIST_MAP::const_iterator it = share_map.begin();
  while (it != share_map.end()) {
    const strade_logic::StockHistInfo &info = it->second; 
    StockHistoryInfo his_info(info.GetHistDate(), info.high(), info.low(),
        info.open(), info.close(), info.volume());
    (*history_map)[it->first] = his_info;
    it++;
  }
  return true;
}

void CandleStickLogic::MonthBeginData(STOCK_HISTORY_MAP *history_map) {
  std::map<std::string, StockHistoryInfo>::iterator it = history_map->begin();
  while (it != history_map->end()) {
    if (CheckMonthBegin(it->first))
      return;
    history_map->erase(it++);
  }
}

void CandleStickLogic::WeekBeginData(STOCK_HISTORY_MAP *history_map) {
  std::map<std::string, StockHistoryInfo>::iterator it = history_map->begin();
  while (it != history_map->end()) {
    if (CheckWeekBegin(it->first))
      return;
    history_map->erase(it++);
  }
}

bool CandleStickLogic::CheckMonthBegin(std::string date) {
  StockDate stock_date;
  bool r = StockCandleStickData::ParseDate(date, &stock_date); 
  if (!r)
    return r;
  return stock_date.day == 1;
}

bool CandleStickLogic::CheckWeekBegin(std::string date) {
  StockDate stock_date;
  bool r = StockCandleStickData::ParseDate(date, &stock_date); 
  if (!r)
    return r;
  return stock_date.week_day == 1;
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
    SEND_ERROR_INFO(-1, "no stock_code");
    return false;
    // send_error(err, socket, 0, "");
  }
  stock_code = stock_code.substr(0, stock_code.length() - 1);
  r = netbase->GetBigInteger(L"record_type", &record_type);
  LOG_MSG2("stockcodes %s record_type %d\n", stock_code.c_str(), record_type);
  if (stock_code.length() < 5) {
    SEND_ERROR_INFO(-50, "stock code error");
    return false;
  }
  if (!r || record_type > 2 || record_type < 0) {
    err = -1;
    LOG_ERROR("OnSingleCandle error");
    SEND_ERROR_INFO(-51, "record_type error");
    return false;
  }
  SendCandleStickHistoryProtocol candle_pro;
  std::list<SingleStockInfo> ll;
  STOCK_HISTORY_MAP history_map;
  LOG_MSG2("Now get %x", time(NULL));
  strade_share::STOCK_HIST_MAP hist_map = GetStradeShareEngine()
      ->GetStockHistMapByCodeCopy(stock_code);
  LOG_MSG2("Get ok  %x", time(NULL));
  if (hist_map.size() == 0) {
    SEND_ERROR_INFO(-52, "hist_map null");
    return false;
  } else {
    LOG_MSG2("history results %d", hist_map.size());
  }
  GetStockData(hist_map, &history_map);
  strade_logic::StockRealInfo  real_info;
  LOG_MSG2("%x", time(NULL));
  if (!GetStradeShareEngine()->
      GetStockCurrRealMarketInfo(stock_code, real_info)) {
    SEND_ERROR_INFO(-53, "real_map find null");
    LOG_MSG2("%s", "Get real info null");
  } else {
    char real_date[64];
    struct tm now;
    time_t current = time(NULL);
    localtime_r(&current, &now);
    snprintf(real_date, sizeof(real_date), "%04d-%02d-%02d", now.tm_year + 1900, now.tm_mon + 1, now.tm_mday);
    std::string date = real_date;
    LOG_MSG2("%s", real_date);
    LOG_MSG2("the open is %.3f the priceis %.3f high %.3f low %.3f vol %.2f", real_info.open, real_info.price, real_info.high, real_info.low, real_info.vol);
    if (history_map.find(real_date) == history_map.end()) {
       StockHistoryInfo today(real_date, real_info.high, real_info.low,        
           real_info.open, real_info.price, real_info.vol);                     
       LOG_MSG2("%s", "Add today realinfo");
       history_map[real_date] = today; 
    }

  }
  
  StockHistoryType type = static_cast<StockHistoryType>(record_type);
  if (type == STOCK_HISTORY_MONTH) {
    MonthBeginData(&history_map);
  } else if (type == STOCK_HISTORY_WEEK) {
    WeekBeginData(&history_map);
  } 
  LOG_MSG2("%x", time(NULL));
  SingleStockCandleStickData::GetInstance()->LoadSingleStockHisData(
      stock_code, type, history_map);
   
  LOG_MSG2("%x", time(NULL));
  SingleStockCandleStickData::GetInstance()->GetSingleStockHisData(stock_code,
                                                                   type, 
                                                                   &ll);

  LOG_MSG2("%x", time(NULL));
  candle_pro.set_history_info(ll);
  std::string json = candle_pro.GetJson();
  LOG_MSG2("%x", time(NULL));
  SEND_HTTP_INFO(json);
  LOG_MSG2("%x", time(NULL));
  return r;
}

}  // candlestick

