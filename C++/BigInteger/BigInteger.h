#include <iostream>
#include <vector>

class BigInteger {
 public:
  BigInteger() = default;
  BigInteger(std::string s);
  BigInteger(long long x, size_t i);
  BigInteger(long long x);
  BigInteger& operator++();
  BigInteger operator++(int);
  BigInteger& operator--();
  BigInteger operator--(int);
  BigInteger operator-();
  BigInteger& operator+=(const BigInteger& s);
  BigInteger& operator-=(const BigInteger& s);
  BigInteger& operator*=(const BigInteger& b);
  BigInteger& operator/=(const BigInteger& b);
  BigInteger& operator%=(const BigInteger& b);
  explicit operator bool() const;
  std::string toString() const;
  std::string ToFormattedString(int x) const;
  std::string FirstDigitToFormattedString() const;
  friend bool operator<(const BigInteger& a, const BigInteger& b);
  friend bool operator==(const BigInteger& a, const BigInteger& b);
  int KBase = 1'000'000'000;
  int KPow = 9;

 private:
  void FastMinus(const BigInteger& b, size_t indext);
  bool FastCompare(const BigInteger& b, size_t indext);
  bool AbsCompare(const BigInteger& a, const BigInteger& b);
  void ChangeSign(size_t index);
  void Repair(size_t index);
  void DeleteExtraSpaces();
  void Changesign();
  void Swap(BigInteger& s);
  bool Normalize(int x);
  std::vector<int> arr_ = {0};
  bool is_not_negative_ = true;
};

void BigInteger::FastMinus(const BigInteger& b, size_t indext) {
  for (size_t i = 0; i < b.arr_.size(); ++i) {
    if (i + indext < arr_.size()) {
      arr_[i + indext] -= b.arr_[i];
    } else {
      while (i + indext > arr_.size()) {
        arr_.push_back(0);
      }
      arr_.push_back(-b.arr_[i]);
    }
    Repair(i + indext);
  }
}

bool BigInteger::FastCompare(const BigInteger& b, size_t indext) {
  int index = indext;
  if (arr_.size() > b.arr_.size() + indext) {
    return true;
  }
  if (arr_.size() < b.arr_.size() + indext) {
    return false;
  }
  int x = arr_.size() - 1;
  for (int i = x; i >= index; --i) {
    int j = b.arr_.size() + i - x - 1;
    if (arr_[i] < b.arr_[j]) {
      return false;
    } else if (arr_[i] > b.arr_[j]) {
      return true;
    }
  }
  return true;
}

bool BigInteger::AbsCompare(const BigInteger& a, const BigInteger& b) {
  if (a.arr_.size() != b.arr_.size()) {
    return (a.arr_.size() < b.arr_.size());
  }
  for (int i = a.arr_.size() - 1; i >= 0; --i) {
    if (a.arr_[i] != b.arr_[i]) {
      return (a.arr_[i] < b.arr_[i]);
    }
  }
  return true;
}

void BigInteger::ChangeSign(size_t index) {
  for (size_t i = 0; i < index; ++i) {
    arr_[i] = KBase - 1 - arr_[i];
  }
  if (is_not_negative_) {
    ++*this;
  } else {
    --*this;
  }
}

void BigInteger::Repair(size_t index) {
  DeleteExtraSpaces();
  if ((index == arr_.size() - 1) and arr_[index] < 0) {
    is_not_negative_ = not is_not_negative_;
    arr_[index] *= -1;
    arr_[index] -= 1;
    ChangeSign(index);
  }
  if (!Normalize(arr_[index])) {
    int loan = arr_[index] - ((arr_[index] % KBase) + KBase) % KBase;
    loan /= KBase;
    arr_[index] %= KBase;
    arr_[index] += KBase;
    arr_[index] %= KBase;
    if (index + 1 < arr_.size()) {
      arr_[index + 1] += loan;
    } else {
      arr_.push_back(loan);
    }
    Repair(++index);
  }
}

void BigInteger::DeleteExtraSpaces() {
  int n = arr_.size() - 1;
  while (n != 0) {
    if (arr_[n] == 0) {
      arr_.pop_back();
    } else {
      break;
    }
    --n;
  }
  if (arr_.size() == 1 and arr_[0] == 0) {
    is_not_negative_ = true;
  }
}

