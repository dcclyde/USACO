/**
 * Description: shortest path
 * Source: own
 * Verification: https://open.kattis.com/problems/shortestpath1
 */

template<class C, bool directed> struct Dijkstra {
	int SZ; V<C> dist;
	V<V<pair<int,C>>> adj;
	void init(int _SZ) { SZ = _SZ; adj.clear(); adj.rsz(SZ); }
	void ae(int u, int v, C cost) {
		adj[u].pb({v,cost}); if (!directed) adj[v].pb({u,cost}); }
	void gen(int st) {
		dist.assign(SZ,numeric_limits<C>::max());
		using T = pair<C,int>; pqg<T> pq;
		auto ad = [&](int a, C b) {
			if (dist[a] <= b) return;
			pq.push({dist[a] = b,a});
		}; ad(st,0);
		while (sz(pq)) {
			T x = pq.top(); pq.pop(); if (dist[x.s] < x.f) continue;
			each(y,adj[x.s]) ad(y.f,x.f+y.s);
		}
	}
};

// dcclyde's extension. "validated" (used once) on https://codeforces.com/contest/1749/problem/E
template<class C, bool directed> struct DijkstraWithPath {
	int SZ; V<C> dist; V<int> prev;
	V<V<pair<int,C>>> adj;
	void init(int _SZ) { SZ = _SZ; adj.clear(); adj.rsz(SZ); prev.assign(SZ,-1); }
	void ae(int u, int v, C cost) {
		adj[u].pb({v,cost}); if (!directed) adj[v].pb({u,cost}); }
	void gen(int st) {
		dist.assign(SZ,numeric_limits<C>::max());
		using T = tuple<C,int,int>; pqg<T> pq;
		auto ad = [&](int a, int from, C b) {
			if (dist[a] <= b) return;
            pq.emplace(dist[a] = b, a, from);
		}; ad(st,-1,0);
		while (sz(pq)) {
            auto [xcost, xi, xfrom] = pq.top(); pq.pop(); if (dist[xi] < xcost) continue;
            prev[xi] = xfrom;
            each(y,adj[xi]) ad(y.f, xi, xcost + y.s);
		}
	}
};