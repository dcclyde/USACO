/**
 * Description: Lazy SegTree supports modifications of the form
     * \texttt{ckmax(a\_i,t)} for all $l\le i\le r$, range max and sum queries.
 * Time: O(\log N)
 * Source: http://codeforces.com/blog/entry/57319
 * Verification: http://acm.hdu.edu.cn/showproblem.php?pid=5306
 */

struct SegTreeBeats {
    int N, SZ, n, orig_n;
    V<V<ll>> mx;
    V<int> maxCnt;
    V<ll> sum;
    void pull(int ind) {
        F0R(i,2) mx[ind][i] = max(mx[2*ind][i],mx[2*ind+1][i]);
        maxCnt[ind] = 0;
        F0R(i,2) {
            if (mx[2*ind+i][0] == mx[ind][0])
                maxCnt[ind] += maxCnt[2*ind+i];
            else ckmax(mx[ind][1],mx[2*ind+i][0]);
        }
        sum[ind] = sum[2*ind]+sum[2*ind+1];
    }
    void init(ll N_) {
        N = N_;
        SZ = 1;
        while (SZ < N) {SZ *= 2;}
        mx.assign(2*SZ, V<ll>(2));
        maxCnt.resize(2*SZ);
        sum.resize(2*SZ);
    }
    void build(auto& a, int ind = 1, int L = 0, int R = -1) {
        if (R == -1) { R = SZ-1; }
        if (L == R) {
            mx[ind][0] = sum[ind] = (L < N ? a[L] : 0);
            maxCnt[ind] = 1; mx[ind][1] = -1;
            return;
        }
        int M = (L+R)/2;
        build(a,2*ind,L,M); build(a,2*ind+1,M+1,R); pull(ind);
        n = SZ; orig_n = N;
    }
    void push(int ind, int L, int R) {
        if (L == R) return;
        F0R(i,2) if (mx[2*ind^i][0] > mx[ind][0]) {
            sum[2*ind^i] -= (ll)maxCnt[2*ind^i]*
                            (mx[2*ind^i][0]-mx[ind][0]);
            mx[2*ind^i][0] = mx[ind][0];
        }
    }
    void push_all(int ind=1, int L=0, int R=-1) {
        if ( R == -1 ) {R = SZ-1;} push(ind, L, R);
        if (L < R) {int M = (L+R)/2; push_all(2*ind, L, M); push_all(2*ind+1, M+1, R);}
    }
    void upd(int x, int y, ll t, int ind=1, int L=0, int R=-1) {
        if (R == -1) R += SZ;
        if (R < x || y < L || mx[ind][0] <= t) return;
        push(ind,L,R);
        if (x <= L && R <= y && mx[ind][1] < t) {
            sum[ind] -= (ll)maxCnt[ind]*(mx[ind][0]-t);
            mx[ind][0] = t;
            return;
        }
        if (L == R) return;
        int M = (L+R)/2;
        upd(x,y,t,2*ind,L,M); upd(x,y,t,2*ind+1,M+1,R); pull(ind);
    }
    ll qsum(int x, int y, int ind = 1, int L = 0, int R = -1) {
        if (R == -1) R += SZ;
        if (R < x || y < L) return 0;
        push(ind,L,R);
        if (x <= L && R <= y) return sum[ind];
        int M = (L+R)/2;
        return qsum(x,y,2*ind,L,M)+qsum(x,y,2*ind+1,M+1,R);
    }
    ll qmax(int x, int y, int ind = 1, int L = 0, int R = -1) {
        if (R == -1) R += SZ;
        if (R < x || y < L) return -1;
        push(ind,L,R);
        if (x <= L && R <= y) return mx[ind][0];
        int M = (L+R)/2;
        return max(qmax(x,y,2*ind,L,M),qmax(x,y,2*ind+1,M+1,R));
    }
};

string to_string(SegTreeBeats st) {
    st.push_all(); vector<ll> out;
    FOR(k, st.n, st.n + st.orig_n) { out.push_back(st.sum[k]); }
    return to_string(out);
}
