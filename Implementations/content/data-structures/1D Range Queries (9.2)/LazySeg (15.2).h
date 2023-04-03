#pragma region  // LazySeg
/**
 * Description: 1D range increment and sum query.
 * Source: USACO Counting Haybales
 * Verification: SPOJ Horrible
 */

tcT, class S> struct LazySeg {
    static const ll SENTINEL_R = -17;
	const T idT{}; const S idS{};  // ! identity
	int n; V<T> seg; V<S> lazy; int orig_n; int SZ;
	T cmb(T a, T b) {  // ! seg * seg
		return a+b;
	}
	void init(int _n) {
		orig_n = _n; for (n = 1; n < _n; ) n *= 2;
		SZ = n; seg.assign(2*n,idT); lazy.assign(2*n, idS);  // ! initialize
	}
	void push(int ind, int L, int R) { /// modify values for current node
		seg[ind] += (R-L+1)*lazy[ind]; // ! lazy * seg
		if (L != R) F0R(i,2) lazy[2*ind+i] += lazy[ind]; // ! lazy * lazy
		lazy[ind] = idS;
	} // recalc values for current node
	void pull(int ind){seg[ind]=cmb(seg[2*ind],seg[2*ind+1]);}
	void build() { ROF(i,1,SZ) pull(i); }
    void push_all(int ind=1, int L=0, int R=SENTINEL_R) {
        if ( R == SENTINEL_R ) {R = SZ-1;} push(ind, L, R);
        if (L < R) {int M = (L+R)/2; push_all(2*ind, L, M); push_all(2*ind+1, M+1, R);}
    }
	void upd(int lo,int hi,S inc,int ind=1,int L=0, int R=SENTINEL_R) {
        if ( R == SENTINEL_R ) {R = SZ-1;}
		push(ind,L,R); if (hi < L || R < lo) return;
		if (lo <= L && R <= hi) {
			lazy[ind] = inc; push(ind,L,R); return; }
		int M = (L+R)/2; upd(lo,hi,inc,2*ind,L,M);
		upd(lo,hi,inc,2*ind+1,M+1,R); pull(ind);
	}
	T query(int lo, int hi, int ind=1, int L=0, int R=SENTINEL_R) {
        if ( R == SENTINEL_R ) {R = SZ-1;}
		push(ind,L,R); if (lo > R || L > hi) return idT;
		if (lo <= L && R <= hi) return seg[ind];
		int M = (L+R)/2; return cmb(query(lo,hi,2*ind,L,M),
			query(lo,hi,2*ind+1,M+1,R));
	}
    #pragma region  // first_satisfying
    // // return smallest x s.t. query(base, x) satisfies some criterion
	// int first_satisfying_R(int base, int val, int ind=1, int l=0, int r=-1) {
	// 	if (r == -1) {r = n-1;}
    //     // ! is there a good idx in [l, r]?
    //     bool ok = (query(l,r,ind,l,r) >= val);
	// 	if (r < base || !ok) return -1;
	// 	if (l == r) return l;
	// 	int m = (l+r)/2;
	// 	int res = first_satisfying_R(base,val,2*ind,l,m); if (res != -1) return res;
    //     // ! Look for something different in other child if needed (e.g. if we want sum >= X)
	// 	return first_satisfying_R(base,val,2*ind+1,m+1,r);
	// }
    // // return largest x s.t. query(x, base) satisfies some criterion
	// int first_satisfying_L(int base, int val, int ind=1, int l=0, int r=-1) {
	// 	if (r == -1) {r = n-1;}
    //     // ! is there a good idx in [l, r]?
    //     bool ok = (query(l,r,ind,l,r) >= val);
	// 	if (l > base || !ok) return -1;
	// 	if (l == r) return l;
	// 	int m = (l+r)/2;
	// 	int res = first_satisfying_L(base,val,2*ind+1,m+1,r); if (res != -1) return res;
    //     // ! Look for something different in other child if needed (e.g. if we want sum >= X)
	// 	return first_satisfying_L(base,val,2*ind,l,m);
	// }
    #pragma endregion  // first_satisfying
    void detailed_printouts() {
        #pragma region
        dbg_only(
        int ST_SIZE = n;
        int ST_PRINT_SIZE = orig_n;
        // ST_PRINT_SIZE = ST_SIZE;  // toggle whether to print irrelevant suffix
        el;
        dbgc("LazySeg DETAILS");
        FOR(k, 1, ST_SIZE + ST_PRINT_SIZE) {
            if ( k >= ST_SIZE) {
                int p = k - ST_SIZE;
                dbgP(k, p, seg[k], lazy[k]);
            } else {
                vector<int> binary_digits;
                int temp = k;
                while ( temp > 0 ) {
                    binary_digits.push_back( temp % 2 );
                    temp /= 2;
                }
                reverse(all(binary_digits));
                int L = 0; int R = ST_SIZE-1;
                FOR(didx, 1, binary_digits.size()) {
                    int M = (L+R) / 2;
                    if ( binary_digits[didx] == 1 ) {
                        L = M+1;
                    } else {
                        R = M;
                    }
                }
                if ( L < ST_PRINT_SIZE ) {
                    dbgY(k, MP(L,R), seg[k], lazy[k]);
                }
            }
        }
        el;
        );  // end dbg_only
        #pragma endregion
    }
};

