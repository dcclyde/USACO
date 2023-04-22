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
    void inc(int p, T val) { upd(p, cmb(val, seg[n+p])); }
	T query(int l, int r) {	// associative op on [l, r]
        if (l > r) {return ID;}
		T ra = ID, rb = ID;
		for (l += n, r += n+1; l < r; l /= 2, r /= 2) {
			if (l&1) ra = cmb(ra,seg[l++]);
			if (r&1) rb = cmb(seg[--r],rb);
		}
		return cmb(ra,rb);
	}
    #pragma region  // lstTrue_from_base, fstTrue_from_base
    // ~ lstTrue_from_base(L, check(v,r)) == lstTrue(L, orig_n-1, check(query(L,r), r)).
    int lstTrue_from_base(int L, auto&& check) {
        int ind = 1; int l = 0; int r = n-1;
        // dbgcBold("lsR", L, MT(ind,l,r), *this);
        // push(ind,l,r);  // ~ leave these commented for analogy to LazySeg.
        if (L == 0 && check(seg[ind], r)) {return orig_n-1;}

        while (l != L) {
            // dbgc("stage 1", ind,l,r, L);
            int m = (l+r)/2;
            if (L <= m) {ind = 2*ind; r = m;}
            else {ind = 2*ind+1; l = m+1;}
            // push(ind,l,r);
        }

        T wip = ID;
        while (true) {
            T fut = cmb(wip, seg[ind]);
            // dbgcP("stage 2", ind,l,r, "", fut);
            if (check(fut,r)) {  // If we can safely add this chunk...
                wip = fut;
                if (r == n-1) {
                    // We've reached the end.
                    return orig_n-1;
                } else if (ind & 1) {
                    // This was a right child.
                    ind = ind/2 + 1;
                    int len = r-l+1;
                    l += len; r += 2*len;
                } else {
                    // This was a left child.
                    ++ind;
                    int len = r-l+1;
                    l += len; r += len;
                }
                // push(ind,l,r);
            } else { break; }
        }

        // (ind,l,r) would reach too far.
        while (l != r) {
            // does DL reach too far?
            int m = (l+r)/2;
            // push(2*ind, l, m);
            T fut = cmb(wip, seg[2*ind]);
            // dbgcW("stage 3", ind,l,r, "", m, fut);
            if (check(fut,m)) {
                wip = fut;
                ind = 2*ind+1;
                l = m+1;
                // push(ind,l,r);
            } else {
                ind = 2*ind; r = m;
            }
        }
        int out = l-1;
        ckmin(out, orig_n-1);
        // dbgR(out); el;
        return out;
    }

    // fstTrue_from_base(R, check(v,l)) == fstTrue(0, R, check(query(l,R), l)).
    int fstTrue_from_base(int R, auto&& check) {
        int ind = 1; int l = 0; int r = n-1;
        // dbgcBold("fsb", R, MT(ind,l,r), *this);
        // push(ind,l,r);  // ~ leave these commented for analogy to LazySeg.
        if (R == orig_n-1 && check(seg[ind], 0)) {return 0;}

        while (r != R) {
            // dbgc("stage 1", ind,l,r, R);
            int m = (l+r)/2;
            if (R <= m) {ind = 2*ind; r = m;}
            else {ind = 2*ind+1; l = m+1;}
            // push(ind,l,r);
        }

        T wip = ID;
        while (true) {
            T fut = cmb(wip, seg[ind]);
            // dbgcP("stage 2", ind,l,r, "", fut);
            if (check(fut,l)) {  // If we can safely add this chunk...
                wip = fut;
                if (l == 0) {
                    // We've reached the end.
                    return 0;
                } else if (ind & 1) {
                    // This was a right child.
                    --ind;
                    int len = r-l+1;
                    l -= len; r -= len;
                } else {
                    // This was a left child.
                    ind = ind/2 - 1;
                    int len = r-l+1;
                    l -= 2*len; r -= len;
                }
                // push(ind,l,r);
            } else { break; }
        }

        // (ind,l,r) would reach too far.
        while (l != r) {
            // does DR reach too far?
            int m = (l+r)/2;
            // push(2*ind+1, m+1, r);
            T fut = cmb(wip, seg[2*ind+1]);
            // dbgcW("stage 3", ind,l,r, "", m, fut);
            if (check(fut,m+1)) {
                wip = fut; ind *= 2; r = m;
                // push(ind,l,r);
            } else {
                ind = 2*ind+1; l = m+1;
            }
        }
        int out = r+1;
        return out;
    }
    #pragma endregion  // lstTrue_from_base, fstTrue_from_base
    void detailed_printouts() {
        #pragma region
        dbg_only(
        int ST_SIZE = n;
        int ST_PRINT_SIZE = orig_n;
        // ST_PRINT_SIZE = ST_SIZE;  // toggle whether to print irrelevant suffix
        el;
        dbgc("SegTree DETAILS");
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
        );  // end dbg_only
        #pragma endregion
    }
};
template<class T>
string tsdbg(SegTree<T> st) {
    vector<T> out;
    FOR(k, st.n, st.n + st.orig_n) { out.push_back( st.seg[k] ); }
    return tsdbg(out);
}


#pragma region  // range updates, point queries
tcT> struct SegTree { // cmb(ID,b) = b
	const T ID{};  // ! identity
    T cmb(T a, T b) { return a+b; }  // ! seg * seg
	int n; V<T> seg; int orig_n;
	void init(int _n) { // upd, query also work if n = _n
		for (n = 1; n < _n; ) n *= 2;
		seg.assign(2*n,ID); orig_n = _n;}
	void pull(int p) { seg[p] = cmb(seg[2*p],seg[2*p+1]); }
    void build() { ROF(i,1,n) pull(i); }
	T query(int p) { // set val at position p
        T out = seg[p += n]; for (p /= 2; p; p /= 2) out = cmb(out, seg[p]); }
	void upd(int l, int r, T val) {	// associative op on [l, r]
        if (l > r) {return;}
		for (l += n, r += n+1; l < r; l /= 2, r /= 2) {
			if (l&1) {seg[l] = cmb(seg[l], val); ++l;}
			if (r&1) {--r; seg[r] = cmb(seg[r], val);}
		}
	}
    void detailed_printouts() {
        #pragma region
        dbg_only(
        int ST_SIZE = n;
        int ST_PRINT_SIZE = orig_n;
        // ST_PRINT_SIZE = ST_SIZE;  // toggle whether to print irrelevant suffix
        el;
        dbgc("SegTree DETAILS");
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
        );  // end dbg_only
        #pragma endregion
    }
};
template<class T>
string tsdbg(SegTree<T> st) {
    vector<T> out;
    FOR(k, st.n, st.n + st.orig_n) { out.push_back( st.seg[k] ); }
    return tsdbg( out );
}

