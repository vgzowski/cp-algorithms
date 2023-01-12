// 1_1
#include <bits/stdc++.h>
#define fi first
#define se second
#define ll long long
#define ve vector
#define sz(x)       (int)((x).size())
#define all(x)      (x).begin(),(x).end()
#define pii pair<int,int>
#define pb push_back
const int iinf = 1e9;
const ll inf = 3e18;

using namespace std;

const int N = 3e5 + 10;
const int lg = 19;

int m = 300;
int p[N], p2[N], c[lg][N], cnt[N];

inline void solve() {
    string s; cin >> s;
    s += '$'; int n = sz(s);

    for (auto &x : s) ++cnt[x];
    for (int i = 1; i < m; ++i) cnt[i] += cnt[i - 1];
    for (int i = n - 1; ~i; --i) p[--cnt[s[i]]] = i;
    for (int i = 1; i < n; ++i) c[0][p[i]] = c[0][p[i - 1]] + (s[p[i]] != s[p[i - 1]]);

    for (int l = 1, f = 1; l < n; l <<= 1, ++f) {
        memset(cnt, 0, 4 * m);
        for (int i = 0; i < n; ++i) {
            p[i] -= l;
            if (p[i] < 0) p[i] += n;
            ++cnt[c[f - 1][i]];
        }
        for (int i = 1; i < m; ++i) cnt[i] += cnt[i - 1];
        for (int i = n - 1; ~i; --i) p2[--cnt[c[f - 1][p[i]]]] = p[i];
        memcpy(p, p2, 4 * n); m = 1; c[f][p[0]] = 0;
        pii lst = {c[f - 1][p[0]], c[f - 1][(p[0] + l) % n]};

        for (int i = 1; i < n; ++i) {
            pii nxt = {c[f - 1][p[i]], c[f - 1][(p[i] + l) % n]};
            m += (lst != nxt); lst = nxt; c[f][p[i]] = m - 1;
        }
    }

    ve<int> rnk(n);
    for (int i = 0; i < n; ++i) {
        rnk[p[i]] = i;
    }

    auto compare = [&](int l1, int r1, int l2, int r2) {
//        string A = s.substr(l1, r1 - l1 + 1);
//        string B = s.substr(l2, r2 - l2 + 1);
//        return (A == B ? 0 : (A < B ? -1 : 1));

        int mn = min(r1 - l1 + 1, r2 - l2 + 1);
        int p = 31 - __builtin_clz(mn);

        pii A = {c[p][l1], c[p][l1 + mn - 1 - (1 << p) + 1]};
        pii B = {c[p][l2], c[p][l2 + mn - 1 - (1 << p) + 1]};
        if (A != B) return (A < B ? -1 : 1);
        else {
            return (r1 - l1 == r2 - l2 ? 0 : (r1 - l1 < r2 - l2 ? -1 : 1));
        }
    }; --n;

    ve<int> mn_suf(n, -1), mx_suf(n, -1);
    for (int i = n - 2; ~i; --i) {
        if (mn_suf[i + 1] == -1 || compare(mn_suf[i + 1], n - 1, i + 1, n - 1) >= 0) {
            mn_suf[i] = i + 1;
        }
        else {
            mn_suf[i] = mn_suf[i + 1];
        }

        if (mx_suf[i + 1] == -1 || compare(mx_suf[i + 1], n - 1, i + 1, n - 1) <= 0) {
            mx_suf[i] = i + 1;
        }
        else {
            mx_suf[i] = mx_suf[i + 1];
        }
    }

    for (int l = 1; l + 1 < n; ++l) {
        for (int r : {mn_suf[l] - 1, mx_suf[l] - 1, n - 2, n - 3}) {
            if (l > r) continue;

            int l1 = 0, r1 = l - 1;
            int l2 = l, r2 = r;
            int l3 = r + 1, r3 = n - 1;

            if (compare(l2, r2, l1, r1) >= 0 && compare(l2, r2, l3, r3) >= 0) {
                cout << s.substr(l1, r1 - l1 + 1) << " ";
                cout << s.substr(l2, r2 - l2 + 1) << " ";
                cout << s.substr(l3, r3 - l3 + 1);
                return;
            }
            if (compare(l2, r2, l1, r1) <= 0 && compare(l2, r2, l3, r3) <= 0) {
                cout << s.substr(l1, r1 - l1 + 1) << " ";
                cout << s.substr(l2, r2 - l2 + 1) << " ";
                cout << s.substr(l3, r3 - l3 + 1);
                return;
            }
        }
    }
    cout << ":(";
}

