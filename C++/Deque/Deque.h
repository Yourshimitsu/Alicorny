#include <iostream>
#include <vector>

const int kMOD = 32;

struct Track {
  int block = 0;
  int mod = 0;

  Track() = default;

  Track(int a, int b) : block(a), mod(b) {
  }

  Track& operator--() {
    if (mod == 0) {
      --block;
      mod = kMOD - 1;
    } else {
      --mod;
    }
    return *this;
  }

  Track& operator-=(int x) {
    block -= x / kMOD;
    mod -= x % kMOD;
    if (mod < 0) {
      --block;
      mod += kMOD;
    }
    return *this;
  }

  Track& operator++() {
    ++mod;
    block += mod / kMOD;
    mod %= kMOD;
    return *this;
  }

  Track& operator+=(int x) {
    block += x / kMOD;
    mod += x % kMOD;
    if (mod >= kMOD) {
      ++block;
      mod -= kMOD;
    }
    return *this;
  }
};

template<typename T>
class Deque {
 public:
  ~Deque();
  Deque() = default;
  Deque(const Deque<T>& d);
  Deque(size_t count, const T& value);
  Deque(size_t count);
  Deque<T>& operator=(Deque<T> d);
  T& operator[](size_t index);
  const T& operator[](size_t index) const;
  void push_back(const T& value);
  void pop_back();
  void push_front(const T& value);
  void pop_front();
  T& at(size_t index);
  const T& at(size_t index) const;
  size_t size() const;
  void swap(Deque<T>& d);
  template<bool IsConst>
  class common_iterator;
  using iterator = common_iterator<false>;
  using const_iterator = common_iterator<true>;
  using reverse_iterator = std::reverse_iterator<iterator>;
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;
  void insert(iterator iter, const T&);
  void erase(iterator iter);
  iterator begin() { return iterator(&arr_[start_.block], start_); }
  reverse_iterator rbegin() { return reverse_iterator(this->end()); }
  const_iterator begin() const { return const_iterator(&arr_[start_.block], start_); }
  const_reverse_iterator rbegin() const { return const_reverse_iterator(this->end()); }
  const_iterator cbegin() { return iterator(&arr_[start_.block], start_); }
  iterator end() { return iterator(&arr_[end_.block], end_); }
  reverse_iterator rend() { return reverse_iterator(this->begin()); }
  const_iterator end() const { return const_iterator(&arr_[end_.block], end_); }
  const_reverse_iterator rend() const { return const_reverse_iterator(this->begin()); }
  const_iterator cend() { return iterator(&arr_[end_.block], end_); }

 private:
  std::vector<T*> arr_;
  size_t sz_ = 0;
  Track start_;
  Track end_;
};

template<typename T>
size_t Deque<T>::size() const {
  return sz_;
}

template<typename T>
const T& Deque<T>::at(size_t index) const {
  if (index >= sz_) {
    throw std::out_of_range("");
  }
  Track i = start_;
  i += index;
  return arr_[i.block][i.mod];
}

template<typename T>
void Deque<T>::push_front(const T& value) {
  int i = start_.block;
  try {
    if ((start_.block == 0) and (start_.mod == 0)) {
      std::vector<T*> newarr(4 * arr_.capacity() + 4);
      for (; i < end_.block; ++i) {
        newarr[i + newarr.capacity() / 2] = arr_[i];
      }
      if (end_.mod != 0) {
        newarr[i + newarr.capacity() / 2] = arr_[i];
      }
      start_.block = newarr.capacity() / 2;
      end_.block += newarr.capacity() / 2;
      std::swap(arr_, newarr);
    }
  } catch (...) {
    throw;
  }
  try {
    --start_;
    if (start_.mod == kMOD - 1) {
      arr_[start_.block] = reinterpret_cast<T*>(new char[kMOD * sizeof(T)]);;
    }
    new(arr_[start_.block] + start_.mod) T(value);
  } catch (...) {
    if (start_.mod == kMOD - 1) {
      delete arr_[start_.block];
    }
    ++start_;
    throw;
  }
  ++sz_;
}

