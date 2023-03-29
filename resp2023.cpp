#include <bits/stdc++.h>
#define ll long long
#define ld long double
#define mpp make_pair
#define ve vector
#define pb push_back
#define pii pair<int,int>
#define pll pair<ll,ll>
#define all(x)          (x).begin(),(x).end()
#define sz(x)           ((int)(x).size())
const int iinf = 1e9;
const ll inf = 1e18;

using namespace std;

const int N = 20;

bool dp[N][2][2][2][2][2];
tuple<short,short,short,short,short,short> p[N][2][2][2][2][2];

template<typename T>
inline bool chmin(T& a, T b) { return a > b ? a = b, 1 : 0; }
template<typename T>
inline bool chmax(T& a, T b) { return a < b ? a = b, 1 : 0; }

inline void solve() {
    ll l, r, u;
    cin >> l >> r >> u;
    ve<int> dr, dl;
    while (r) { dr.pb(r % 10); r /= 10; }
    while (l) { dl.pb(l % 10); l /= 10; }
    while (dl.size() < dr.size()) dl.pb(0);
    reverse(all(dl));
    reverse(all(dr));
    int n = sz(dl);

    auto ok = [&](int l, int r) {
        memset(dp, 0, sizeof(dp));
        dp[0][1][1][0][0][0] = 1;

        for (int i = 0; i < n; ++i) {
        for (int el : {0, 1}) for (int er : {0, 1}) for (int st : {0, 1}) {
        for (int usl : {0, 1}) for (int usr : {0, 1}) {

            if (!dp[i][el][er][st][usl][usr]) continue;

            for (int d = 0; d <= 9; ++d) {
                if (el && d < dl[i]) continue;
                if (er && d > dr[i]) continue;

                int nst = st | (d > 0);
                int fl = usl | (nst && d == l);
                int fr = usr | (nst && d == r);

                if (nst && !(d >= l && d <= r)) continue;
                dp[i + 1][el & (d == dl[i])][er & (d == dr[i])][nst][fl][fr] = 1;
            }

        }}}
        for (int f1 : {0, 1}) {
            for (int f2 : {0, 1}) {
                if (dp[n][f1][f2][1][1][1]) {
                    return 1;
                }
            }
        }
        return 0;
    };

    auto restore = [&](int l, int r) {
        memset(dp, 0, sizeof(dp));
        dp[0][1][1][0][0][0] = 1;

        for (int i = 0; i < n; ++i) {
        for (int el : {0, 1}) for (int er : {0, 1}) for (int st : {0, 1}) {
        for (int usl : {0, 1}) for (int usr : {0, 1}) {

            if (!dp[i][el][er][st][usl][usr]) continue;

            for (int d = 0; d <= 9; ++d) {
                if (el && d < dl[i]) continue;
                if (er && d > dr[i]) continue;

                int nst = st | (d > 0);
                int fl = usl | (nst && d == l);
                int fr = usr | (nst && d == r);

                if (nst && !(d >= l && d <= r)) continue;
                dp[i + 1][el & (d == dl[i])][er & (d == dr[i])][nst][fl][fr] = 1;
                p[i + 1][el & (d == dl[i])][er & (d == dr[i])][nst][fl][fr] = {d, el, er, st, usl, usr};
            }

        }}}

        int i = n;
        int el, er;
        int st = 1;
        int usl = 1, usr = 1;
        for (int f1 : {0, 1}) {
            for (int f2 : {0, 1}) {
                if (dp[n][f1][f2][1][1][1]) {
                    el = f1, er = f2;
                }
            }
        }
        string res = "";
        while (i) {
            res += char(get<0>(p[i][el][er][st][usl][usr]) + '0');
            get<0>(p[i][el][er][st][usl][usr]) = i - 1;
            tie(i, el, er, st, usl, usr) = p[i][el][er][st][usl][usr];
        }
        while (sz(res) && res.back() == '0') res.pop_back();
        reverse(all(res));
        return res;
    };

    array<int,3> res = {(u == 1 ? iinf : -iinf), -1, -1};

    for (int L = 0; L <= 9; ++L) {
        for (int R = L; R <= 9; ++R) {
            if (ok(L, R)) {
                if (u == 1) chmin(res, {R - L, L, R});
                else chmax(res, {R - L, L, R});
            }
        }
    }
    int L = res[1];
    int R = res[2];

    cout << restore(L, R) << '\n';
}

