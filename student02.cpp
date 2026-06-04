#include<iostream>
#include<string>
#include<vector>
#include<unordered_map>
#include<fstream>

using namespace std;

struct student {
    string name;
    string xuehao;
    string xingbie;
    vector<unordered_map<string,int>> chengji;
};

vector<struct student> students;

unordered_map<string,int> xingmingchazhaoxiabiao;

unordered_map<string,long long>xingmingchaxuehao;
unordered_map<long long,string>xuehaochaxingming;

void tianjia() {
    struct student m;
    ofstream duru("xueshengxinxi",ios::app);
    string n,xh,xb;
    cout<<"姓名：";
    cin>>n;
    while (xingmingchaxuehao.count(n)) {
        cout<<"姓名已经存在，请重新输入："<<endl;
        cin>>n;
    }
    cout<<"\n学号：";
    cin>>xh;
    while (xuehaochaxingming.count(stoll(xh))) {
        cout<<"学号已存在，请重新输入：";
        cin>>xh;
    }
    cout<<"\n性别：";
    cin>>xb;
    m.name=n;
    m.xuehao=xh;
    m.xingbie=xb;
    duru<<n<<endl<<xh<<endl<<xb<<endl;
    xingmingchaxuehao[n]=stoll(xh);
    xuehaochaxingming[stoll(xh)]=n;
    int x;
    cout<<"\n0是停止输入学科成绩，1是输入学科成绩，请输入：";
    cin>>x;
    while (x) {
string ke;
        int fenshu;
        cout<<"请输入学科：";
        cin>>ke;
        cout<<"\n成绩：";
        cin>>fenshu;
        duru<<ke<<endl<<fenshu<<endl;
        unordered_map<string,int> p;
        p[ke]=fenshu;
        m.chengji.push_back(p);
        cout<<"继续请按1，退出请按0\n";
        cin>>x;
    }
    duru<<'#'<<endl;
    students.push_back(m);
    xingmingchazhaoxiabiao[n]=students.size()-1;
    duru.close();
}

bool jiazai() {
    ifstream tiqv("xueshengxinxi");
    if (!tiqv.is_open()) {
        return false;
    }
    string n;
    while (tiqv>>n) {
        struct student s;
        s.name=n;
        tiqv>>s.xuehao;
        tiqv>>s.xingbie;
        xingmingchaxuehao[n]=stoll(s.xuehao);
        xuehaochaxingming[stoll(s.xuehao)]=n;
        string ke;
        while (tiqv>>ke&&ke!="#") {
            int fenshu;
            tiqv>>fenshu;
            unordered_map<string,int> mp;
            mp[ke]=fenshu;
            s.chengji.push_back(mp);
        }
        students.push_back(s);
        xingmingchazhaoxiabiao[n]=students.size()-1;
    }
    return true;
}

void xingmingchakanxinxi() {
    string ss;
    cout<<"请输入姓名：";
    cin>>ss;
    if (xingmingchaxuehao.count(ss)==0) {
        cout<<"查无此人"<<endl;
        return ;
    }
    int xiabiao=xingmingchazhaoxiabiao[ss];
    cout<<"姓名："<<students[xiabiao].name<<endl;
    cout<<"学号："<<students[xiabiao].xuehao<<endl;
    cout<<"性别："<<students[xiabiao].xingbie<<endl;
    for (auto x:students[xiabiao].chengji) {
        cout<<(*x.begin()).first<<" "<<(*x.begin()).second<<endl;;
    }
}

    void xuehaochaxinxi() {
    long long xiabiao;
    string ss,xuehao1;
    cout<<"请输入学号：";
    cin>>xuehao1;
    if (xuehaochaxingming.count(stoll(xuehao1))==0) {
        cout<<"查无此人"<<endl;
        return ;
    }
    xiabiao=xingmingchazhaoxiabiao[xuehaochaxingming[stoll(xuehao1)]];
    cout<<"姓名："<<students[xiabiao].name<<endl;
    cout<<"学号："<<students[xiabiao].xuehao<<endl;
    cout<<"性别："<<students[xiabiao].xingbie<<endl;
    for (auto x:students[xiabiao].chengji) {
        cout<<(*x.begin()).first<<" "<<(*x.begin()).second<<endl;;
}

}

int main() {
    if (jiazai()) {
        cout<<"数据加载成功"<<endl;
    }else {
        cout<<"数据加载失败，将从零开始"<<endl;
    }
cout<<"*********\n0是退出\n1是读入\n2是输入姓名查看信息\n3是输入学号查看信息*********\n请输入：";
    int x;
    cin>>x;
    while (x) {
if (x==1) {
    tianjia();
}else if (x==2) {
    xingmingchakanxinxi();
}else if (x==3) {
xuehaochaxinxi();
}
        cout<<"请继续输入：";
        cin>>x;
    }
    return 0;
}
