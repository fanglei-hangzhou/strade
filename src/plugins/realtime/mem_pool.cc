//  Copyright (c) 2015-2015 The KID Authors. All rights reserved.
//  Created on: 2016/10/13 Author: zjc

#include "../realtime/mem_pool.h"

#include <algorithm>

#include "logic/logic_comm.h"

#define ALIGN(d, a) (((d) + (a - 1)) & ~(a - 1))
#define ALIGNMENT   16

namespace stock_logic {

int MemPool::fragmentation_size = 0;
const int MemPool::MIN_BLOCK_SIZE = sizeof(Data) + 1 << 9;
const int MemPool::MAX_BLOCK_SIZE = 1 << 10 << 10 << 2;

MemPool::MemPool(size_t size)
    : head_(NULL),
      current_(NULL),
      block_size_(size) {
  block_size_ = std::max(block_size_, MIN_BLOCK_SIZE);
  block_size_ = std::min(block_size_, MAX_BLOCK_SIZE);
  block_size_ = ALIGN(block_size_, ALIGNMENT);
  LOG_MSG2("block_size: %d", block_size_);

  head_ = static_cast<Data*>(::operator new(block_size_));
  head_->reset();
  head_->last = reinterpret_cast<Byte*>(head_ + 1);
  head_->end = reinterpret_cast<Byte*>(head_) + block_size_;
  current_ = head_;
  LOG_MSG2("mem_pool head: %p ~ %p", head_->last, head_->end);
}

MemPool::~MemPool() {
  Data* p = NULL;
  Data* n = NULL;
  for (p = head_, n = p->next; p; p = n, n = n->next) {
    ::operator delete(p);
  }
}

void MemPool::Reset() {
  Data* p = head_;
  current_ = head_;
  while (p) {
    p->last = reinterpret_cast<Byte*>(p + 1);
    p = p->next;
  }
}

void* MemPool::AllocBlock(size_t size) {
  Byte* m = NULL;

  Data* p = static_cast<Data*>(::operator new(block_size_));
  p->reset();
  p->last = reinterpret_cast<Byte*>(p + 1);
  p->end = reinterpret_cast<Byte*>(p) + block_size_;
  current_ = current_->next = p;

//  LOG_DEBUG2("allocate a new block: %p ~ %p", p->last, p->end);
  m = p->last;
  p->last += size;

  return m;
}

void* MemPool::Alloc(size_t size) {
  if (size > block_size_) {
    LOG_ERROR2("alloc size: %d, block_size: %d", size, block_size_);
    return NULL;
  }

  Byte* m = NULL;
  Data* p = current_;
  intptr_t remain = 0;
  while (p) {
    remain = static_cast<intptr_t>(p->end - p->last);
    if (remain >= (intptr_t)size) {
      m = p->last;
      p->last += size;
      return m;
    }
//    LOG_DEBUG2("framentation size: %d", remain);
    current_ = p;
    p = p->next;
  }
  fragmentation_size += remain;
  return AllocBlock(size);
}
} /* namespace stock_logic */