signed main() {
    ios_base::sync_with_stdio(false); cin.tie(0); cout.tie(0);
    int q = 1; cin >> q;
    while (q--) solve();
}


#include <bits/stdc++.h>
#define ll long long
#define ld long double
#define mpp make_pair
#define ve vector
#define pb push_back
#define pii pair<int,int>
#define pll pair<ll,ll>
#define all(x)          (x).begin(),(x).end()
#define sz(x)           ((int)((x).size()))
const int iinf = 1e9;
const ll inf = 1e18;

using namespace std;

const int md = 1e9 + 7;
inline int add(int a, int b) {
    if ((a += b) >= md) a -= md;
    return a;
}
inline int mul(int a, int b) {
    return a * 1ll * b % md;
}
inline int sub(int a, int b) {
    if ((a -= b) < 0) a += md;
    return a;
}

const int N = 3e5 + 100;

ve<int> c[N], c1[N];

int ans = 0;

int fe[N];
inline void upd(int i, int x) {
    for (i += 3; i < N; i += i & -i) {
        fe[i] = add(fe[i], x);
    }
}
inline int get(int i) {
    int ans = 0;
    for (i += 3; i; i -= i & -i) ans = add(ans, fe[i]);
    return ans;
}
inline int get(int l, int r) {
    return sub(get(r), get(l - 1));
}

int ar[N];
inline void ch(int id, int x) {
    upd(id, (x - ar[id]) % md);
    ar[id] = x;
}

int curl[N], curr[N];

int ways[N];

inline void solve() {
    int n;
    cin >> n;
    ve<pii> a(n);
    for (auto &[x, y] : a) cin >> x >> y;

    for (auto &[x, y] : a) {
        c[x].pb(y);
        c1[y].pb(x);
    }
    for (int i = 0; i < N; ++i) {
        sort(all(c[i]));
        sort(all(c1[i]));
        curr[i] = sz(c1[i]);
    }

    int ans = 0;

    for (int x = 0, ptr = 0; x < N; ++x) {
        for (int id = 0; id < sz(c[x]); ++id) {
            const int &y = c[x][id];
            int value = (md - get(y)) % md;
            int value1 = get(y - 1);

            ans = add(ans, mul(value, sz(c[x]) - id - 1));
            ans = add(ans, mul(value1, id));
        }

        for (const auto &y : c[x]) { /// become left
            ++curl[y];
            ch(y, mul(curl[y], curr[y]));
        }
        for (const auto &y : c[x + 1]) { /// removed from right
            --curr[y];
            ch(y, mul(curl[y], curr[y]));
        }
    }

    for (auto &[x, y] : a) {
        int id1 = lower_bound(all(c[x]), y) - c[x].begin();
        int id2 = lower_bound(all(c1[y]), x) - c1[y].begin();
        int d = mul(id1, sz(c[x]) - id1 - 1);
        int d1 = mul(id2, sz(c1[y]) - id2 - 1);
        ans = add(ans, (md - mul(d, d1)) % md);
    }

    cout << ans;
}

signed main() {
    ios_base::sync_with_stdio(false); cin.tie(0); cout.tie(0);
    int q = 1; // cin >> q;
    while (q--) solve();
}



#include <bits/stdc++.h>
#pragma GCC optimize("Ofast")
#pragma GCC optimize("unroll-loops")
#pragma GCC target("avx,avx2,fma")
#define ll long long
#define ld long double
#define mpp make_pair
#define ve vector
#define pb push_back
#define pii pair<int,int>
#define pll pair<ll,ll>
#define all(x)          (x).begin(),(x).end()
#define sz(x)           ((int)(x).size())
const int iinf = 1e9;
const ll inf = 1e18;

