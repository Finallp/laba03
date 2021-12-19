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
  SharedPtr(SharedPtr&& r) noexcept;
  ~SharedPtr();
  auto operator=(const SharedPtr& r) -> SharedPtr&;
  auto operator=(SharedPtr&& r) noexcept -> SharedPtr&;

  // проверяет, указывает ли указатель на объект
  operator bool() const;
  auto operator*() const -> T&;
  auto operator->() const -> T*;

  auto get() -> T*;
  [[nodiscard]] auto get_counter() const -> const std::atomic_uint*;
  void reset();
  void reset(T* ptr);
  void swap(SharedPtr& r);
  // возвращает количество объектов SharedPtr,
  // которые ссылаются на тот же управляемый объект
  [[nodiscard]] auto use_count() const -> size_t;
  //auto use_count() const -> size_t;
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
SharedPtr<T>::SharedPtr(SharedPtr&& r) noexcept
{
  _ptr = std::move(r._ptr);
  _counter = std::move(r._counter);
}

template<typename T>
SharedPtr<T>::~SharedPtr()
{
  if (_counter) {
    if ((*_counter) == 1) {
      delete _counter;
      delete _ptr;
    }
    else{
      (*_counter)--;
    }
  }
}

template<typename T>
auto SharedPtr<T>::operator=(const SharedPtr& r) -> SharedPtr&{
  if (this != &r) {
    return *this;
  }
  this->~SharedPtr();
  _ptr = r._ptr;
  _counter = r._counter;
  (*_counter)++;
  return *this;
}

template<typename T>
auto SharedPtr<T>::operator=(SharedPtr&& r) noexcept -> SharedPtr&{
  if (this != &r) {
    this->~SharedPtr();
    _ptr = std::move(r._ptr);
    _counter = std::move(r._counter);
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

template <typename T>
auto SharedPtr<T>::get_counter() const -> const std::atomic_uint * {
  return _counter;
}

template<typename T>
void SharedPtr<T>::reset(){
  *this = std::move(SharedPtr());
}

template<typename T>
void SharedPtr<T>::reset(T* ptr){
  *this = std::move(SharedPtr(ptr));
}

template<typename T>
void SharedPtr<T>::swap(SharedPtr<T>& r){
  std::swap(_ptr, r._ptr);
  std::swap(_counter, r._counter);
}

template<typename T>
auto SharedPtr<T>::use_count() const -> size_t{
  return static_cast<size_t>(*_counter);
}

#endif // INCLUDE_SHAREDPTR_HPP_

