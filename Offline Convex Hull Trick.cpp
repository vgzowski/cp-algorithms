/// CHT for minimum
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