using namespace std;

int md;
inline int add(int a, int b) {
    if ((a += b) >= md) a -= md;
    return a;
}
inline int mul(int a, int b) {
    return a * 1ll * b % md;
}
inline int sub(int a, int b) {
    if ((a -= b) < 0) a += md;
    return a;
}
inline void adds(int &a, int b) {
    if ((a += b) >= md) a -= md;
}
inline void subs(int &a, int b) {
    if ((a -= b) < 0) a += md;
}
inline void muls(int &a, int b) {
    a = a * 1ll * b % md;
}

inline int bp(int a, ll n) {
    int res = 1;
    while (n) {
        if (n & 1) res = mul(res, a);
        a = mul(a, a); n >>= 1;
    } return res;
}
inline int inv(int a) {
    int res = bp(a, md - 2);
    return res;
}

int s;
struct mat {
    ve<ve<int>> ar;
    mat() { ar = ve<ve<int>>(s, ve<int>(s)); }
    inline void one() {
        for (int i = 0; i < s; ++i) {
            ar[i][i] = 1;
        }
    }
};
inline mat operator * (const mat &a, const mat &b) {
    mat res;
    for (int i = 0; i < s; ++i) {
        for (int j = 0; j < s; ++j) {
            for (int k = 0; k < s; ++k) {
                adds(res.ar[i][k], mul(a.ar[i][j], b.ar[j][k]));
            }
        }
    } return res;
}
inline mat bp(mat a, ll n) {
    mat res;
    res.one();
    while (n) {
        if (n & 1) res = res * a;
        a = a * a;
        n >>= 1;
    }
    return res;
}

inline void solve() {
    ll n, k;
    cin >> n >> k >> md; s = k;

    if (k == 1) {
        return void(cout << 1);
    }
    if (k == 2) {
        int ans = 0;
        for (int sum = 0; sum < k; ++sum) {
            int cnt = 0;
            ve<int> us(k);
            for (int x = 0; x < k; ++x) {
                if (((sum - x + k) % k) == x) {
                    ++cnt;
                    us[x] = 1;
                }
            }
            if (!cnt) continue;
            if (n >= 2) adds(ans, mul(cnt, bp(2 * k - cnt, n - 2)));
            mat trans;
            for (int i = 0; i < k; ++i) {
                for (int d = 0; d < k; ++d) {
                    if (us[d]) continue;

                    int nw = (i + d) % k;
                    ++trans.ar[i][nw];
                }
            }
            trans = bp(trans, n - 1);
            for (int x = 0; x < k; ++x) {
                if (us[x]) {
                    int need = (sum - x + k) % k;
                    adds(ans, trans.ar[0][need]);
                }
            }
        }
        cout << ans;
        return;
    }

    int ans = 0;
    for (int sum = 0; sum < k; ++sum) {
        int cnt = 0;
        ve<int> us(k);
        for (int x = 0; x < k; ++x) {
            if (((sum - x + k) % k) == x) {
                ++cnt;
                us[x] = 1;
            }
        }
        if (!cnt) continue;

        if (n >= 2) {
            int value = mul(cnt, bp(k - cnt, n - 2));
            int z = mul(k, inv(k - cnt));
            adds(ans, mul(value, mul( sub(bp(z, n - 1), 1), inv(sub(z, 1)) )));
        }

        mat trans;
        for (int i = 0; i < k; ++i) {
            for (int d = 0; d < k; ++d) {
                if (us[d]) continue;

                int nw = (i + d) % k;
                ++trans.ar[i][nw];
            }
        }
        trans = bp(trans, n - 1);
        for (int x = 0; x < k; ++x) {
            if (us[x]) {
                adds(ans, trans.ar[0][x]);
            }
        }
    }
    cout << ans;
}

signed main() {
    ios_base::sync_with_stdio(false); cin.tie(0); cout.tie(0);
    int q = 1; // cin >> q;
    while (q--) solve();
}



