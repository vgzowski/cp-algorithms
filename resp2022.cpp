/// Bad Dlinka
class BigInt {
private:
    static const int base = 1e9;
    static const int len = 9;
    bool sgn; // 1 - pos, 0 - neg

    vector <int> dig;

    void normalize();
    BigInt babs() const {
        return sgn ? *this : -*this;
    };
public:
    BigInt() {}
    BigInt(const int &);
    BigInt(const string &);

    bool operator < (const BigInt &) const;
    bool operator == (const BigInt &) const;
    friend istream & operator >> (istream &, BigInt &);
    friend ostream & operator << (ostream &, const BigInt &);

    BigInt operator + (const BigInt &) const;
    BigInt& operator++ (int);

    BigInt operator * (const BigInt &) const;
    BigInt operator * (const int &) const;

    friend BigInt operator - (const BigInt &);
    BigInt operator - (const BigInt &) const;

    BigInt operator / (const int &) const;
    int operator % (const int &) const;
};

BigInt::BigInt(const int &x) {
    sgn = (x >= 0 ? 1 : 0);
    int r = abs(x);

    while (r) {
        dig.push_back(r % base);
        r /= base;
    }
}
BigInt::BigInt(const string &s) {
    int pos = 0;
    sgn = 1;
    if (s[0] == '-') {
        sgn = 0;
        pos = 1;
    }
    for (int i = (int)s.size() - len; i >= pos; i -= len)
        dig.push_back(stoi(s.substr(i, len)));
    if (((int)s.size() - pos) % len != 0)
        dig.push_back(stoi(s.substr(pos, ((int)s.size() - pos) % len)));
}
BigInt operator - (const BigInt &a) {
    BigInt res = a;
    res.sgn ^= 1;
    return res;
}
void BigInt::normalize() {
    for (int i = 0; i < dig.size(); ++i) {
        if (dig[i] >= base) {
            int r = dig[i] / base;
            dig[i] %= base;

            if (i + 1 == dig.size())
                dig.push_back(0);
            dig[i + 1] += r;
        }
        if (dig[i] < 0) {
            dig[i] += base;
            --dig[i + 1];
        }
    }
    while (dig.size() > 1 && dig[dig.size() - 1] == 0)
        dig.pop_back();
    if (dig.size() == 1 && dig[0] == 0)
        sgn = 1;
}

bool BigInt::operator < (const BigInt &a) const {
    if (!sgn && a.sgn)
        return true;
    else if (sgn && !a.sgn)
        return false;

    const bool change = (sgn == 0);

    if (dig.size() < a.dig.size())
        return true ^ change;
    else if (dig.size() > a.dig.size())
        return false ^ change;
    else {
        for (int i = (int)dig.size() - 1; i >= 0; --i) {
            if (dig[i] < a.dig[i])
                return true ^ change;
            else if (dig[i] > a.dig[i])
                return false ^ change;
        }
    }
    return false;
}
bool BigInt::operator == (const BigInt &a) const {
    if (sgn != a.sgn || dig.size() != a.dig.size())
        return false;
    for (int i = 0; i < dig.size(); ++i) {
        if (dig[i] != a.dig[i])
            return false;
    }
    return true;
}

istream & operator >> (istream &in, BigInt &a) {
    string s;
    in >> s;
    a = BigInt(s);
    return in;
}
ostream & operator << (ostream &out, const BigInt &a) {
    if (a.dig.empty()) {
        out << 0;
        return out;
    }

    if (!a.sgn)
        out << '-';

    out << a.dig.back();

    out.fill('0');
    for (int i = (int)a.dig.size() - 2; i >= 0; --i) {
        out << setw(a.len) << a.dig[i];
    }
    return out;
}

