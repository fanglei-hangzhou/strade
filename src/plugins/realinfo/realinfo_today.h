#ifndef SRC_REALINFO_REALINFO_TODAY_H
#define SRC_REALINFO_REALINFO_TODAY_H
#include "realinfo_pro.h"

namespace realinfo {

class RealInfoToday {
 public:
  static time_t GetTradeStart() {
    struct tm start;
    time_t ts = time(NULL);
    localtime_r(&ts, &start);
    start.tm_hour = 9;
    start.tm_min = 30;
    start.tm_sec = 0;
    return mktime(&start);
  }  

  static std::string BuildDate(time_t ts) {
    struct tm start;
    localtime_r(&ts, &start);
    char buf[64];
    snprintf(buf, sizeof(buf), "%02d:%02d", start.tm_hour, start.tm_min);
    return buf;
  }
  
  static bool RemoveDuplicate(std::list<StockRealInfo> *today) {
  
    time_t start = GetTradeStart();
    std::list<StockRealInfo>::iterator it = today->begin();
    std::string old_time;
    while (it != today->end()) {
      if (it->ts < start) {
        LOG_MSG2("Remove %u", it->ts);
        today->erase(it++);
        continue;
      }
      if (it->time == old_time) { 
        LOG_MSG2("Remove %u", it->ts);
        today->erase(it++);
      } else {
        old_time = it->time;
        it++;
      }
    }
    return true;
  }

};

} // namespace
#endif
