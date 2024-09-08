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

/**
big modulos
constexpr ll md = (ll)1e16 + 61;
inline ll add(ll a, ll b) { a += b; if (a >= md){a-=md;} return a; }
inline ll sub(ll a, ll b) { a -= b; if (a < 0){a+=md;} return a; }
inline ll mul(ll a, ll b) { ll q=(ld)a*(ld)b/(ld)md;ll r=a*b-q*md;if(r<0){r+=md;}if(r>=md){r-=md;} return r; }
*/

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











// ------------------------------------------------
// Multiple base hahs
// 15 primes
constexpr long long md[] = {10000000000000061, 10000000000000069, 10000000000000079, 10000000000000099, 10000000000000453, 10000000000000481, 10000000000000597, 10000000000000613, 10000000000000639, 10000000000000669, 10000000000000753, 10000000000000793, 10000000000000819, 10000000000000861, 10000000000000897};

constexpr int p = 1e9+9;

//constexpr int sz = sizeof(md) / sizeof(long long);
constexpr int sz = 2;

template <int ID>
inline long long add(long long a, long long b) {
	return a + b >= md[ID] ? a + b - md[ID] : a + b;
}
template <int ID>
inline void sadd(long long &a, long long b) {
	a += b;
	if (a >= md[ID]) {
		a -= md[ID];
	}
}

template <int ID>
inline long long sub(long long a, long long b) {
	return a - b < 0 ? a - b + md[ID] : a - b;
}
template <int ID>
inline void ssub(long long &a, long long b) {
	a -= b;
	if (a < 0) {
		a += md[ID];
	}
}

template <int ID>
inline long long mul(long long a, long long b) {
	long long q = (long double)a * b / md[ID];
	long long r = (a * b - q * md[ID]) % md[ID];
	if (r < 0) r += md[ID];
	return r;
}
template <int ID>
inline void smul(long long &a, long long b) {
	a = mul <ID> (a, b);
}

template <int ID>
inline long long bp(long long a, long long n) {
	long long res = 1;
	while (n) {
		if (n & 1) {
			smul <ID> (res, a);
		}
		smul <ID> (a, a);
		n >>= 1;
	}
	return res;
}

template <int ID>
inline long long inv(long long a) {
	return bp <ID> (a, md[ID] - 2);
}

class number_hash {
public:
	long long rem[sz];
	number_hash(long long number = 0) {
		for (int i = 0; i < sz; ++i) {
			rem[i] = number;
			if (number < 0) {
				rem[i] += md[i];
			}
		}
	}

	inline number_hash & operator += (const number_hash &other) {
		this->template add_ <0> (other);
		return *this;
	}
	inline number_hash & operator -= (const number_hash &other) {
		this->template sub_ <0> (other);
		return *this;
	}
	inline number_hash & operator *= (const number_hash &other) {
		this->template mul_ <0> (other);
		return *this;
	}

	inline number_hash operator + (const number_hash & other) const {
		auto copy = *this;
		return copy += other;
	}
	inline number_hash operator - (const number_hash & other) const {
		auto copy = *this;
		return copy -= other;
	}
	inline number_hash operator * (const number_hash & other) const {
		auto copy = *this;
		return copy *= other;
	}

	inline bool operator == (const number_hash & other) const {
		for (int i = 0; i < sz; ++i) {
			if (rem[i] != other.rem[i]) {
				return false;
			}
		}
		return true;
	}
	inline bool operator < (const number_hash & other) const {
		for (int i = 0; i < sz; ++i) {
			if (rem[i] != other.rem[i]) {
				return rem[i] < other.rem[i];
			}
		}
		return false;
	}

	template <int I = 0>
	inline void to_inv() {
		rem[I] = inv <I> (rem[I]);
		if constexpr (I+1 < sz) {
			this->template to_inv <I + 1> ();
		}
	}

	inline number_hash new_inv() const {
		auto copy = *this;
		copy.template to_inv <0> ();
		return copy;
	}
private:
	template <int I = 0>
	inline void add_(const number_hash & other) {
		sadd <I> (rem[I], other.rem[I]);
		if constexpr (I+1 < sz) {
			this->template add_ <I+1> (other);
		}
	}
	template <int I = 0>
	inline void sub_(const number_hash & other) {
		ssub <I> (rem[I], other.rem[I]);
		if constexpr (I+1 < sz) {
			this->template sub_ <I+1> (other);
		}
	}
	template <int I = 0>
	inline void mul_(const number_hash & other) {
		smul <I> (rem[I], other.rem[I]);
		if constexpr (I+1 < sz) {
			this->template mul_ <I+1> (other);
		}
	}
};

const int N = 2.5e5 + 10;
const int ZERO = N + 10;

number_hash pw_[2 * N + 30];

inline void precalc() {
	pw_[ZERO] = number_hash(1);

	number_hash P(p);

	for (int i = 1; i < N; ++i) {
		pw_[ZERO + i] = pw_[ZERO + i - 1] * P;
	}

	number_hash iP = P.new_inv();
	for (int i = 1; i < N; ++i) {
		pw_[ZERO - i] = pw_[ZERO - i + 1] * iP;
	}
}

inline number_hash & pw(int ind) {
	return pw_[ZERO + ind];
}
