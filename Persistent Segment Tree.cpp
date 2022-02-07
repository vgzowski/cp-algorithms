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





















// ----------------------------------
/* - Persistent Segment Tree with range updates - */
// ----------------------------------
struct node {
	node *l = nullptr;
	node *r = nullptr;
	int h = 0, s = 0, push = -1;
};
 
inline node* build(int vl, int vr) {
	if (vl == vr) return new node();
	int m = vl + vr >> 1;
	node *v = new node();
	v->l = build(vl, m);
	v->r = build(m + 1, vr);
	return v;
}
 
inline node* copy(node *v) {
	node *nw = new node();
	nw->l = v->l, nw->r = v->r;
	nw->h = v->h, nw->s = v->s;
	nw->push = v->push;
	return nw;
}
 
inline void apply(node *v, int x, int vl, int vr) {
	v->h = mul(x, get(vl, vr));
	v->s = (vr - vl + 1) * x;
}
inline void push(node *v, int vl, int vr) {
	if (v->push == -1) return;
 
	if (vl != vr) {
 
		v->l = copy(v->l);
		v->r = copy(v->r);
		int m = vl + vr >> 1;
 
		apply(v->l, v->push, vl, m);
		apply(v->r, v->push, m + 1, vr);
		v->l->push = v->push;
		v->r->push = v->push;
	}
	v->push = -1;
}
inline void upd(node *v, int vl, int vr, int l, int r, int x) {
	if (l > r) return;
	if (vl == l && vr == r) {
		apply(v, x, vl, vr);
		v->push = x;
		return;
	}
 
	int m = vl + vr >> 1;
	push(v, vl, vr);
 
	if (l <= min(r, m)) {
		v->l = copy(v->l);
		upd(v->l, vl, m, l, min(r, m), x);
	}
	if (r >= max(l, m + 1)) {
		v->r = copy(v->r);
		upd(v->r, m + 1, vr, max(l, m + 1), r, x);
	}
	v->h = add(v->l->h, v->r->h);
	v->s = v->l->s + v->r->s;
}
inline bool cmp(node *v, node *u, int vl, int vr) {
	if (v->h == u->h) return 0;
	if (vl == vr) {
		if (v->s < u->s) return 1;
		else return 0;
	}
	push(v, vl, vr); push(u, vl, vr);
	int m = vl + vr >> 1;
	
	if (v->r->h != u->r->h) return cmp(v->r, u->r, m + 1, vr);
	else return cmp(v->l, u->l, vl, m);
}
inline int get(node *v, int vl, int vr, int sf) {
	if (v->s == vr - vl + 1) return -1;
	if (vl == vr) return vl;
	
	push(v, vl, vr);
	int m = vl + vr >> 1;
	
	if (sf > m) return get(v->r, m + 1, vr, sf);
	else {
		int a = get(v->l, vl, m, sf);
		if (a != -1) return a;
		return get(v->r, m + 1, vr, sf);
	}
}
