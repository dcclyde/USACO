/**
 * Description: 1D range increment and sum query.
 * Source: USACO Counting Haybales
 * Verification: SPOJ Horrible
 */

// tcT, int SZ> struct LazySeg {
// 	static_assert(pct(SZ) == 1); // SZ must be power of 2
// 	const T ID = 0; T cmb(T a, T b) { return a+b; }
// 	T seg[2*SZ], lazy[2*SZ];
// 	LazySeg() { F0R(i,2*SZ) seg[i] = lazy[i] = ID; }
// 	void push(int ind, int L, int R) { /// modify values for current node
// 		seg[ind] += (R-L+1)*lazy[ind]; // dependent on operation
// 		if (L != R) F0R(i,2) lazy[2*ind+i] += lazy[ind]; /// prop to children
// 		lazy[ind] = 0;
// 	} // recalc values for current node
// 	void pull(int ind){seg[ind]=cmb(seg[2*ind],seg[2*ind+1]);}
// 	void build() { ROF(i,1,SZ) pull(i); }
// 	void upd(int lo,int hi,T inc,int ind=1,int L=0, int R=SZ-1) {
// 		push(ind,L,R); if (hi < L || R < lo) return;
// 		if (lo <= L && R <= hi) {
// 			lazy[ind] = inc; push(ind,L,R); return; }
// 		int M = (L+R)/2; upd(lo,hi,inc,2*ind,L,M);
// 		upd(lo,hi,inc,2*ind+1,M+1,R); pull(ind);
// 	}
// 	T query(int lo, int hi, int ind=1, int L=0, int R=SZ-1) {
// 		push(ind,L,R); if (lo > R || L > hi) return ID;
// 		if (lo <= L && R <= hi) return seg[ind];
// 		int M = (L+R)/2; return cmb(query(lo,hi,2*ind,L,M),
// 			query(lo,hi,2*ind+1,M+1,R));
// 	}
// };

tcT, class S> struct LazySeg {
	const T idT{}; const S idS{};
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

template<class T, class S>
string to_string(LazySeg<T, S> st) {
    st.push_all();
    vector<T> out;
    for ( int k = st.n ; k < st.n + st.orig_n ; ++k ) {
        out.push_back( st.seg[k] );
    }
    return to_string( out );
}
