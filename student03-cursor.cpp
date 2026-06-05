#include<iostream>
#include<string>
#include<vector>
#include<unordered_map>
#include<fstream>
#include<ctime>

using namespace std;

struct student {
    string name;
    string xuehao;
    string xingbie;
    vector<unordered_map<string,int>> chengji;
};

vector<struct student> students;

unordered_map<string,long long>xingmingchaxuehao;
unordered_map<long long,string>xuehaochaxingming;

void rizhijilu(string caozuo) {
    ofstream rizhi("caozuorizhi",ios::app);
    time_t now=time(0);
    char buf[80];
    strftime(buf,sizeof(buf),"%Y-%m-%d %H:%M:%S",localtime(&now));
    rizhi<<buf<<"  "<<caozuo<<endl;
    rizhi.close();
}

void dayincaidan() {
    cout<<"*********\n0是退出\n1是读入\n2是按姓名删除\n3是按学号删除\n4是学科成绩统计\n5是查看日志\n6是按姓名查询\n7是按学号查询\n*********"<<endl;
}

void baocun() {
    ofstream duru("xueshengxinxi");
    for (int i=0;i<students.size();i++) {
        duru<<students[i].name<<endl;
        duru<<students[i].xuehao<<endl;
        duru<<students[i].xingbie<<endl;
        for (int j=0;j<students[i].chengji.size();j++) {
            for (unordered_map<string,int>::iterator it=students[i].chengji[j].begin();it!=students[i].chengji[j].end();it++) {
                duru<<it->first<<endl;
                duru<<it->second<<endl;
            }
        }
        duru<<'#'<<endl;
    }
    duru.close();
}

void tianjia() {
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
    duru<<n<<endl<<xh<<endl<<xb<<endl;
    xingmingchaxuehao[n]=stoll(xh);
    xuehaochaxingming[stoll(xh)]=n;

    struct student s;
    s.name=n;
    s.xuehao=xh;
    s.xingbie=xb;

    string rizhixinxi="添加学生：姓名"+n+" 学号"+xh+" 性别"+xb;

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
        unordered_map<string,int> mp;
        mp[ke]=fenshu;
        s.chengji.push_back(mp);
        rizhixinxi+="  "+ke+":"+to_string(fenshu);
        cout<<"继续请按1，退出请按0\n";
        cin>>x;
    }
    duru<<'#'<<endl;
    duru.close();
    students.push_back(s);
    rizhijilu(rizhixinxi);
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
    }
    return true;
}

void shanchuxingming() {
    string n;
    cout<<"请输入要删除的学生姓名：";
    cin>>n;
    if (!xingmingchaxuehao.count(n)) {
        cout<<"未找到该学生"<<endl;
        rizhijilu("按姓名删除失败，未找到："+n);
        return;
    }
    long long xh=xingmingchaxuehao[n];
    xuehaochaxingming.erase(xh);
    xingmingchaxuehao.erase(n);
    for (int i=0;i<students.size();i++) {
        if (students[i].name==n) {
            students.erase(students.begin()+i);
            break;
        }
    }
    baocun();
    cout<<"删除成功"<<endl;
    rizhijilu("按姓名删除学生："+n+" 学号"+to_string(xh));
}

void shanchuxuehao() {
    string xh;
    cout<<"请输入要删除的学号：";
    cin>>xh;
    long long xhn=stoll(xh);
    if (!xuehaochaxingming.count(xhn)) {
        cout<<"未找到该学号"<<endl;
        rizhijilu("按学号删除失败，未找到学号："+xh);
        return;
    }
    string n=xuehaochaxingming[xhn];
    xuehaochaxingming.erase(xhn);
    xingmingchaxuehao.erase(n);
    for (int i=0;i<students.size();i++) {
        if (students[i].name==n) {
            students.erase(students.begin()+i);
            break;
        }
    }
    baocun();
    cout<<"删除成功"<<endl;
    rizhijilu("按学号删除学生："+n+" 学号"+xh);
}

