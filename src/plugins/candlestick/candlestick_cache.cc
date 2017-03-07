/*
 * candlestick_cache.cc
 *
 *  Created on: 2017年1月12日
 *      Author: pangh
 */
#include "src/candlestick_cache.h"
namespace candlestick {
CandlestickCache::CandlestickCache(std::string pool_key) {
  pool_key_ = pool_key;
}

CandlestickCache::~CandlestickCache() {
}

int UpdateCache(std::string key, std::string value) {
  int rc = 0;
  return rc;
}

int GetCache(std::string key, std::string *value) {
  int rc = 0;
  return rc;
}

}  // namespace candlestick {
