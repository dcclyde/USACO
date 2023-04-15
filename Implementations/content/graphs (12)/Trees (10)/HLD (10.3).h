/**
 * Description: Heavy-Light Decomposition.
 * 		Run any lazy segtree queries/updates on path(a, b) or subtree(a).
 *      Same for on complement of path(a, b) or complement of subtree(a) (costs extra log(log(N)))
 *      Bonus: depth, LCA, distance, jmp to kth ancestor.
 * Time: any tree path is split into $O(\log N)$ parts
 * Source: http://codeforces.com/blog/entry/22072, https://codeforces.com/blog/entry/53170
 * Verification: *
 */

#include "../../data-structures/1D Range Queries (9.2)/LazySeg (15.2).h"

template<int SZ, bool VALS_IN_EDGES, class SEG=ll, class LAZY=ll> struct HLD {
	int N; vi adj[SZ];
	int par[SZ], root[SZ], depth[SZ], sz[SZ], ti;
	int pos[SZ]; vi rpos; // rpos not used but could be useful
	LazySeg<SEG,LAZY> tree; // segtree for sum
    void clear() {
        FOR(i, 0, N) { adj[i].clear(); }
        rpos.clear();
        N = -1;
    }
	void ae(int x, int y) { adj[x].pb(y), adj[y].pb(x); }
	void dfsSz(int x) {
		sz[x] = 1;
		each(y,adj[x]) {
			par[y] = x; depth[y] = depth[x]+1;
			adj[y].erase(find(all(adj[y]),x)); /// remove parent from adj list
			dfsSz(y); sz[x] += sz[y];
			if (sz[y] > sz[adj[x][0]]) swap(y,adj[x][0]);
		}
	}
	void dfsHld(int x) {
		pos[x] = ti++; rpos.pb(x);
		each(y,adj[x]) {
			root[y] = (y == adj[x][0] ? root[x] : y);
			dfsHld(y); }
	}
	void init(int _N, int R = 0) { N = _N;
		par[R] = depth[R] = ti = 0; dfsSz(R);
		root[R] = R; dfsHld(R); tree.init(N);
	}
	int lca(int x, int y) {
		for (; root[x] != root[y]; y = par[root[y]])
			if (depth[root[x]] > depth[root[y]]) swap(x,y);
		return depth[x] < depth[y] ? x : y;
	}
    int jmp(int x, int d) {
        // assert(depth[x] >= d);  // optional
        ckmin(d, depth[x]);
        int target_depth = depth[x] - d;
        int y = x;
        while (true) {
            int o = root[y];
            if (depth[o] <= target_depth) {
                int out = rpos[pos[o] + target_depth - depth[o]];
                return out;
            }
            y = par[o];
        }
        assert(false);
    }
	int dist(int x, int y) { // # edges on path
		return depth[x]+depth[y]-2*depth[lca(x,y)]; }
	template <class BinaryOp>
	void processPath(int x, int y, BinaryOp op) {
		for (; root[x] != root[y]; y = par[root[y]]) {
			if (depth[root[x]] > depth[root[y]]) swap(x,y);
			op(pos[root[y]],pos[y]); }
		if (depth[x] > depth[y]) swap(x,y);
		op(pos[x]+VALS_IN_EDGES,pos[y]);
	}
	void modifyPath(int x, int y, LAZY v) {
		processPath(x,y,[this,&v](int l, int r) {
			tree.upd(l,r,v); }); }
	SEG queryPath(int x, int y) {
		SEG res = tree.idT; processPath(x,y,[this,&res](int l, int r) {
            res = tree.cmb(res, tree.query(l,r)); });
		return res; }
	void modifySubtree(int x, LAZY v) {
		tree.upd(pos[x]+VALS_IN_EDGES,pos[x]+sz[x]-1,v); }
    SEG querySubtree(int x) {
        return tree.query(pos[x]+VALS_IN_EDGES,pos[x]+sz[x]-1); }
	V<pii> get_path_chunks(int x, int y) {
		V<pii> res = {{-1,-1},{N,N}};
		processPath(x,y,[this,&res](int l, int r) {
			res.pb({l,r}); });
		sort(all(res));
		return res; }
	SEG queryExceptPath(int x, int y) {
		SEG res = tree.idT;
		auto chunks = get_path_chunks(x, y);
		FOR(k, 1, chunks.size()) { res = tree.cmb(res, tree.query(chunks[k-1].s + 1, chunks[k].f - 1)); }
		return res; }
	void updExceptPath(int x, int y, LAZY v) {
		auto chunks = get_path_chunks(x, y);
		FOR(k, 1, chunks.size()) { tree.upd(chunks[k-1].s + 1, chunks[k].f - 1, v); } }
	SEG queryExceptSubtree(int x) {
		SEG res = tree.idT;
		res = tree.cmb(res, tree.query(0, pos[x]+VALS_IN_EDGES - 1));
		res = tree.cmb(res, tree.query(pos[x] + sz[x], N - 1));
		return res; }
	void updExceptSubtree(int x, LAZY v) {
		tree.upd(0, pos[x]+VALS_IN_EDGES - 1, v);
		tree.upd(pos[x] + sz[x], N - 1, v); }
};
