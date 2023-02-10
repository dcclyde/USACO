/**
 * Description: Pollard-rho randomized factorization algorithm. Returns prime
   * factors of a number, in arbitrary order (e.g. 2299 -> \{11, 19, 11\}).
 * Time: $O(N^{1/4})$, less for numbers with small factors
 * Source: KACTL
	* https://en.wikipedia.org/wiki/Pollard%27s_rho_algorithm
	* https://codeforces.com/contest/1033/submission/44009089 is faster?
 * Verification: https://www.spoj.com/problems/FACT0/
 */

#include "MillerRabin.h"
#include "../Modular Arithmetic/ModMulLL.h"

ul pollard(ul n) { // return some nontrivial factor of n
	auto f = [n](ul x) { return modMul(x, x, n) + 1; };
	ul x = 0, y = 0, t = 30, prd = 2, i = 1, q;
	while (t++ % 40 || gcd(prd, n) == 1) { /// speedup: don't take gcd every it
		if (x == y) x = ++i, y = f(x);
		if ((q = modMul(prd, max(x,y)-min(x,y), n))) prd = q;
		x = f(x), y = f(f(y));
	}
	return gcd(prd, n);
}
void factor_rec(ul n, map<ul,int>& cnt) {
	if (n == 1) return;
	if (prime(n)) { ++cnt[n]; return; }
	ul u = pollard(n);
	factor_rec(u,cnt), factor_rec(n/u,cnt);
}
V<pair<ul,int>> factor(ul n) {
	map<ul,int> cnt; factor_rec(n,cnt);
	return V<pair<ul,int>>(all(cnt));
}

const ll SIEVE_MAX = 1'000'000;
#pragma region  // factor fast
/**
 * Description: Tests primality up to $SZ$. Runs faster if only
 	* odd indices are stored.
 * Time: O(SZ\log\log SZ) or O(SZ)
 * Source: KACTL
 * Verification: https://open.kattis.com/problems/primesieve
 */

template<int SZm1> struct Sieve {
	static constexpr int SZ = SZm1 + 1;
	bitset<SZ> is_prime; vll primes;
	// Sieve() {
	// 	is_prime.set(); is_prime[0] = is_prime[1] = 0;
	// 	for (int i = 4; i < SZ; i += 2) is_prime[i] = 0;
	// 	for (int i = 3; i*i < SZ; i += 2) if (is_prime[i])
	// 		for (int j = i*i; j < SZ; j += i*2) is_prime[j] = 0;
	// 	F0R(i,SZ) if (is_prime[i]) primes.pb(i);
	// }
	ll sp[SZ]{}; // smallest prime that divides
	Sieve() { // above is faster
		FOR(i,2,SZ) {
			if (sp[i] == 0) sp[i] = i, primes.pb(i);
			for (ll p: primes) {
				if (p > sp[i] || i*p >= SZ) break;
				sp[i*p] = p;
			}
		}
        for(auto& p : primes) {is_prime[p] = 1;}
	}
};
Sieve<SIEVE_MAX> sieve;

using ul = uint64_t;
ul modMul(ul a, ul b, const ul mod) {
	ll ret = a*b-mod*(ul)((db)a*b/mod);
	return ret+((ret<0)-(ret>=(ll)mod))*mod; }
ul modPow(ul a, ul b, const ul mod) {
	if (b == 0) return 1;
	ul res = modPow(a,b/2,mod); res = modMul(res,res,mod);
	return b&1 ? modMul(res,a,mod) : res;
}
ul modSqrt(ul a, ul p) {
	if (p == 2) {return a;}
	ul q = modPow(a,(p-1)/2, p);
	if (q != 1) return q == 0 ? 0 : -1;
	/// check if 0 or no sqrt
	ul s = p-1; int r = 0; while (s%2 == 0) s /= 2, ++r;
	ul n = 1; while (modPow(n,(p-1)/2,p) == 1) ++n;
	// n non-square, ord(g)=2^r, ord(b)=2^m, ord(g)=2^r, m<r

	for (ul x = modPow(a,(s+1)/2,p), b = modPow(a,s,p), g = modPow(n,s,p);;) {
		if (b == 1) return min(x,p-x); // x^2=ab
		int m = 0; for (ul t = b; t != 1; t = modMul(t, t, p)) ++m;
		rep(r-m-1) g = modMul(g, g, p); // ord(g)=2^{m+1}
        x = modMul(x,g,p); g = modMul(g,g,p); b = modMul(b,g,p); r=m;
	}
}

bool prime(ul n) { // not ll!
	if (n < 2 || n % 6 % 4 != 1) return n-2 < 2;
	ul A[] = {2, 325, 9375, 28178, 450775, 9780504, 1795265022},
	    s = __builtin_ctzll(n-1), d = n>>s;
	each(a,A) {   // ^ count trailing zeroes
		ul p = modPow(a,d,n), i = s;
		while (p != 1 && p != n-1 && a%n && i--) p = modMul(p,p,n);
		if (p != n-1 && i != s) return 0;
	}
	return 1;
}
ul pollard(ul n) { // return some nontrivial factor of n
	auto f = [n](ul x) { return modMul(x, x, n) + 1; };
	ul x = 0, y = 0, t = 30, prd = 2, i = 1, q;
	while (t++ % 40 || gcd(prd, n) == 1) { /// speedup: don't take gcd every it
		if (x == y) x = ++i, y = f(x);
		if ((q = modMul(prd, max(x,y)-min(x,y), n))) prd = q;
		x = f(x), y = f(f(y));
	}
	return gcd(prd, n);
}
template<class A, class B>
void factor_rec(ul n, map<A,B>& cnt) {
// void factor_rec(ul n, map<ul,int>& cnt) {
    if (n < SIEVE_MAX) {
        while (n > 1) {
            ll p = sieve.sp[n];
            n /= p; ++cnt[p];
        }
        return;
    }
	if (prime(n)) { ++cnt[n]; return; }
	ul u = pollard(n);
	factor_rec(u,cnt), factor_rec(n/u,cnt);
}
V<pair<ul,int>> factor(ul n) {
	map<ul,int> cnt; factor_rec(n,cnt);
	return V<pair<ul,int>>(all(cnt));
}
#pragma endregion

#pragma region  // divisors, totient (big N, costs N^.25 + garbage)
ll num_divisors(ul n) {
	map<ul,int> cnt; factor_rec(n,cnt);
	ll out = 1;
	for(auto& [p, e] : cnt) {out *= e+1;}
	return out;
}
V<ul> divisors(ul n) {
	map<ul,int> cnt; factor_rec(n,cnt);
	V<ul> out = {1};
	for(auto& [p, e] : cnt) {
        int sz = out.size(); ul ppow = p;
		rep(e) {
			FOR(k, 0, sz) { out.push_back(out[k] * ppow); }
            ppow *= p;
		}
	}
	sort(all(out)); return out;
}
ll totient(ul n) {
    map<ul,int> cnt; factor_rec(n,cnt);
    ll out = n;
    for(auto& [p, e] : cnt) {out /= p; out *= p-1;}
    return out;
}
#pragma endregion

#pragma region  // divisors (small N, sqrt(N))
// This version is good for small N.
V<int> divisorsSmallN(int n) {
	V<int> out;
	for ( int d = 1 ; d*d <= n ; ++d ) {
		if ( n % d == 0 ) {
			out.push_back(d);
			out.push_back(n/d);
		}
	}
	remDup(out);
	return out;
}
#pragma endregion