BigInt& BigInt::operator ++ (int) {
    if (*this == BigInt(0)) {
        *this = BigInt(1);
        return *this;
    }
    if (sgn == 1) {
        ++dig[0];
        normalize();
        return *this;
    }
    else {
        --dig[0];
        normalize();
        return *this;
    }
}
BigInt BigInt::operator + (const BigInt &a) const {
    if (a == BigInt(0))
        return *this;
    if (sgn == a.sgn) {
        BigInt res = *this;
        res.dig.resize(max(dig.size(), a.dig.size()) + 1);

        for (int i = 0; i < a.dig.size(); ++i) {
            res.dig[i] += a.dig[i];
        }
        res.normalize();
        return res;
    }
    else {
        return *this - (-a);
    }
}
BigInt BigInt::operator - (const BigInt &a) const {
    if (a == BigInt(0))
        return *this;
    if (*this == a)
        return BigInt(0);
    if (sgn == a.sgn) {
        if (babs() < a.babs()) {
            BigInt res = a - *this;
            res.sgn = a.sgn ^ 1;
            return res;
        }
        else {
            BigInt res = *this;
            res.sgn = sgn;
            for (int i = 0; i < a.dig.size(); ++i) {
                res.dig[i] -= a.dig[i];
            }
            res.normalize();
            return res;
        }
    }
    else {
        return *this + (-a);
    }
}

BigInt BigInt::operator * (const BigInt &a) const {
    if (a == BigInt(0) || *this == BigInt(0))
        return BigInt(0);

    BigInt res;
    res.sgn = !((!sgn) ^ (!a.sgn));
    res.dig.resize(dig.size() + a.dig.size());

    for (int i = 0; i < dig.size(); ++i) {
        for (int j = 0; j < a.dig.size(); ++j) {
            long long f = dig[i] * 1ll * a.dig[j] + res.dig[i + j];
            res.dig[i + j] = f % base;
            res.dig[i + j + 1] += f / base;
        }
    }
    res.normalize();
    return res;
}
BigInt BigInt::operator / (const int &x) const {
    BigInt res;
    res.sgn = sgn;
    res.dig.resize(dig.size());
    if (x < 0)
        res.sgn ^= 1;

    int X = abs(x), carry = 0;
    for (int i = (int)dig.size() - 1; i >= 0; --i) {
        long long cur = base * 1ll * carry + dig[i];
        res.dig[i] = cur / X;
        carry = cur % X;
    }
    res.normalize();
    if (res.sgn == 0 && carry > 0)
        res = res - BigInt(1);
    return res;
}
int BigInt::operator % (const int &x) const {
    bool Sgn = !((!sgn) ^ (x < 0));

    int X = abs(x), carry = 0;
    for (int i = dig.size() - 1; i >= 0; --i) {
        long long cur = base * 1ll * carry + dig[i];
        carry = cur % X;
    }
    if (Sgn == 0)
        carry = (X - carry) % X;
    return carry;
}

/// 1_1
#include <bits/stdc++.h>
#define ll long long
#define ld long double
#define pii pair<int,int>
#define ve vector
#define pb push_back
#define fi first
#define se second
#define all(x)          (x).begin(),(x).end()

using namespace std;

const int N = 1e6 + 5;
int p[N], rnk[N];
inline void build(int n) { for (int i = 0; i < n; ++i) p[i] = i, rnk[i] = 1; }
inline int par(int v) { if (p[v] == v) { return v; } return (p[v] = par(p[v])); }
inline bool uni(int a, int b) {
    a = par(a), b = par(b);
    if (a == b) return 0;
    if (rnk[a] > rnk[b]) swap(a, b);
    p[a] = b, rnk[b] += rnk[a]; return 1;
}

signed main() {
    ios_base::sync_with_stdio(false); cin.tie(0); cout.tie(0);

    int k, n, m;
    cin >> k >> n >> m;
    ve<ve<ve<char>>> a(k, ve<ve<char>> (n, ve<char>(m)));
    for (auto &i : a) for (auto &j : i) for (auto &z : j) cin >> z;

    ve<ve<int>> us(n,ve<int>(m));

    int c = n * m; build(c);
    for (int i = 0; i < n; ++i) {
        for (int j = 1; j < m; ++j) {
            bool ok = 1;
            for (int z = 0; z < k; ++z) {
                ok &= (a[z][i][j] == a[z][i][j - 1]);
                if (!ok) break;
            }
            if (ok) uni(i * m + j, i * m + j - 1);
        }
    }
    int cnt = 0;
    for (int i = 0; i < c; ++i) cnt += (par(i) == i);
    cout << cnt;
}



