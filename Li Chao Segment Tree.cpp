/// Usual Li Chao (coordiantes compressed)
map <ll, int> cp; /// used to compress
ll dc[N]; /// reverse compression {for (auto &[a, b] : cp) dc[b] = a} to get the actual values
struct line {
	ll k = 0, b = 0;
	line(ll k = 0, ll b = inf) : k(k), b(b) {}
	inline ll operator()(const ll &x) { return k * dc[x] + b; }
} t[4 * N];
inline void upd(int v, int l, int r, line f) {
	if (l + 1 == r) { t[v] = (t[v](l) < f(l) ? t[v] : f); return; }
	int m = l + r >> 1;
	bool le = (f(l) < t[v](l));
	bool me = (f(m) < t[v](m));
 
	if (me) swap(t[v], f);
 
	if (le != me) upd(2 * v + 1, l, m, f);
	else upd(2 * v + 2, m, r, f);
}
inline ll get(int v, int l, int r, ll x) {
	if (l + 1 == r) return t[v](x);
	int m = l + r >> 1;
	if (x < m) return min(t[v](x), get(2 * v + 1, l, m, x));
	else return min(t[v](x), get(2 * v + 2, m, r, x));
}

/*
----------------------------------------------------------
*/

/// Implicit Li Chao
struct line {
	ll k, b;
	line() : k(0), b(-inf) {}
	line(ll k, ll b) : k(k), b(b) {}
	inline ll operator()(const ll &x) { return k * x + b; }
};
struct node {
	node *l = nullptr;
	node *r = nullptr;
	line f;
	node() {}
	node(line f) : f(f) {}
};
inline void upd(node *&v, ll l, ll r, line f) {
	if (!v) return void(v = new node(f));
	if (l == r) return void(v->f = (f(l) > v->f(l) ? f : v->f));
	ll m = l + r >> 1;
	bool le = f(l) > v->f(l);
	bool me = f(m) > v->f(m);
	if (me) swap(f, v->f);
	if (le != me) upd(v->l, l, m, f);
	else upd(v->r, m + 1, r, f);
}
inline ll get(node *v, ll l, ll r, ll x) {
	if (!v) return -inf;
	ll re = v->f(x);
	if (l == r) return re;
	ll m = l + r >> 1;
	if (x <= m) return max(re, get(v->l, l, m, x));
	else return max(re, get(v->r, m + 1, r, x));
}

/*
----------------------------------------------------------
*/

/// Persistent Li Chao
map <int, int> cp;
ll decomp[N]; /// same way to compress
constexpr int N = 1e5 + 10;
constexpr int M = 4 * N + N * ((int)log2(N) + 2);
struct line {
	ll k, b;
	line() { k = 0, b = 1e9; }
	line(ll k, ll b) : k(k), b(b) {}
	inline ll operator()(const int &x) { return k * decomp[x] + b; }
	inline bool operator == (const line &other) { return (k == other.k && b == other.b); }
};

struct node { int l, r; line f; };
node t[M]; int w = 0;
inline int node(line f) { int v = w++; t[v] = {-1, -1, f}; return v; }
inline int node(int l, int r, line f) { int v = w++; t[v] = {l, r, f}; return v; }
inline int build(int l, int r) {
	if (l + 1 == r) return node(line(0, 0));
	int m = l + r >> 1;
	return node(build(l, m), build(m, r), line(0, 0));
}
inline int upd(int v, int l, int r, line f) {
	if (l + 1 == r) {
		line nw = (t[v].f(l) < f(l) ? t[v].f : f);
		return node(nw);
	}
	int m = l + r >> 1;
	bool le = (f(l) < t[v].f(l));
	bool me = (f(m) < t[v].f(m));
	if (le != me) {
		line prop = (me ? t[v].f : f);
		int left = upd(t[v].l, l, m, prop);
		return node(left, t[v].r, (prop == t[v].f ? f : t[v].f));
	}
	else {
		line prop = (me ? t[v].f : f);
		int right = upd(t[v].r, m, r, prop);
		return node(t[v].l, right, (prop == t[v].f ? f : t[v].f));
	}
}
inline ll get(int v, int l, int r, int x) {
	if (l + 1 == r) { return t[v].f(x); }
	int m = l + r >> 1;
	if (x < m) return min(t[v].f(x), get(t[v].l, l, m, x));
	else return min(t[v].f(x), get(t[v].r, m, r, x));
}
