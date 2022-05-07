#include <bits/stdc++.h>
using namespace std;
inline void solve() {
	string s; cin >> s; s += '$'; int n = s.size();
	vector <int> p(n), p2(n), c(n), c2(n), pos(n), lcp(n);

	vector < pair <char, int> > a(n);
	for (int i = 0; i < n; ++i) a[i] = {s[i], i};
	sort(a.begin(), a.end()); for (int i = 0; i < n; ++i) p[i] = a[i].second;
	for (int i = 1; i < n; ++i) { c[p[i]] = c[p[i - 1]]; if (s[p[i]] != s[p[i - 1]]) pos[++c[p[i]]] = i; }
	for (int l = 1; l < n; l <<= 1) {
		for (int i = 0; i < n; ++i) {
			int j = p[i] - l; if (j < 0) j += n;
			p2[pos[c[j]]++] = j;
		} p.swap(p2); c2[p[0]] = 0, pos[0] = 0;
		for (int i = 1; i < n; ++i) {
			c2[p[i]] = c2[p[i - 1]];
			if (c[p[i]] != c[p[i - 1]] || c[(p[i] + l) % n] != c[(p[i - 1] + l) % n]) {
				pos[++c2[p[i]]] = i;
			}
		} c.swap(c2);
	}
	for (int i = 0, k = 0; i < n - 1; ++i) {
		int j = p[c[i] - 1];
		while (s[i + k] == s[j + k]) ++k;
		lcp[c[i]] = k; if (k) --k;
	}
	for (int i = 0; i < n; ++i) cout << p[i] << " " << s.substr(p[i], n - p[i]) << " " << lcp[i] << '\n';
}

/// ----------------------------------------------------------------

const int N = 5e5 + 100; int m = 300;
int p[N], p2[N], c[N], c2[N], cnt[N], lcp[N];
for (int i = 0; i < n; ++i) ++cnt[s[i]];
for (int i = 1; i < m; ++i) cnt[i] += cnt[i - 1];
for (int i = n - 1; ~i; --i) p[--cnt[s[i]]] = i;
for (int i = 1; i < n; ++i) c[p[i]] = c[p[i - 1]] + (s[p[i]] != s[p[i - 1]]);
for (int l = 1; l < n; l <<= 1) {
	memset(cnt, 0, 4 * m);
	for (int i = 0; i < n; ++i) { p[i] -= l; if (p[i] < 0) p[i] += n; }
	for (int i = 0; i < n; ++i) ++cnt[c[i]];
	for (int i = 1; i < m; ++i) cnt[i] += cnt[i - 1];
	for (int i = n - 1; ~i; --i) p2[--cnt[c[p[i]]]] = p[i];
	memcpy(p, p2, 4 * n); c2[p[0]] = 0, m = 1;
	pii prev = mpp(c[p[0]], c[(p[0] + l) % n]);
	for (int i = 1; i < n; ++i) {
		pii cur = mpp(c[p[i]], c[(p[i] + l) % n]);
		m += (prev != cur); c2[p[i]] = m - 1;
		swap(prev, cur);
	}
	memcpy(c, c2, 4 * n);
	if (m == n) break;
}
for (int i = 0, k = 0; i < n - 1; ++i) {
	int j = p[c[i] - 1];
	while (s[i + k] == s[j + k]) ++k;
	lcp[c[i]] = k; if (k) --k;
}
