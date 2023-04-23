#pragma region  // LazySeg
/**
 * Description: 1D range increment and sum query.
 * Source: USACO Counting Haybales
 * Verification: SPOJ Horrible
 *
 * lstTrue_from_base, fstTrue_from_base tested on https://codeforces.com/contest/1771/problem/B
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
        if (lazy[ind] == idS) return;
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
    #pragma region  // lstTrue_from_base, fstTrue_from_base
    // ~ lstTrue_from_base(L, check(v,r)) == lstTrue(L, orig_n-1, check(query(L,r), r)).
    int lstTrue_from_base(int L, auto&& check) {
        int ind = 1; int l = 0; int r = n-1;
        // dbgcBold("lsR", L, MT(ind,l,r), *this);
        push(ind,l,r);
        if (L == 0 && check(seg[ind], r)) {return orig_n-1;}

        while (l != L) {
            // dbgc("stage 1", ind,l,r, L);
            int m = (l+r)/2;
            if (L <= m) {ind = 2*ind; r = m;}
            else {ind = 2*ind+1; l = m+1;}
            push(ind,l,r);
        }

        T wip = idT;
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
                push(ind,l,r);
            } else { break; }
        }

        // (ind,l,r) would reach too far.
        while (l != r) {
            // does DL reach too far?
            int m = (l+r)/2;
            push(2*ind, l, m);
            T fut = cmb(wip, seg[2*ind]);
            // dbgcW("stage 3", ind,l,r, "", m, fut);
            if (check(fut,m)) {
                wip = fut;
                ind = 2*ind+1;
                l = m+1;
                push(ind,l,r);
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
        push(ind,l,r);  // ~ leave these commented for analogy to LazySeg.
        if (R == orig_n-1 && check(seg[ind], 0)) {return 0;}

        while (r != R) {
            // dbgc("stage 1", ind,l,r, R);
            int m = (l+r)/2;
            if (R <= m) {ind = 2*ind; r = m;}
            else {ind = 2*ind+1; l = m+1;}
            push(ind,l,r);
        }

        T wip = idT;
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
                push(ind,l,r);
            } else { break; }
        }

        // (ind,l,r) would reach too far.
        while (l != r) {
            // does DR reach too far?
            int m = (l+r)/2;
            push(2*ind+1, m+1, r);
            T fut = cmb(wip, seg[2*ind+1]);
            // dbgcW("stage 3", ind,l,r, "", m, fut);
            if (check(fut,m+1)) {
                wip = fut; ind *= 2; r = m;
                push(ind,l,r);
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