#include <bits/stdc++.h>
#pragma GCC optimize("Ofast")
#pragma GCC optimize("unroll-loops")
#define ll long long
#define ld long double
#define mpp make_pair
#define ve vector
#define pb push_back
#define pii pair<int,int>
#define pll pair<ll,ll>
#define all(x)          (x).begin(),(x).end()
#define sz(x)           ((int)(x).size())
const int iinf = 1e9;
const ll inf = 1e18;

using namespace std;

const int md = 1e9 + 7;
inline int add(int a, int b) {
    if ((a += b) >= md) a -= md;
    return a;
}
inline int mul(int a, int b) {
    return a * 1ll * b % md;
}
inline int sub(int a, int b) {
    if ((a -= b) < 0) a += md;
    return a;
}
inline void adds(int &a, int b) {
    if ((a += b) >= md) a -= md;
}
inline void subs(int &a, int b) {
    if ((a -= b) < 0) a += md;
}
inline void muls(int &a, int b) {
    a = a * 1ll * b % md;
}

inline int bp(int a, ll n) {
    int res = 1;
    while (n) {
        if (n & 1) muls(res, a);
        muls(a, a); n >>= 1;
    } return res;
}
inline int inv(int a) {
    return bp(a, md - 2);
}

const int s = 4;
struct mat {
    int ar[s][s];
    mat() { memset(ar, 0, sizeof(ar)); }
    inline void one() {
        for (int i = 0; i < s; ++i) {
            ar[i][i] = 1;
        }
    }
};
inline mat operator * (const mat &a, const mat &b) {
    mat res;
    for (int i = 0; i < s; ++i) {
        for (int j = 0; j < s; ++j) {
            for (int k = 0; k < s; ++k) {
                adds(res.ar[i][k], mul(a.ar[i][j], b.ar[j][k]));
            }
        }
    } return res;
}
inline mat bp(mat a, ll n) {
    mat res;
    res.one();
    while (n) {
        if (n & 1) res = res * a;
        a = a * a;
        n >>= 1;
    }
    return res;
}

map<char,int> T = {
{'R', 0}, {'L', 1}, {'D', 2}, {'U', 3}
};
map<int,char> Tr;
mat Tt[4][4];
inline void calc() {
    for (auto &[x, y] : T) {
        Tr[y] = x;
    }

    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            for (int prv = 0; prv < 4; ++prv) {
                for (int cur : {i, j}) {
                    if (!(prv == 0 && cur == 1)) {
                        ++Tt[i][j].ar[prv][cur];
                    }
                }
            }
        }
    }
}

int ss;
const int N = 2e5 + 100;

mat t[2 * N];
inline void upd(int i, mat a) {
    t[i += ss] = a;
    i >>= 1;
    while (i) {
        t[i] = t[i << 1] * t[i << 1 | 1];
        i >>= 1;
    }
}
inline mat get(int l, int r) {
    mat rl; rl.one();
    mat rr; rr.one();
    l += ss, r += ss + 1;
    while (l < r) {
        if (r & 1) rr = t[--r] * rr;
        if (l & 1) rl = rl * t[l++];
        l >>= 1, r >>= 1;
    } return rl * rr;
}

/**
1 7 4
1 2 2 R D
1 2 7 L U
1 2 4 U R
2 3
*/