signed main() {
    ios_base::sync_with_stdio(false); cin.tie(0); cout.tie(0);
    int q = 1; // cin >> q;
    while (q--) solve();
}

// 1_2
#include <bits/stdc++.h>
#define fi first
#define se second
#define ll long long
#define ve vector
#define sz(x)       (int)((x).size())
#define all(x)      (x).begin(),(x).end()
#define pii pair<int,int>
#define pb push_back
const int iinf = 1e9;
const ll inf = 3e18;

using namespace std;

const int N = 65;
ll dp[N][2][2];

inline void solve() {
    ll n, x;
    cin >> n >> x;

    auto get_first_off = [](ll l, int b) {
        ll ans = 0;
        for (int i = 0; i <= 63; ++i) {
            if (i != b) {
                ans |= (1ll << i);
            }
        }
        for (int i = 63; ~i; --i) {
            if ((ans >> i & 1) && (ans - (1ll << i)) >= l) {
                ans ^= (1ll << i);
            }
        }
        return ans;
    };
    ve<ll> ev{n};
    for (int i = 0; i <= 63; ++i) {
        ll c = get_first_off(n + 1, i);
//        cout << i << "'th bit: " << c << '\n';

        ev.pb(c);
    }
    sort(all(ev));

    ll answer = -1;
    ll res = 0;
    for (int i = 0; i <= 63; ++i) res |= (1ll << i);

    for (const auto &c : ev) {
        res &= c;
        if (res == x) {
            cout << c;
            return;
        }
    }
    cout << -1;
}

signed main() {
    ios_base::sync_with_stdio(false); cin.tie(0); cout.tie(0);
    int q = 1; // cin >> q;
    while (q--) solve();
}

// 1_3
#include <bits/stdc++.h>
#define fi first
#define se second
#define ll long long
#define ve vector
#define sz(x)       (int)((x).size())
#define all(x)      (x).begin(),(x).end()
#define pii pair<int,int>
#define pb push_back
const int iinf = 1e9;
const ll inf = 3e18;

using namespace std;

const int N = 3e5 + 10;

ve<int> pr;
int lp[N];

set<int> have[N];

inline void add(int x, int id) {
    while (x > 1) {
        int d = lp[x];
        have[d].insert(id);
        while (x % d == 0) x /= d;
    }
}
inline void del(int x, int id) {
    while (x > 1) {
        int d = lp[x];
        have[d].erase(id);
        while (x % d == 0) x /= d;
    }
}

inline void solve() {
    for (int i = 2; i < N; ++i) {
        if (!lp[i]) lp[i] = i, pr.pb(i);
        for (int j = 0; j < sz(pr) && i * pr[j] < N && pr[j] <= lp[i]; ++j) {
            lp[i * pr[j]] = pr[j];
        }
    }

    int n;
    cin >> n;
    ve<int> a(n);
    for (auto &i : a) cin >> i;

    for (int i = 0; i < n; ++i) {
        add(a[i], i);
    }

    int s, t;
    cin >> s >> t, --s, --t;

    ve<int> dist(n, -1);
    ve<int> pr(n);
    dist[s] = 0;

    queue<int> q; q.push(s);
    del(a[s], s);

    while (sz(q)) {
        int id = q.front(); q.pop();

        ve<int> primes;
        int X = a[id];
        while (X > 1) {
            int d = lp[X];
            primes.pb(d);
            while (X % d == 0) X /= d;
        }

        ve<int> add;

        for (const auto &p : primes) {
            for (auto &to : have[p]) {
                if (dist[to] == -1) {
                    add.pb(to);
                    dist[to] = dist[id] + 1;
                    pr[to] = id;
                }
            }
        }
        sort(all(add));
        add.erase(unique(all(add)), add.end());

        for (auto &pos : add) {
            del(a[pos], pos);
            q.push(pos);
        }
    }

    if (dist[t] == -1) {
        cout << -1;
        return;
    }

    ve<int> ans;
    while (1) {
        ans.pb(t);
        if (t == s) break;
        t = pr[t];
    }
    reverse(all(ans));
    cout << sz(ans) << '\n';
    for (auto &x : ans) cout << x + 1 << " ";
}

