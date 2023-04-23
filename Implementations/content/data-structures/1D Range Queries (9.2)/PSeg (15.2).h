/**
 * Description: Persistent min segtree with lazy updates, no propagation.
 	* If making \texttt{d} a vector then save the results of
 	* \texttt{upd} and \texttt{build} in local variables first to
 	* avoid issues when vector resizes in C++14 or lower.
 * Memory: O(N+Q\log N)
 * Source: CF, Franklyn Wang
 * Verification:
 	* https://codeforces.com/contest/1090/problem/G
 	* https://codeforces.com/gym/102423/submission/70170291

	PSEG_NODES_ALLOCATED sizing:
		(int, 1.25e7) made my code use 220mb here: https://codeforces.com/contest/1771/submission/202989566
		(int, 5e6 barely worked on that problem. 4e6 is too small though.)
 */

#pragma region  // PersistentSegTree
template<class T, class S, int NODES_ALLOCATED> struct PersistentSegTree {
    static const int LIM = NODES_ALLOCATED;
    int SZ=-1; int n=-1; int orig_n = -1;
    static const T idT = INF_i;  // ! IDseg
    static const S idS = -INF_i;  // ! IDlazy
	T cmb(T a, T b) { return min(a,b); }  // ! seg * seg
    static T lazy_seg(S lazy, T seg) { return (lazy==idS) ? seg : lazy; }  // ! lazy * seg
	struct node {
		int l=-1, r=-1; T val = idT, lazy = idS;
		void inc(S x) { if (lazy != idS) lazy = x; }  // ! lazy * lazy
		T get() { return lazy_seg(lazy, val); }
	};
	node d[NODES_ALLOCATED]; int nex = 0;
	int copy(int c) { d[nex] = d[c]; return nex++; }
	void pull(int c) {
        d[c].val =
		cmb(d[d[c].l].get(), d[d[c].r].get()); }
	//// MAIN FUNCTIONS
	T query(int c, int lo, int hi, int L, int R) {
		if (lo <= L && R <= hi) return d[c].get();
		if (R < lo || hi < L) return idT;
		int M = (L+R)/2;
		return lazy_seg(d[c].lazy, cmb(query(d[c].l,lo,hi,L,M),
							query(d[c].r,lo,hi,M+1,R)));
	}
	int upd(int c, int lo, int hi, S v, int L, int R) {
		if (R < lo || hi < L) return c;
		int x = copy(c);
		if (lo <= L && R <= hi) { d[x].inc(v); return x; }
		int M = (L+R)/2;
		d[x].l = upd(d[x].l,lo,hi,v,L,M);
		d[x].r = upd(d[x].r,lo,hi,v,M+1,R);
		pull(x); return x;
	}
	int build(const auto& arr, int L, int R) {
		int c = nex++;
		if (L == R) {
			if (L < sz(arr)) d[c].val = arr[L];
			return c;
		}
		int M = (L+R)/2;
		d[c].l = build(arr,L,M), d[c].r = build(arr,M+1,R);
		pull(c); return c;
	}

    #pragma region  // lstTrue_from_base. TODO add fstTrue_from_base.
    // ~ lstTrue_from_base(L, check(v,r)) == lstTrue(L, orig_n-1, check(query(L,r), r)).
    int lstTrue_from_base(int L, int time, auto&& check) {
        int c = time;
        int l = 0; int r = n-1;
        auto m = [&]() -> int {return (l+r)>>1;};

        // dbgcBold("lsR", L, MT(l,r), this);
        // Special case: Check if the whole array is valid.
        if (l == r) {return l-1+check(d[c].get(), r);}
        if (L == 0 && check(d[c].get(), r)) {return orig_n-1;}
        V<int> path;  // path holds all ancestors of c.
        auto move_DL = [&]() -> void { path.push_back(c); c = d[c].l; r=m(); };
        auto move_DR = [&]() -> void { path.push_back(c); c = d[c].r; l=m()+1; };
        auto move_U = [&]() -> void {
            int len = r-l+1;
            if (d[path.back()].l == c) { r += len; }
            else { l -= len; }
            c = path.back(); path.pop_back();
        };
        auto move_R = [&]() -> void {
            int steps = 0;
            while (d[path.back()].r == c) { move_U(); ++steps; }
            c = d[path.back()].r; int len=(r-l+1); l+=len; r+=len;
            rep(steps) { move_DL(); }
        };

        while (l != L) {
            // dbgc("stage 1", l,r, L, path);
            if (L <= m()) {move_DL();}
            else {move_DR();}
        }

        T wip = idT;
        while (true) {
            T fut = cmb(wip, d[c].get());
            // dbgcP("stage 2", l,r, "", fut, path);
            if (check(fut,r)) {  // If we can safely add this chunk...
                wip = fut;
                if (r == n-1) {
                    // We've reached the end.
                    return orig_n-1;
                } else if (d[path.back()].r == c) {
                    // This was a right child.
                    move_U(); move_R();
                } else {
                    // This was a left child.
                    move_R();
                }
            } else { break; }
        }

        // (l,r) would reach too far.
        while (l != r) {
            // does DL reach too far?
            T fut = cmb(wip, d[d[c].l].get());
            // dbgcB("stage 3", l,r, "", fut, path);
            if (check(fut,m())) {wip = fut; move_DR();}
            else {move_DL();}
        }
        int out = l-1;
        ckmin(out, orig_n-1);
        // dbgR(out); el;
        return out;
    }
    #pragma endregion  // lstTrue_from_base


	vi loc; //// PUBLIC
	void upd(int lo, int hi, S v) {
		loc.pb(upd(loc.bk,lo,hi,v,0,SZ-1)); }
	T query(int ti, int lo, int hi) {
		return query(loc[ti],lo,hi,0,SZ-1); }

    void clear() { FOR(c, 0, nex) {d[c] = {};} nex = 0; orig_n = n = SZ = -1; loc.clear(); }
	template<typename A> void build(const V<A>& arr) {
        assert(arr.size()==orig_n && SZ>0); loc.pb(build(arr,0,SZ-1));}
    template<typename A> void init(const V<A>& arr) {
        clear(); orig_n = arr.size(); for (n = 1; n < orig_n; ) n *= 2; SZ=n; build(arr); }
	void init(int n_) { init(V<T>(n_)); }
};

template<class T, class S, int NODES>
string tsdbg(PersistentSegTree<T, S, NODES>* st) {
    int c = st->loc.bk;
	assert(st->orig_n != -1);
	V<T> out;
	FOR(k, 0, st->orig_n) {out.pb(st->query(c, k, k));}
    return tsdbg(out);
}
#pragma endregion  // PersistentSegTree

// ! Watch out for overflow! Are ints really okay?
const int PSEG_NODES_ALLOCATED = 1.25e7;  // ~ TODO tune using custom invocation!
PersistentSegTree<int, int, PSEG_NODES_ALLOCATED> pst;
