/// 1D rsq + change single element
const int N = 1e6 + 10;

int n;
int t[2 * N];
inline void build(const ve<int> &a) {
	for (int i = 0; i < n; ++i) t[i + n] = a[i];
	for (int i = n - 1; i; --i) t[i] = t[i << 1] + t[i << 1 | 1];
}
inline void upd(int i, int x) {
	t[i += n] = x;
	while (i > 1) {
		t[i >> 1] = t[i] + t[i ^ 1];
		i >>= 1;
	}
}
inline int get(int l, int r) {
	int res = 0;
	l += n, r += n + 1;
	while (l < r) {
		if (l & 1) res += t[l++];
		if (r & 1) res += t[--r];
		l >>= 1, r >>= 1;
	} return res;
}

/// 2D rmq + change single element
int n, m;
const int N = 501;

struct seg {
	pii t[2 * N];
	inline void build(const ve<pii> &a) {
		for (int i = 0; i < m; ++i) t[i + m] = a[i];
		for (int i = m - 1; i; --i) t[i] = max(t[i << 1], t[i << 1 | 1]);
	}
	inline void upd(int i, pii x) {
		t[i += m] = x;
		while (i > 1) {
			t[i >> 1] = max(t[i], t[i ^ 1]);
			i >>= 1;
		}
	}
	inline pii get(int l, int r) {
		pii res = {-iinf, -iinf};
		l += m, r += m + 1;
		while (l < r) {
			if (l & 1) chmax(res, t[l++]);
			if (r & 1) chmax(res, t[--r]);
			l >>= 1, r >>= 1;
		} return res;
	}
} t[2 * N];
inline void build() { ve<pii> a(m);
	for (int i = 0; i < n; ++i) {
		for (int j = 0; j < m; ++j) a[j] = {iinf, i * m + j};
		t[i + n].build(a);
	}
	for (int i = n - 1; i; --i) {
		for (int j = 0; j < m; ++j) a[j] = max(t[i << 1].t[j + m], t[i << 1 | 1].t[j + m]);
		t[i].build(a);
	}
}
inline void upd(int x, int y, pii val) {
	t[x += n].upd(y, val);
	while (x > 1) {
		pii cur = max(t[x].t[y + m], t[x ^ 1].t[y + m]);
		t[x >> 1].upd(y, cur);
		x >>= 1;
	}
}
inline pii get(int lx, int rx, int ly, int ry) {
	lx += n, rx += n + 1;
	pii res = {-iinf, -iinf};
	while (lx < rx) {
		if (lx & 1) chmax(res, t[lx++].get(ly, ry));
		if (rx & 1) chmax(res, t[--rx].get(ly, ry));
		lx >>= 1, rx >>= 1;
	} return res;
}
