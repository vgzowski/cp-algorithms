const int N = 3e5 + 100; /// Can perform a[i] += x, a[i] = min(a[i], x) updates
struct node {
	ll mx, cmx, smx;
	ll s, add;
	node() {}
	node(ll x) : s(x) { mx = x, cmx = 1, smx = -inf; add = 0; }
} t[4 * N];
inline node merge(const node &a, const node &b) {
	node res;
	res.add = 0;
	res.s = a.s + b.s;
	res.mx = max(a.mx, b.mx);
	res.cmx = (res.mx == a.mx ? a.cmx : 0) + (res.mx == b.mx ? b.cmx : 0);
	res.smx = max(a.smx, b.smx);
	if (a.mx != res.mx) chmax(res.smx, a.mx);
	if (b.mx != res.mx) chmax(res.smx, b.mx);
	return res;
}
inline void mg(int v) { t[v] = merge(t[2 * v + 1], t[2 * v + 2]); }
inline void cmin(int v, ll x) {
	if (t[v].mx > x) {
		t[v].s -= (t[v].mx - x) * t[v].cmx;
		t[v].mx = x;
	}
}
inline void ad(int v, int vl, int vr, ll x) {
	t[v].mx += x;
	if (t[v].smx != -inf) t[v].smx += x;
	t[v].s += (vr - vl + 1) * 1ll * x;
}
inline void push(int v, int vl, int vr) {
	int m = vl + vr >> 1;
	ad(2 * v + 1, vl, m, t[v].add); t[2 * v + 1].add += t[v].add;
	ad(2 * v + 2, m + 1, vr, t[v].add); t[2 * v + 2].add += t[v].add;
	for (auto u : {2 * v + 1, 2 * v + 2}) cmin(u, t[v].mx);
	t[v].add = 0;
}
int a[N];
inline void build(int v, int vl, int vr) {
	if (vl == vr) return void(t[v] = node(a[vl]));
	int m = vl + vr >> 1;
	build(2 * v + 1, vl, m);
	build(2 * v + 2, m + 1, vr);
	mg(v);
}
inline ll get(int v, int vl, int vr, int l, int r) {
	if (l > r) return 0;
	else if (vl == l && vr == r) return t[v].s;
	push(v, vl, vr);
	int m = vl + vr >> 1;
	ll a = get(2 * v + 1, vl, m, l, min(r, m));
	ll b = get(2 * v + 2, m + 1, vr, max(l, m + 1), r);
	return a + b;
}
inline void uadd(int v, int vl, int vr, int l, int r, ll x) {
	if (l > r) return;
	else if (vl == l && vr == r) {
		ad(v, vl, vr, x); t[v].add += x;
		return;
	}
	push(v, vl, vr);
	int m = vl + vr >> 1;
	uadd(2 * v + 1, vl, m, l, min(r, m), x);
	uadd(2 * v + 2, m + 1, vr, max(l, m + 1), r, x);
	mg(v);
}
inline void umin(int v, int vl, int vr, int l, int r, ll x) {
	if (l > r || t[v].mx <= x) return;
	else if (vl == l && vr == r && t[v].smx < x) {
		cmin(v, x);
		return;
	}
	push(v, vl, vr);
	int m = vl + vr >> 1;
	umin(2 * v + 1, vl, m, l, min(r, m), x);
	umin(2 * v + 2, m + 1, vr, max(l, m + 1), r, x);
	mg(v);
}




/**
-------------------------------------
----------------
-------------------------------------
*/