std::string BigInteger::ToFormattedString(int x) const {
  std::string s;
  while (x != 0) {
    s += std::to_string(x % 10);
    x /= 10;
  }
  while (s.size() < KPow) {
    s.push_back('0');
  }
  std::string ans(s.rbegin(), s.rend());
  return ans;
}

std::string BigInteger::FirstDigitToFormattedString() const {
  std::string s;
  long long x = arr_[0];
  while (x != 0) {
    s += std::to_string(x % 10);
    x /= 10;
  }
  while (s.size() < KPow) {
    s.push_back('0');
  }
  std::string ans(s.rbegin(), s.rend());
  return ans;
}

std::string BigInteger::toString() const {
  std::string s;
  s.reserve(arr_.size() * KPow + 5);
  if (!((arr_.size() == 1) and (arr_[0] == 0)) and (not is_not_negative_)) {
    s.push_back('-');
  }
  int x = 0;
  s += std::to_string(arr_[arr_.size() - 1]);
  for (int i = arr_.size() - 2; i >= 0; --i) {
    x = arr_[i];
    s += ToFormattedString(x);
  }
  if (s == "") {
    s = "0";
  }
  return s;
}

BigInteger::BigInteger(std::string s) {
  arr_.clear();
  if (s[0] == '-') {
    is_not_negative_ = false;
    s = s.substr(1, s.size() - 1);
  }
  for (int i = s.size() - KPow; i > -KPow; i -= KPow) {
    if (i >= 0) {
      arr_.push_back(std::stoi(s.substr(i, KPow)));
    } else {
      arr_.push_back(std::stoi(s.substr(0, KPow + i)));
    }
  }
}

BigInteger::BigInteger(long long x, size_t i) : arr_(std::vector<int>(i, 0)) {
  arr_.push_back(x % KBase);
  if (x / KBase != 0) {
    arr_.push_back(x / KBase);
  }
}

BigInteger::BigInteger(long long int x) {
  arr_.clear();
  is_not_negative_ = true;
  if (x < 0) {
    is_not_negative_ = false;
    x *= -1;
  }
  while (x > 0) {
    arr_.push_back(x % KBase);
    x /= KBase;
  }
  if (arr_.size() == 0) {
    arr_.push_back(0);
  }
}

BigInteger& BigInteger::operator++() {
  if (is_not_negative_) {
    arr_[0] += 1;
  } else {
    arr_[0] -= 1;
  }
  Repair(0);
  return *this;
}

BigInteger BigInteger::operator++(int) {
  BigInteger ans = *this;
  ++*this;
  return ans;
}

BigInteger& BigInteger::operator--() {
  if (is_not_negative_) {
    arr_[0] -= 1;
  } else {
    arr_[0] += 1;
  }
  Repair(0);
  return *this;
}

BigInteger BigInteger::operator--(int) {
  BigInteger ans = *this;
  --*this;
  return ans;
}

BigInteger& BigInteger::operator+=(const BigInteger& s) {
  arr_.reserve(std::max(arr_.size(), s.arr_.size()) + 2);
  int ch = 1;
  if (is_not_negative_ != s.is_not_negative_) {
    ch = -1;
  }
  for (size_t i = 0; i < s.arr_.size(); ++i) {
    if (i < arr_.size()) {
      arr_[i] += ch * s.arr_[i];
    } else {
      while (i > arr_.size()) {
        arr_.push_back(0);
      }
      arr_.push_back(ch * s.arr_[i]);
    }
    Repair(i);
  }
  return *this;
}

BigInteger& BigInteger::operator-=(const BigInteger& s) {
  arr_.reserve(std::max(arr_.size(), s.arr_.size()) + 2);
  int ch = 1;
  if (is_not_negative_ != s.is_not_negative_) {
    ch = -1;
  }
  for (size_t i = 0; i < s.arr_.size(); ++i) {
    if (i < arr_.size()) {
      arr_[i] -= ch * s.arr_[i];
    } else {
      while (i > arr_.size()) {
        arr_.push_back(0);
      }
      arr_.push_back(-ch * s.arr_[i]);
    }
    Repair(i);
  }
  return *this;
}