/// 1_2
#include <bits/stdc++.h>
#define ll long long
#define ld long double
#define pii pair<int,int>
#define ve vector
#define pb push_back
#define fi first
#define se second
#define all(x)          (x).begin(),(x).end()

using namespace std;

const int N = 2e5 + 10; pii p[4 * N]; int a[N];
struct node { int s, fr0, ls0, fr1, ls1; } t[4 * N];
inline node cr(int x, int vl) {
    node res;
    res.s = x;
    res.fr0 = res.fr1 = +1e9;
    res.ls0 = res.ls1 = -1e9;
    if (!x) res.fr0 = res.ls0 = vl;
    else res.fr1 = res.ls1 = vl;
    return res;
}
inline node mg(const node &a, const node &b) {
    node res;
    res.s = a.s + b.s;
    res.fr0 = min(a.fr0, b.fr0);
    res.fr1 = min(a.fr1, b.fr1);
    res.ls0 = max(a.ls0, b.ls0);
    res.ls1 = max(a.ls1, b.ls1);
    return res;
}
inline void merg(pii &a, pii b) {
    if (b.fi != -1) a = b;
    else a.se ^= b.se;
}
inline void apply(int v, int vl, int vr, pii kek) {
    if (kek.fi != -1) {
        int x = kek.fi;
        t[v].s = x * (vr - vl + 1);
        t[v].fr0 = t[v].fr1 = 1e9;
        t[v].ls0 = t[v].ls1 = -1e9;
        if (!x) t[v].fr0 = vl, t[v].ls0 = vr;
        else if (x) t[v].fr1 = vl, t[v].ls1 = vr;
    }
    if (kek.se) {
        t[v].s = vr - vl + 1 - t[v].s;
        swap(t[v].fr0, t[v].fr1);
        swap(t[v].ls0, t[v].ls1);
    }
    kek = {-1, 0};
}
inline void push(int v, int vl, int vr) {
    if (p[v] == make_pair(-1, 0)) return;
    if (vl != vr) for (auto u : {2 * v + 1, 2 * v + 2}) merg(p[u], p[v]);
    apply(v, vl, vr, p[v]);
    p[v] = {-1, 0};
}
inline void build(int v, int vl, int vr) {
    if (vl == vr) return void(t[v] = cr(a[vl], vl));
    int m = vl + vr >> 1;
    build(2 * v + 1, vl, m);
    build(2 * v + 2, m + 1, vr);
    t[v] = mg(t[2 * v + 1], t[2 * v + 2]);
}
inline node get(int v, int vl, int vr, int l, int r) {
    push(v, vl, vr);
    if (vl == l && vr == r) return t[v];
    int m = vl + vr >> 1;
    if (r <= m) return get(2 * v + 1, vl, m, l, r);
    else if (l > m) return get(2 * v + 2, m + 1, vr, l, r);
    else {
        node a = get(2 * v + 1, vl, m, l, m);
        node b = get(2 * v + 2, m + 1, vr, m + 1, r);
        return mg(a, b);
    }
}
inline void upd1(int v, int vl, int vr, int l, int r, int x) {
    push(v, vl, vr);
    if (l > r) return;
    else if (vl == l && vr == r) {
        p[v] = {x, 0}; push(v, vl, vr);
        return;
    }
    int m = vl + vr >> 1;
    upd1(2 * v + 1, vl, m, l, min(r, m), x);
    upd1(2 * v + 2, m + 1, vr, max(l, m + 1), r, x);
    t[v] = mg(t[2 * v + 1], t[2 * v + 2]);
}
inline void upd2(int v, int vl, int vr, int l, int r, int x) {
    push(v, vl, vr);
    if (l > r) return;
    else if (vl == l && vr == r) {
        p[v].se ^= x; push(v, vl, vr);
        return;
    }
    int m = vl + vr >> 1;
    upd2(2 * v + 1, vl, m, l, min(r, m), x);
    upd2(2 * v + 2, m + 1, vr, max(l, m + 1), r, x);
    t[v] = mg(t[2 * v + 1], t[2 * v + 2]);
}
int n;
inline void build() { build(0, 0, n - 1); }
inline node get(int l, int r) { return get(0, 0, n - 1, l, r); }
inline void upd1(int l, int r, int x) { upd1(0, 0, n - 1, l, r, x); }
inline void upd2(int l, int r, int x) { upd2(0, 0, n - 1, l, r, x); }
inline int get1(int l, int r) { return get(l, r).s; }
inline int get0(int l, int r) { return r - l + 1 - get(l, r).s; }

