inline void solve() {
	int n;
	cin >> n;
	ve<int> pr;
	ve<int> lp(n + 1); /// lp[i] - least prime that divides i
	for (int i = 2; i <= n; ++i) {
		if (!lp[i]) lp[i] = i, pr.pb(i);
		for (int j = 0; j < sz(pr) && i * pr[j] <= n; ++j) {
			lp[i * pr[j]] = pr[j];
			if (i % pr[j] == 0) break;
		}
	}
}
