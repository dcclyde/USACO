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

//// V<pair<ul,int>> factor(ul n) {
//// 	map<ul,int> cnt; factor_rec(n,cnt);
//// 	return V<pair<ul,int>>(all(cnt));
//// }

//// // This version is good for big N. Costs N^.25 + some garbage.
//// V<ul> divisors(ul n) {
//// 	map<ul,int> cnt; factor_rec(n,cnt);
//// 	V<ul> out = {1};
//// 	for(auto& [p, e] : cnt) {
////         int sz = out.size(); ul ppow = p;
//// 		rep(e) {
//// 			FOR(k, 0, sz) { out.push_back(out[k] * ppow); }
////             ppow *= p;
//// 		}
//// 	}
//// 	sort(all(out)); return out;
//// }

//// // This version is good for small N.
//// V<int> divisors(int n) {
//// 	V<int> out;
//// 	for ( int d = 1 ; d*d <= n ; ++d ) {
//// 		if ( n % d == 0 ) {
//// 			out.push_back(d);
//// 			out.push_back(n/d);
//// 		}
//// 	}
//// 	remDup(out);
//// 	return out;
//// }