BigInteger& BigInteger::operator*=(const BigInteger& b) {
  is_not_negative_ = (is_not_negative_ == b.is_not_negative_);
  int arr1_size = arr_.size() - 1;
  int arr2_size = b.arr_.size() - 1;
  std::string null_string((arr2_size + arr1_size) * KPow, '0');
  long long bigger_digit, smaller_digit;
  bigger_digit = arr_[arr1_size];
  bigger_digit *= b.arr_[arr2_size];
  BigInteger a(bigger_digit, arr2_size + arr1_size);
  a.is_not_negative_ = is_not_negative_;
  Swap(a);
  for (size_t j = 0; j < b.arr_.size(); ++j) {
    if (b.arr_[j]) {
      for (size_t i = 0; i < a.arr_.size(); ++i) {
        if (!((j == b.arr_.size() - 1) and (i == a.arr_.size() - 1))) {
          bigger_digit = a.arr_[i];
          bigger_digit *= b.arr_[j];
          smaller_digit = bigger_digit % KBase;
          bigger_digit /= KBase;
          arr_[i + j] += smaller_digit;
          arr_[i + j + 1] += bigger_digit;
          Repair(i + j);
          Repair(i + j + 1);
        }
      }
    }
  }
  return *this;
}

BigInteger& BigInteger::operator/=(const BigInteger& b) {
  std::vector<int> result;
  int savesign = 1;
  if (is_not_negative_ != b.is_not_negative_) {
    savesign = -1;
  }
  is_not_negative_ = true;
  BigInteger b_copy(b);
  int index = static_cast<int>(arr_.size() - b.arr_.size());
  long long number, left, right, medium;
  while (index >= 0 and AbsCompare(b, *this)) {
    number = arr_[arr_.size() - 1];
    if (index + b.arr_.size() < arr_.size()) {
      number *= KBase;
      number += arr_[arr_.size() - 2];
    }
    left = number / (b.arr_[b.arr_.size() - 1] + 1);
    right = number / b.arr_[b.arr_.size() - 1] + 1;
    while (left + 1 < right) {
      medium = (left + right) / 2;
      b_copy *= medium;
      if (FastCompare(b_copy, index)) {
        left = medium;
      } else {
        right = medium;
      }
      b_copy = b;
    }
    if (left != 0) {
      b_copy *= left;
      FastMinus(b_copy, index);
      b_copy = b;
    }
    result.push_back(left);
    --index;
  }
  while (--index >= -1) {
    result.push_back(0);
  }
  if (result.size() == 0) {
    result.push_back(0);
  }
  arr_.clear();
  for (int z = static_cast<int>(result.size()) - 1; z >= 0; --z) {
    arr_.push_back(result[z]);
  }
  DeleteExtraSpaces();
  is_not_negative_ = (savesign == 1);
  return *this;
}

BigInteger& BigInteger::operator%=(const BigInteger& b) {
  int savesign = -1;
  if (is_not_negative_) {
    savesign = 1;
  }
  is_not_negative_ = true;
  BigInteger copyb(b);
  int index = static_cast<int>(arr_.size() - b.arr_.size());
  long long l, r, x, ch;
  while (index >= 0 and AbsCompare(b, *this)) {
    ch = arr_[arr_.size() - 1];
    if (index + b.arr_.size() < arr_.size()) {
      ch *= KBase;
      ch += arr_[arr_.size() - 2];
    }
    if (b.arr_[b.arr_.size() - 1] != 0 and b.arr_[b.arr_.size() - 1] != -1) {
      l = ch / (b.arr_[b.arr_.size() - 1] + 1);
      r = ch / b.arr_[b.arr_.size() - 1] + 1;
    } else {
      l = 0;
      r = KBase;
    }
    while (l + 1 < r) {
      x = (l + r) / 2;
      copyb *= x;
      if (FastCompare(copyb, index)) {
        l = x;
      } else {
        r = x;
      }
      copyb = b;
    }
    if (l != 0) {
      copyb *= l;
      FastMinus(copyb, index);
      copyb = b;
    }
    --index;
  }
  is_not_negative_ = (savesign == 1);
  return *this;
}

BigInteger operator+(const BigInteger& a, const BigInteger& b) {
  BigInteger result{a};
  result += b;
  return result;
}

BigInteger BigInteger::operator-() {
  if (this) {
    this->Changesign();
  }
  return *this;
}

BigInteger operator*(const BigInteger& a, const BigInteger& b) {
  BigInteger result{a};
  result *= b;
  return result;
}

BigInteger operator/(const BigInteger& a, const BigInteger& b) {
  BigInteger result{a};
  result /= b;
  return result;
}

BigInteger operator%(const BigInteger& a, const BigInteger& b) {
  BigInteger result{a};
  result %= b;
  return result;
}

