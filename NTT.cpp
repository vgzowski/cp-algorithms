#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
#define fi first
#define se second
#define ll long long
#define ld long double
#define sz(x) ((int)(x).size())
#define all(x) 	(x).begin(), (x).end()
#define pb push_back
#define mpp make_pair
#define ve vector
using namespace std;
using namespace __gnu_pbds;
template<class T> using oset = tree<T,null_type,less<T>,rb_tree_tag,tree_order_statistics_node_update>;
const ll inf = 1e18; const int iinf = 1e9;
typedef pair<ll, ll> pll;
typedef pair<int, int> pii;
mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());
template <typename T> inline bool chmin(T& a, T b) { return (a > b ? a = b, 1 : 0); }
template <typename T> inline bool chmax(T& a, T b) { return (a < b ? a = b, 1 : 0); }

constexpr int md = 998244353; // 119 * 2^23 + 1
constexpr int g = 3; // primitive root
constexpr int c = 119; // 119
constexpr int root23 = 15311432; // w_23 = g^c
constexpr int roots[24] = { // w_i
1,998244352,911660635,372528824,
929031873,452798380,922799308,
781712469,476477967,166035806,
258648936,584193783,63912897,
350007156,666702199,968855178,
629671588,24514907,996173970,
363395222,565042129,733596141,
267099868,15311432
};
inline int add(int a, int b) { a += b; if(a>=md){a-=md;} return a; }
inline int sub(int a, int b) { a -= b; if(a<0){a+=md;} return a; }
inline int mul(int a, int b) { return (ll)a * b % md; }
inline int bp(int a, int n) { int res = 1;
	for (; n; n >>= 1, a = mul(a, a)) {
		if (n & 1) res = mul(res, a);
	} return res;
}
inline int inv(int a) { return bp(a, md - 2); }
#define lg2(n) (32 - __builtin_clz((n) - 1))
#define get(n) (1 << (32 - __builtin_clz((n) - 1)))

const int N = 1 << 20;
int w[N], iw[N], rev[N];
inline void precalc() {
	for (int k = 1, p = 0; k < N; k <<= 1, ++p) {
		const int R = roots[p];
		const int iR = bp(R, md - 2);
		w[k] = 1, iw[k] = 1;
		for (int i = k + 1; i < 2 * k; ++i) {
			w[i] = mul(w[i - 1], R);
			iw[i] = mul(iw[i - 1], iR);
		}
	}
}
inline void ntt(ve<int> &a, bool iv = 0) {
	int n = sz(a); int l = lg2(n);
	if (n == 1) return;
	for (int i = 1; i < n; ++i) rev[i] = (rev[i >> 1] | ((i & 1) << l)) >> 1;
	for (int i = 0; i < n; ++i) if (i < rev[i]) swap(a[i], a[rev[i]]);
	for (int k = 1; k < n; k <<= 1) {
		for (int i = 0; i < n; i += 2 * k) {
			for (int j = 0; j < k; ++j) {
				int f = a[i + j];
				int s = mul(iv ? iw[2 * k + j] : w[2 * k + j], a[i + j + k]);
				a[i + j] = add(f, s), a[i + j + k] = sub(f, s);
			}
		}
	}
	if (iv) {
		int o = inv(n);
		for (auto &i : a) i = mul(i, o);
	}
}
inline ve<int> operator * (const ve<int> &A, const ve<int> &B) {
	int s = get(sz(A) + sz(B) - 1);
	ve<int> a = A; a.resize(s);
	ve<int> b = B; b.resize(s);
	ntt(a), ntt(b); for (int i = 0; i < s; ++i) a[i] = mul(a[i], b[i]);
	ntt(a, 1); while (sz(a) > 1 && !a.back()) a.pop_back();
	return a;
}

inline void solve() {
	int n, m;
	cin >> n >> m;
	ve<int> a(n), b(m);
	for (auto &i : a) cin >> i;
	for (auto &i : b) cin >> i;

	auto c = a * b;
	for (auto i : c) cout << i << " ";
}

signed main() { precalc();
	ios_base::sync_with_stdio(false); cin.tie(0); cout.tie(0);
	int q = 1; // cin >> q;
	while (q--) solve();
	cerr << fixed << setprecision(3) << "Time execution: " << (double)clock() / CLOCKS_PER_SEC << endl;
}
