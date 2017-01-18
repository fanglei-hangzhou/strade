/*
 * candlestick_cache.h
 *
 *  Created on: 2017年1月12日
 *      Author: Administrator
 */

#ifndef CANDLESTICK_CANDLESTICK_CACHE_H_
#define CANDLESTICK_CANDLESTICK_CACHE_H_
#include "string"

namespace candlestick {
class CandlestickCache {
 public:
  explicit CandlestickCache(std::string pool_key);
  ~CandlestickCache();
  int UpdateCache(std::string key, std::string value);
  int GetCache(std::string key, std::string *value);
 private:
  std::string pool_key_;
};

}  // candlestick
#endif /* CANDLESTICK_CANDLESTICK_CACHE_H_ */
