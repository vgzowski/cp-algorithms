/// Someones from CF (632 F), can support modulo
typedef long long ll;
const double PI=acos(-1.0);
const int maxn=400005;
namespace fft
{
    struct num{
        double x,y;
        num() {x=y=0;}
        num(double x,double y):x(x),y(y){}
    };
    inline num operator+(num a,num b) {return num(a.x+b.x,a.y+b.y);}
    inline num operator-(num a,num b) {return num(a.x-b.x,a.y-b.y);}
    inline num operator*(num a,num b) {return num(a.x*b.x-a.y*b.y,a.x*b.y+a.y*b.x);}
    inline num conj(num a) {return num(a.x,-a.y);}
    int base=1;
    vector<num> roots={{0,0},{1,0}};
    vector<int> rev={0,1};
    const double PI=acosl(-1.0);
    void ensure_base(int nbase){
        if(nbase<=base) return;
        rev.resize(1<<nbase);
        for(int i=0;i<(1<<nbase);i++)
            rev[i]=(rev[i>>1]>>1)+((i&1)<<(nbase-1));
        roots.resize(1<<nbase);
        while(base<nbase){
            double angle=2*PI/(1<<(base+1));
            for(int i=1<<(base-1);i<(1<<base);i++){
                roots[i<<1]=roots[i];
                double angle_i=angle*(2*i+1-(1<<base));
                roots[(i<<1)+1]=num(cos(angle_i),sin(angle_i));
            }
            base++;
        }
    }
    void fft(vector<num> &a,int n=-1){
        if(n==-1) n=a.size();
        assert((n&(n-1))==0);
        int zeros=__builtin_ctz(n);
        ensure_base(zeros);
        int shift=base-zeros;
        for(int i=0;i<n;i++)
            if(i<(rev[i]>>shift))
                swap(a[i],a[rev[i]>>shift]);
        for(int k=1;k<n;k<<=1){
            for(int i=0;i<n;i+=2*k){
                for(int j=0;j<k;j++){
                    num z=a[i+j+k]*roots[j+k];
                    a[i+j+k]=a[i+j]-z;
                    a[i+j]=a[i+j]+z;
                }
            }
        }
    }
    vector<num> fa,fb;
    vector<ll> multiply(vector<int> &a, vector<int> &b){
        int need=a.size()+b.size()-1;
        int nbase=0;
        while((1<<nbase)<need) nbase++;
        ensure_base(nbase);
        int sz=1<<nbase;
        if(sz>(int)fa.size()) fa.resize(sz);
        for(int i=0;i<sz;i++){
            int x=(i<(int)a.size()?a[i]:0);
            int y=(i<(int)b.size()?b[i]:0);
            fa[i]=num(x,y);
        }
        fft(fa,sz);
        num r(0,-0.25/sz);
        for(int i=0;i<=(sz>>1);i++){
            int j=(sz-i)&(sz-1);
            num z=(fa[j]*fa[j]-conj(fa[i]*fa[i]))*r;
            if(i!=j) fa[j]=(fa[i]*fa[i]-conj(fa[j]*fa[j]))*r;
            fa[i]=z;
        }
        fft(fa,sz);
        vector<ll> res(need);
        for(int i=0;i<need;i++) res[i]=fa[i].x+0.5;
        return res;
    }
    vector<int> multiply_mod(vector<int> &a,vector<int> &b,int m,int eq=0){
        int need=a.size()+b.size()-1;
        int nbase=0;
        while((1<<nbase)<need) nbase++;
        ensure_base(nbase);
        int sz=1<<nbase;
        if(sz>(int)fa.size()) fa.resize(sz);
        for(int i=0;i<(int)a.size();i++){
            int x=(a[i]%m+m)%m;
            fa[i]=num(x&((1<<15)-1),x>>15);
        }
        fill(fa.begin()+a.size(),fa.begin()+sz,num{0,0});
        fft(fa,sz);
        if(sz>(int)fb.size()) fb.resize(sz);
        if(eq) copy(fa.begin(),fa.begin()+sz,fb.begin());
        else{
            for(int i=0;i<(int)b.size();i++){
                int x=(b[i]%m+m)%m;
                fb[i]=num(x&((1<<15)-1),x>>15);
            }
            fill(fb.begin()+b.size(),fb.begin()+sz,num{0,0});
            fft(fb,sz);
        }
        double ratio=0.25/sz;
        num r2(0,-1),r3(ratio,0),r4(0,-ratio),r5(0,1);
        for(int i=0;i<=(sz>>1);i++){
            int j=(sz-i)&(sz-1);
            num a1=(fa[i]+conj(fa[j]));
            num a2=(fa[i]-conj(fa[j]))*r2;
            num b1=(fb[i]+conj(fb[j]))*r3;
            num b2=(fb[i]-conj(fb[j]))*r4;
            if(i!=j){
                num c1=(fa[j]+conj(fa[i]));
                num c2=(fa[j]-conj(fa[i]))*r2;
                num d1=(fb[j]+conj(fb[i]))*r3;
                num d2=(fb[j]-conj(fb[i]))*r4;
                fa[i]=c1*d1+c2*d2*r5;
                fb[i]=c1*d2+c2*d1;
            }
            fa[j]=a1*b1+a2*b2*r5;
            fb[j]=a1*b2+a2*b1;
        }
        fft(fa,sz);fft(fb,sz);
        vector<int> res(need);
        for(int i=0;i<need;i++){
            ll aa=fa[i].x+0.5;
            ll bb=fb[i].x+0.5;
            ll cc=fa[i].y+0.5;
            res[i]=(aa+((bb%m)<<15)+((cc%m)<<30))%m;
        }
        return res;
    }
    vector<int> square_mod(vector<int> &a,int m){
        return multiply_mod(a,a,m,1);
    }
};

