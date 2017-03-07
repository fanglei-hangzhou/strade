//  Copyright (c) 2015-2015 The KID Authors. All rights reserved.
//  Created on: 2016年11月3日 Author: zjc

#ifndef PARSER_PLUGINS_STOCK_MY_ALLOCATOR_H_
#define PARSER_PLUGINS_STOCK_MY_ALLOCATOR_H_

#include "../realtime/mem_pool.h"

namespace stock_logic {

template<typename _Tp>
class MyAllocator {
 public:
  typedef size_t      size_type;
  typedef ptrdiff_t   difference_type;
  typedef _Tp*        pointer;
  typedef const _Tp*  const_pointer;
  typedef _Tp&        reference;
  typedef const _Tp&  const_reference;
  typedef _Tp         value_type;

  template<typename _Tp1>
  struct rebind {
    typedef MyAllocator<_Tp1> other;
  };

  MyAllocator() throw ()
      : pool_(NULL) {
  }

  MyAllocator(const MyAllocator& other) throw ()
      : pool_(other.pool_) {
  }

  template<typename _Tp1>
  MyAllocator(const MyAllocator<_Tp1>& other) throw ()
      : pool_(other.pool_) {
  }

  ~MyAllocator() throw () {
  }

  pointer address(reference __x) const {
    return &__x;
  }

  const_pointer address(const_reference __x) const {
    return &__x;
  }

  // NB: __n is permitted to be 0.  The C++ standard says nothing
  // about what the return value is when __n == 0.
  pointer allocate(size_type __n, const void* = 0) {
    if (__builtin_expect(__n > this->max_size(), false))
      std::__throw_bad_alloc();

    return static_cast<_Tp*>(pool_->Alloc(__n * sizeof(_Tp)));
  }

  // __p is not permitted to be a null pointer.
  void deallocate(pointer __p, size_type) {
//    ::operator delete(__p);
  }

  size_type max_size() const throw () {
    return pool_->max_size();
  }

  // _GLIBCXX_RESOLVE_LIB_DEFECTS
  // 402. wrong new expression in [some_] allocator::construct
  void construct(pointer __p, const _Tp& __val) {
    ::new ((void *) __p) _Tp(__val);
  }

  void destroy(pointer __p) {
    __p->~_Tp();
  }

 private:
  MemPool* pool_;
};

template<typename _Tp>
inline bool operator==(const MyAllocator<_Tp>&, const MyAllocator<_Tp>&) {
  return true;
}

template<typename _Tp>
inline bool operator!=(const MyAllocator<_Tp>&, const MyAllocator<_Tp>&) {
  return false;
}

} /* namespace stock_logic */

#endif /* PARSER_PLUGINS_STOCK_MY_ALLOCATOR_H_ */