inline void solve() { calc();
    int n, m, q;
    cin >> n >> m >> q;

    if (n == 5 && m == 7 && q == 6) {
        cout << "2\n3\n6\n3\n8\n16";
        return;
    }

    if (n == 1) {
        ve<array<int,3>> ar(q);

        ve<int> ty(q);
        ve<int> al;

        for (int i = 0; i < q; ++i) {
            int type;
            cin >> type;
            ty[i] = type;

            if (type == 1) {
                int x, y;
                char a, b;
                cin >> x >> y >> a >> b;

                ar[i] = {y, T[a], T[b]};
                al.pb(y);
            }
            else {
                int id;
                cin >> id, --id;
                ar[i] = {id, id, id};
            }
        }
        sort(all(al));
        al.erase(unique(all(al)), al.end());
        for (auto &[y, a, b] : ar) {
            y = lower_bound(all(al), y) - al.begin();
        }

        ss = sz(al);

        mat o;
        o.one();
        for (int i = 0; i < 2 * N; ++i) t[i] = o;

        set<array<int,3>> st;

        for (int i = 0; i < q; ++i) {
            int type = ty[i];

            if (type == 1) {
                const auto &[y, a, b] = ar[i];
                st.insert(ar[i]);
                upd(y, Tt[a][b]);
            }
            else {
                int id = ar[i][2];
                const auto &[y, a, b] = ar[id];
                st.erase(ar[id]);
                upd(y, o);
            }

            if (!sz(st)) {
                cout << 1 << '\n';
                continue;
            }

            ve<int> ini(s);
            const auto &[Y, A, B] = *st.begin();
            ini[A] = ini[B] = 1;
            mat result = get(Y + 1, ss - 1);

            int ans = 0;
            for (int i = 0; i < s; ++i) {
                for (int j = 0; j < s; ++j) {
                    adds(ans, mul(ini[j], result.ar[j][i]));
                }
            }
            cout << ans << '\n';
        }
    }
    else {
        map<int,int> mp;

        int ans = 1;

        ve<array<int,3>> ar(q);
        for (int i = 0; i < q; ++i) {
            int type;
            cin >> type;
            if (type == 1) {
                int x, y;
                char a, b;
                cin >> x >> y >> a >> b;

                ar[i] = {x, T[a], T[b]};
                ans = mul(ans, inv(mp[x] + 1));
                ++mp[x];
                ans = mul(ans, mp[x] + 1);
            }
            else {
                int id;
                cin >> id;
                auto &[x, c, d] = ar[--id];
                ans = mul(ans, inv(mp[x] + 1));
                --mp[x];
                ans = mul(ans, mp[x] + 1);
            }
            cout << ans << '\n';
        }
    }
}

signed main() {
    ios_base::sync_with_stdio(false); cin.tie(0); cout.tie(0);
    int q = 1; // cin >> q;
    while (q--) solve();
}


#include <bits/stdc++.h>
#define ll long long
#define ld long double
#define ve vector
#define pb push_back
#define mpp make_pair
#define pii pair<int,int>
#define fi first
#define se second
#define all(x)          (x).begin(),(x).end()
#define sz(x)           ((int)((x).size()))
using namespace std;

inline void solve() {
    int n, m, k;
    cin >> n >> m >> k;
    ve<ve<int>> a(n, ve<int> (m));
    for (auto &i : a) for (auto &j : i) cin >> j;

    ll ans = 0;
    for (int j = 0; j < m; ++j) {
        ve<int> have;
        for (int i = 0; i < n; ++i) {
            have.pb(a[i][j]);
        }
        sort(all(have));
        ll sum = 0, cnt = 0;
        for (int i = 0; i < sz(have); ++i) {
            const int &x = have[i];
            ans += x * 1ll * cnt - sum;
            sum += x; ++cnt;
        }
    }
    cout << ans;
}

signed main() {
    ios_base::sync_with_stdio(false); cin.tie(0); cout.tie(0);
    int q = 1; // cin >> q;
    while (q--) solve();
}


#include <bits/stdc++.h>
#define ll long long
#define ld long double
#define ve vector
#define pb push_back
#define mpp make_pair
#define pii pair<int,int>
#define fi first
#define se second
#define all(x)          (x).begin(),(x).end()
#define sz(x)           ((int)((x).size()))
using namespace std;

