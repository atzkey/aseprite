// Aseprite Base Library
// Copyright (c) 2001-2013 David Capello
//
// This source file is distributed under MIT license,
// please read LICENSE.txt for more information.

#ifndef BASE_OBSERVERS_H_INCLUDED
#define BASE_OBSERVERS_H_INCLUDED

#include <algorithm>
#include <vector>

namespace base {

template<typename T>
class Observers
{
public:
  typedef T observer_type;
  typedef std::vector<observer_type*> list_type;
  typedef typename list_type::iterator iterator;
  typedef typename list_type::const_iterator const_iterator;

  iterator begin() { return m_observers.begin(); }
  iterator end() { return m_observers.end(); }
  const_iterator begin() const { return m_observers.begin(); }
  const_iterator end() const { return m_observers.end(); }

  bool empty() const { return m_observers.empty(); }
  size_t size() const { return m_observers.size(); }

  Observers() { }

  ~Observers() {
    ASSERT(m_observers.empty());
  }

  // Adds the observer in the collection. The observer is owned by the
  // collection and will be destroyed calling the T::dispose() member
  // function.
  void addObserver(observer_type* observer) {
    m_observers.push_back(observer);
  }

  // Removes the observer from the collection. After calling this
  // function you own the observer so you have to dispose it.
  void removeObserver(observer_type* observer) {
    iterator it = std::find(m_observers.begin(), m_observers.end(), observer);
    if (it != end())
      m_observers.erase(it);
  }

  void notifyObservers(void (observer_type::*method)()) {
    for (iterator
           it = this->begin(),
           end = this->end(); it != end; ++it) {
      ((*it)->*method)();
    }
  }

  template<typename A1>
  void notifyObservers(void (observer_type::*method)(A1), A1 a1) {
    for (iterator
           it = this->begin(),
           end = this->end(); it != end; ++it) {
      ((*it)->*method)(a1);
    }
  }

  template<typename A1, typename A2>
  void notifyObservers(void (observer_type::*method)(A1, A2), A1 a1, A2 a2) {
    for (iterator
           it = this->begin(),
           end = this->end(); it != end; ++it) {
      ((*it)->*method)(a1, a2);
    }
  }

  template<typename A1, typename A2, typename A3>
  void notifyObservers(void (observer_type::*method)(A1, A2, A3), A1 a1, A2 a2, A3 a3) {
    for (iterator
           it = this->begin(),
           end = this->end(); it != end; ++it) {
      ((*it)->*method)(a1, a2, a3);
    }
  }

private:
  list_type m_observers;
};

} // namespace base

#endif  // BASE_OBSERVERS_H_INCLUDED
