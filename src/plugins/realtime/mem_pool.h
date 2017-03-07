//  Copyright (c) 2015-2015 The KID Authors. All rights reserved.
//  Created on: 2016/10/13 Author: zjc

#ifndef SRC_PLUGINS_REALTIME_MEM_POOL_H_
#define SRC_PLUGINS_REALTIME_MEM_POOL_H_

#include <string.h>

namespace stock_logic {

class MemPool {
 public:
  typedef unsigned char Byte;
  struct Data {
    Byte* last;
    Byte* end;
    Data* next;
    Data() {
      memset(this, 0, sizeof(*this));
    }
    void reset() {
      memset(this, 0, sizeof(*this));
    }
  };
  const static int MIN_BLOCK_SIZE;
  const static int MAX_BLOCK_SIZE;
  static int fragmentation_size;
 public:
  MemPool(size_t size);
  ~MemPool();
  void* Alloc(size_t size);

  template <typename T>
  T* Construct(const T& val);

  void Reset();
  int max_size() const { return block_size_ - sizeof(Data); }
 private:
  MemPool(const MemPool&);
  MemPool& operator=(const MemPool&);
  void *AllocBlock(size_t size);
 private:
  int block_size_;
  Data* head_;
  Data* current_;
};

template <typename T>
T* MemPool::Construct(const T& val) {
  T* p = static_cast<T*>(Alloc(sizeof(T)));
  ::new (p) T(val);
  return p;
}

} /* namespace stock_logic */

#endif /* SRC_PLUGINS_REALTIME_MEM_POOL_H_ */
