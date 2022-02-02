struct F { int a, b, c; F(int a = -1, int b = 1, int c = 0) : a(a), b(b), c(c) {} };
inline void mg(F& a, const F& b) { if (b.a == -1) { a.b = mul(a.b, b.b); a.c = mul(a.c, b.b); a.c = add(a.c, b.c); } else a = b; }
inline bool operator == (const F &a, const F &b) { return a.a == b.a && a.b == b.b && a.c == b.c; }
struct node {
	int s;
	F push;
	node *l = nullptr;
	node *r = nullptr;
	node(int x = 0) : s(x) {}
};
inline void push(node *v, int vl, int vr) {
	if (!v || v->push == F(-1, 1, 0)) return;
	if (vl != vr) {
		if (!v->l) v->l = new node();
		if (!v->r) v->r = new node();
		mg(v->l->push, v->push);
		mg(v->r->push, v->push);
	}
	auto [a, b, c] = v->push;
	if (a != -1) v->s = mul(a, vr - vl + 1);
	v->s = mul(v->s, b); v->s = add(v->s, mul(c, vr - vl + 1));
	v->push = F(-1, 1, 0);
}
inline void upd(node *v, int vl, int vr, int l, int r, F op) {
	push(v, vl, vr);
	if (l > r) return;
	else if (vl == l && vr == r) {
		mg(v->push, op); push(v, vl, vr);
		return;
	}
	int m = vl + vr >> 1;
	if (!v->l) v->l = new node();
	upd(v->l, vl, m, l, min(r, m), op);
	if (!v->r) v->r = new node();
	upd(v->r, m + 1, vr, max(l, m + 1), r, op);
	v->s = add(v->l->s, v->r->s);
}
inline int get(node *v, int vl, int vr, int l, int r) {
	push(v, vl, vr);
	if (l > r) return 0;
	else if (vl == l && vr == r) return v->s;
	int m = vl + vr >> 1;
	int a = (!v->l ? 0 : get(v->l, vl, m, l, min(r, m)));
	int b = (!v->r ? 0 : get(v->r, m + 1, vr, max(l, m + 1), r));
	return add(a, b);
}
