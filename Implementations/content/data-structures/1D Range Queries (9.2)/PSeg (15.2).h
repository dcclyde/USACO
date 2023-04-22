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

// ! Remember to update LIM, SZ using custom invocation.
const int PSEG_NODES_ALLOCATED = 1.25e7;  // ~ TODO tune using custom invocation
const int PSEG_POSITIONS = 200'001;  // ~ TODO update using problem constraints

template<class T, int SZ> struct PersistentSegTree {
	static const int LIM = PSEG_NODES_ALLOCATED;
	static const int sz = PSEG_POSITIONS;
	int orig_n = -1;
	struct node {
		int l, r; T val = 0, lazy = 0;  // ! IDlazy  (val is unused)
		void inc(T x) { lazy += x; }  // ! lazy * lazy
		T get() { return val+lazy; }  // ! lazy * seg
	};
	node d[LIM]; int nex = 0;
	int copy(int c) { d[nex] = d[c]; return nex++; }
	T cmb(T a, T b) { return min(a,b); }  // ! seg * seg
	void pull(int c) { d[c].val =
		cmb(d[d[c].l].get(), d[d[c].r].get()); }
	//// MAIN FUNCTIONS
	T query(int c, int lo, int hi, int L, int R) {
		if (lo <= L && R <= hi) return d[c].get();
		if (R < lo || hi < L) return MOD;  // ! IDseg
		int M = (L+R)/2;
		return d[c].lazy+cmb(query(d[c].l,lo,hi,L,M),
							query(d[c].r,lo,hi,M+1,R));  // ! lazy * seg (again)
	}
	int upd(int c, int lo, int hi, T v, int L, int R) {
		if (R < lo || hi < L) return c;
		int x = copy(c);
		if (lo <= L && R <= hi) { d[x].inc(v); return x; }
		int M = (L+R)/2;
		d[x].l = upd(d[x].l,lo,hi,v,L,M);
		d[x].r = upd(d[x].r,lo,hi,v,M+1,R);
		pull(x); return x;
	}
	int build(const V<T>& arr, int L, int R) {
		int c = nex++;
		if (L == R) {
			if (L < sz(arr)) d[c].val = arr[L];
			return c;
		}
		int M = (L+R)/2;
		d[c].l = build(arr,L,M), d[c].r = build(arr,M+1,R);
		pull(c); return c;
	}

	vi loc; //// PUBLIC
	void upd(int lo, int hi, T v) {
		loc.pb(upd(loc.bk,lo,hi,v,0,SZ-1)); }
	T query(int ti, int lo, int hi) {
		return query(loc[ti],lo,hi,0,SZ-1); }

    void clear() { FOR(c, 0, nex) {d[c] = {};} nex = 0; orig_n = -1; loc.clear(); }
	void init(int n_) { clear(); orig_n = n_; build(V<T>(n_)); }
	void build(const V<T>&arr) {orig_n = arr.size(); loc.pb(build(arr,0,SZ-1));}
};

template<class T, int SZ>
string tsdbg(PersistentSegTree<T, SZ> st, int time) {
	assert(st.orig_n != -1);
	V<T> out;
	FOR(k, 0, st.orig_n) { out.pb(st.query(time, k, k)); }
    return tsdbg(out);
}


PersistentSegmentTree<int, PSEG_POSITIONS> pst;
