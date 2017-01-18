//
// Created by Harvey on 2017/1/4.
//

#include "strade_share_engine.h"

#define DEFAULT_CONFIG_PATH  "./strade_share/stradeshare_config.xml"

strade_share::SSEngine* GetStradeShareEngine(void) {
  return strade_share::SSEngineImpl::GetInstance();
}

namespace strade_share {

SSEngineImpl* SSEngineImpl::instance_ = NULL;
SSEngineImpl::SSEngineImpl() {
  if (!InitParam()) {
    LOG_ERROR("StradeShareEngineImpl Init error");
    assert(0);
  }
}

SSEngineImpl::~SSEngineImpl() {
  if (NULL != mysql_engine_) {
    delete mysql_engine_;
    mysql_engine_ = NULL;
  }
  DeinitThreadrw(lock_);
}

SSEngineImpl* SSEngineImpl::GetInstance() {
  if (NULL == instance_) {
    instance_ = new SSEngineImpl();
  }
  return instance_;
}

bool SSEngineImpl::Init() {

  return true;
}

bool SSEngineImpl::InitParam() {
  InitThreadrw(&lock_);
  bool r = false;
  std::string path = DEFAULT_CONFIG_PATH;
  config::FileConfig* config = config::FileConfig::GetFileConfig();
  if (config == NULL) {
    return false;
  }
  r = config->LoadConfig(path);
  if (!r) {
    return false;
  }
  mysql_engine_ = new StradeShareDB(config);
  LoadAllStockBasicInfo();
  return true;
}

void SSEngineImpl::AttachObserver(strade_logic::Observer* observer) {
  if (NULL != observer) {
    base_logic::WLockGd lk(lock_);
    this->Attach(observer);
  }
}

void SSEngineImpl::LoadAllStockBasicInfo() {
  base_logic::WLockGd lk(lock_);
  std::vector<strade_logic::StockTotalInfo> stock_vec;
  mysql_engine_->FetchAllStockList(stock_vec);
  LOG_DEBUG2("LoadAllStockBasicInfo size=%d", stock_vec.size());
  std::vector<strade_logic::StockTotalInfo>::iterator iter(stock_vec.begin());
  for (; iter != stock_vec.end(); ++iter) {
    strade_logic::StockTotalInfo& stock_total_info = (*iter);
    std::vector<strade_logic::StockHistInfo> stock_hist_vec;
    mysql_engine_->FetchStockHistList(
        stock_total_info.GetStockCode(), stock_hist_vec);
    stock_total_info.AddStockHistVec(stock_hist_vec);
    AddStockTotalInfoNonblock(stock_total_info);
  }
}

void SSEngineImpl::UpdateStockRealMarketData(
    REAL_MARKET_DATA_VEC& stocks_market_data) {

  {
    base_logic::WLockGd lk(lock_);
    int total_count = 0;
    REAL_MARKET_DATA_VEC::const_iterator iter(stocks_market_data.begin());
    for (; iter != stocks_market_data.end(); ++iter) {
      const strade_logic::StockRealInfo& stock_real_info = *iter;
      const std::string& stock_code = stock_real_info.GetStockCode();
      const time_t& trade_time = stock_real_info.GetTradeTime();
      strade_logic::StockTotalInfo stock_total_info;
      if (!GetStockTotalNonBlock(stock_code, stock_total_info)) {
        LOG_ERROR2("UpdateStockRealMarketData stock_code=%s, not exists!!!!",
                   stock_code.c_str());
        continue;
      }
      stock_total_info.AddStockRealInfoByTime(trade_time, stock_real_info);
      ++total_count;
    }
    LOG_DEBUG2("UpdateStockRealMarketData total_count=%d, current_time=%d",
               total_count, time(NULL));
  }

  // 通知所有需要实时行情数据的观察者
  this->Notify(strade_logic::REALTIME_MARKET_VALUE_UPDATE);
}

bool SSEngineImpl::UpdateStockHistInfoByDate(const std::string& stock_code,
                                             const std::string& date,
                                             strade_logic::StockHistInfo& stock_hist_info) {
  base_logic::WLockGd lk(lock_);
  strade_logic::StockTotalInfo stock_total_info;
  if (!GetStockTotalNonBlock(stock_code, stock_total_info)) {
    return false;
  }
  return stock_total_info.AddStockHistInfoByDate(date, stock_hist_info);
}

bool SSEngineImpl::ClearOldRealTradeMap() {
  base_logic::WLockGd lk(lock_);
  STOCKS_MAP::iterator iter(share_cache_.stocks_map_.begin());
  for (; iter != share_cache_.stocks_map_.end(); ++iter) {
    iter->second.ClearRealMap();
  }
  return true;
}

bool SSEngineImpl::UpdateStockHistDataVec(
    const std::string& stock_code,
    STOCK_HIST_DATA_VEC& stock_vec) {
  base_logic::WLockGd lk(lock_);
  strade_logic::StockTotalInfo stock_total_info;
  if (!GetStockTotalNonBlock(stock_code, stock_total_info)) {
    return false;
  }
  stock_total_info.AddStockHistVec(stock_vec);
  return true;
}

bool SSEngineImpl::AddStockTotalInfoNonblock(
    const strade_logic::StockTotalInfo& stock_total_info) {
  const std::string& stock_code = stock_total_info.GetStockCode();
  share_cache_.stocks_map_[stock_code] = stock_total_info;
  return true;
}

bool SSEngineImpl::AddStockTotalInfoBlock(
    const strade_logic::StockTotalInfo& stock_total_info) {
  base_logic::WLockGd lk(lock_);
  AddStockTotalInfoNonblock(stock_total_info);
  return true;
}

STOCKS_MAP SSEngineImpl::GetAllStockTotalMapCopy() {
  base_logic::RLockGd lk(lock_);
  return share_cache_.stocks_map_;
}

STOCK_HIST_MAP SSEngineImpl::GetStockHistMapByCodeCopy(
    const std::string& stock_code) {
  base_logic::RLockGd lk(lock_);
  strade_logic::StockTotalInfo stock_total_info;
  if (!GetStockTotalNonBlock(stock_code, stock_total_info)) {
    return STOCK_HIST_MAP();
  }
  return stock_total_info.GetStockHistMap();
}

STOCK_REAL_MAP SSEngineImpl::GetStockRealInfoMapCopy(
    const std::string& stock_code) {
  base_logic::RLockGd lk(lock_);
  strade_logic::StockTotalInfo stock_total_info;
  if (!GetStockTotalNonBlock(stock_code, stock_total_info)) {
    return STOCK_REAL_MAP();
  }
  return stock_total_info.GetStockRealMap();
}

bool SSEngineImpl::GetStockTotalInfoByCode(
    const std::string& stock_code,
    strade_logic::StockTotalInfo& stock_total_info) {
  base_logic::WLockGd lk(lock_);
  GetStockTotalNonBlock(stock_code, stock_total_info);
}

bool SSEngineImpl::GetStockHistInfoByDate(
    const std::string& stock_code,
    const std::string& date,
    strade_logic::StockHistInfo& stock_hist_info) {
  base_logic::WLockGd lk(lock_);
  strade_logic::StockTotalInfo stock_total_info;
  if (!GetStockTotalNonBlock(stock_code, stock_total_info)) {
    return false;
  }
  return stock_total_info.GetStockHistInfoByDate(date, stock_hist_info);
}

bool SSEngineImpl::GetStockRealMarketDataByTime(
    const std::string& stock_code,
    const time_t& time,
    strade_logic::StockRealInfo& stock_real_info) {
  base_logic::WLockGd lk(lock_);
  strade_logic::StockTotalInfo stock_total_info;
  if (!GetStockTotalNonBlock(stock_code, stock_total_info)) {
    return false;
  }
  return stock_total_info.GetStockRealInfoByTradeTime(time, stock_real_info);
}

bool SSEngineImpl::GetStockCurrRealMarketInfo(
    const std::string& stock_code,
    strade_logic::StockRealInfo& stock_real_info) {
  base_logic::WLockGd lk(lock_);
  strade_logic::StockTotalInfo stock_total_info;
  if (!GetStockTotalNonBlock(stock_code, stock_total_info)) {
    return false;
  }
  return stock_total_info.GetCurrRealMarketInfo(stock_real_info);
}

bool SSEngineImpl::ReadDataRows(
    const std::string& sql, std::vector<MYSQL_ROW>& rows_vec) {
  base_logic::WLockGd lk(lock_);
  return mysql_engine_->ReadDataRows(sql, rows_vec);
}

bool SSEngineImpl::WriteData(const std::string& sql) {
  base_logic::WLockGd lk(lock_);
  return mysql_engine_->WriteData(sql);
}

bool SSEngineImpl::ExcuteStorage(
    const std::string& sql, std::vector<MYSQL_ROW>& rows_vec) {
  return mysql_engine_->ExcuteStorage(sql, rows_vec);
}

} /* namespace strade_share */