BigInteger operator-(const BigInteger& a, const BigInteger& b) {
  BigInteger result{a};
  result -= b;
  return result;
}

bool operator<(const BigInteger& a, const BigInteger& b) {
  if (a.is_not_negative_ != b.is_not_negative_) {
    return a.is_not_negative_ < b.is_not_negative_;
  }
  bool change = not a.is_not_negative_;
  if (a.arr_.size() != b.arr_.size()) {
    return (change xor (a.arr_.size() < b.arr_.size()));
  }
  for (int i = a.arr_.size() - 1; i >= 0; --i) {
    if (a.arr_[i] != b.arr_[i]) {
      return (change xor (a.arr_[i] < b.arr_[i]));
    }
  }
  return false;
}

bool operator>(const BigInteger& a, const BigInteger& b) {
  return b < a;
}

bool operator>=(const BigInteger& a, const BigInteger& b) {
  return !(a < b);
}

bool operator<=(const BigInteger& a, const BigInteger& b) {
  return !(b < a);
}

bool operator==(const BigInteger& a, const BigInteger& b) {
  if (a.is_not_negative_ != b.is_not_negative_) {
    return false;
  }
  if (a.arr_.size() != b.arr_.size()) {
    return false;
  }
  for (int i = a.arr_.size() - 1; i >= 0; --i) {
    if (a.arr_[i] != b.arr_[i]) {
      return false;
    }
  }
  return true;
}

void BigInteger::Changesign() {
  is_not_negative_ = not is_not_negative_;
}

BigInteger::operator bool() const {
  return (arr_.size() != 1 or arr_[0] != 0);
}

void BigInteger::Swap(BigInteger& s) {
  std::swap(arr_, s.arr_);
  std::swap(is_not_negative_, s.is_not_negative_);
}

bool BigInteger::Normalize(int x) {
  return (x >= 0) and (x < KBase);
}

bool operator!=(const BigInteger& a, const BigInteger& b) {
  return !(a == b);
}

std::ostream& operator<<(std::ostream& out, const BigInteger& s) {
  out << s.toString();
  return out;
}

std::istream& operator>>(std::istream& in, BigInteger& a) {
  std::string s;
  in >> s;
  BigInteger b(s);
  a = b;
  return in;
}

BigInteger operator "" _bi(unsigned long long val) {
  return BigInteger(val);
}

BigInteger operator "" _bi(const char* val) {
  std::string s(val);
  return BigInteger(s);
}

class Rational {
 public:
  Rational() = default;
  Rational(const BigInteger& x);
  Rational(const BigInteger& a, const BigInteger& b);
  Rational(long long x);
  Rational operator-() const;
  Rational& operator+=(const Rational& s);
  Rational& operator-=(const Rational& s);
  Rational& operator*=(const Rational& s);
  Rational& operator/=(const Rational& s);
  friend bool operator<(const Rational& a, const Rational& b);
  friend bool operator==(const Rational& a, const Rational& b);
  std::string asDecimal(size_t precision = 0) const;
  std::string toString() const;
  explicit operator double() const;
  int KBase = 1'000'000'000;
  int KPow = 9;

 private:
  BigInteger GCD(BigInteger a, BigInteger b);
  void ReduceFraction();
  void Swap(Rational& s);
  int is_not_negative_ = true;
  BigInteger numerator{0};
  BigInteger denominator{1};
};

BigInteger Rational::GCD(BigInteger a, BigInteger b) {
  if (a < 0) {
    a = -a;
  }
  if (b < 0) {
    b = -b;
  }
  int i = 0;
  while (a != 0 and b != 0) {
    if (++i % 2) {
      a %= b;
    } else {
      b %= a;
    }
  }
  return (a + b);
}

Rational operator+(const Rational& a, const Rational& b) {
  Rational result{a};
  result += b;
  return result;
}

Rational operator-(const Rational& a, const Rational& b) {
  Rational result{a};
  result -= b;
  return result;
}

Rational operator*(const Rational& a, const Rational& b) {
  Rational result{a};
  result *= b;
  return result;
}

Rational operator/(const Rational& a, const Rational& b) {
  Rational result{a};
  result /= b;
  return result;
}

bool operator<(const Rational& a, const Rational& b) {
  if (a.is_not_negative_ != b.is_not_negative_) {
    return a.is_not_negative_ < b.is_not_negative_;
  }
  return (a.numerator * b.denominator < b.numerator * a.denominator);
}

