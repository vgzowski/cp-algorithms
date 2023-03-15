/// Usual fenwick
const int N = 1e5 + 5; int f[N];
inline void upd(int i, int x) { i += 3; for (; i < N; i += i & -i) f[i] += x; }
inline int get(int i) { i += 3; int ans = 0; for (; i; i -= i & -i) {ans += f[i];} return ans; }
inline int get(int l, int r) { return get(r) - get(l - 1); }

/*
-----------------------------------------
*/

/// [2d fenwick]
/// Only works if we know quieries beforehead
/// If we don't use hash tables as a 2d array
/// | #define ve vector | is used
struct fenw {
	int n;
	ve<ve<int>> q;
	ve<ve<int>> fe;
	fenw(int N = 0) : n(N + 20) { q.resize(N + 20); fe.resize(N + 20); }
 
	inline void pget(int x, int y) { x += 2; for (; x; x -= x & -x) q[x].pb(y); }
	inline void pupd(int x, int y) { x += 2; for (; x < n; x += x & -x) q[x].pb(y); }
	inline void build() { for (int i = 0; i < n; ++i) { sort(all(q[i])); q[i].erase(unique(all(q[i])), q[i].end()); fe[i].resize(sz(q[i]) + 5); } }
 
	inline void upd(int x, int y, int delta) { x += 2;
		for (; x < n; x += x & -x) {
			int pos = lower_bound(all(q[x]), y) - q[x].begin() + 2;
			for (int j = pos; j < sz(fe[x]); j += j & -j) fe[x][j] += delta;
		}
	}
	inline int get(int x, int y) {
		int ans = 0; x += 2;
		for (; x; x -= x & -x) {
			int pos = lower_bound(all(q[x]), y) - q[x].begin() + 2;
			for (int j = pos; j; j -= j & -j) ans += fe[x][j];
		} return ans;
	}
};

/*
-----------------------------------------
*/

/*
Fenwick with range +=, range sum
basically the idea is that after upd(l, r, x)
if i < l then delate is 0
if l <= i <= r then delta is x * i - x * (l - 1)
if r < i then delta is x * (r - l + 1)

so for l <= i <= r we add x (for the [x * i] term) in first BIT
add x * (l - 1) to l
add x * (-r) to (r + 1) in second BIT
it's not hard to see that by i * get1(i) - get2(i) we get the desired delta
*/

ll f1[N], f2[N];
inline void upd1(int i, ll x) { for (i += 3; i < N; i += i & -i) f1[i] += x; }
inline void upd2(int i, ll x) { for (i += 3; i < N; i += i & -i) f2[i] += x; }
inline ll get1(int i) { ll ans = 0; for (i += 3; i; i -= i & -i) ans += f1[i]; return ans; } 
inline ll get2(int i) { ll ans = 0; for (i += 3; i; i -= i & -i) ans += f2[i]; return ans; } 
inline void upd(int l, int r, ll x) {
	upd1(l, x); upd1(r + 1, -x);
	upd2(l, x * (l - 1)); upd2(r + 1, x * (-r));
}
inline ll get_pref(int i) { return get1(i) * i - get2(i); }
inline ll get(int l, int r) { return get_pref(r) - get_pref(l - 1); }
