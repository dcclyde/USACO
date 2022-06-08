/**
 * Description: Multiply two 64-bit integers mod another if 128-bit is not available.
	* modMul is equivalent to \texttt{(ul)(\_\_int128(a)*b\%mod)}.
	* Works for $0\le a,b<mod<2^{63}.$
 * Source: KACTL
 * Verification: see "Faster Factoring"
 */

/// using db = long double;
using ul = uint64_t;
ul modMul(ul a, ul b, const ul mod) {
	ll ret = a*b-mod*(ul)((db)a*b/mod);
	return ret+((ret<0)-(ret>=(ll)mod))*mod; }
ul modPow(ul a, ul b, const ul mod) {
	if (b == 0) return 1;
	ul res = modPow(a,b/2,mod); res = modMul(res,res,mod);
	return b&1 ? modMul(res,a,mod) : res;
}
ul modInv(ul a, const ul p) {
	assert(a != 0);
	return modPow(a, p-2, p);
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


// __int128_t modInv(__int128_t a, __int128_t b){
//     return 1<a ? b - modInv(b%a,a)*b/a : 1;
// }