signed main() {
    ios_base::sync_with_stdio(false); cin.tie(0); cout.tie(0);
    int q = 1; // cin >> q;
    while (q--) solve();
}

// 1_4 (52)
#include <bits/stdc++.h>
#define fi first
#define se second
#define ll long long
#define ve vector
#define sz(x)       (int)((x).size())
#define all(x)      (x).begin(),(x).end()
#define pii pair<int,int>
#define pb push_back
#define mpp make_pair
const int iinf = 1e9;
const ll inf = 3e18;

using namespace std;

int md;
inline int add(int a, int b) {
    if ((a += b) >= md) a -= md;
    return a;
}
inline int mul(int a, int b) {
    return a * 1ll * b % md;
}

inline pii mg(const pii &a, const pii &b) {
    if (a.fi < b.fi) return a;
    else if (a.fi > b.fi) return b;
    return mpp(a.fi, add(a.se, b.se));
}

inline void solve() {
    int tests, G;
    cin >> tests >> G;

    if (G == 1) {
        while (tests--) {
            int n;
            cin >> n;

            int x = (int)sqrt(n);
            int y = n / x;

            assert(x <= y);

            ve<ve<char>> answer(x + 1, ve<char> (y + 1, '.'));

            int cnt = n;
            for (int i = 0; i < x; ++i) {
                for (int j = 0; j < y; ++j) {
                    answer[i][j] = '#';
                    --cnt;
                }
            }
            if (cnt == 0) {
                cout << x << " " << y << '\n';
                for (int i = 0; i < x; ++i) {
                    for (int j = 0; j < y; ++j) {
                        cout << answer[i][j];
                    }
                    cout << '\n';
                }
            }
            else if (cnt <= y) {
                cout << x + 1 << " " << y << '\n';
                for (int i = 0; i <= x; ++i) {
                    for (int j = 0; j < y; ++j) {
                        if (i < x) {
                            cout << answer[i][j];
                        }
                        else {
                            cout << (cnt > 0 ? '#' : '.');
                            --cnt;
                        }
                    }
                    cout << '\n';
                }
            }
            else {
                cout << x + 1 << " " << y + 1 << '\n';
                ve<pii> ord;
                ord.pb({x, y - 1});
                ord.pb({x - 1, y});
                ord.pb({x, y});
                for (int i = x - 2; ~i; --i) ord.pb({i, y});
                for (int i = y - 2; ~i; --i) ord.pb({x, i});

                for (int i = 0; i < cnt; ++i) answer[ord[i].fi][ord[i].se] = '#';

                for (int i = 0; i <= x; ++i) {
                    for (int j = 0; j <= y; ++j) {
                        cout << answer[i][j];
                    }
                    cout << '\n';
                }
            }
        }
    }
    else {
        cin >> md;

        while (tests--) {
            int n;
            cin >> n;

            int mn = iinf;
            for (int i = 1; i <= n; ++i) mn = min(mn, 2 * (i + n / i + !!(n % i)));

            int sq = min(n, 2 * (int)sqrt(n) + 5);

            ve<ve<ve<pii>>> dp(sq + 1, ve<ve<pii>> (n + 1, ve<pii> (sq + 1, mpp(iinf, iinf))));
            for (int i = 1; i <= sq; ++i) dp[1][i][i] = mpp(2 * i, 1);

            for (int i = 1; i < sq; ++i) {
                for (int s = 1; s <= n; ++s) {
                    for (int last = 1; last <= sq; ++last) {
                        if (dp[i][s][last].fi > mn) continue;

                        for (int cur = 1; s + cur <= n && cur <= sq; ++cur) {
                            int nw = dp[i][s][last].fi + 2 * cur - 2 * min(cur, last);
                            int ways = mul(dp[i][s][last].se, abs(cur - last) + 1);
                            dp[i + 1][s + cur][cur] = mg(dp[i + 1][s + cur][cur], mpp(nw, ways));
                        }
                    }
                }
            }

            pii ans = {iinf, iinf};
            for (int i = 1; i <= sq; ++i) {
                for (int last = 1; last <= sq; ++last) {
                    ans = mg(ans, mpp(dp[i][n][last].fi + 2 * i, dp[i][n][last].se));
                }
            }
            cout << ans.fi << " " << ans.se << '\n';
        }
    }
}

