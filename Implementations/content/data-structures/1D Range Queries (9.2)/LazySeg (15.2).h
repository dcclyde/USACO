/**
 * Description: 1D range increment and sum query.
 * Source: USACO Counting Haybales
 * Verification: SPOJ Horrible
 */

tcT, class S> struct LazySeg {
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
    void push_all(int ind=1, int L=0, int R=-1) {
        if ( R == -1 ) {R = SZ-1;} push(ind, L, R);
        if (L < R) {int M = (L+R)/2; push_all(2*ind, L, M); push_all(2*ind+1, M+1, R);}
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
    int first_satisfying(int start, int ind=1, int L=0, int R=-1) {
        if (R == -1) {R = SZ-1;}
        // ! Is there a good idx in [l, r]?
        bool ok = (query(L, R, ind, L, R) != 0);
        if (R < start || !ok) {return -1;}
        if (L == R) {return L;}
        int M = (L+R)/2;
        int out = first_satisfying(2*ind, L, M);
        if (out == -1) {
            out = first_satisfying(2*ind+1, M+1, R);
        }
        return out;
    }
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
string to_string(LazySeg<T, S> st) {
    st.push_all();
    vector<T> out;
    for ( int k = st.n ; k < st.n + st.orig_n ; ++k ) {
        out.push_back( st.seg[k] );
    }
    return to_string( out );
}


#pragma region  // Ops: (add constant, set to constant). Query: (min)
/*
    Ops: add a constant; set whole range to a constant
    Query: min, plus "dest" which is something like "most recent SET operation"
*/
const int INF = 2e9;
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
        lazy_seg(seg[ind], lazy[ind]);
		// seg[ind] += (R-L+1)*lazy[ind]; // ! lazy * seg
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
    void detailed_printouts() {
        #pragma region  // call like `dbg_only(st.detailed_printouts);`.
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
                for ( int didx = 1 ; didx < binary_digits.size() ; ++didx ) {
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
        #pragma endregion
    }
};
#pragma endregion
