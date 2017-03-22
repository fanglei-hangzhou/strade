//  Copyright (c) 2015-2015 The KID Authors. All rights reserved.
//  Created on: 2016年10月13日 Author: zjc

#include "../realtime/market_time_manager.h"

#include "logic/logic_comm.h"

namespace stock_logic {

MarketTimeManager::MarketTimeManager(int start_time)
    : max_mtime_(-1),
      start_mtime_(start_time),
      next_mtime_(start_mtime_) {

}

MarketTimeManager::~MarketTimeManager() {
}

void MarketTimeManager::Add(int mtime) {
  if (mtime > max_mtime_) {
    max_mtime_ = mtime;
  }

  if (mtime < START_TRADE_TIME_AM
      || mtime > END_TRADE_TIME_PM) {
    return ;
  }

  LOG_DEBUG2("add mtime: %d, pos: %d, max_time: %d",
             mtime, time_to_pos(mtime), max_mtime_);
  mtime_set_[time_to_pos(mtime)] = true;
}

int MarketTimeManager::NewMarketTime() {
  int nearest_mtime = 0;
  int next_time = next_mtime_;
  IntTime t(next_mtime_);
  if (END_TRADE_TIME_AM == next_mtime_ || 150000 == next_mtime_) {
    t -= 60;
    nearest_mtime = NearestMarketTime(t);
  } else {
    t += 60;
    if (next_mtime_ > 150000 || t > max_mtime_) {
      return -1;
    }

    int l, r, p;
    l = r = p = time_to_pos(t);
    --r;
    while (--l >= 0 && !mtime_set_[l]);
    p = l;
    if (p < 0) {
      LOG_ERROR2("min data lose: time: %d", next_time);
      while (++r >= 0 && !mtime_set_[r]);
      p = r;
    }

    LOG_DEBUG2("nearest pos: %d", p);
    nearest_mtime = pos_to_time(p);
  }

  min_time_map_.insert(MinTimeMap::value_type(next_time, nearest_mtime));

  LOG_DEBUG2("%d ~= %d", next_time, nearest_mtime);
  advance_time();
  return nearest_mtime;
}

int MarketTimeManager::NearestMarketTime(int min_time) {
  MinTimeMap::iterator it = min_time_map_.find(min_time);
  if (min_time_map_.end() == it) {
    return -1;
  }
  return it->second;
}

} /* namespace stock_logic */