signed main() {
    ios_base::sync_with_stdio(false); cin.tie(0); cout.tie(0);
    int q = 1; // cin >> q;
    while (q--) solve();
}

// 2_1
#include <bits/stdc++.h>
#define fi first
#define se second
#define pii pair<int,int>
#define mpp make_pair
#define ve vector
#define pb push_back
#define ll long long
#define sz(x)       (int)((x).size())
#define all(x)      (x).begin(),(x).end()
const int iinf = 1e9;
const ll inf = 1e18;

using namespace std;

inline void solve() {
//    int r = 1, pr = 0;
//    for (int i = 1; i <= 30; ++i) {
//        if (i > 1) r += (r % 10);
//        cout << i << ": " << r - pr << " " << r << '\n';
//        pr = r;
//    }
//    return;

    auto ar = [](ll a0, ll cnt, ll d) {
        ll an = a0 + (cnt - 1) * d;
        return ((a0 + an) / 2) * cnt;
    };

    auto cntrem = [](ll r, ll k, ll rem) {
        ll cnt = (r - rem) / k;
        return cnt + (rem <= r);
    };

    int q;
    cin >> q;
    while (q--) {
        ll x;
        cin >> x;

        if (x == 1) cout << 1 << '\n';
        else if (x == 2) cout << 2 << '\n';
        else {
            x -= 2;
            ll ans = 0;

            {
                ll cnt2 = cntrem(x - 1, 4, 0);
                ans += 2 * cnt2;
            }
            {
                ll cnt4 = cntrem(x - 1, 4, 1);
                ans += 4 * cnt4;
            }
            {
                ll cnt8 = cntrem(x - 1, 4, 2);
                ans += 8 * cnt8;
            }
            {
                ll cnt6 = cntrem(x - 1, 4, 3);
                ans += 6 * cnt6;
            }

            ans += 2;
            cout << ans << '\n';
        }
    }
}

signed main() {
    ios_base::sync_with_stdio(false); cin.tie(0); cout.tie(0);
    int q = 1; // cin >> q;
    while (q--) solve();
}

// 2_2
#include <bits/stdc++.h>
#define fi first
#define se second
#define pii pair<int,int>
#define mpp make_pair
#define ve vector
#define pb push_back
#define ll long long
#define sz(x)       (int)((x).size())
#define all(x)      (x).begin(),(x).end()
const int iinf = 1e9;
const ll inf = 1e18;

using namespace std;

const int N = 1e7 + 10;

int lp[N];
ve<int> pr;

ve<int> lst[N];

int have[N];

inline void solve() {
    for (int i = 2; i < N; ++i) {
        if (!lp[i]) lp[i] = i, pr.pb(i);
        for (int j = 0; j < sz(pr) && i * pr[j] < N && pr[j] <= lp[i]; ++j) {
            lp[i * pr[j]] = pr[j];
        }
    }

    int n;
    cin >> n;
    ve<int> a(n);

    for (int i = 0; i < n; ++i) {
        cin >> a[i];

        int x = a[i];
        while (x > 1) {
            int d = lp[x], c = 0;
            ++have[d];

            while (x % d == 0) ++c, x /= d;

            lst[d].pb(c);
        }
    }

    int ans = 0;
    for (int i = 2; i < N; ++i) {
        if (!sz(lst[i])) continue;

        sort(all(lst[i]));

        int sum = 0;
        for (auto &x : lst[i]) sum += x;

        int need = sum;

        int pf = 0;
        for (int j = 0; j < sz(lst[i]); ++j) {
            int result = 0;

            int x = lst[i][j];

            pf += x;
            result += x * (j + 1 + n - have[i]) - pf;
            result += (sum - pf) - x * (sz(lst[i]) - (j + 1));

            need = min(need, result);
        }
        ans += need;
    }
    cout << ans;
}

signed main() {
    ios_base::sync_with_stdio(false); cin.tie(0); cout.tie(0);
    int q = 1; // cin >> q;
    while (q--) solve();
}

