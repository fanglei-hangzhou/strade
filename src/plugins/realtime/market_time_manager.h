//  Copyright (c) 2015-2015 The KID Authors. All rights reserved.
//  Created on: 2016年10月13日 Author: zjc

#ifndef SRC_PLUGINS_REALTIME_MARKET_TIME_MANAGER_H_
#define SRC_PLUGINS_REALTIME_MARKET_TIME_MANAGER_H_

#include <time.h>

#include <map>
#include <bitset>
#include <vector>

#include "logic/stock_util.h"
namespace stock_logic {

// 9:00 ~ 11:30 13:00 ~ 15:30
const static int START_TRADE_TIME_AM = 90000;
const static int END_TRADE_TIME_AM = 113000;
const static int START_TRADE_TIME_PM = 130000;
const static int END_TRADE_TIME_PM = 153000;
const static int SECONDS_PER_MIN = 60;
const static int MINS_PER_HOUR = 60;
const static int HOURS_PER_DAY = 24;
const static int SECONDS_PER_HOUR = 3600;

// 9:00 ~ 15:30
const static int SECONDS_PER_TRADE_DAY = 6 * 60 * 60 + 30 * 60;

class IntTime {
 public:
  // time FORMAT: HHMMSS
  IntTime(int init_time) {
    sec_ = init_time % 100;
    init_time /= 100;
    min_ = init_time % 100;
    hour_ = init_time / 100;
  }

  // prefix operator
  IntTime& operator++() {
    min_ += ++sec_ / SECONDS_PER_MIN;
    sec_ %= SECONDS_PER_MIN;

    hour_ += min_ / MINS_PER_HOUR;
    hour_ %= HOURS_PER_DAY;

    min_ %= MINS_PER_HOUR;
    return *this;
  }

  IntTime& operator--() {
    if (--sec_ < 0) {
      sec_ += SECONDS_PER_MIN;
      if (--min_ < 0) {
        min_ += MINS_PER_HOUR;
        if (--hour_ < 0) {
          hour_ += HOURS_PER_DAY;
        }
      }
    }
    return *this;
  }

  // postfix operator
  IntTime operator++(int) {
    IntTime ret(*this);
    ++*this;
    return ret;
  }

  IntTime operator--(int) {
    IntTime ret(*this);
    --*this;
    return ret;
  }

  IntTime& operator+=(int sec) {
    sec_ += sec;
    min_ += sec_ / SECONDS_PER_MIN;
    sec_ %= SECONDS_PER_MIN;

    hour_ += min_ / MINS_PER_HOUR;
    hour_ %= HOURS_PER_DAY;

    min_ %= MINS_PER_HOUR;
    return *this;
  }

  IntTime& operator-=(int sec) {
    int hour = sec / SECONDS_PER_HOUR;
    sec %= SECONDS_PER_HOUR;
    int min = sec / SECONDS_PER_MIN;
    sec %= SECONDS_PER_MIN;

    sec_ -= sec;
    if (sec_ < 0) {
      --min_;
      sec_ += SECONDS_PER_MIN;
    }

    min_ -= min;
    if (min_ < 0) {
      --hour_;
      min_ += MINS_PER_HOUR;
    }

    hour_ -= hour;
    if (hour_ < 0) {
      hour_ += HOURS_PER_DAY;
    }
    return *this;
  }

  operator int() const {
    int ret = hour_;
    ret = ret * 100 + min_;
    ret = ret * 100 + sec_;
    return ret;
  }

  void hour(int hour) { hour_ = hour; }
  void min(int min) { min_ = min; }
  void sec(int sec) { sec_ = sec; }

  int hour() const { return hour_; }
  int min() const { return min_; }
  int sec() const { return sec_; }

 private:
  int hour_;
  int min_;
  int sec_;
};

IntTime operator+(const IntTime& t1, const IntTime& t2);
IntTime operator-(const IntTime& t1, const IntTime& t2);

class MarketTimeManager {
 public:
  typedef std::map<int, int> MinTimeMap;

  MarketTimeManager(int start_time = 92500);
  ~MarketTimeManager();

  void reset() {
    max_mtime_ = -1;
    next_mtime_ = start_mtime_;
    mtime_set_.reset();
    min_time_map_.clear();
  }

  void advance_time() {
    next_mtime_ += 60;
    int t = next_mtime_ / 100;
    if (t == 1131) {
      next_mtime_.hour(13);
      next_mtime_.min(0);
      next_mtime_.sec(0);
    }
  }

  int market_time() const {
    if (92500 == next_mtime_) {
      return -1;
    }

    if (START_TRADE_TIME_PM == next_mtime_) {
      return END_TRADE_TIME_AM;
    }

    IntTime t(next_mtime_);
    t -= 60;
    return t;
  }

  size_t time_to_pos(int mtime) {
    StockUtil* util = StockUtil::Instance();
    time_t ts1 = util->to_timestamp(START_TRADE_TIME_AM);
    time_t ts2 = util->to_timestamp(mtime);
    return ts2 - ts1;
  }

  int pos_to_time(size_t pos) {
    StockUtil* util = StockUtil::Instance();
    time_t ts = util->to_timestamp(START_TRADE_TIME_AM);
    ts += pos;
    struct tm *pt = localtime(&ts);
    int mtime = pt->tm_hour;
    mtime = mtime * 100 + pt->tm_min;
    mtime = mtime * 100 + pt->tm_sec;
    return mtime;
  }

  bool exist(int mtime) {
    if (mtime < START_TRADE_TIME_AM
        || mtime > END_TRADE_TIME_PM) {
      return false;
    }
    return mtime_set_[time_to_pos(mtime)];
  }

  int max_mtime() const { return max_mtime_; }
  // FORMAT: hhmmss
  void Add(int mtime);
  int NewMarketTime();
  int NearestMarketTime(int min_time);
 private:
  int max_mtime_;
  IntTime start_mtime_;
  IntTime next_mtime_;
  std::bitset<SECONDS_PER_TRADE_DAY> mtime_set_;
  MinTimeMap min_time_map_;
};

} /* namespace stock_logic */

#endif /* SRC_PLUGINS_REALTIME_MARKET_TIME_MANAGER_H_ */