/**
a[i] = min(a[i], x)
a[i] = max(a[i], x)
a[i] = x;
a[i] += x;
sum(a[i])
min(a[i])
max(a[i])
gcd(a[i])
*/
const int N = 2e5 + 100;
struct node {
	ll mx, cmx, smx;
	ll mn, cmn, smn;
	ll dgcd;
	ll s, add;
	node() {}
	node(ll x) : s(x) {
		mx = x, cmx = 1, smx = -inf;
		mn = x, cmn = 1, smn = +inf;
		dgcd = 0;
		add = 0;
	}
} t[4 * N];
inline node merge(const node &a, const node &b) {
	node res;
	res.add = 0;
	res.s = a.s + b.s;

	res.mx = max(a.mx, b.mx);
	res.cmx = (res.mx == a.mx ? a.cmx : 0) + (res.mx == b.mx ? b.cmx : 0);
	res.smx = max(a.smx, b.smx);
	if (a.mx != res.mx) chmax(res.smx, a.mx);
	if (b.mx != res.mx) chmax(res.smx, b.mx);

	res.mn = min(a.mn, b.mn);
	res.cmn = (res.mn == a.mn ? a.cmn : 0) + (res.mn == b.mn ? b.cmn : 0);
	res.smn = min(a.smn, b.smn);
	if (a.mn != res.mn) chmin(res.smn, a.mn);
	if (b.mn != res.mn) chmin(res.smn, b.mn);

	res.dgcd = gcd(a.dgcd, b.dgcd);
	if (a.smx > a.mn && b.smx > b.mn) res.dgcd = gcd(res.dgcd, abs(a.smx - b.smx));
	
	ll any = -1;
	if (a.smx > a.mn) any = a.smx;
	else if (b.smx > b.mn) any = b.smx;
	for (ll val : {a.mx, b.mx, a.mn, b.mn, a.smx, b.smx, a.smn, b.smn}) {
		if (val > res.mn && val < res.mx) {
			if (~any) res.dgcd = gcd(res.dgcd, abs(any - val));
			else any = val;
		}
	}
	return res;
}
inline void mg(int v) { t[v] = merge(t[2 * v + 1], t[2 * v + 2]); }
inline ll trgcd(node a) {
	if (a.smx == -inf) return a.mx;
	else if (a.smx == a.mn) return gcd(a.mx, a.mn);
	else {
		ll res = gcd(a.smx - a.mn, a.mx - a.smx);
		res = gcd(res, a.dgcd);
		return gcd(res, a.mx);
	}
}
inline void cmin(int v, ll x) {
	if (t[v].mx > x) {
		t[v].s -= (t[v].mx - x) * t[v].cmx;
		if (t[v].mn == t[v].mx) t[v].mn = x;
		if (t[v].smn == t[v].mx) t[v].smn = x;
		t[v].mx = x;
	}
}
inline void cmax(int v, ll x) {
	if (t[v].mn < x) {
		t[v].s += (x - t[v].mn) * t[v].cmn;
		if (t[v].mn == t[v].mx) t[v].mx = x;
		if (t[v].smx == t[v].mn) t[v].smx = x;
		t[v].mn = x;
	}
}
inline void ad(int v, int vl, int vr, ll x) {
	t[v].s += (vr - vl + 1) * 1ll * x;
	t[v].mx += x, t[v].mn += x;
	t[v].add += x;
	if (t[v].smx != -inf) t[v].smx += x;
	if (t[v].smn != +inf) t[v].smn += x;
}
inline void push(int v, int vl, int vr) {
	int m = vl + vr >> 1;
	ad(2 * v + 1, vl, m, t[v].add);
	ad(2 * v + 2, m + 1, vr, t[v].add);
	cmin(2 * v + 1, t[v].mx);
	cmin(2 * v + 2, t[v].mx);
	cmax(2 * v + 1, t[v].mn);
	cmax(2 * v + 2, t[v].mn);
	t[v].add = 0;
}
int a[N];
inline void build(int v, int vl, int vr) {
	if (vl == vr) return void(t[v] = node(a[vl]));
	int m = vl + vr >> 1;
	build(2 * v + 1, vl, m);
	build(2 * v + 2, m + 1, vr);
	mg(v);
}
inline node get(int v, int vl, int vr, int l, int r) {
	if (vl == l && vr == r) return t[v];
	push(v, vl, vr);
	int m = vl + vr >> 1;
	if (r <= m) return get(2 * v + 1, vl, m, l, r);
	else if (l > m) return get(2 * v + 2, m + 1, vr, l, r);
	else {
		node a = get(2 * v + 1, vl, m, l, m);
		node b = get(2 * v + 2, m + 1, vr, m + 1, r);
		return merge(a, b);
	}
}
inline void uadd(int v, int vl, int vr, int l, int r, ll x) {
	if (l > r) return;
	else if (vl == l && vr == r) {
		ad(v, vl, vr, x);
		return;
	}
	push(v, vl, vr);
	int m = vl + vr >> 1;
	uadd(2 * v + 1, vl, m, l, min(r, m), x);
	uadd(2 * v + 2, m + 1, vr, max(l, m + 1), r, x);
	mg(v);
}
inline void umin(int v, int vl, int vr, int l, int r, ll x) {
	if (l > r || t[v].mx <= x) return;
	else if (vl == l && vr == r && t[v].smx < x) {
		cmin(v, x);
		return;
	}
	push(v, vl, vr);
	int m = vl + vr >> 1;
	umin(2 * v + 1, vl, m, l, min(r, m), x);
	umin(2 * v + 2, m + 1, vr, max(l, m + 1), r, x);
	mg(v);
}
inline void umax(int v, int vl, int vr, int l, int r, ll x) {
	if (l > r || t[v].mn >= x) return;
	else if (vl == l && vr == r && t[v].smn > x) {
		cmax(v, x);
		return;
	}
	push(v, vl, vr);
	int m = vl + vr >> 1;
	umax(2 * v + 1, vl, m, l, min(r, m), x);
	umax(2 * v + 2, m + 1, vr, max(l, m + 1), r, x);
	mg(v);
}
