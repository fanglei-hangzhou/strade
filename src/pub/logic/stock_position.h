//  Copyright (c) 2015-2015 The KID Authors. All rights reserved.
//  Created on: 2017/1/10 Author: zjc

#ifndef SRC_PUB_LOGIC_STOCK_POSITION_H_
#define SRC_PUB_LOGIC_STOCK_POSITION_H_

#include <string>
#include <vector>

#include "macros.h"
#include "user_defined_types.h"

namespace strade_user {

class FakeStockPosition;
class GroupStockPosition;

typedef std::vector<FakeStockPosition> FakeStockPositionList;
typedef std::vector<GroupStockPosition> GroupStockPositionList;

class FakeStockPosition {
 public:
  FakeStockPosition(StockPositionId id, uint32 count);
  REFCOUNT_DECLARE(FakeStockPosition);
 public:
  StockPositionId id() const { return data_->id_; }
  uint32 count() const { return data_->count_; }
 private:
  class Data {
     public:
      Data()
          : refcount_(1),
            id_(0),
            count_(0) {
      }

     public:
      StockPositionId id_;
      uint32 count_;

      void AddRef() {
        __sync_fetch_and_add(&refcount_, 1);
      }

      void Release() {
        __sync_fetch_and_sub(&refcount_, 1);
        if (!refcount_)
          delete this;
      }

     private:
      int refcount_;
    };
 private:
  Data* data_;
};


class GroupStockPosition {
 public:
  GroupStockPosition(GroupId id, const std::string& code, uint32 count);
  REFCOUNT_DECLARE(GroupStockPosition);
 public:
  void set_group_id(GroupId id) { data_->group_id_ = id; }
  GroupId group_id() const { return data_->group_id_; }

  uint32 count() const { return data_->count_; }
 private:
  class Data {
   public:
    Data()
        : refcount_(1),
          group_id_(0),
          count_(0) {
    }

   public:
    GroupId group_id_;
    std::string code_;
    uint32 count_;
    FakeStockPositionList fake_stock_position_list_;

    void AddRef() {
      __sync_fetch_and_add(&refcount_, 1);
    }

    void Release() {
      __sync_fetch_and_sub(&refcount_, 1);
      if (!refcount_)
        delete this;
    }

   private:
    int refcount_;
  };
 private:
  Data* data_;
};

class StockPosition {
 public:
  StockPosition(const std::string& code, uint32 count);
  REFCOUNT_DECLARE(StockPosition);
 private:
  class Data {
   public:
    Data()
        : refcount_(1),
          count_(0) {
    }

   public:
    std::string code_;
    uint32 count_;
    GroupStockPositionList group_stock_position_list_;

    void AddRef() {
      __sync_fetch_and_add(&refcount_, 1);
    }

    void Release() {
      __sync_fetch_and_sub(&refcount_, 1);
      if (!refcount_)
        delete this;
    }

   private:
    int refcount_;
  };
 private:
  Data* data_;
};

} /* namespace strade_user */

#endif /* SRC_PUB_LOGIC_STOCK_POSITION_H_ */