void xuekechengjitongji() {
    if (students.size()==0) {
        cout<<"暂无学生数据"<<endl;
        rizhijilu("查看学科成绩统计失败，暂无学生数据");
        return;
    }

    unordered_map<string,int> zongfen;
    unordered_map<string,int> renshu;
    unordered_map<string,int> zuigaofen;
    unordered_map<string,string> zuigaoxingming;
    unordered_map<string,string> zuigaoxuehao;
    unordered_map<string,int> zuidifen;
    unordered_map<string,string> zuidixingming;
    unordered_map<string,string> zuidixuehao;

    for (int i=0;i<students.size();i++) {
        for (int j=0;j<students[i].chengji.size();j++) {
            for (unordered_map<string,int>::iterator it=students[i].chengji[j].begin();it!=students[i].chengji[j].end();it++) {
                string ke=it->first;
                int fenshu=it->second;

                zongfen[ke]+=fenshu;
                renshu[ke]++;

                if (!zuigaofen.count(ke)||fenshu>zuigaofen[ke]) {
                    zuigaofen[ke]=fenshu;
                    zuigaoxingming[ke]=students[i].name;
                    zuigaoxuehao[ke]=students[i].xuehao;
                }
                if (!zuidifen.count(ke)||fenshu<zuidifen[ke]) {
                    zuidifen[ke]=fenshu;
                    zuidixingming[ke]=students[i].name;
                    zuidixuehao[ke]=students[i].xuehao;
                }
            }
        }
    }

    if (renshu.size()==0) {
        cout<<"暂无学科成绩"<<endl;
        rizhijilu("查看学科成绩统计失败，暂无学科成绩");
        return;
    }

    rizhijilu("查看学科成绩统计");

    for (unordered_map<string,int>::iterator it=renshu.begin();it!=renshu.end();it++) {
        string ke=it->first;
        double pingjun=(double)zongfen[ke]/renshu[ke];
        cout<<"\n学科："<<ke<<endl;
        cout<<"最高分："<<zuigaofen[ke]<<"  姓名："<<zuigaoxingming[ke]<<"  学号："<<zuigaoxuehao[ke]<<endl;
        cout<<"最低分："<<zuidifen[ke]<<"  姓名："<<zuidixingming[ke]<<"  学号："<<zuidixuehao[ke]<<endl;
        cout<<"平均分："<<pingjun<<endl;
    }
}

void duqurizhi() {
    ifstream rizhi("caozuorizhi");
    if (!rizhi.is_open()) {
        cout<<"暂无日志文件"<<endl;
        return;
    }
    string hang;
    bool youneirong=false;
    while (getline(rizhi,hang)) {
        cout<<hang<<endl;
        youneirong=true;
    }
    rizhi.close();
    if (!youneirong) {
        cout<<"日志文件为空"<<endl;
    }
    rizhijilu("查看操作日志");
}

void chaxunxingming() {
    string n;
    cout<<"请输入要查询的学生姓名：";
    cin>>n;
    if (!xingmingchaxuehao.count(n)) {
        cout<<"未找到该学生"<<endl;
        rizhijilu("按姓名查询失败，未找到："+n);
        return;
    }
    for (int i=0;i<students.size();i++) {
        if (students[i].name==n) {
            cout<<"\n姓名："<<students[i].name<<endl;
            cout<<"学号："<<students[i].xuehao<<endl;
            cout<<"性别："<<students[i].xingbie<<endl;
            if (students[i].chengji.size()==0) {
                cout<<"暂无学科成绩"<<endl;
            }else {
                cout<<"学科成绩："<<endl;
                for (int j=0;j<students[i].chengji.size();j++) {
                    for (unordered_map<string,int>::iterator it=students[i].chengji[j].begin();it!=students[i].chengji[j].end();it++) {
                        cout<<it->first<<"："<<it->second<<endl;
                    }
                }
            }
            rizhijilu("按姓名查询学生："+n);
            return;
        }
    }
}

void chaxunxuehao() {
    string xh;
    cout<<"请输入要查询的学号：";
    cin>>xh;
    long long xhn=stoll(xh);
    if (!xuehaochaxingming.count(xhn)) {
        cout<<"未找到该学号"<<endl;
        rizhijilu("按学号查询失败，未找到学号："+xh);
        return;
    }
    string n=xuehaochaxingming[xhn];
    for (int i=0;i<students.size();i++) {
        if (students[i].name==n) {
            cout<<"\n姓名："<<students[i].name<<endl;
            cout<<"学号："<<students[i].xuehao<<endl;
            cout<<"性别："<<students[i].xingbie<<endl;
            if (students[i].chengji.size()==0) {
                cout<<"暂无学科成绩"<<endl;
            }else {
                cout<<"学科成绩："<<endl;
                for (int j=0;j<students[i].chengji.size();j++) {
                    for (unordered_map<string,int>::iterator it=students[i].chengji[j].begin();it!=students[i].chengji[j].end();it++) {
                        cout<<it->first<<"："<<it->second<<endl;
                    }
                }
            }
            rizhijilu("按学号查询学生："+n+" 学号"+xh);
            return;
        }
    }
}

int main() {
    if (jiazai()) {
        cout<<"数据加载成功"<<endl;
        rizhijilu("程序启动，数据加载成功，共加载"+to_string(students.size())+"名学生");
    }else {
        cout<<"数据加载失败，将从零开始"<<endl;
        rizhijilu("程序启动，数据加载失败，将从零开始");
    }
    dayincaidan();
    cout<<"请输入：";
    int x;
    cin>>x;
    while (x) {
        if (x==1) {
            tianjia();
        }
        if (x==2) {
            shanchuxingming();
        }
        if (x==3) {
            shanchuxuehao();
        }
        if (x==4) {
            xuekechengjitongji();
        }
        if (x==5) {
            duqurizhi();
        }
        if (x==6) {
            chaxunxingming();
        }
        if (x==7) {
            chaxunxuehao();
        }
        dayincaidan();
        cout<<"请继续输入：";
        cin>>x;
    }
    rizhijilu("退出程序");
    return 0;
}
