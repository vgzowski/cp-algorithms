/* --- Recursive Segment-tree-style --- */
/* --- Find (a[l] * a[l + 1] * ... * a[r]) % md --- */
const int N = (1 << 20);
int a[N];
int ans[20][N];
inline void build(int vl, int vr, int p) {
	if (vl == vr) return;
	int m = vl + vr >> 1;
	build(vl, m, p - 1);
	build(m + 1, vr, p - 1);
	ans[p][m] = a[m], ans[p][m + 1] = a[m + 1];
	for (int i = m - 1; i >= vl; --i) ans[p][i] = mul(ans[p][i + 1], a[i]);
	for (int i = m + 2; i <= vr; ++i) ans[p][i] = mul(ans[p][i - 1], a[i]);
}
inline int get(int l, int r) {
	if (l == r) return a[l];
	int p = 32 - __builtin_clz(l ^ r); // technicaly (l + sz) ^ (r + sz), but l, r < sz
	return mul(ans[p][l], ans[p][r]);
}

inline void solve() {
	cin >> n >> md;
	for (int i = 0; i < n; ++i) cin >> a[i];
	if (n & (n - 1)) { n = 1 << (32 - __builtin_clz(n)); }
	L = 31 - __builtin_clz(n); // max level of recursion
	build(1, 0, n - 1, 0, 0);
}
// input
void input() {
	int n, q;
	cin >> n >> md >> q;
	for (int i = 0; i < n; ++i) cin >> a[i], a[i] %= md;

	int s = n;
	if (s & (s - 1)) s = 1 << (32 - __builtin_clz(s));
	build(0, s - 1, 31 - __builtin_clz(s));
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
