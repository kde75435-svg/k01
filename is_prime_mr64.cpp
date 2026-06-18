bool is_prime(int64_t x) {
    if (x<2) {
        return false;
    }
    using ll=int64_t ;
    const int mp[]={2,3,5,7,11,13,17,19,23,29,31,37};
    for (auto y:mp) {
        if (x==y) {
            return true;
        }
        if (x%y==0) {
            return false;
        }
    }
    auto kuaimi=[](ll a,ll x,ll p)->ll {
        ll b=a,sum=1;
        while (x) {
            if ((x&1)) {
                sum=(ll)((__int128)sum*b)%p;
            }
            b=(ll)((__int128)b*b)%p;
            x=x>>1;
        }
        return sum;
    };
auto pl=[&](ll a,ll p)->bool {
    ll x=__builtin_ctzll(p-1),y=(p-1)>>x,w=kuaimi(a,y,p);
    x-=1;
    if (w==1||w==(p-1)) {
        return true;
    }
    while (x--) {
        w=(ll)((__int128)w*w)%p;
        if (w==(p-1)) {
            return true;
        }
    }
    return false;
};
    for (auto y:mp) {
        if (!pl(y,x)) {
            return false;
        }
    }
    return true;
}