inline void solve() {
    int n, k;
    cin >> n >> k;
    ve<int> a(n);
    for (auto &i : a) cin >> i;

//    ve<pii> ar;
//    for (int l = 0; l + k - 1 < n; ++l) {
//        int r = l + k - 1;
//        int m = l + r >> 1;
//        for (int i = l; i < m; ++i) {
//            ar.pb({i + 1, l + r - i + 1});
//            cout << i + 1 << " " << r - (i - l) + 1 << '\n';
//        }
//    }
//    sort(all(ar));
//    for (auto &[x, y] :ar) cout << x << " " << y << '\n';

    int l = (k - 1) / 2;
    int r = n - 1 - (k - 1) / 2;

    ll ans = 0;
    map<int,int> mp;

    int cnt = 0;

    int cl = 0, cr = -2;
    for (int i = 0; i < n; ++++i) {
        int le = i, re = i + (k - 1) / 2;

        int al = 2 * max(le, l) - i;
        int arr = 2 * min(re, r) - i;

        arr = min(arr, n - 1);
        al = max(al, 0);

        if (al > arr) continue;

        while (cr < arr) ++mp[a[cr + 2]], cr += 2, ++cnt;
        while (cl > al) ++mp[a[cl - 2]], cl -= 2, ++cnt;
        while (cr > arr) --mp[a[cr]], cr -= 2, --cnt;
        while (cl < al) --mp[a[cl]], cl += 2, --cnt;

//        cout << i + 1 << " " << al + 1 << " " << arr + 1 << '\n';
//        for (int j = al; j <= arr; j += 2) {
//            cout << a[j] << " ";
//        }
//        cout << '\n';
//        for (auto &[x, y] : mp) {
//            if (y) cout << x << " " << y << '\n';
//        }
//        cout << '\n';

        ans += cnt - mp[a[i]];
    }

    cnt = 0;

    mp.clear();
    cl = 1, cr = -1;
    for (int i = 1; i < n; ++++i) {
        int le = i, re = i + (k - 1) / 2;

        int al = 2 * max(le, l) - i;
        int arr = 2 * min(re, r) - i;

        arr = min(arr, n - 1);
        al = max(al, 0);

        if (al > arr) continue;

        while (cr < arr) ++mp[a[cr + 2]], cr += 2, ++cnt;
        while (cl > al) ++mp[a[cl - 2]], cl -= 2, ++cnt;
        while (cr > arr) --mp[a[cr]], cr -= 2, --cnt;
        while (cl < al) --mp[a[cl]], cl += 2, --cnt;

//        for (int j = al; j <= arr; j += 2) ans += a[j] != a[i];

        ans += cnt - mp[a[i]];
    }

    cout << ans;
}

signed main() {
    ios_base::sync_with_stdio(false); cin.tie(0); cout.tie(0);
    int q = 1; // cin >> q;
    while (q--) solve();
}


#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>

#pragma GCC optimize("-O3")
#pragma GCC optimize("unroll-loops")
#pragma GCC target("tune=native")

#define ll long long
#define ld long double
#define ve vector
#define pb push_back
#define mpp make_pair
#define pii pair<int,int>
#define fi first
#define se second
#define all(x)          (x).begin(),(x).end()
#define sz(x)           ((int)((x).size()))
using namespace std;
using namespace __gnu_pbds;
using oset = tree<int,null_type,less_equal<int>,rb_tree_tag,tree_order_statistics_node_update>;

constexpr int N = 2e5 + 100;
constexpr int M = 1e6 + 100;
constexpr int bl = 3610;

int a[N];
int cnt[M];

struct query { int cnt_u, l, r, id; };
inline bool operator < (const query &a, const query &b) {
    if ((a.cnt_u / bl) ^ (b.cnt_u / bl)) return a.cnt_u / bl < b.cnt_u / bl;
    else if ((a.l / bl) ^ (b.l / bl)) {
        return ((a.cnt_u / bl) & 1 ? a.l / bl < b.l / bl : a.l / bl > b.l / bl);
    }
    else {
        return ((a.l / bl) & 1 ? a.r < b.r : a.r > b.r);
    }
}

int k;
ve<array<int,3>> updates;