template<typename T>
void Deque<T>::push_back(const T& value) {
  int i = start_.block;
  try {
    if ((end_.block == static_cast<int>(arr_.capacity())) and (end_.mod == 0)) {
      std::vector<T*> newarr(4 * arr_.capacity() + 4);
      for (; i < end_.block; ++i) {
        newarr[i + newarr.capacity() / 2] = arr_[i];
      }
      start_.block += newarr.capacity() / 2;
      end_.block += newarr.capacity() / 2;
      std::swap(arr_, newarr);
    }
  } catch (...) {
    throw;
  }
  try {
    if (end_.mod == 0) {
      arr_[end_.block] = reinterpret_cast<T*>(new char[kMOD * sizeof(T)]);
    }
    new(arr_[end_.block] + end_.mod) T(value);
  } catch (...) {
    if (end_.mod == 0) {
      delete[] reinterpret_cast<char*>(arr_[end_.block]);
    }
    throw;
  }
  ++end_;
  ++sz_;
}

template<typename T>
void Deque<T>::pop_back() {
  (arr_[end_.block] + end_.mod - 1)->~T();
  if (end_.mod == 1) {
    delete[] reinterpret_cast<char*>(arr_[end_.block]);
  }
  --end_;
  --sz_;
}

template<typename T>
void Deque<T>::pop_front() {
  (arr_[start_.block] + start_.mod)->~T();
  ++start_;
  if (start_.mod == 0) {
    delete[] reinterpret_cast<char*>(arr_[start_.block - 1]);
  }
  --sz_;
}

template<typename T>
template<bool IsConst>
class Deque<T>::common_iterator {
 private:
  mutable std::conditional_t<IsConst, T* const*, T**> ptr_;
  mutable Track ind_;
 public:
  using value_type = std::conditional_t<IsConst, const T, T>;
  using difference_type = int;
  using pointer = value_type*;
  using reference = value_type&;
  using iterator_category = std::random_access_iterator_tag;
  common_iterator(std::conditional_t<IsConst, T* const*, T**> ptr, Track ind) : ptr_(ptr), ind_(ind) {}

  operator const_iterator() {
    return const_iterator(ptr_, ind_);
  }

  std::conditional_t<IsConst, const T&, T&> operator*() const {
    return *(*ptr_ + ind_.mod);
  }

  std::conditional_t<IsConst, const T*, T*> operator->() const {
    return (*ptr_ + ind_.mod);
  }

  auto operator<(const common_iterator& iter) const {
    return (ind_.block * kMOD + ind_.mod < iter.ind_.block * kMOD + iter.ind_.mod);
  }

  auto operator<=(const common_iterator& iter) const {
    return (ind_.block * kMOD + ind_.mod <= iter.ind_.block * kMOD + iter.ind_.mod);
  }

  auto operator>(const common_iterator& iter) const {
    return (ind_.block * kMOD + ind_.mod > iter.ind_.block * kMOD + iter.ind_.mod);
  }

  auto operator>=(const common_iterator& iter) const {
    return (ind_.block * kMOD + ind_.mod >= iter.ind_.block * kMOD + iter.ind_.mod);
  }

  auto operator!=(const common_iterator& iter) const {
    return (ind_.block * kMOD + ind_.mod != iter.ind_.block * kMOD + iter.ind_.mod);
  }

  bool operator==(const common_iterator& iter) const {
    return (ind_.block * kMOD + ind_.mod == iter.ind_.block * kMOD + iter.ind_.mod);
  }

  int operator-(const common_iterator& b) const {
    return (kMOD * (ind_.block - b.ind_.block) + ind_.mod - b.ind_.mod);
  }

  common_iterator& operator++() {
    ++ind_;
    if (ind_.mod == 0) {
      ++ptr_;
    }
    return *this;
  }

