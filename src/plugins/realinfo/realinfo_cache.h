/*
 * realinfo_cache.h
 *
 *  Created on: 2017年1月12日
 *      Author: Administrator
 */

#ifndef REALINFO_REALINFO_CACHE_H_
#define REALINFO_REALINFO_CACHE_H_
#include "string"

namespace realinfo {
class RealInfoCache {
 public:
  explicit RealInfoCache(std::string pool_key);
  ~RealInfoCache();
  int UpdateRealInfoLatestCache(std::string stock_code, std::string value);
  int UpdateRealInfoTodayCache(std::string stock_code, std::string value);
  int GetRealInfoLatestCache(std::string stock_code, std::string *value);
  int GetRealInfoTodayCache(std::string stock_code, std::string *value);
  int UpdateRealInfoIndexCache(std::string value);
  int GetRealInfoIndexCache(std::string *value);

  std::string GetRealInfoIndexKey() {
    std::string result = "realinfolatest:";
    return result + "index";
  }

  std::string GetRealInfoLatestKey(std::string code) {
    std::string result = "realinfolatest:";
    return result + code;
  }
  std::string GetRealInfoTodayKey(std::string code) {
    std::string result = "realinfotoday:";
    return result + code;
  }
 private:
  std::string pool_key_;
};

}  // realinfo
#endif /* REALINFO_REALINFO_CACHE_H_ */