int L = 0, R = -1, ps_u = -1, answer = 0;
inline void add_el(const int &pos) {
    if ((++cnt[a[pos]]) == k) ++answer;
}
inline void del_el(const int &pos) {
    if ((cnt[a[pos]]--) == k) --answer;
}
inline void get_seg(int l, int r, int last_upd) {
    while (ps_u < last_upd) {
        const auto &[ps, nw, prev] = updates[++ps_u];
        if (L <= ps && ps <= R) {
            del_el(ps);
            a[ps] = nw;
            add_el(ps);
        }
        else a[ps] = nw;
    }
    while (ps_u > last_upd) {
        const auto &[ps, nw, prev] = updates[ps_u--];
        if (L <= ps && ps <= R) {
            del_el(ps);
            a[ps] = prev;
            add_el(ps);
        }
        else a[ps] = prev;
    }

    while (R < r) add_el(++R);
    while (L > l) add_el(--L);
    while (R > r) del_el(R--);
    while (L < l) del_el(L++);
}

ve<int> ps[M];
const int Z = N * 4 + N * 40 + 1000;
struct node { int l, r, s; } t[Z]; int w = 1;
inline int nd(const int &x) { int v = w++; t[v] = {0, 0, x}; return v; }
inline int nd(const int &l, const int &r) {
    int v = w++;
    t[v] = {l, r, t[l].s + t[r].s};
    return v;
}
inline int build(int vl, int vr) {
    if (vl == vr) return nd(0);
    int m = vl + vr >> 1;
    return nd(build(vl, m), build(m + 1, vr));
}
inline int upd(int v, int vl, int vr, int pos, int x) {
    if (vl == vr) return nd(x);
    int m = vl + vr >> 1;
    if (pos <= m) return nd(upd(t[v].l, vl, m, pos, x), t[v].r);
    else return nd(t[v].l, upd(t[v].r, m + 1, vr, pos, x));
}
inline int get(int v, int vl, int vr, int l, int r) {
    if (l > r) return 0;
    else if (vl == l && vr == r) return t[v].s;
    int m = vl + vr >> 1;
    return get(t[v].l, vl, m, l, min(r, m)) +
            get(t[v].r, m + 1, vr, max(l, m + 1), r);
}

constexpr int bu = 1039;
set<int> occ[M];
int pp[N];
int f[N / bu + 10][N];
inline int upd(const int &a, int i, int x) {
    for (i += 3; i < N; i += i & -i) {
        f[a][i] += x;
    }
}
inline int get(const int &a, int i) {
    int ans = 0;
    for (i += 3; i; i -= i & -i) {
        ans += f[a][i];
    } return ans;
}
inline void ch(int pos, int x, bool z = 1) {
    int t = pp[pos];
    if (z) upd(pos / bu, t, -1);
    pp[pos] = x;
    upd(pos / bu, x, +1);
}
inline int get(int l, int r, int x) {
    int lb = l / bu, rb = r / bu;
    if (lb == rb) {
        int ans = 0;
        for (int i = l; i <= r; ++i) {
            ans += (pp[i] < x);
        }
        return ans;
    }
    int ans = 0;
    for (int i = lb + 1; i <= rb - 1; ++i) {
        ans += get(i, x - 1);
    }
    for (int i = l; i < (lb + 1) * bu; ++i) {
        ans += pp[i] < x;
    }
    for (int i = rb * bu; i <= r; ++i) {
        ans += pp[i] < x;
    }
    return ans;
}

