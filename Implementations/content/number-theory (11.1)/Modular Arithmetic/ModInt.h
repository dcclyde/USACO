
// const int MOD = 1'000'000'007;
// const int MOD = 998'244'353;
// const int MOD = 1234567891;  // WATCH OUT, this is bigger than 2^31 / 2!
#pragma region  // mint
/**
 * Description: modular arithmetic operations
 * Source:
	* KACTL
	* https://codeforces.com/blog/entry/63903
	* https://codeforces.com/contest/1261/submission/65632855 (tourist)
	* https://codeforces.com/contest/1264/submission/66344993 (ksun)
	* also see https://github.com/ecnerwala/cp-book/blob/master/src/modnum.hpp (ecnerwal)
 * Verification:
	* https://open.kattis.com/problems/modulararithmetic
 */

#ifndef BENQ_MODINT
#define BENQ_MODINT

template<int MOD, int MODPHI, int RT> struct mint {
	static const int mod = MOD;
	static const int modphi = MODPHI;
	static constexpr mint rt() { return RT; } // primitive root for FFT
	int v; explicit operator int() const { return v; } // explicit -> don't silently convert to int
	mint():v(0) {}
	mint(ll _v) { v = int((-MOD < _v && _v < MOD) ? _v : _v % MOD);
		if (v < 0) v += MOD; }
	bool operator==(const mint& o) const {
		return v == o.v; }
	friend bool operator!=(const mint& a, const mint& b) {
		return !(a == b); }
	friend bool operator<(const mint& a, const mint& b) {
		return a.v < b.v; }
	friend void re(mint& a) { ll x; re(x); a = mint(x); }
	friend str ts(mint a) { return ts(a.v); }
	friend str tsdbg(mint a) { return tsdbg(a.v); }

	mint& operator+=(const mint& o) {
		if ((v += o.v) >= MOD) v -= MOD;
		return *this; }
	mint& operator-=(const mint& o) {
		if ((v -= o.v) < 0) v += MOD;
		return *this; }
	mint& operator*=(const mint& o) {
		v = int((ll)v*o.v%MOD);
		return *this; }
	mint& operator/=(const mint& o) { return (*this) *= inv(o); }
	friend mint pow(mint a, ll p) {
		mint ans = 1;
        if (p < 0) {return pow(inv(a), -p);}
		for (; p; p /= 2, a *= a) if (p&1) ans *= a;
		return ans; }
	friend mint inv(const mint& a) { assert(a.v != 0);
		return pow(a,modphi-1);
		// if MOD isn't prime use second version instead.
		// Alternatively, find totient of MOD and use pow(a, totient-1).
		// return (1<a) ? (MOD - ll(inv(MOD%a,a))*MOD/a) : (1);
	}
	mint operator-() const { return mint(-v); }
	mint& operator++() { return *this += 1; }
	mint& operator--() { return *this -= 1; }
	friend mint operator+(mint a, const mint& b) { return a += b; }
	friend mint operator-(mint a, const mint& b) { return a -= b; }
	friend mint operator*(mint a, const mint& b) { return a *= b; }
	friend mint operator/(mint a, const mint& b) { return a /= b; }
};

using mi = mint<MOD,MOD-1,5>; // 5 is primitive root for both common mods
using vmi = V<mi>;
using pmi = pair<mi,mi>;
using vpmi = V<pmi>;

V<vmi> scmb; // small combinations
void genComb(int SZ) {
	scmb.assign(SZ,vmi(SZ)); scmb[0][0] = 1;
	FOR(i,1,SZ) F0R(j,i+1)
		scmb[i][j] = scmb[i-1][j]+(j?scmb[i-1][j-1]:0);
}

template <int MOD, int MODPHI, int RT>
string to_string(mint<MOD, MODPHI, RT> modint) {
    return to_string((int)modint);
}

#endif
#pragma endregion  // mint
