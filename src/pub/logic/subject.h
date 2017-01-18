//
// Created by Harvey on 2017/1/11.
//

#ifndef STRADE_STRADE_SUBJECT_H
#define STRADE_STRADE_SUBJECT_H

#include <list>
#include <map>
#include <iterator>
#include <algorithm>

namespace strade_logic {

class Observer;

class Subject {
 public:
  Subject();
  virtual ~Subject();

  virtual void Notify(int opcode);
  virtual void Attach(Observer*);
  virtual void Detach(Observer*);

  std::list<Observer*> m_lst;
};

} /* namespace strade_logic */

#endif //STRADE_STRADE_SUBJECT_H
