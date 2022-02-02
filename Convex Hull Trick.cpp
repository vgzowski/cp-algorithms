/* - Offline CHT for minimum - */
struct ln { ll k, b; inline ll operator()(const ll &x) const { return k * x + b; } };
inline bool operator < (const ln &a, const ln &b) { return (a.k > b.k || (a.k == b.k && a.b < b.b)); }
inline bool bad(const ln &a, const ln &b, const ln &c) { return (ld)(b.b - a.b) * (a.k - c.k) >= (ld)(c.b - a.b) * (a.k - b.k); }

vector<ln> s;
inline void add(ln cur) {
	if (s.empty()) return void(s.pb(cur));
	while (sz(s) >= 2) {
		ln a = s[sz(s) - 2];
		ln b = s[sz(s) - 1];
		if (b.k == cur.k) return;
		if (bad(a, b, cur)) s.pop_back();
		else break;
	}
	if (s.back().k == cur.k && s.back().b >= cur.b) s.pop_back();
	s.pb(cur);
}
int ptr = 0;
inline ll answer(ll x) { chmin(ptr, sz(s) - 1); /// Can be used if query values are sorted
	while (ptr - 1 >= 0 && s[ptr - 1](x) < s[ptr](x)) --ptr;
	while (ptr + 1 < sz(s) && s[ptr](x) > s[ptr + 1](x)) ++ptr;
	return s[ptr](x);
}
inline ll answerdix(ll x) {
	int l = 0, r = sz(s) - 2, mid, ans = -1;
	while (l <= r) { mid = l + r >> 1; s[mid](x) > s[mid + 1](x) ? ans = mid, l = mid + 1 : r = mid - 1; }
	return s[ans + 1](x);
}

/* - Online CHT for maximum - */
struct cht {
	struct line {
		ld k, b, x, q;
		line() : x(-inf), k(0), b(0), q(0) {}
		line(ld k, ld b) : x(-inf), k(k), b(b), q(0) {}

		inline ld eval(ld X) const { return k * X + b; }
		inline bool parallel(const line &cur) const { return k == cur.k; }
		inline ld isec(line cur) const { return (parallel(cur) ? inf : ((b - cur.b) / (cur.k - k))); }
		inline bool operator < (const line &a) const {
			if (a.q) return x < a.x;
			else return k < a.k;
		}
	};

	set <line> st;
	typedef set <line>::iterator iter;

	inline bool hp(iter x) { return x != st.begin(); }
	inline bool hn(iter x) { return (x != st.end() && next(x) != st.end()); }
	inline bool bad(const line &a, const line &b, const line &c) { return (a.isec(b) >= b.isec(c)); }
	inline bool bad(iter x) {
		if (!hp(x) || !hn(x)) return 0;
		return bad(*prev(x), *x, *next(x));
	}

	inline iter update(iter x) {
		if (!hp(x)) return x;
		line f = *x; f.x = x->isec(*prev(x));
		x = st.erase(x); return (st.insert(x, f));
	}
	inline void add(ld k, ld b) {
		line cur(k, b); auto it = st.lower_bound(cur);
		if (it != st.end() && cur.parallel(*it)) {
			if (cur.b > it->b) it = st.erase(it);
			else return;
		}
		it = st.insert(it, cur);
		if (bad(it)) return (void)(st.erase(it));
		while (hp(it) && bad(prev(it))) st.erase(prev(it));
		while (hn(it) && bad(next(it))) st.erase(next(it));
		it = update(it);
		if (hp(it)) update(prev(it));
		if (hn(it)) update(next(it));
	}
	inline ld eval(ld x) {
		line cur; cur.x = x; cur.q = 1;
		auto it = *--st.lower_bound(cur);
		return it.eval(x);
	}
};
