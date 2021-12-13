    // Copyright 2021 Your Name <your_email>

#ifndef INCLUDE_SHAREDPTR_HPP_
#define INCLUDE_SHAREDPTR_HPP_

#include <cstdio>
#include "atomic"
#include "utility"
template <typename T>
class SharedPtr {
 private:
  T* _ptr;
  std::atomic_uint* _counter;

 public:
  SharedPtr();
  explicit SharedPtr(T* ptr);
  SharedPtr(const SharedPtr& r);
  SharedPtr(SharedPtr&& r);
  ~SharedPtr();
  auto operator=(const SharedPtr& r) -> SharedPtr&;
  auto operator=(SharedPtr&& r) -> SharedPtr&;

  // проверяет, указывает ли указатель на объект
  operator bool() const;
  auto operator*() const -> T&;
  auto operator->() const -> T*;

  auto get() -> T*;
  void reset();
  void reset(T* ptr);
  void swap(SharedPtr& r);
  // возвращает количество объектов SharedPtr,
  // которые ссылаются на тот же управляемый объект
  auto use_count() const -> size_t;
};

template<typename T>
SharedPtr<T>::SharedPtr()
    : _ptr(nullptr),
      _counter(nullptr)
{
}

template<typename T>
SharedPtr<T>::SharedPtr(T* ptr)
    : _ptr(ptr),
      _counter(new std::atomic_uint{1})
{
}

template<typename T>
SharedPtr<T>::SharedPtr(const SharedPtr& r)
{
  _ptr = r._ptr;
  _counter = r._counter;
  ++(*_counter);
}

template<typename T>
SharedPtr<T>::SharedPtr(SharedPtr&& r)
{
  _ptr = r._ptr;
  _counter = r._counter;
  r._ptr = nullptr;
  r._counter = nullptr;
}

template<typename T>
SharedPtr<T>::~SharedPtr()
{
  if (_counter != nullptr) {
    (*_counter)--;
    if ((*_counter) == 0) {
      delete _counter;
      _counter = nullptr;
      delete _ptr;
      _ptr = nullptr;
    }
  }
}

template<typename T>
auto SharedPtr<T>::operator=(const SharedPtr& r) -> SharedPtr&{
  if (this != &r) {
    this->~SharedPtr();
    _ptr = r._ptr;
    _counter = r._counter;
    if (_counter != nullptr) (*_counter)++;
  }
  return *this;
}

template<typename T>
auto SharedPtr<T>::operator=(SharedPtr&& r) -> SharedPtr&{
  if (this != &r) {
    this->~SharedPtr();
    _ptr = r._ptr;
    _counter = r._counter;
    r._counter = nullptr;
    r._ptr = nullptr;
  }
  return *this;
}

template<typename T>
SharedPtr<T>::operator bool() const {
  return (_ptr != nullptr);
}

template<typename T>
auto SharedPtr<T>::operator*() const -> T & {
  return *_ptr;
}

template<typename T>
auto SharedPtr<T>::operator->() const -> T * {
  return _ptr;
}

template<typename T>
auto SharedPtr<T>::get() -> T*{
  return _ptr;
}

template<typename T>
void SharedPtr<T>::reset(){
  *this = SharedPtr();
}

template<typename T>
void SharedPtr<T>::reset(T* ptr){
  *this = SharedPtr(ptr);
}

template<typename T>
void SharedPtr<T>::swap(SharedPtr<T>& r){
  std::swap(_ptr, r._ptr);
  std::swap(_counter, r._counter);
}

template<typename T>
auto SharedPtr<T>::use_count() const -> size_t{
  if (_counter == nullptr){
    return 0;
  }
  return *_counter;
}

#endif // INCLUDE_SHAREDPTR_HPP_

