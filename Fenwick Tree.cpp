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