  common_iterator operator++(int) {
    auto iter = *this;
    ++ind_;
    if (ind_.mod == 0) {
      ++ptr_;
    }
    return iter;
  }

  common_iterator& operator--() {
    --ind_;
    if (ind_.mod == kMOD - 1) {
      --ptr_;
    }
    return *this;
  }

  common_iterator operator--(int) {
    auto iter = *this;
    --ind_;
    if (ind_.mod == kMOD - 1) {
      --ptr_;
    }
    return iter;
  }

  common_iterator& operator+=(int n) {
    ptr_ += n / kMOD;
    if (n % kMOD + ind_.mod >= kMOD) {
      ++ptr_;
    }
    ind_ += n;
    return *this;
  }

  common_iterator operator+(int n) const {
    common_iterator iter = common_iterator(ptr_, ind_);
    iter += n;
    return iter;
  }

  common_iterator& operator-=(int n) {
    ptr_ -= n / kMOD;
    if (n % kMOD > ind_.mod) {
      --ptr_;
    }
    ind_ -= n;
    return *this;
  }

  common_iterator operator-(int n) const {
    common_iterator iter = common_iterator(ptr_, ind_);
    iter -= n;
    return iter;
  }
};

template<typename T>
void Deque<T>::insert(typename Deque<T>::iterator iter, const T& value) {
  if (iter == this->end()) {
    this->push_back(value);
    return;
  }
  Deque<T>::iterator right = this->end();
  push_back(value);
  while (iter != right) {
    *right = *(right - 1);
    --right;
  }
  *iter = value;
}

template<typename T>
void Deque<T>::erase(typename Deque<T>::iterator iter) {
  while (iter + 1 != this->end()) {
    *iter = *(iter + 1);
    ++iter;
  }
  this->pop_back();
}

template<typename T>
T& Deque<T>::at(size_t index) {
  if (index >= sz_) {
    throw std::out_of_range("");
  }
  Track i = start_;
  i += index;
  return arr_[i.block][i.mod];
}

template<typename T>
const T& Deque<T>::operator[](size_t index) const {
  Track i = start_;
  i += index;
  return arr_[i.block][i.mod];
}

template<typename T>
T& Deque<T>::operator[](size_t index) {
  Track i = start_;
  i += index;
  return arr_[i.block][i.mod];
}

template<typename T>
Deque<T>& Deque<T>::operator=(Deque<T> d) {
  swap(d);
  return *this;
}

template<typename T>
void Deque<T>::swap(Deque<T>& d) {
  std::swap(start_, d.start_);
  std::swap(end_, d.end_);
  std::swap(arr_, d.arr_);
  std::swap(sz_, d.sz_);
}

template<typename T>
Deque<T>::Deque(size_t count): sz_(count) {
  if (sz_ != 0) {
    int i = 0;
    arr_.reserve((sz_ / kMOD) * 4 + 4);
    start_ = Track(static_cast<int>((sz_ / kMOD) * 2 + 2), 0);
    end_ = Track();
    end_.block = start_.block + sz_ / kMOD;
    end_.mod = sz_ % kMOD;
    for (; i < static_cast<int>(sz_); ++i) {
      if (i % kMOD == 0) {
        arr_[i / kMOD + start_.block] = reinterpret_cast<T*>(new char[kMOD * sizeof(T)]);
      }
      try {
        new(arr_[i / kMOD + start_.block] + i % kMOD) T();
      } catch (...) {
        if (i % kMOD == 0) {
          delete[] reinterpret_cast<char*>(arr_[i / kMOD + start_.block]);
        }
        --i;
        for (; i >= 0; --i) {
          (arr_[i / kMOD + start_.block] + i % kMOD)->~T();
          if (i % kMOD == 0) {
            delete[] reinterpret_cast<char*>(arr_[i / kMOD + start_.block]);
          }
        }
        throw;
      }
    }
  }
}