// 2_3
#include <bits/stdc++.h>
#pragma GCC optimize("-O3")
#pragma GCC target("popcnt,bmi")
#define fi first
#define se second
#define pii pair<int,int>
#define mpp make_pair
#define ve vector
#define pb push_back
#define ll long long
#define sz(x)       (int)((x).size())
#define all(x)      (x).begin(),(x).end()
const int iinf = 1e9;
const ll inf = 1e18;

using namespace std;

constexpr int S = 8;

array<int,3> trans_xor[S][S][S][S];
array<int,3> trans_and[S][S][S][S];
array<int,3> trans_or[S][S][S][S];
array<int,3> T[S][S][S][S][S][S];

const int N = 2e5 + 10;

struct node {
    int ans, f, l;
    int cnt[S][S];

    node(int x = 0) : ans(0), f(x), l(x) { memset(cnt, 0, sizeof(cnt)); }
} t[4 * N];

inline node mg(const node &a, const node &b) {
    node res; res.f = a.f, res.l = b.l;
    res.ans = a.ans + b.ans + (a.l != b.f);

    for (int i = 0; i < S; ++i) {
    for (int j = 0; j < S; ++j) {
        res.cnt[i][j] = a.cnt[i][j] + b.cnt[i][j];
    }} ++res.cnt[a.l][b.f];
    return res;
}
inline void mg(int v) { t[v] = mg(t[2 * v + 1], t[2 * v + 2]); }

int GCNT[S][S];
array<int,3> p[4 * N];
inline void mgp(array<int,3> &pv, const array<int,3> &pu) {
    pv = T[pv[0]][pv[1]][pv[2]][pu[0]][pu[1]][pu[2]];
}
inline void app(int v, const array<int,3> &push) {
    const auto &[a, b, c] = push;
    t[v].f = ((t[v].f & a) | b) ^ c;
    t[v].l = ((t[v].l & a) | b) ^ c;

    t[v].ans = 0;
    for (int i = 0; i < S; ++i) {
    for (int j = 0; j < S; ++j) {
        GCNT[((i & a) | b) ^ c][((j & a) | b) ^ c] += t[v].cnt[i][j];
    }}
    for (int i = 0; i < S; ++i) {
    for (int j = 0; j < S; ++j) {
        t[v].ans += (i != j) * GCNT[i][j];
        t[v].cnt[i][j] = GCNT[i][j];
        GCNT[i][j] = 0;
    }}
}
inline void push(int v, int vl, int vr) {
    if (p[v][0] == S - 1 && p[v][1] == 0 && p[v][2] == 0) return;
    app(v, p[v]);
    if (vl != vr) {
        mgp(p[2 * v + 1], p[v]);
        mgp(p[2 * v + 2], p[v]);
    }
    p[v] = {S - 1, 0, 0};
}

int n;

int a[N];
inline void build(int v, int vl, int vr) {
    p[v] = {S - 1, 0, 0};
    if (vl == vr) return void(t[v] = node(a[vl]));
    int m = vl + vr >> 1;
    build(2 * v + 1, vl, m);
    build(2 * v + 2, m + 1, vr);
    mg(v);
}
inline void upd(int v, int vl, int vr, int l, int r, const array<int,3> &x) {
    push(v, vl, vr);
    if (l > r) return;
    else if (vl == l && vr == r) {
        mgp(p[v], x);
        push(v, vl, vr);
        return;
    }
    int m = vl + vr >> 1;
    upd(2 * v + 1, vl, m, l, min(r, m), x);
    upd(2 * v + 2, m + 1, vr, max(l, m + 1), r, x);
    mg(v);
}
inline array<int,3> mg1(const array<int,3> &a, const array<int,3> &b) {
    return {a[0] + b[0] + (a[2] != b[1]), a[1], b[2]};
}
inline array<int,3> get(int v, int vl, int vr, int l, int r) {
    push(v, vl, vr);
    if (vl == l && vr == r) return {t[v].ans, t[v].f, t[v].l};
    int m = vl + vr >> 1;
    if (r <= m) return get(2 * v + 1, vl, m, l, r);
    else if (l > m) return get(2 * v + 2, m + 1, vr, l, r);
    else return mg1(get(2 * v + 1, vl, m, l, m), get(2 * v + 2, m + 1, vr, m + 1, r));
}

inline void build() { build(0, 0, n - 1); }
inline void upd(int l, int r, const array<int,3> &x) { upd(0, 0, n - 1, l, r, x); }
inline int get(int l, int r) { return 1 + get(0, 0, n - 1, l, r)[0]; }