signed main() {
    ios_base::sync_with_stdio(false); cin.tie(0); cout.tie(0);
    int q; cin >> n >> q;
    for (int i = 0; i < n; ++i) {
        char x;
        cin >> x;
        a[i] = x - '0';
    }
    build();
    for (int i = 0; i < 4 * n; ++i) p[i] = {-1, 0};
    while (q--) {
        int te;
        cin >> te;
        if (te == 1) {
            int l, r;
            cin >> l >> r, --l, --r;
            upd2(l, r, 1);
        }
        else if (te == 2) {
            int l, r, k;
            cin >> l >> r >> k, --l, --r;
            int len = r - l + 1;

            int le = 1, re = len / 2, mid, ans = 0, cnt = k;
            while (le <= re) {
                mid = le + re >> 1;
                int f = get0(l, l + mid - 1);
                int g = get1(r - mid + 1, r);
                if (f + g <= k) ans = mid, cnt = k - f - g, le = mid + 1;
                else re = mid - 1;
            }
            upd1(l, l + ans - 1, 1);
            upd1(r - ans + 1, r, 0);
            cnt &= 1;
            if (cnt) {
                auto cur = get(l, r);
                int aa = cur.fr0, bb = cur.ls1;
                if (aa - l + 1 <= r - bb + 1) upd1(aa, aa, 1);
                else upd1(bb, bb, 0);
            }
        }
        else {
            int l, r;
            cin >> l >> r, --l, --r;
            cout << get1(l, r) << '\n';
        }
    }
}



/// 1_3
#include <bits/stdc++.h>
#define ll long long
#define ld long double
#define pii pair<int,int>
#define ve vector
#define pb push_back
#define fi first
#define se second
#define all(x)          (x).begin(),(x).end()

using namespace std;

const int N = 2e5 + 10; ll t[4 * N], p[4 * N];
inline void build(int v, int vl, int vr, ve<ll> &ar) {
    if (vl == vr) return void(t[v] = ar[vl]);
    int m = vl + vr >> 1;
    build(2 * v + 1, vl, m, ar);
    build(2 * v + 2, m + 1, vr, ar);
    t[v] = min(t[2 * v + 1], t[2 * v + 2]);
}
inline void push(int v, int vl, int vr) {
    if (!p[v]) return;
    if (vl != vr) for (auto u : {2 * v + 1, 2 * v + 2}) p[u] += p[v];
    t[v] += p[v];
    p[v] = 0;
}
inline void upd(int v, int vl, int vr, int l, int r, ll x) {
    push(v, vl, vr);
    if (l > r) return;
    else if (vl == l && vr == r) {
        p[v] += x; push(v, vl, vr);
        return;
    }
    int m = vl + vr >> 1;
    upd(2 * v + 1, vl, m, l, min(r, m), x);
    upd(2 * v + 2, m + 1, vr, max(l, m + 1), r, x);
    t[v] = min(t[2 * v + 1], t[2 * v + 2]);
}
inline ll get(int v, int vl, int vr, int l, int r) {
    push(v, vl, vr);
    if (l > r) return (ll)1e18;
    else if (vl == l && vr == r) return t[v];
    int m = vl + vr >> 1;
    ll a = get(2 * v + 1, vl, m, l, min(r, m));
    ll b = get(2 * v + 2, m + 1, vr, max(l, m + 1), r);
    return min(a, b);
}

ll f[N];
inline void upd(int i, ll x) { i += 2; for (; i < N; i += i & -i) f[i] += x; }
inline ll get(int i) { ll ans = 0; i += 2; for (; i; i -= i & -i) { ans += f[i]; } return ans; }

