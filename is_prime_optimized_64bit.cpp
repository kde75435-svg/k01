bool is_prime(int64_t x) {
    if (x<2) {
        return false;
    }
    using ll=int64_t;
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
        ll b=a%p,sum=1;
        while (x) {
            if (x&1) {
                sum=(ll)((__int128)sum*b)%p;
            }
            b=(ll)((__int128)b*b)%p;
            x=x>>1;
        }
        return sum;
    };

    auto f=[&](ll a,ll p)->bool {
return kuaimi(a,p-1,p)==1;
    };//是1就返回true，不是1就返回false

    auto p0=[](ll q)->ll{
        ll sum=0;
        while ((q&1)==0) {
            sum++;
            q=q>>1;
        }
        return sum;
    };//判定一个数的二进制末尾有多少个0

    auto pl=[&](ll a,ll n) ->bool{
        ll x=p0(n-1),y=(n-1)>>x,first=kuaimi(a,y,n);
        if (first==1||first==n-1) {
            return true;
        }
        while (x--) {
            first=kuaimi(first,2,n);
            if (first==n-1) {
                return true;
            }
        }
        return false;
    };//如果第一次是1或者-1返回true，接下来如果是-1就返回true，如果没有-1了就返回false
    //for (auto a:mp) {
        //if (f(a,x)!=1) {
            //return false;
        //}
    //}
    for (int a:mp) {
        if (!pl(a,x)) {
            return false;
        }
    }
    return true;
}
