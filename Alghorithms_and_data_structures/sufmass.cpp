#include <math.h>

#include <iostream>
#include <vector>

using std::cin;
using std::cout;
using std::pair;
using std::string;
using std::vector;

int string_size;

template <>
int& vector<int>::operator[](size_t index) noexcept {
  return *(this->_M_impl._M_start + (index % string_size));
}

struct Para {
  int first;
  int second;
  int index;
};

bool operator!=(Para& para1, Para& para2) {
  return para1.first != para2.first or para1.second != para2.second;
}

void StableCountingSort(vector<Para>& array_a, int size_n) {
  vector<int> array_c(size_n);
  for (int i = 0; i < size_n; ++i) {
    array_c[i] = 0;
  }
  for (int i = 0; i < size_n; ++i) {
    ++array_c[array_a[i].second];
  }
  for (int i = 1; i < size_n; ++i) {
    array_c[i] += array_c[i - 1];
  }
  vector<Para> vec_b(size_n);
  for (int i = size_n - 1; i >= 0; --i) {
    vec_b[--array_c[array_a[i].second]] = array_a[i];
  }
  std::swap(vec_b, array_a);
  for (int i = 0; i < size_n; ++i) {
    array_c[i] = 0;
  }
  for (int i = 0; i < size_n; ++i) {
    ++array_c[array_a[i].first];
  }
  for (int i = 1; i < size_n; ++i) {
    array_c[i] += array_c[i - 1];
  }
  for (int i = size_n - 1; i >= 0; --i) {
    vec_b[--array_c[array_a[i].first]] = array_a[i];
  }
  std::swap(vec_b, array_a);
}

void SufMass(const string& str, vector<int>& suf_mas) {
  string_size = str.size();
  const int kMagicNumber = 32;
  int cnt[kMagicNumber];
  for (int i = 0; i < kMagicNumber; ++i) {
    cnt[i] = 0;
  }
  for (int i = 0; i < string_size; ++i) {
    ++cnt[str[i] - '`'];
  }
  for (int i = 1; i < kMagicNumber; ++i) {
    cnt[i] += cnt[i - 1];
  }
  vector<int> array_p(string_size);
  vector<int> array_c(string_size);
  for (int i = string_size - 1; i >= 0; --i) {
    array_p[--cnt[str[i] - '`']] = i;
  }
  int x = 0;
  for (int i = 0; i < string_size; ++i) {
    if (i != 0 and str[array_p[i]] != str[array_p[i - 1]]) {
      ++x;
    }
    array_c[array_p[i]] = x;
  }
  int ch = 1;
  for (int k = 1; k <= int(log(string_size) / log(2)) + 1; ++k) {
    vector<Para> vec(string_size);
    for (int i = 0; i < string_size; ++i) {
      vec[i].first = array_c[i];
      vec[i].second = array_c[i + ch];
      vec[i].index = i;
    }
    StableCountingSort(vec, string_size);
    for (int i = 0; i < string_size; ++i) {
      array_p[i] = vec[i].index;
    }
    x = 0;
    for (int i = 0; i < string_size; ++i) {
      if (i != 0 and vec[i] != vec[i - 1]) {
        ++x;
      }
      array_c[array_p[i]] = x;
    }
    if (x == string_size - 1) {
      for (int i = 0; i < string_size; ++i) {
        suf_mas[i] = array_p[i];
      }
      return;
    }
    ch *= 2;
  }
}

int main() {
  string basic_string;
  cin >> basic_string;
  vector<int> suf_mas(basic_string.size());
  SufMass(basic_string, suf_mas);
  for (size_t i = 0; i < basic_string.size(); ++i) {
    cout << basic_string[(suf_mas[i] + basic_string.size() - 1) %
                         basic_string.size()];
  }
}
