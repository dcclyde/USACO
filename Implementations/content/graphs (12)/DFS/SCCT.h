/**
 * Description: Tarjan's, DFS once to generate
 	* strongly connected components in topological order. $a,b$
 	* in same component if both $a\to b$ and $b\to a$ exist.
 	* Uses less memory than Kosaraju b/c doesn't store reverse edges.
 * Time: O(N+M)
 * Source: KACTL
 	* https://github.com/kth-competitive-programming/kactl/blob/master/content/graph/SCC.h
 * Verification: https://cses.fi/problemset/task/1686/
 */

struct SCC {
	int N, ti = 0; V<vi> adj;
	vi disc, comp, stk, comps;
	void init(int _N){ N = _N, adj.rsz(N);
		disc.rsz(N), comp.rsz(N,-1);}
	void ae(int x, int y) { adj[x].pb(y); }
	int dfs(int x) {
		int low = disc[x] = ++ti; stk.pb(x);
		each(y,adj[x]) if (comp[y] == -1) // comp[y] == -1,
			ckmin(low,disc[y]?:dfs(y));  // disc[y] != 0 -> in stack
		if (low == disc[x]) { // make new SCC
			// pop off stack until you find x
			comps.pb(x); for (int y = -1; y != x;)
				comp[y = stk.bk] = x, stk.pop_back();
		}
		return low;
	}
	void gen() {
		F0R(i,N) if (!disc[i]) dfs(i);
		reverse(all(comps));
	}
};

#pragma region  // demo
// SCC scc;
// scc.init(8);
// scc.ae(0, 1);
// scc.ae(1, 0);

// scc.ae(2, 3);
// scc.ae(3, 4);
// scc.ae(4, 2);
// scc.ae(2, 0);

// scc.ae(5, 6);
// scc.ae(6, 7);
// scc.ae(7, 3);

// scc.gen();
// dbg(scc.N, scc.ti, scc.disc);
// dbg(scc.comp);
// dbg(scc.comps);
#pragma endregion
