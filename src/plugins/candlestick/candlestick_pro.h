/*
 * candlestick_pro.h
 *
 *  Created on: 2017年1月12日
 *      Author: pangh
 */

#ifndef CANDLESTICK_CANDLESTICK_PRO_H_
#define CANDLESTICK_CANDLESTICK_PRO_H_

#include "net/http_data.h"
#include "src/candlestick_info.h"

namespace candlestick {

enum CandleStickRequestInterval {
  CANDLESTICK_REQUEST_MONTH,
  CANDLESTICK_REQUEST_WEEK,
  CANDLESTICK_REQUEST_DAY
};

class SendCandleStickHistoryProtocol {
 public:

  SendCandleStickHistoryProtocol()
      : request_interval_(CANDLESTICK_REQUEST_DAY) {
    result_.reset(new NetBase());
  }
  ~SendCandleStickHistoryProtocol() {
    DeleteGroup(L"SingleInfo", result_.get());
  }

  void set_request_interval(CandleStickRequestInterval request_interval) {
    request_interval_ = request_interval;
  }

  CandleStickRequestInterval get_request_interval() {
    return request_interval_;
  }

  void DeleteGroup(std::wstring key, base_logic::DictionaryValue* root) {
    base_logic::ListValue* listvalue;
    bool r = root->GetList(key, &listvalue);
    if (!r) {
      return;
    }
    while (listvalue->GetSize()) {
      base_logic::Value* result_value;
      listvalue->Remove(0, &result_value);
      base_logic::DictionaryValue* dvalue =
          static_cast<base_logic::DictionaryValue*>(result_value);
      delete result_value;
    }

  }

  void AddGroup(std::wstring group_name,
                const std::list<SingleStockInfo> &result,
                base_logic::DictionaryValue* root) {
    std::list<SingleStockInfo>::const_iterator it = result.begin();
    base_logic::ListValue* list = new base_logic::ListValue();
    while (it != result.end()) {
      base_logic::DictionaryValue* info_value =
          new base_logic::DictionaryValue();
      info_value->SetString(L"date", it->get_date());
      info_value->SetReal(L"high", it->get_high_price());
      info_value->SetReal(L"low", it->get_low_price());
      info_value->SetReal(L"open", it->get_open_price());
      info_value->SetReal(L"close", it->get_close_price());
      list->Append((base_logic::Value*) (info_value));
      it++;
    }

    root->Set(group_name, list);
  }

  std::string GetJson() {
    std::string json;
    scoped_ptr <base_logic::ValueSerializer> serializer(
        base_logic::ValueSerializer::Create(base_logic::IMPL_JSONP));
    bool r = serializer->Serialize(*result_, &json);
    if (!r)
      return "";
    return json;
  }

  void set_history_info(const std::list<SingleStockInfo> &stock_history_info) {
    stock_history_info_ = stock_history_info;
    AddGroup(L"SingleInfo", stock_history_info_, result_.get());
  }

 private:
  CandleStickRequestInterval request_interval_;
  scoped_ptr<NetBase> result_;
  std::list<SingleStockInfo> stock_history_info_;
};

}  // namespace candlestick

#endif /* CANDLESTICK_CANDLESTICK_PRO_H_ */
