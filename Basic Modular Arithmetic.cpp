int md; // basic modular arithmetic (make const for improving speed)
inline int add(int a, int b) { a += b; if(a>=md){a-=md;} return a; }
inline int sub(int a, int b) { a -= b; if(a<0){a+=md;} return a; }
inline int mul(int a, int b) { return (ll)a * b % md; }
inline int bp(int a, int n) { int res = 1;
	for (; n; n >>= 1, a = mul(a, a)) {
		if (n & 1) res = mul(res, a);
	} return res;
}
inline int inv(int a) { return bp(a, md - 2); }

inline ve<int> inverses(int n) {
	/*
	if p is prime
	p mod i = p - (p / i) * i
	p mod i = -(p / i) * i (mod p)
	(multiply by (i^-1) and (p mod i)^-1)
	i^-1 = -(p / i) * (p mod i)^-1
	*/
	ve<int> iv(n + 1); iv[0] = iv[1] = 1;
	for (int i = 2; i <= n; ++i) iv[i] = mul(md - md / i, iv[md % i]);
	for (int i = 1; i <= n; ++i) assert(mul(i, iv[i]) == 1);
	return iv;
}

inline int primitive_root() {
	/*
	primitive root g for (mod p) is such a number
	that all numbers x coprime with p can be written as
	x = g^c (mod p) for some natural number c

	g is primitive root of p
	<=>
	smallest x for which g^x = 1 (mod p) is phi(p)
	*/
	int n = md - 1;
	ve<int> f;
	for (int i = 2; i * i <= n; ++i) {
		if (n % i) continue;
		f.pb(i); while (n % i == 0) n /= i;
	} if (n > 1) f.pb(n);

	for (int g = 2; g < md; ++g) {
		bool ok = 1;
		for (int &p : f) {
			ok &= (bp(g, (md - 1) / p) != 1);
			if (!ok) break;
		} if (ok) return g;
	}
	return -1;
}
inline int discrete_log(int a, int b) {
	/*
	a^x = b (mod p)
	rewrite x as (np - q)
	where p is a giant step and q is a baby step and n is some constant
	
	a^(np - q) = b (mod p)
	assuming gcd(a, p) = 1 ->
	a^np = ba^q (mod p)

	now use meet in the middle
	calculate a^np for all p in range [1, ceil(m / n)]
	and ba^q in range [0, n]

	choosing n = sqrt(m) works perfectly
	*/

	int n = sqrt(md) + 1;
	gp_hash_table<int,int> have; int res = b; // ba^q
	for (int q = 0; q <= n; ++q) { have[res] = q; res = mul(res, a); }

	int an = 1; // a to the power of n
	for (int i = 0; i < n; ++i) an = mul(an, a);

	res = 1;
	for (int p = 1; p <= n; ++p) {
		res = mul(res, an);
		if (have.find(res) != have.end()) {
			return sub(mul(n, p), have[res]);
		}
	}
	return -1;
}

inline void solve() {
	mt19937_64 rnd(264463);

	cin >> md;

	cout << "primitive root is ";
	cout << primitive_root() << '\n';

	for (int i = 1; i <= 10; ++i) {
		int a = 1 + rnd() % (md - 1);
		int b = 1 + rnd() % (md - 1);
		cout << "log(" << b << ") in base " << a << " ";

		int res = discrete_log(a, b);
		if (res != -1) {
			cout << res << '\n';
			assert(bp(a, res) == b);
		}
		else cout << "does not exist" << '\n';
	}

	int n;
	cin >> n;
	ve<int> inv = inverses(n);

	for (int i = 1; i <= n; ++i) cout << i << " * " << inv[i] << " = 1 (mod " << md << ")" << '\n';
}