inline void solve() {
    for (int pa = 0; pa <= S - 1; ++pa) {
    for (int pb = 0; pb <= S - 1; ++pb) {
    for (int pc = 0; pc <= S - 1; ++pc) {
        for (int c = 0; c <= S - 1; ++c) {

            bool found = 0;
            for (int na = 0; na <= S - 1; ++na) {
            if (found) break;

            for (int nb = 0; nb <= S - 1; ++nb) {
            if (found) break;

            for (int nc = 0; nc <= S - 1; ++nc) {

                bool ok = 1;
                for (int i = 0; i <= S - 1; ++i) {
                    int F = (((i & pa) | pb) ^ pc) | c;
                    int S = (((i & na) | nb) ^ nc);
                    ok &= (F == S);
                }

                if (ok) {
                    found = 1;
                    trans_or[pa][pb][pc][c] = {na, nb, nc};
                    break;
                }
            }}}
            if (!found) {
                cout << "ERROR " << pa << " " << pb << " " << pc << " " << c;
                return;
            }
        }
    }}}
    for (int pa = 0; pa <= S - 1; ++pa) {
    for (int pb = 0; pb <= S - 1; ++pb) {
    for (int pc = 0; pc <= S - 1; ++pc) {
        for (int c = 0; c <= S - 1; ++c) {

            bool found = 0;
            for (int na = 0; na <= S - 1; ++na) {
            if (found) break;

            for (int nb = 0; nb <= S - 1; ++nb) {
            if (found) break;

            for (int nc = 0; nc <= S - 1; ++nc) {

                bool ok = 1;
                for (int i = 0; i <= S - 1; ++i) {
                    int F = (((i & pa) | pb) ^ pc) & c;
                    int S = (((i & na) | nb) ^ nc);
                    ok &= (F == S);
                }

                if (ok) {
                    found = 1;
                    trans_and[pa][pb][pc][c] = {na, nb, nc};
                    break;
                }
            }}}
            if (!found) {
                cout << "ERROR " << pa << " " << pb << " " << pc << " " << c;
                return;
            }
        }
    }}}
    for (int pa = 0; pa <= S - 1; ++pa) {
    for (int pb = 0; pb <= S - 1; ++pb) {
    for (int pc = 0; pc <= S - 1; ++pc) {
        for (int c = 0; c <= S - 1; ++c) {

            bool found = 0;
            for (int na = 0; na <= S - 1; ++na) {
            if (found) break;

            for (int nb = 0; nb <= S - 1; ++nb) {
            if (found) break;

            for (int nc = 0; nc <= S - 1; ++nc) {

                bool ok = 1;
                for (int i = 0; i <= S - 1; ++i) {
                    int F = (((i & pa) | pb) ^ pc) ^ c;
                    int S = (((i & na) | nb) ^ nc);
                    ok &= (F == S);
                }

                if (ok) {
                    found = 1;
                    trans_xor[pa][pb][pc][c] = {na, nb, nc};
                    break;
                }
            }}}
            if (!found) {
                cout << "ERROR " << pa << " " << pb << " " << pc << " " << c;
                return;
            }
        }
    }}}

//    cout << 4 * pow(8, 4) * 3 * 4 / (double)(1024 * 1024) << endl;
    for (int a = 0; a < S; ++a) {
    for (int b = 0; b < S; ++b) {
    for (int c = 0; c < S; ++c) {
    for (int d = 0; d < S; ++d) {
    for (int e = 0; e < S; ++e) {
    for (int f = 0; f < S; ++f) {
        array<int,3> ar = {a, b, c};
        ar = trans_and[ar[0]][ar[1]][ar[2]][d];
        ar = trans_or[ar[0]][ar[1]][ar[2]][e];
        ar = trans_xor[ar[0]][ar[1]][ar[2]][f];
        T[a][b][c][d][e][f] = ar;
    }}}}}}

    #ifdef LOCAL
        ifstream cin("input.txt");
        ofstream cout("output.txt");
    #endif

    int q; cin >> n >> q;
    for (int i = 0; i < n; ++i) cin >> a[i];

    if (n <= 2000 && q <= 2000) {
        for (int it = 0; it < q; ++it) {
            string type; cin >> type;

            if (type == "ask") {
                int l, r, ans = 1;
                cin >> l >> r;
                for (int i = l; i < r; ++i) ans += (a[i] != a[i - 1]);
                cout << ans << '\n';
            }
            else if (type == "and") {
                int l, r, x;
                cin >> l >> r >> x;
                for (int i = l - 1; i < r; ++i) a[i] &= x;
            }
            else if (type == "or") {
                int l, r, x;
                cin >> l >> r >> x;
                for (int i = l - 1; i < r; ++i) a[i] |= x;
            }
            else if (type == "xor") {
                int l, r, x;
                cin >> l >> r >> x;
                for (int i = l - 1; i < r; ++i) a[i] ^= x;
            }
        }
        return;
    }

    build();

    while (q--) {
        string type;
        cin >> type;

        if (type == "ask") {
            int l, r;

            cin >> l >> r;
            cout << get(--l, --r) << '\n';
        }
        else if (type == "and") {
            int l, r, x;
            cin >> l >> r >> x;
            upd(--l, --r, {x, 0, 0});
        }
        else if (type == "or") {
            int l, r, x;
			cin >> l >> r >> x;
            upd(--l, --r, {S - 1, x, 0});
        }
        else {
            int l, r, x;
            cin >> l >> r >> x;
            upd(--l, --r, {S - 1, 0, x});
        }
    }
}

