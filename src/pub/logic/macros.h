//  Copyright (c) 2015-2015 The KID Authors. All rights reserved.
//  Created on: 2017/1/12 Author: zjc

#ifndef SRC_PUB_LOGIC_MACROS_H_
#define SRC_PUB_LOGIC_MACROS_H_

#define REFCOUNT_DECLARE(TypeNmae)            \
  TypeNmae(const TypeNmae& other);            \
  TypeNmae& operator=(const TypeNmae& other); \
  ~TypeNmae()

#define REFCOUNT_DEFINE(TypeNmae)             \
  TypeNmae::TypeNmae(const TypeNmae& other)   \
      : data_(other.data_) {                  \
    data_->AddRef();                          \
  }                                           \
                                              \
  TypeNmae& TypeNmae::operator=(              \
      const TypeNmae& other) {                \
    if (&other == this) {                     \
      return *this;                           \
    }                                         \
    data_->Release();                         \
    data_ = other.data_;                      \
    data_->AddRef();                          \
    return *this;                             \
  }                                           \
                                              \
  TypeNmae::~TypeNmae() {                     \
    data_->Release();                         \
  }

#endif /* SRC_PUB_LOGIC_MACROS_H_ */
