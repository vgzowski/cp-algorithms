// -------------------------------
/* - Dinitz algorithm */
// -------------------------------

struct edge { int u, rv, cap, flow; };
const int N = 1e5 + 5; int s, t; int nn;
queue <int> q; vector <edge> gr[N]; int d[N], ptr[N];
inline void add(int v, int u, int cap) {
	gr[v].push_back({u, gr[u].size(), cap, 0});
	gr[u].push_back({v, gr[v].size() - 1, 0, 0}); /// Change capacity to 'c' if edge is unoriented
}
inline bool bfs() {
	fill(d, d + nn, -1);
	d[s] = 0; q.push(s);
	while (!q.empty()) {
		int v = q.front(); q.pop();
		for (auto &[to, rv, cap, flow] : gr[v]) {
			if (d[to] == -1 && cap - flow > 0) {
				d[to] = d[v] + 1;
				q.push(to);
			}
		}
	} return (d[t] != -1);
}
inline int dfs(int v, int f = 1e9) {
	if (v == t || !f) return f;
	for (; ptr[v] < gr[v].size(); ++ptr[v]) {
		auto &[to, rv, cap, flow] = gr[v][ptr[v]];
		if (d[to] == d[v] + 1 && cap - flow > 0) {
			if (int push = dfs(to, min(f, cap - flow))) {
				flow += push, gr[to][rv].flow -= push;
				return push;
			}
		}
	}
	return 0;
}
inline int flow() {
	int ans = 0;
	while (bfs()) {
		fill(ptr, ptr + nn, 0);
		while (int push = dfs(s)) ans += push;
	} return ans;
} /// from now on - restoring the pathes
inline bool bfsRestoring() {
	fill(d, d + nn, -1); d[s] = 0; q.push(s);
	while (!q.empty()) {
		int v = q.front(); q.pop();
		for (auto &[to, rv, cap, flow] : gr[v]) {
			if (d[to] == -1 && flow > 0) {
				d[to] = d[v] + 1;
				q.push(to);
			}
		}
	} return (d[t] != -1);
}
inline bool dfsRestoring(int v, vector<int>&res, int f = 1e9) {
	if (v == t) { res.push_back(v); return 1; }
	for (; ptr[v] < gr[v].size(); ++ptr[v]) {
		auto &[to, rv, cap, flow] = gr[v][ptr[v]];
		if (d[to] == d[v] + 1 && flow > 0) {
			if (dfsRestoring(to, res, min(f, flow))) {
				res.push_back(v), flow = 0;
				return 1;
			}
		}
	} return 0;
}
vector<vector<int>> flow1() {
	vector<int> res;
	vector<vector<int>>ans;
	while (bfsRestoring()) {
		fill(ptr, ptr + nn, 0); res.clear();
		while (dfsRestoring(s, res)) {
			reverse(res.begin(), res.end());
			ans.push_back(res);
			res.clear();
		}
	} return ans;
}




















// -------------------------------
/* - MCMF with potentials - */
// -------------------------------

struct edge { int u, rv, cap, flow, cost; };
const int N = 1e5 + 5; int s, t; int nn;
set<pair<int, int>> st;
vector <edge> gr[N];
int d[N], pot[N];
pair<int, int> p[N];
inline void add(int v, int u, int cap, int cost) {
	gr[v].push_back({u, gr[u].size(), cap, 0, cost});
	gr[u].push_back({v, gr[v].size() - 1, 0, 0, -cost});
}
inline void bellman() {
    fill(d, d + nn, 1e9); d[s] = 0;
    fill(p, p + nn, make_pair(-1, -1));
    while (1) {
        bool ch = 0;
        for (int i = 0; i < nn; ++i) {
            for (int id = 0; id < gr[i].size(); ++id) {
                auto &[j, rv, cap, flow, w] = gr[i][id];
                if (d[j] > d[i] + w && cap - flow > 0) {
                    d[j] = d[i] + w;
                    p[j] = {i, id};
                    ch = 1;
                }
            }
        }
        if (!ch) break;
    }
}
inline void dijkstra2() { // can be reaplced with O(mlog(n)) dijkstra
	fill(p, p + nn, make_pair(-1, -1));
	fill(d, d + nn, 1e9); d[s] = 0;
	vector <bool> used(nn);
	for (int iter = 0; iter < nn; ++iter) {
		int v = -1;
		for (int i = 0; i < nn; ++i) {
			if (!used[i] && (v == -1 || d[i] < d[v])) {
				v = i;
			}
		}
		if (d[v] == 1e9) break;
		used[v] = 1;
		for (int i = 0; i < gr[v].size(); ++i) {
			auto &[to, rv, cap, flow, w] = gr[v][i];
			if (cap - flow > 0 && d[to] > d[v] + w + pot[v] - pot[to]) {
				d[to] = d[v] + w + pot[v] - pot[to]; p[to] = {v, i};
			}
		}
	}
}
inline pair<int,int> mcf(int K) {
	int ansf = 0, ansc = 0;
	bellman(); for (int i = 0; i < nn; ++i) pot[i] = d[i];
	while (K) { dijkstra2(); if (d[t] == 1e9) break;
		for (int i = 0; i < nn; ++i) pot[i] += d[i];
		int v = t, flow = 1e9;
		while (v != s) {
			auto &[u, id] = p[v];
			flow = min(flow, gr[u][id].cap - gr[u][id].flow);
			v = u;
		}
		flow = min(flow, K);
		K -= flow, ansf += flow, v = t;
		while (v != s) {
			auto &[u, id] = p[v];
			gr[u][id].flow += flow;
			gr[v][gr[u][id].rv].flow -= flow;
			ansc += gr[u][id].cost * flow;
			v = u;
		}
	} return {ansc, ansf};
}