template<typename T>
Deque<T>::Deque(size_t count, const T& value): sz_(count) {
  if (sz_ != 0) {
    int i = 0;
    try {
      arr_.reserve((sz_ / kMOD) * 4 + 4);
      start_ = Track(static_cast<int>((sz_ / kMOD) * 2 + 2), 0);
      end_ = Track();
      end_.block = start_.block + sz_ / kMOD;
      end_.mod = sz_ % kMOD;
      for (; i < static_cast<int>(sz_); ++i) {
        if (i % kMOD == 0) {
          arr_[i / kMOD + start_.block] = reinterpret_cast<T*>(new char[kMOD * sizeof(T)]);
        }
        new(arr_[i / kMOD + start_.block] + i % kMOD) T(value);
      }
    } catch (...) {
      if (i % kMOD == 0) {
        delete[] reinterpret_cast<char*>(arr_[i / kMOD + start_.block]);
      }
      --i;
      for (; i >= 0; --i) {
        (arr_[i / kMOD + start_.block] + i % kMOD)->~T();
        if (i % kMOD == 0) {
          delete[] reinterpret_cast<char*>(arr_[i / kMOD + start_.block]);
        }
      }
      throw;
    }
  }
}

template<typename T>
Deque<T>::Deque(const Deque<T>& d) : sz_(d.sz_), start_(d.start_), end_(d.end_) {
  if (sz_ != 0) {
    size_t i = start_.block;
    size_t j = 0;
    try {
      arr_.reserve(end_.block * 2 + 2);
      for (; i < static_cast<size_t>(end_.block); ++i) {
        arr_[i] = reinterpret_cast<T*>(new char[kMOD * sizeof(T)]);
        for (size_t j = 0; j < kMOD; ++j) {
          new(arr_[i] + j) T(d.arr_[i][j]);
        }
      }
      arr_[i] = reinterpret_cast<T*>(new char[kMOD * sizeof(T)]);
      for (; j <= static_cast<size_t>(end_.mod); ++j) {
        new(arr_[end_.block] + j) T(d.arr_[i][j]);
      }
    } catch (...) {
      for (; i >= start_.block; --i) {
        if (j <= kMOD) {
          for (int k = 0; k < j; ++k) {
            (arr_[end_.block] + k)->~T();
          }
          j = kMOD + 1;
        } else {
          for (int k = 0; k < kMOD; ++k) {
            (arr_[end_.block] + k)->~T();
          }
        }
        delete[] reinterpret_cast<char*>(arr_[i]);
      }
      throw;
    }
  }
}

template<typename T>
Deque<T>::~Deque() {
  if (sz_ != 0) {
    for (int i = start_.block; i <= end_.block; ++i) {
      delete[] reinterpret_cast<char*>(arr_[i]);
    }
  }
}

template<typename Iterator>
struct reverse_iterator {
 private:
  Iterator iter_;
 public:
  reverse_iterator(const Iterator& iter) : iter_(iter) {}

  reverse_iterator<Iterator>& operator++() {
    --iter_;
    return *this;
  }

  reverse_iterator<Iterator>& operator+=(size_t n) const {
    iter_ -= n;
    return *this;
  }

  reverse_iterator<Iterator>& operator+(size_t n) const {
    reverse_iterator<Iterator> itercopy = iter_;
    itercopy -= n;
    return itercopy;
  }

  reverse_iterator<Iterator>& operator--() {
    ++iter_;
    return *this;
  }

  reverse_iterator<Iterator>& operator-=(size_t n) const {
    iter_ += n;
    return *this;
  }

  reverse_iterator<Iterator>& operator-(size_t n) const {
    reverse_iterator<Iterator> itercopy = iter_;
    itercopy += n;
    return itercopy;
  }

  int operator-(const reverse_iterator<Iterator>& a) const {
    return (const_cast<Iterator>(a) - const_cast<Iterator>(iter_));
  }
};