signed main() {
    ios_base::sync_with_stdio(false); cin.tie(0); cout.tie(0);

    int n, q;
    cin >> n >> q;
    ve<ll> a(n);
    for (auto &i : a) cin >> i;
    ve<ll> pf(n); pf[0] = a[0];
    for (int i = 1; i < n; ++i) pf[i] = pf[i - 1] + a[i];
    ve<ll> ar(n - 1); int m = n - 2;
    for (int i = 0; i + 1 < n; ++i) ar[i] = 2 * pf[i] - pf[i + 1];

    if (n == 1) {
        while (q--) {
            int te;
            cin >> te;
            if (te == 1) {
                int pos, x;
                cin >> pos >> x, --pos;
            }
            else {
                int l, r;
                cin >> l >> r, --l, --r;
                int ans = l;
                cout << ans + 1 << '\n';
            }
        }
        return 0;
    }

    build(0, 0, m, ar);

    while (q--) {
        int te;
        cin >> te;
        if (te == 1) {
            int pos, x;
            cin >> pos >> x, --pos;
            ll df = x - a[pos];
            a[pos] = x;

            upd(0, 0, m, pos, m, 2 * df);
            upd(0, 0, m, max(0, pos - 1), m, -df);
            upd(pos, df);
        }
        else {
            int l, r;
            cin >> l >> r, --l, --r;
            ll need = (!l ? 0 : pf[l - 1] + get(l - 1));

            int le = l, re = r - 1, mid, ans = l;
            while (le <= re) {
                mid = le + re >> 1;
                if (get(0, 0, m, mid, r - 1) <= need) ans = mid + 1, le = mid + 1;
                else re = mid - 1;
            }
            cout << ans + 1 << '\n';
        }
    }
}



/// 1_4(32)
#include <bits/stdc++.h>
#define ll long long
#define ld long double
#define pii pair<int,int>
#define ve vector
#define pb push_back
#define fi first
#define se second
#define all(x)          (x).begin(),(x).end()

using namespace std;

const int N = 4e3 + 5;
int p[N], rnk[N];
inline void build(int n) { for (int i = 0; i < n; ++i) p[i] = i, rnk[i] = 1; }
inline int par(int v) { if (p[v] == v) { return v; } return (p[v] = par(p[v])); }
inline bool uni(int a, int b) {
    a = par(a), b = par(b);
    if (a == b) return 0;
    if (rnk[a] > rnk[b]) swap(a, b);
    p[a] = b, rnk[b] += rnk[a]; return 1;
}

ve<int> tr = {0,1,1,0,1,0,1,0,1,1,1,0,1,1,1,0,1,0,1,0};
const bool debug = 0;

inline ll ask(ve<int> &p) {
    if (!debug) {
        cout << "? ";
        for (auto i : p) cout << i + 1 << " ";
        cout << endl;
        fflush(stdout);
    }

    if (debug) {
        int n = tr.size();
        ve<int> r(n);
        for (int i = 0; i < n; ++i) r[i] = tr[p[i]];
        ll v = 0;
        for (int i = 0; i < n; ++i) {
            for (int j = i + 1; j < n; ++j) {
                v += (r[i] > r[j]);
            }
        } return v;
    }
    else {
        ll v; cin >> v; return v;
    }
}

signed main() {
    ios_base::sync_with_stdio(false); cin.tie(0); cout.tie(0);

    int n;
    if (debug) n = tr.size();
    else cin >> n;

    ve<int> val(n, -1);
    ve<int> perm(n); iota(all(perm), 0);
    ll fck = ask(perm);

    auto cmp = [&](const int &a, const int &b) {
         ve<int> p(n); iota(all(p), 0);
         swap(p[a], p[b]);
         ll val = ask(p);
         return (val == fck ? 0 : (val < fck ? 1 : 2));
    };

    build(n);
    for (int i = 0; i + 1 < n; ++i) {
        int res = cmp(i, i + 1);
        if (!res) uni(i, i + 1);
        else if (res == 1) val[i] = 1, val[i + 1] = 0;
        else val[i] = 0, val[i + 1] = 1;
    }
    for (int i = 0; i < n; ++i) if (val[i] != -1) {
        val[par(i)] = val[i];
    }
    for (int i = 0; i < n; ++i) val[i] = val[par(i)];
    if (!debug) {
        cout << "! ";
        for (auto i : val) cout << i;
        cout << endl;
        fflush(stdout);
    }
    else {
        assert(val == tr);
    }
}














