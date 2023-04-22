const ll GENFAC_MAX = powll(10, 6) + 1;
#pragma region  // genFac, comb, fac, binom
/**
 * Description: pre-compute factorial mod inverses,
 	* assumes $MOD$ is prime and $SZ < MOD$.
 * Time: O(SZ)
 * Source: KACTL
 * Verification: https://dmoj.ca/problem/tle17c4p5
 */


vmi invs, fac, ifac;
bool genFac(int SZ) {
	invs.rsz(SZ), fac.rsz(SZ), ifac.rsz(SZ);
	invs[1] = fac[0] = ifac[0] = 1;
	FOR(i,2,SZ) invs[i] = mi(-(ll)MOD/i*(int)invs[MOD%i]);
	FOR(i,1,SZ) fac[i] = fac[i-1]*i, ifac[i] = ifac[i-1]*invs[i];
    return true;  // dummy value so we can call from global scope
}
mi comb(int a, int b) {
	if (a < b || b < 0) return 0;
	return fac[a]*ifac[b]*ifac[a-b]; }
auto& binom = comb;

bool precompute_genfac_dummy = genFac(GENFAC_MAX + 1);
#pragma endregion
