/* - Treap with implicit keys - */
struct Node {
	ll s = 0;
	int val, p, sz;
	Node *l = nullptr;
	Node *r = nullptr;
	Node() {} /// Can store any additional info about the array if needed
	Node(int x) : val(x), s(x), sz(1) { p = rng() % (int)1e9; }
}; typedef Node* node;
inline int sz(node t) { return (!t ? 0 : t->sz); }
inline ll s(node t) { return (!t ? 0 : t->s); }
inline void upd(node t) { if (!t){return;} t->sz = sz(t->l) + 1 + sz(t->r), t->s = s(t->l) + t->val + s(t->r); }
inline node mg(node a, node b) {
	if (!a || !b) return (!a ? b : a);
	if (a->p > b->p) { a->r = mg(a->r, b); upd(a); return a; }
	else { b->l = mg(a, b->l); upd(b); return b; }
}
inline pair<node,node> sp(node t, int x) {
	if (!t) return {nullptr, nullptr};
	int q = sz(t->l);
	if (x > q) { auto tt = sp(t->r, x - q - 1); t->r = tt.fi; upd(t); return {t, tt.se}; }
	else { auto tt = sp(t->l, x); t->l = tt.se; upd(t); return {tt.fi, t}; }
}

/* - Treap with explicit keys - */
struct Node {
	ll k, p, s = 0;
	Node *l = nullptr;
	Node *r = nullptr;
	Node() {}
	Node(ll x) : s(x), k(x) { p = 1 + rng() % (ll)inf; }
}; typedef Node* node;
inline ll s(node t) { return (!t ? 0 : t->s); }
inline void upd(node t) { t->s = s(t->l) + t->k + s(t->r); }
inline node mg(node a, node b) {
	if (!a || !b) return (!a ? b : a);
	if (a->p > b->p) { a->r = mg(a->r, b); upd(a); return a; }
	else { b->l = mg(a, b->l); upd(b); return b; }
}
inline pair<node, node> sp(node t, int x) {
	if (!t) return {nullptr, nullptr};
	if (x >= t->k) { auto tt = sp(t->r, x); t->r = tt.fi; upd(t); return {t, tt.se}; }
	else { auto tt = sp(t->l, x); t->l = tt.se; upd(t); return {tt.fi, t}; }
}
