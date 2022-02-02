const int N = 5e5 + 5; int m = 300;
int p[N], p2[N], c[N], c2[N], cnt[N], lcp[N];

res += '$'; /// our initial string
n = res.size();
for (auto i : res) ++cnt[i];
for (int i = 1; i < m; ++i) cnt[i] += cnt[i - 1];
for (int i = n - 1; ~i; --i) p[--cnt[res[i]]] = i;
for (int i = 1; i < n; ++i) c[p[i]] = c[p[i - 1]] + (res[p[i]] != res[p[i - 1]]);

for (int l = 1; l < n; l <<= 1) {
  memset(cnt, 0, 4 * m);
  for (int i = 0; i < n; ++i) { p[i] -= l; if (p[i] < 0) p[i] += n; }
  for (int i = 0; i < n; ++i) ++cnt[c[i]];
  for (int i = 1; i < m; ++i) cnt[i] += cnt[i - 1];
  for (int i = n - 1; ~i; --i) p2[--cnt[c[p[i]]]] = p[i];
  memcpy(p, p2, 4 * n);
  m = 1, c2[p[0]] = 0;
  for (int i = 1; i < n; ++i) {
    if (c[p[i]] != c[p[i - 1]] ||
        c[(p[i] + l) % n] != c[(p[i - 1] + l) % n]) ++m;
    c2[p[i]] = m - 1;
  }
  memcpy(c, c2, 4 * n);
  if (m == n) break;
}
for (int i = 0, k = 0; i < n - 1; ++i) {
  int j = p[c[i] - 1];
  while (res[i + k] == res[j + k]) ++k;
  lcp[c[i]] = k; if (k) --k;
}