template<class T, class S>
string tsdbg(LazySeg<T, S> st) {
    st.push_all(); vector<T> out;
    FOR(k, st.n, st.n + st.orig_n) { out.push_back(st.seg[k]); }
    return tsdbg(out);
}
#pragma endregion  // LazySeg

#pragma region  // Customized with ops: (add constant, set to constant). Query: (min)
/*
    Ops: add a constant; set whole range to a constant
    Query: min, plus "dest" which is something like "most recent SET operation"
*/
// const ll INF = INF_ll;
tcT, class S> struct LazySeg {
	const T idT{INF, -1}; const S idS{0, -INF};  // ! identity
	int n; V<T> seg; V<S> lazy; int orig_n; int SZ;
	T cmb(T a, T b) {  // ! seg * seg
        return min(a, b);
	}
	void init(int _n) {
		orig_n = _n; for (n = 1; n < _n; ) n *= 2;
		SZ = n; seg.assign(2*n,idT); lazy.assign(2*n, idS);  // ! initialize
	}
    void lazy_seg(T& s, const S& l) {
        auto& [add_l, set_l] = l;
        auto& [min_s, dest_s] = s;
        if (set_l == -INF) {
            min_s += add_l;
        } else {
            min_s = set_l;
            dest_s = add_l;  // first slot stores dest if `set` is active.
        }
    }
    void lazy_lazy(S& a, const S& b) {
        auto& [add_a, set_a] = a;
        auto& [add_b, set_b] = b;
        if (set_b != -INF) {
            // set o add
            add_a = add_b;  // set destination
            set_a = set_b;
        } else if (set_a == -INF) {
            // add o add
            add_a += add_b;
        } else {
            // add o set
            set_a += add_b;
        }
    }
	void push(int ind, int L, int R) { /// modify values for current node
		// seg[ind] += (R-L+1)*lazy[ind]; // ! lazy * seg
        lazy_seg(seg[ind], lazy[ind]);
		if (L != R) F0R(i,2) lazy_lazy(lazy[2*ind+i], lazy[ind]); // ! lazy * lazy
		lazy[ind] = idS;
	} // recalc values for current node
	void pull(int ind){seg[ind]=cmb(seg[2*ind],seg[2*ind+1]);}
	void build() { ROF(i,1,SZ) pull(i); }
    void push_all(int ind=1, int L=0, int R=-1) {
        if ( R == -1 ) {R = SZ-1;} push(ind, L, R);
        if (L < R) {int M = (L+R)/2; push_all(2*ind, L, M); push_all(2*ind+1, M+1, R);}
    }
	void upd_set(int lo,int hi,int inc, int dest) {
        upd(lo, hi, MP(dest, inc));
    }
	void upd_add(int lo,int hi,int inc) {
        upd(lo, hi, MP(inc, -INF));
    }
	void upd(int lo,int hi,S inc,int ind=1,int L=0, int R=-1) {
        if ( R == -1 ) {R = SZ-1;}
		push(ind,L,R); if (hi < L || R < lo) return;
		if (lo <= L && R <= hi) {
			lazy[ind] = inc; push(ind,L,R); return; }
		int M = (L+R)/2; upd(lo,hi,inc,2*ind,L,M);
		upd(lo,hi,inc,2*ind+1,M+1,R); pull(ind);
	}
	T query(int lo, int hi, int ind=1, int L=0, int R=-1) {
        if ( R == -1 ) {R = SZ-1;}
		push(ind,L,R); if (lo > R || L > hi) return idT;
		if (lo <= L && R <= hi) return seg[ind];
		int M = (L+R)/2; return cmb(query(lo,hi,2*ind,L,M),
			query(lo,hi,2*ind+1,M+1,R));
	}
    #pragma region  // first_satisfying
    // // return smallest x s.t. query(base, x) satisfies some criterion
	// int first_satisfying_R(int base, int val, int ind=1, int l=0, int r=-1) {
	// 	if (r == -1) {r = n-1;}
    //     // ! is there a good idx in [l, r]?
    //     bool ok = (query(l,r,ind,l,r) >= val);
	// 	if (r < base || !ok) return -1;
	// 	if (l == r) return l;
	// 	int m = (l+r)/2;
	// 	int res = first_satisfying_R(base,val,2*ind,l,m); if (res != -1) return res;
    //     // ! Look for something different in other child if needed (e.g. if we want sum >= X)
	// 	return first_satisfying_R(base,val,2*ind+1,m+1,r);
	// }
    // // return largest x s.t. query(x, base) satisfies some criterion
	// int first_satisfying_L(int base, int val, int ind=1, int l=0, int r=-1) {
	// 	if (r == -1) {r = n-1;}
    //     // ! is there a good idx in [l, r]?
    //     bool ok = (query(l,r,ind,l,r) >= val);
	// 	if (l > base || !ok) return -1;
	// 	if (l == r) return l;
	// 	int m = (l+r)/2;
	// 	int res = first_satisfying_L(base,val,2*ind+1,m+1,r); if (res != -1) return res;
    //     // ! Look for something different in other child if needed (e.g. if we want sum >= X)
	// 	return first_satisfying_L(base,val,2*ind,l,m);
	// }
    #pragma endregion  // first_satisfying
    void detailed_printouts() {
        #pragma region
        dbg_only(
        int ST_SIZE = n;
        int ST_PRINT_SIZE = orig_n;
        // ST_PRINT_SIZE = ST_SIZE;  // toggle whether to print irrelevant suffix
        el;
        dbgc("LazySeg DETAILS");
        FOR(k, 1, ST_SIZE + ST_PRINT_SIZE) {
            if ( k >= ST_SIZE) {
                int p = k - ST_SIZE;
                dbgP(k, p, seg[k], lazy[k]);
            } else {
                vector<int> binary_digits;
                int temp = k;
                while ( temp > 0 ) {
                    binary_digits.push_back( temp % 2 );
                    temp /= 2;
                }
                reverse(all(binary_digits));
                int L = 0; int R = ST_SIZE-1;
                FOR(didx, 1, binary_digits.size()) {
                    int M = (L+R) / 2;
                    if ( binary_digits[didx] == 1 ) {
                        L = M+1;
                    } else {
                        R = M;
                    }
                }
                if ( L < ST_PRINT_SIZE ) {
                    dbgY(k, MP(L,R), seg[k], lazy[k]);
                }
            }
        }
        el;
        );  // end dbg_only
        #pragma endregion
    }
};

template<class T, class S>
string tsdbg(LazySeg<T, S> st) {
    st.push_all(); vector<T> out;
    FOR(k, st.n, st.n + st.orig_n) { out.push_back(st.seg[k]); }
    return tsdbg(out);
}
#pragma endregion
