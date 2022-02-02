int w = 0; st t[N];
inline int node(int x, int vl) { int v = w++; t[v] = {mpp(x, vl), -1, -1}; return v; }
inline int node1(int l, int r) { int v = w++; t[v] = {min(t[l].v, t[r].v), l, r}; return v; }
inline int build(int vl, int vr) {
	if (vl == vr) return node((int)1e9, vl);
	int m = vl + vr >> 1;
	return node1(build(vl, m), build(m + 1, vr));
}
inline int upd(int v, int vl, int vr, int pos, int x) {
	if (vl == vr) return node(x, vl);
	int m = vl + vr >> 1;
	if (pos <= m) return node1(upd(t[v].l, vl, m, pos, x), t[v].r);
	else return node1(t[v].l, upd(t[v].r, m + 1, vr, pos, x));
}
inline pii get(int v, int vl, int vr, int l, int r) {
	if (vl == l && vr == r) return t[v].v;
	int m = vl + vr >> 1;
	if (r <= m) return get(t[v].l, vl, m, l, r);
	else if (l > m) return get(t[v].r, m + 1, vr, l, r);
	else {
		auto a = get(t[v].l, vl, m, l, m);
		auto b = get(t[v].r, m + 1, vr, m + 1, r);
		return min(a, b);
	}
}