signed main() {
    ios_base::sync_with_stdio(false); cin.tie(0); cout.tie(0);
    int q = 1; // cin >> q;
    while (q--) solve();
    cerr << "time: " << clock() / (double)CLOCKS_PER_SEC;
}

// 2_4 (32)
#include <bits/stdc++.h>
#define fi first
#define se second
#define pii pair<int,int>
#define mpp make_pair
#define ve vector
#define pb push_back
#define ll long long
#define sz(x)       (int)((x).size())
#define all(x)      (x).begin(),(x).end()
const int iinf = 1e9;
const ll inf = 1e18;

using namespace std;

ve<pii> fig = {
{2, -1},
{3, -1},
{3, -1},
{1, 1},
{3, -1},
{2, -1},
{2, -1}
};

inline void solve() {
    int t, q, n, m;
    cin >> t >> q >> n >> m;

    for (int i = 0; i < q; ++i) {
        ve<int> a(8);
        for (auto &x : a) cin >> x;
    }

    if (t == 3) srand(239084);
    else if (t == 5) srand(8420661);
    else srand(9235259);

    if (t == 1) {
        int cur = 0;

        int x;
        while (cin >> x) {
            if (x == 0) break;
            if (x == -1) break;

            cout << cur + 1 << " " << 0 << endl;
            fflush(stdout);

            cur = (cur + 1) % m;
        }
    }
    else if (t == 2) {
        int cp = 0, cs = 0;

        int x;
        while (cin >> x) {
            if (x == 0) break;
            if (x == -1) break;

            if (x != 1) {
                cout << 1 + 0 * 4 << " " << 90 << endl;
                fflush(stdout);
                cp = (cp + 1) % 2;
            }
            else {
                cout << 5 + 2 * cs << " " << 0 << endl;
                fflush(stdout);
                cs = (cs + 1) % 3;
            }
        }
    }
    else if (t == 3) {
        int x;
        ve<pii> op{{1, 270}, {1, 0}, {3, 180}, {2, 90},
                   {5, 270}, {5, 0}, {7, 180}, {6, 90},
                    {9, 180}, {9, 0}};
        int ptr = 0;
        while (cin >> x) {
            if (x <= 0) break;

            cout << op[ptr].fi << " " << op[ptr].se << endl;
            fflush(stdout);

            ptr = (ptr + 1) % sz(op);
        }
    }
    else {
        int x;
        while (cin >> x) {
            if (x == 0) break;
            else if (x == -1) break;

            auto [len, place] = fig[x - 1];
            if (place != -1) {
                cout << place << " " << 0 << endl;
                fflush(stdout);
            }
            else {
                int ps = rand() % (m - len + 1) + 1;
                cout << ps << " " << 0 << endl;
                fflush(stdout);
            }
        }
    }
}

signed main() {
    ios_base::sync_with_stdio(false); cin.tie(0); cout.tie(0);
    int q = 1; // cin >> q;
    while (q--) solve();
}