/// 2_1
#include <bits/stdc++.h>
#define ll long long
#define ld long double
#define ve vector
#define pb push_back
#define pii pair<int,int>
#define fi first
#define se second
#define all(x)      (x).begin(),(x).end()

using namespace std;

inline ll get(ll r) {
    ve<int> dig;
    while (r) { dig.pb(r & 1); r >>= 1; }
    reverse(all(dig)); int n = dig.size();
    ve<ve<ll>> dp(n + 1, ve<ll>(2)); dp[0][1] = 1;
    ve<ve<ll>> c(n + 1, ve<ll>(2)); c[0][1] = 0;
    for (int i = 0; i < n; ++i) {
        for (int fl : {0, 1}) {
            for (int d = 0; d < 2; ++d) {
                if (fl && d > dig[i]) continue;
                int f = fl & (d == dig[i]);
                dp[i + 1][f] += dp[i][fl];
                c[i + 1][f] += c[i][fl] + d * dp[i][fl];
            }
        }
    } return c[n][0] + c[n][1];
}
inline ll get(ll l, ll r) { if (l > r){return 0ll;} return get(r) - get(l - 1); }

inline ll f(ll k) {
    ll last = 63 - __builtin_clzll(k);
    ll ans = 0;
    for (ll len = 1; len - 1 < last; ++len) {
        ll cnt = (1ll << (len - 1));
        ans += cnt * len;
    }
    ans += (k - (1ll << last) + 1) * (last + 1);
    return ans;
}
inline ll get_big(ll k) {
    ll l = 0, r = (ll)2e17, mid, ans;
    while (l <= r) {
        mid = l + r >> 1;
        if (f(mid) >= k) ans = mid, r = mid - 1;
        else l = mid + 1;
    } return ans;
}

inline void solve() {
    ll l, r;
    cin >> l >> r;

    ll le = get_big(l), re = get_big(r);
    ll ans = get(le + 1, re - 1);

    ll cur = f(le - 1);
    ll lenl = 64 - __builtin_clzll(le);
    ll lenr = 64 - __builtin_clzll(re);
    if (le == re) {
        for (ll bit = 0; bit <= lenl; ++bit) {
            if (cur + bit + 1 >= l && cur + bit + 1 <= r) {
                ans += (le >> (lenl - bit - 1) & 1);
            }
        }
        cout << ans << '\n';
        return;
    }

    for (ll bit = 0; bit <= lenl; ++bit) {
        if (cur + lenl - bit >= l) {
                ans += (le >> bit & 1);
        }
    }
    cur = f(re - 1);
    for (ll bit = 0; bit <= lenr; ++bit) {
        if (cur + bit + 1 <= r) ans += (re >> (lenr - 1 - bit) & 1);
    }
    cout << ans << '\n';
}

signed main() {
    ios_base::sync_with_stdio(false); cin.tie(0); cout.tie(0);
    int q = 1; cin >> q;
    while (q--) solve();
}




/// 2_2(8)
#include <bits/stdc++.h>
#define ll long long
#define ld long double
#define ve vector
#define pb push_back
#define pii pair<int,int>
#define fi first
#define se second
#define all(x)      (x).begin(),(x).end()

using namespace std;

inline void solve() {
    int n, m;
    cin >> n >> m; int ans = 0;
    ve<int> p(n); iota(all(p), 0);
    do {
        ve<int> p1(n); iota(all(p1), 0);
        do {
            int cnt = 0;
            for (int i = 0; i < n; ++i) cnt += (p[i] == p1[i]);
            ans += cnt * cnt;
        } while (next_permutation(all(p1)));
    } while (next_permutation(all(p)));
    cout << ans % m;
}

signed main() {
    ios_base::sync_with_stdio(false); cin.tie(0); cout.tie(0);
    int q = 1; //cin >> q;
    while (q--) solve();
}