/// My FFT
struct cp {
    double r, i;
    cp() : r(0), i(0) {}
    cp(ld r, ld i) : r(r), i(i) {}
    inline cp operator + (const cp &other) { return cp(r + other.r, i + other.i); }
    inline cp operator - (const cp &other) { return cp(r - other.r, i - other.i); }
    inline cp operator * (const cp &other) { return cp(r * other.r - i * other.i, r * other.i + i * other.r); }
    inline cp conj() { return cp(r, -i); }
};
inline int get_n(const int &n) {
    if (!(n & (n - 1))) return n;
    else return (1 << (32 - __builtin_clz(n)));
}
constexpr ld pi = 3.14159265358979323846264338327950;
const int N = 2e7;
cp w[N];
int rev[N];
inline void precalc() {
    for (int k = 0; k <= 23; ++k) {
        int pos_start = 1 << k;
        cp cur(1, 0);
        ld al = 2 * pi / (ld)(1 << k);
        cp to_mul(cos(al), sin(al));
        w[pos_start] = cur;
        for (int i = 1; i < (1 << k); ++i) {
            cur = cur * to_mul;
            w[pos_start + i] = cur;
        }
    }
}

inline void fft(ve<cp> &a, bool inv = 0) {
    int n = sz(a);
    int l = 31 - __builtin_clz(n);
    for (int i = 0; i < n; ++i) rev[i] = (rev[i >> 1] >> 1) | ((i & 1) << (l - 1));
    for (int i = 0; i < n; ++i) if (i < rev[i]) swap(a[i], a[rev[i]]);
    for (int len = 1; len < n; len <<= 1) {
        int step = n / (len * 2);
        for (int i = 0; i < n; i += 2 * len) {
            for (int j = 0; j < len; ++j) {
                cp A = a[i + j], B = a[i + j + len] * (!inv ? w[len * 2 + j] : w[len * 2 + j].conj());
                a[i + j] = A + B;
                a[i + j + len] = A - B;
            }
        }
    }
    if (inv) {
        for (auto &x : a) {
            x.r /= (ld)n;
            x.i /= (ld)n;
        }
    }
}
inline ve<ll> mul(const ve<int> &A, const ve<int> &B) {
    int n = sz(A), m = sz(B);
    int s = get_n(n + m + 2);
    ve<cp> a(s), b(s);
    for (int i = 0; i < n; ++i) a[i].r = A[i];
    for (int i = 0; i < m; ++i) b[i].r = B[i];
    fft(a); fft(b);
    for (int i = 0; i < s; ++i) {
        a[i] = a[i] * b[i];
    }
    fft(a, 1);
    ve<ll> res(s);
    for (int i = 0; i < s; ++i) res[i] = (ll)round(a[i].r);
    while (res.size() > 0 && res.back() == 0) res.pop_back();
    return res;
}
