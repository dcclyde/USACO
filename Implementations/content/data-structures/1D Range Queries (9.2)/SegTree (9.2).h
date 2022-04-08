/**
 * Description: 1D point update, range query where \texttt{cmb} is
 	* any associative operation. If $N=2^p$ then \texttt{seg[1]==query(0,N-1)}.
 * Time: O(\log N)
 * Source:
	* http://codeforces.com/blog/entry/18051
	* KACTL
 * Verification: SPOJ Fenwick
 */

tcT> struct SegTree { // cmb(ID,b) = b
	const T ID{};  // ! identity
    T cmb(T a, T b) { return a+b; }  // ! seg * seg
	int n; V<T> seg; int orig_n;
	void init(int _n) { // upd, query also work if n = _n
		for (n = 1; n < _n; ) n *= 2;
		seg.assign(2*n,ID); orig_n = _n;}
	void pull(int p) { seg[p] = cmb(seg[2*p],seg[2*p+1]); }
    void build() { ROF(i,1,n) pull(i); }
	void upd(int p, T val) { // set val at position p
		seg[p += n] = val; for (p /= 2; p; p /= 2) pull(p); }
	T query(int l, int r) {	// associative op on [l, r]
		T ra = ID, rb = ID;
		for (l += n, r += n+1; l < r; l /= 2, r /= 2) {
			if (l&1) ra = cmb(ra,seg[l++]);
			if (r&1) rb = cmb(seg[--r],rb);
		}
		return cmb(ra,rb);
	}
	// int first_at_least(int lo, int val, int ind=1, int l=0, int r=-1) { // if seg stores max across range
	// 	if ( r == -1 ) {
	// 		r = n-1;
	// 	}
	// 	if (r < lo || val > seg[ind]) return -1;
	// 	if (l == r) return l;
	// 	int m = (l+r)/2;
	// 	int res = first_at_least(lo,val,2*ind,l,m); if (res != -1) return res;
	// 	return first_at_least(lo,val,2*ind+1,m+1,r);
	// }
    void detailed_printouts() {
        #pragma region  // call like `dbg_only(st.detailed_printouts);`.
        int ST_SIZE = n;
        int ST_PRINT_SIZE = orig_n;
        // ST_PRINT_SIZE = ST_SIZE;  // toggle whether to print irrelevant suffix
        el;
        dbgc("ST DETAILS");
        FOR(k, 1, ST_SIZE + ST_PRINT_SIZE) {
            if ( k >= ST_SIZE) {
                int p = k - ST_SIZE;
                dbgP(k, p, seg[k]);
            } else {
                vector<int> binary_digits;
                int temp = k;
                while ( temp > 0 ) {
                    binary_digits.push_back( temp % 2 );
                    temp /= 2;
                }
                reverse(all(binary_digits));
                int L = 0; int R = ST_SIZE-1;
                for ( int didx = 1 ; didx < binary_digits.size() ; ++didx ) {
                    int M = (L+R) / 2;
                    if ( binary_digits[didx] == 1 ) {
                        L = M+1;
                    } else {
                        R = M;
                    }
                }
                if ( L < ST_PRINT_SIZE ) {
                    dbgY(k, MP(L,R), seg[k]);
                }
            }
        }
        el;
        #pragma endregion
    }
};
template<class T>
string to_string(SegTree<T> st) {
    vector<T> out;
    for ( int k = st.n ; k < st.n + st.orig_n ; ++k ) {
        out.push_back( st.seg[k] );
    }
    return to_string( out );
}
