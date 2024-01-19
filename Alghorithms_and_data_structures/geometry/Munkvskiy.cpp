#include <iostream>
#include <vector>

using std::cin;
using std::cout;
using std::vector;

typedef int unsigned long long;

int BinPow(int x, int pow, int mod) {
  int ans = 1;
  while (pow != 0) {
    if (pow % 2 == 1) {
      ans *= x;
      ans %= mod;
    }
    x *= x;
    x %= mod;
    pow >>= 1;
  }
  return ans;
}

int BinPow(int x, int pow) {
  int ans = 1;
  while (pow != 0) {
    if (pow % 2 == 1) {
      ans *= x;
    }
    x *= x;
    pow >>= 1;
  }
  return ans;
}

int Findm(int p, int& s) {
  --p;
  while (p % 2 == 0) {
    p >>= 1;
    ++s;
  }
  return p;
}

int main() {
  int n, a, p, m, s = 0;
  cin >> n;
  for (int i = 0; i < n; ++i) {
    cin >> a >> p;
    if (p == 2) {
      cout << a << '\n';
      continue;
    }
    if (BinPow(a, (p - 1) / 2, p) == p - 1) {
      cout << "IMPOSSIBLE" << '\n';
      continue;
    }
    if (p % 4 == 3) {
      cout << BinPow(a, (p + 1) / 4, p) << '\n';
      continue;
    }
    m = Findm(p, s);
    int u, v;
    v = BinPow(a, (m + 1) / 2, p);
    u = BinPow(a, m, p);
    int b = (4100) % p;
    while (BinPow(b, (p - 1) / 2, p) != p - 1) {
      ++b;
      b %= p;
    }
    int c = BinPow(b, m, p);
    int ri = 1;
    int ui = u;
    while (ui != p - 1) {
      ui *= ui;
      ui %= p;
      ++ri;
    }
    int ch = BinPow(c, BinPow(2, s - ri - 1), p);
    while (u % p != 1 and ri > 0) {
      v *= ch;
      u %= p;
      ch *= ch;
      u *= ch;
      ch %= p;
      u %= p;
      v %= p;
      --ri;
    }
    cout << v << '\n';
  }
}
