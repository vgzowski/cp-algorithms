/* --- Recursive Segment-tree-style --- */
/* --- Find (a[l] * a[l + 1] * ... * a[r]) % md --- */
int n, L; // n is actual size of array (after increasing it to a power of 2
const int N = (1 << 20);
int a[N];
int ans[20][N];
inline void build(int v, int vl, int vr, int level, bool left) {
	if (vl == vr) { ans[level][vl] = a[vl] % md; return; }
	int m = vl + vr >> 1;
	build(2 * v, vl, m, level + 1, 1);
	build(2 * v + 1, m + 1, vr, level + 1, 0);

	if (v > 1) {
		if (left) {
			ans[level][vr] = a[vr];
			for (int i = vr - 1; i >= vl; --i) {
				ans[level][i] = mul(ans[level][i + 1], a[i]);
			}
		}
		else {
			ans[level][vl] = a[vl];
			for (int i = vl + 1; i <= vr; ++i) {
				ans[level][i] = mul(ans[level][i - 1], a[i]);
			}
		}
	}
}
inline int get(int l, int r) {
  if (l == r) return a[l];
	int p = 31 - __builtin_clz((l + n) ^ (r + n));
	return mul(ans[L - p][l], ans[L - p][r]);
}

inline void solve() {
	cin >> n >> md;
	for (int i = 0; i < n; ++i) cin >> a[i];
	if (n & (n - 1)) { n = 1 << (32 - __builtin_clz(n)); }
	L = 31 - __builtin_clz(n); // max level of recursion
	build(1, 0, n - 1, 0, 0);
}






/* --- Non-recursive (slower for some reason) --- */
const int N = (1 << 20);

int a[N];
int ans[20][N];
int n, q;
cin >> n >> md >> q;
for (int i = 0; i < n; ++i) cin >> a[i], a[i] %= md;

int s = n, an = n;
if (s & (s - 1)) s = 1 << (32 - __builtin_clz(s));
for (int p = 0, len = 1; (len << 1) <= s; ++p, len <<= 1) {
  for (int l = 0; l < s; l += 2 * len) {
    int r = l + 2 * len - 1;
    int m = l + len - 1;
    ans[p][m] = a[m], ans[p][m + 1] = a[m + 1];
    for (int i = m - 1; i >= l; --i) ans[p][i] = mul(ans[p][i + 1], a[i]);
    for (int i = m + 2; i <= r; ++i) ans[p][i] = mul(ans[p][i - 1], a[i]);
  }
}
auto get = [&](int l, int r) {
  if (l == r) return a[l];
  int p = 31 - __builtin_clz((l + s) ^ (r + s));
  return mul(ans[p][l], ans[p][r]);
};