/// 2_3
#include <bits/stdc++.h>
#define ll long long
#define ld long double
#define ve vector
#define pb push_back
#define pii pair<int,int>
#define fi first
#define se second
#define all(x)      (x).begin(),(x).end()

using namespace std;

const int N = 2e5 + 5;

int a[N];
ve<int> gr[N], g[N];
int in[N], f[N], used[N];

int timer = 0;
inline void dfsb(int v, int p = -1) {
    int child = 0; used[v] = 1;
    in[v] = f[v] = ++timer;
    for (auto &to : gr[v]) if (to != p) {
        if (used[to]) f[v] = min(f[v], in[to]);
        else {
            dfsb(to, v), ++child;
            g[v].pb(to), g[to].pb(v);
            f[v] = min(f[v], f[to]);
        }
    }
}

int cntw[N], ans[N], pr[N], un[N], t[N];
map<int,int> *mp[N];

inline void dfs(int v, int p) {
    int mx = -1, mx1 = -1;
    for (auto &to : g[v]) {
        if (to == p) continue;
        dfs(to, v); pr[to] = v;
        if (!~mx || (*mp[to]).size() > (*mp[mx]).size()) mx = to;
        if (f[to] >= in[v] && (!~mx1 || (*mp[to]).size() > (*mp[mx1]).size())) mx1 = to;
    }

    if (~mx1) mp[v] = mp[mx1], un[v] = un[mx1];
    else mp[v] = new map<int,int>, un[v] = 0;
    for (auto &to : g[v]) {
        if (f[to] < in[v]) continue;
        if (to == p || to == mx1) continue;
        for (auto &[col, c] : *mp[to]) {
            (*mp[v])[col] += c;
            if ((*mp[v])[col] == cntw[col]) ++un[v];
        }
    }
    ++(*mp[v])[a[v]];
    if ((*mp[v])[a[v]] == cntw[a[v]]) ++un[v];
    t[v] = un[v];

    for (auto &to : g[v]) {
        if (f[to] < in[v]) continue;
        if (to == p || to == mx1) continue;
        for (auto &[col, c] : *mp[to]) {
            (*mp[v])[col] -= c;
            if ((*mp[v])[col] == 0) (*mp[v]).erase(col);
        }
    } --(*mp[v])[a[v]];
    if ((*mp[v])[a[v]] == 0) (*mp[v]).erase(a[v]);
    ve<pii> ar;

    if (~mx) mp[v] = mp[mx], un[v] = un[mx];
    else mp[v] = new map<int,int>, un[v] = 0;

    for (auto &to : g[v]) {
        if (to == p || to == mx) continue;
        for (auto &[col, c] : *mp[to]) {
            (*mp[v])[col] += c;
            if ((*mp[v])[col] == cntw[col]) ++un[v];
        }
    }
    ++(*mp[v])[a[v]];
    if ((*mp[v])[a[v]] == cntw[a[v]]) ++un[v];
    ans[v] = (*mp[v]).size();
}

inline void solve() {
    int n, m;
    cin >> n >> m;
    for (int i = 0; i < n; ++i) cin >> a[i];
    for (int i = 0; i < m; ++i) {
        int v, u;
        cin >> v >> u, --v, --u;
        gr[v].pb(u), gr[u].pb(v);
    } dfsb(0);
    for (int i = 0; i < n; ++i) ++cntw[a[i]];
    dfs(0, 0);

    ve<int> al(n);
    for (int i = 0; i < n; ++i) al[i] = a[i];
    sort(all(al)); al.erase(unique(all(al)), al.end());
    int tot = al.size();

    for (int i = 0; i < n; ++i) {
        ll res = 0;
        for (auto &u : g[i]) {
            if (u == pr[i] || f[u] < in[i]) continue;
            res += ans[u] * 1ll * ans[u];
        } t[i] = tot - t[i];
        res += t[i] * 1ll * t[i];
        cout << res << " ";
    }
}

signed main() {
    ios_base::sync_with_stdio(false); cin.tie(0); cout.tie(0);
    int q = 1; //cin >> q;
    while (q--) solve();
}
