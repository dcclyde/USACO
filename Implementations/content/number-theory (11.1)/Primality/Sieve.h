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
	Sieve() {
		is_prime.set(); is_prime[0] = is_prime[1] = 0;
		for (int i = 4; i < SZ; i += 2) is_prime[i] = 0;
		for (int i = 3; i*i < SZ; i += 2) if (is_prime[i])
			for (int j = i*i; j < SZ; j += i*2) is_prime[j] = 0;
		F0R(i,SZ) if (is_prime[i]) primes.pb(i);
	}
	// ll sp[SZ]{}; // smallest prime that divides
	// Sieve() { // above is faster
	// 	FOR(i,2,SZ) {
	// 		if (sp[i] == 0) sp[i] = i, primes.pb(i);
	// 		for (ll p: primes) {
	// 			if (p > sp[i] || i*p >= SZ) break;
	// 			sp[i*p] = p;
	// 		}
	// 	}
    //     for(auto& p : primes) {is_prime[p] = 1;}
	// }
};