bool operator>(const Rational& a, const Rational& b) {
  return b < a;
}

bool operator>=(const Rational& a, const Rational& b) {
  return !(a < b);
}

bool operator<=(const Rational& a, const Rational& b) {
  return !(a > b);
}

bool operator==(const Rational& a, const Rational& b) {
  if (a.is_not_negative_ != b.is_not_negative_) {
    return false;
  }
  return (a.numerator * b.denominator == b.numerator * a.denominator);
}

bool operator!=(const Rational& a, const Rational& b) {
  return !(a == b);
}

void Rational::ReduceFraction() {
  if (denominator != 1) {
    BigInteger x = GCD(numerator, denominator);
    if (x != 1) {
      numerator /= x;
      denominator /= x;
    }
  }
}

std::string Rational::asDecimal(size_t precision) const {
  Rational x = *this;
  BigInteger ch = (x.numerator / x.denominator);
  std::string ans;
  ans.reserve(ch.toString().size() + precision + 5);
  if (x.numerator < 0) {
    ans += '-';
    x.numerator = -x.numerator;
  }
  if (precision == 0 and !ch) {
    return "0";
  }
  ans += ch.toString();
  if (precision != 0) {
    ans += '.';
  }
  for (size_t i = 0; i < precision / KPow; ++i) {
    x.numerator -= ch * x.denominator;
    x.numerator *= KBase;
    ch = (x.numerator / x.denominator);
    ans += ch.FirstDigitToFormattedString();
  }
  int tmp = 1;
  if (precision % KBase) {
    for (size_t i = 0; i < precision % KPow; ++i) {
      tmp *= 10;
    }
    x.numerator -= ch * x.denominator;
    x.numerator *= tmp;
    ch = (x.numerator / x.denominator);
    std::string ss = ch.toString();
    while (ss.length() < precision % KPow) {
      ss = '0' + ss;
    }
    ans += ss;
  }
  return ans;
}

std::string Rational::toString() const {
  std::string s = numerator.toString();
  if (denominator != 1) {
    s += '/' + denominator.toString();
  }
  return s;
}

Rational::Rational(const BigInteger& x) {
  numerator = x;
  is_not_negative_ = (numerator >= 0);
}

Rational::Rational(const BigInteger& a, const BigInteger& b) {
  numerator = a;
  denominator = b;
  if (denominator < 0) {
    denominator = -denominator;
    numerator = -numerator;
  }
  is_not_negative_ = (numerator >= 0);
}

Rational::Rational(long long int x) {
  numerator = x;
  is_not_negative_ = (numerator >= 0);
}

Rational Rational::operator-() const {
  Rational ans = *this;
  ans.numerator = -ans.numerator;
  ans.is_not_negative_ = !ans.is_not_negative_;
  return ans;
}

Rational& Rational::operator+=(const Rational& s) {
  numerator = numerator * s.denominator + denominator * s.numerator;
  if (numerator) {
    denominator *= s.denominator;
    this->ReduceFraction();
    is_not_negative_ = (numerator >= 0);
  } else {
    denominator = 1;
  }
  return *this;
}

Rational& Rational::operator-=(const Rational& s) {
  numerator = numerator * s.denominator - denominator * s.numerator;
  if (numerator) {
    denominator *= s.denominator;
    this->ReduceFraction();
    is_not_negative_ = (numerator >= 0);
  } else {
    denominator = 1;
  }
  return *this;
}

Rational& Rational::operator*=(const Rational& s) {
  numerator *= s.numerator;
  if (numerator) {
    denominator *= s.denominator;
    this->ReduceFraction();
  } else {
    denominator = 1;
  }
  is_not_negative_ = (numerator >= 0);
  return *this;
}

Rational& Rational::operator/=(const Rational& s) {
  numerator *= s.denominator;
  if (numerator) {
    denominator *= s.numerator;
    this->ReduceFraction();
    if (denominator < 0) {
      denominator = -denominator;
      numerator = -numerator;
      is_not_negative_ = (numerator >= 0);
    }
  } else {
    denominator = 1;
    is_not_negative_ = true;
  }
  return *this;
}

void Rational::Swap(Rational& s) {
  std::swap(numerator, s.numerator);
  std::swap(denominator, s.denominator);
  std::swap(is_not_negative_, s.is_not_negative_);
}

Rational::operator double() const {
  return std::stod(asDecimal(KPow * 3));
}
