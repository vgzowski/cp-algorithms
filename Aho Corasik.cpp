const int N = 4e3 + 10;

struct node {
	int ch[13], go[13];
	int link = -1;
	int p, pch;
	bool leaf = 0;

	node(int p = -1, int pch = 12) : pch(pch), p(p) {
		fill(ch, ch + 13, -1);
		fill(go, go + 13, -1);
	}
} t[N]; int w = 0;
inline void add(const string &s) {
	int v = 0;
	for (int i = 0; i < sz(s); ++i) {
		int c = s[i] - 'a';
		if (!~t[v].ch[c]) {
			int u = ++w;
			t[u] = node(v, c);
			t[v].ch[c] = u;
		} v = t[v].ch[c];
	} t[v].leaf = 1;
}

inline int go(int v, int c);
inline int link(int v) {
	if (!~t[v].link) {
		if (!v || !t[v].p) t[v].link = 0;
		else t[v].link = go(link(t[v].p), t[v].pch);
	} return t[v].link;
}
inline int go(int v, int c) {
	if (!~t[v].go[c]) {
		if (~t[v].ch[c]) t[v].go[c] = t[v].ch[c];
		else t[v].go[c] = !v ? 0 : go(link(v), c);
	}
	return t[v].go[c];
}