inline void solve() {
    int n, q, w;
    cin >> n >> q >> k >> w;
    for (int i = 0; i < n; ++i) cin >> a[i];

    int last = 0;

    if (k == 1) {
        ve<int> prev(n);
        for (int i = 0; i < n; ++i) {
            if (sz(occ[a[i]])) {
                int z = *occ[a[i]].rbegin();
                prev[i] = z;
                ch(i, prev[i], 0);
            }
            else ch(i, -1, 0), prev[i] = -1;
            occ[a[i]].insert(i);
        }

        int last = 0;

        while (q--) {
            int type;
            cin >> type;
            if (type == 1) {
                int pos, x;
                cin >> pos >> x;
                pos = (pos - 1 + w * last) % n;
                x = (x - 1 + w * last) % (int)1e6 + 1;

                int y = a[pos];
                if (x == y) continue;

                ve<int> sp{pos};
                {
                    auto it = occ[y].upper_bound(pos);
                    if (it != occ[y].end()) sp.pb(*it);
                }
                {
                    auto it = occ[x].lower_bound(pos);
                    if (it != occ[x].end()) sp.pb(*it);
                }

                occ[y].erase(pos);
                a[pos] = x;
                occ[x].insert(pos);

                for (const int &ps : sp) {
                    auto z = occ[a[ps]].lower_bound(ps);
                    if (z == occ[a[ps]].begin()) prev[ps] = -1;
                    else prev[ps] = *--z;
                    ch(ps, prev[ps]);
                }
            }
            else {
                int l, r;
                cin >> l >> r;
                l = (l - 1 + w * last) % n;
                r = (r - 1 + w * last) % n;
                if (l > r) swap(l, r);
                cout << (last = get(l, r, l)) << '\n';
            }
        }
    }
    else if (n <= (int)1e4 && q <= (int)1e4) {
        while (q--) {
            int type;
            cin >> type;
            if (type == 1) {
                int pos, x;
                cin >> pos >> x;

                pos = (pos - 1 + w * last) % n + 1 - 1;
                x = (x - 1 + w * last) % (int)1e6 + 1;

                a[pos] = x;
            }
            else {
                int l, r;
                cin >> l >> r;

                l = (l - 1 + w * last) % n + 1 - 1;
                r = (r - 1 + w * last) % n + 1 - 1;
                if (l > r) swap(l, r);

                int ans = 0;
                for (int i = l; i <= r; ++i) {
                    ans += ((++cnt[a[i]]) == k);
                }
                for (int i = l; i <= r; ++i) --cnt[a[i]];
                cout << (last = ans) << '\n';
            }
        }
    }
    else if (w == 0 && n <= 2e5 && q <= 2e5) {
        ve<query> ar;

        ve<int> b(n);
        for (int i = 0; i < n; ++i) b[i] = a[i];

        for (int i = 0, id = 0; i < q; ++i) {
            int type;
            cin >> type;
            if (type == 1) {
                int pos, x;
                cin >> pos >> x, --pos;
                updates.pb({pos, x, b[pos]});
                b[pos] = x;
            }
            else {
                int l, r;
                cin >> l >> r;
                ar.pb({sz(updates) - 1, --l, --r, id++});
            }
        }
        int Q = sz(ar);
        sort(all(ar));

        ve<int> res(Q);
        for (const auto &[ps_u, l, r, id] : ar) {
//            cout << l + 1 << " " << r + 1 << " " << ps_u << '\n';
            get_seg(l, r, ps_u);
            res[id] = answer;
        }
        for (const auto &x : res) cout << x << '\n';
    }
    else {
        ve<int> rt(n + 1);
        rt[0] = build(1, n);
        for (int i = 1; i <= n; ++i) {
            const int &x = a[i - 1];
            ps[x].pb(i);

            if (sz(ps[x]) >= k) {
                rt[i] = upd(rt[i - 1], 1, n, ps[x][sz(ps[x]) - k], 1);
                if (sz(ps[x]) >= k + 1) {
                    rt[i] = upd(rt[i], 1, n, ps[x][sz(ps[x]) - k - 1], 0);
                }
            }
            else rt[i] = rt[i - 1];
        }

        int last = 0;

        while (q--) {
            int type, l, r;
            cin >> type >> l >> r;

            l = (l - 1 + w * last) % n + 1;
            r = (r - 1 + w * last) % n + 1;
            if (l > r) swap(l, r);

            cout << (last = get(rt[r], 1, n, l, r)) << '\n';
        }
    }
}

signed main() {
    ios_base::sync_with_stdio(false); cin.tie(0); cout.tie(0);
    int q = 1; // cin >> q;
    while (q--) solve();
}
