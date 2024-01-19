// no_concepts

#include <algorithm>
#include <iostream>
#include <vector>

using std::cin;
using std::cout;
using std::make_pair;
using std::max;
using std::pair;
using std::sort;
using std::string;
using std::vector;

const long long kAlphPower = 26;
long long size = 1;
long long end = 0;
long long ans = 0;

struct Node {
  vector<long long> to = vector<long long>(kAlphPower, -1);
  long long link = 0;
  long long len = -1;
  long long r_first = -1;
  long long r_second = -1;

  Node(long long len, long long index_r)
      : len(len), r_first(index_r), r_second(index_r) {}

  Node() = default;
};

void Increase(vector<Node>& suf_auto, long long ind) {
  if (ind != 0) {
    long long link = suf_auto[ind].link;
    suf_auto[link].r_second =
        max(suf_auto[link].r_second, suf_auto[ind].r_second);
    long long max_length = std::min(
        suf_auto[ind].len, suf_auto[ind].r_second - suf_auto[ind].r_first);
    long long min_length = suf_auto[link].len + 1;
    if (max_length > 0) {
      ans += max(0LL, max_length - min_length + 1);
    }
  }
}

void Add(vector<Node>& suf_auto, long long letter, long long len) {
  long long sc = size++;
  suf_auto[sc] = Node(len, len);
  long long node_p = end;
  while (node_p >= 0 and suf_auto[node_p].to[letter] == -1) {
    suf_auto[node_p].to[letter] = sc;
    node_p = suf_auto[node_p].link;
  }
  if (node_p != -1) {
    long long node_q = suf_auto[node_p].to[letter];
    if (suf_auto[node_p].len + 1 == suf_auto[node_q].len) {
      suf_auto[sc].link = node_q;
    } else {
      long long clone = size++;
      suf_auto[clone] = suf_auto[node_q];
      suf_auto[clone].r_second = len;
      suf_auto[clone].len = suf_auto[node_p].len + 1;
      suf_auto[node_q].link = clone;
      suf_auto[sc].link = clone;
      while (node_p >= 0 and suf_auto[node_p].to[letter] == node_q) {
        suf_auto[node_p].to[letter] = clone;
        node_p = suf_auto[node_p].link;
      }
    }
  }
  end = sc;
}

int main() {
  string basic_string;
  cin >> basic_string;
  long long string_length = basic_string.length();
  vector<Node> suf_auto(2 * string_length);
  suf_auto[0].len = 0;
  suf_auto[0].link = -1;
  for (long long i = 0; i < string_length; ++i) {
    Add(suf_auto, basic_string[i] - 'a', i + 1);
  }
  vector<pair<long long, long long>> all_nodes(size);
  for (int i = 0; i < size; ++i) {
    all_nodes[i] = make_pair(-suf_auto[i].len, i);
  }
  sort(all_nodes.begin(), all_nodes.end());
  for (int i = 0; i < size; ++i) {
    Increase(suf_auto, all_nodes[i].second);
  }
  cout << ans;
}
