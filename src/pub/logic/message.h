//  Copyright (c) 2015-2015 The KID Authors. All rights reserved.
//  Created on: 2017/1/9 Author: zjc

#include <string>
#include <sstream>
#include <vector>

#include "basic/basictypes.h"
#include "user_defined_types.h"

namespace base_logic {
class Value;
class ListValue;
class DictionaryValue;
}

using base_logic::Value;
using base_logic::ListValue;
using base_logic::DictionaryValue;

namespace strade_user {

typedef uint32 UserId;
typedef uint32 GroupId;

struct Head {
  uint16 opcode;
  UserId user_id;
  std::string token;

  Head()
      : opcode(0),
        user_id(0) {}
  virtual ~Head() {}
  bool StartDeserialize(DictionaryValue& dict);
  void StartDump(std::ostringstream& oss);
  virtual bool Deserialize(DictionaryValue& dict);
  virtual void Dump(std::ostringstream& oss);
};

///////////////////////////////////////////////////////////////////////////////
// create stock group
struct CreateGroupReq : Head {
  const static uint32 ID = 101;

  std::string group_name;
  StockCodeList code_list;
  bool Deserialize(DictionaryValue& dict);
  void Dump(std::ostringstream& oss);
};

struct CreateGroupRes {
  GroupId group_id;
  bool Serialize(DictionaryValue& dict);
};

///////////////////////////////////////////////////////////////////////////////
// add stock
struct AddStockReq : Head {
  const static uint32 ID = 102;

  GroupId group_id;
  StockCodeList code_list;
  bool Deserialize(DictionaryValue& dict);
  void Dump(std::ostringstream& oss);
};

// del stock
struct DelStockReq : Head {
  const static uint32 ID = 103;

  GroupId group_id;
  StockCodeList code_list;

  bool Deserialize(DictionaryValue& dict);
  void Dump(std::ostringstream& oss);
};

// query group
struct QueryGroupReq : Head {
  const static uint32 ID = 104;

  bool Deserialize(DictionaryValue& dict);
  void Dump(std::ostringstream& oss);
};

struct QueryGroupsRes {
  struct GroupInfo {
    GroupId id;
    std::string name;
    bool Serialize(DictionaryValue& dict);
  };
  typedef std::vector<GroupInfo> GroupList;
  GroupList group_list;

  bool Serialize(DictionaryValue& dict);
};

// query stock
struct QueryStocksReq : Head {
  const static uint32 ID = 105;

  GroupId group_id;
  bool Deserialize(DictionaryValue& dict);
  void Dump(std::ostringstream& oss);
};

struct QueryStocksRes {
  struct StockInfo {
    std::string code;
    std::string name;
    uint32 visit_heat;
    double price;
    double change;
    uint64 volume;          // 单位手
    std::string industry;   // 行业
    bool Serialize(DictionaryValue& dict);
  };
  typedef std::vector<StockInfo> StockList;

  StockList stock_list;

  bool Serialize(DictionaryValue& dict);
};

// query holding stocks
struct QueryHoldingStocksReq : Head {
  const static uint32 ID = 106;

  bool Deserialize(DictionaryValue& dict);
  void Dump(std::ostringstream& oss);

};

struct QueryHoldingStocksRes {
  struct StockInfo {
    std::string code;
    uint32 holding;
    uint32 available;
    double cost;
    double price;
    double market_value;
    double profit;
    double profit_ratio;
    double position;        // 仓位
    bool Serialize(DictionaryValue& dict);
  };
  typedef std::vector<StockInfo> StockList;
  StockList stock_list;

  bool Serialize(DictionaryValue& dict);
};

// query today orders
struct QueryTodayOrdersReq : Head {
  const static uint32 ID = 107;

  bool Deserialize(DictionaryValue& dict);
  void Dump(std::ostringstream& oss);
};

struct QueryTodayOrdersRes {
  struct OrderInfo {
    std::string code;
    OrderOperation op;
    double order_price;
    uint32 order_nums;
    time_t order_time;
    OrderStatus status;
    bool Serialize(DictionaryValue& dict);
  };
  typedef std::vector<OrderInfo> OrderList;
  OrderList order_list;

  bool Serialize(DictionaryValue& dict);
};

// query today finished orders
struct QueryTodayFinishedOrdersReq : Head {
  const static uint32 ID = 108;

  bool Deserialize(DictionaryValue& dict);
  void Dump(std::ostringstream& oss);
};

struct QueryTodayFinishedOrdersRes {
  struct OrderInfo {
    std::string code;
    OrderOperation op;
    double order_price;
    uint32 order_nums;
    double amount;
    time_t order_time;
    bool Serialize(DictionaryValue& dict);
  };
  typedef std::vector<OrderInfo> OrderList;
  OrderList order_list;

  bool Serialize(DictionaryValue& dict);
};

// query history finished orders
struct QueryHistoryFinishedOrdersReq : Head {
  const static uint32 ID = 109;

  std::string begin_time;
  std::string end_time;

  bool Deserialize(DictionaryValue& dict);
  void Dump(std::ostringstream& oss);
};

struct QueryHistoryFinishedOrdersRes {
  struct OrderInfo {
    std::string code;
    OrderOperation op;
    double order_price;
    double order_nums;
    double amount;
    time_t order_time;
    bool Serialize(DictionaryValue& dict);
  };
  typedef std::vector<OrderInfo> OrderList;
  OrderList order_list;

  bool Serialize(DictionaryValue& dict);
};

// query statement
struct QueryStatementReq : Head {
  const static uint32 ID = 110;
  std::string begin_time;
  std::string end_time;

  bool Deserialize(DictionaryValue& dict);
  void Dump(std::ostringstream& oss);
};

struct QueryStatementRes {
  struct StatementRecord {
    std::string code;
    OrderOperation op;
    double order_price;
    uint32 order_nums;
    double commission;
    double stamp_duty;
    double transfer_fee;
    double amount;
    double available_capital;
    bool Serialize(DictionaryValue& dict);
  };
  typedef std::vector<StatementRecord> StatementRecordList;
  StatementRecordList statement_list;

  bool Serialize(DictionaryValue& dict);
};

struct SubmitOrderReq : Head {
  const static uint32 ID = 111;

  GroupId group_id;
  std::string code;
  double order_price;
  uint32 order_nums;
  OrderOperation op;

  bool Deserialize(DictionaryValue& dict);
  void Dump(std::ostringstream& oss);
};

}


