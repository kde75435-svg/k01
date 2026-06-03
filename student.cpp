#include<iostream>
#include<string>
#include<fstream>
#include<unordered_map>
#include<cstdio>

using namespace std;

const int N=1e5+10;

int count;


struct studet {
    string name;
    int shuxue;
    int yingyv;
    int yvwen;

}s[N];

unordered_map<string,int> mp;

bool jiazai() {
    ifstream tiqv("xueshengxinxi");
    count++;
    while (tiqv>>s[count].name>>s[count].shuxue>>s[count].yingyv>>s[count].yvwen) {
        mp[s[count].name]=count;
        count++;
    }
    count--;
    bool flag=0;
    if (tiqv.is_open()) {
        flag=1;
    }
    tiqv.close();
    return flag;
}

bool tianjia() {
    ofstream duru("xueshengxinxi",ios::app);
    bool flag=0;
    if (duru.is_open()) {
        flag=1;
    }
    string n;
    int shu,ying,yv;
    cout<<"姓名：";
    cin>>n;
    cout<<"\n数学成绩：";
    cin>>shu;
    cout<<"\n英语成绩：";
    cin>>ying;
    cout<<"\n语文成绩：";
    cin>>yv;
    duru<<n<<"\n"<<shu<<"\n"<<ying<<"\n"<<yv<<"\n";
    count++;
    mp[n]=count;
    s[count].name=n;
    s[count].shuxue=shu;
    s[count].yingyv=ying;
    s[count].yvwen=yv;
    duru.close();
    return flag;
}

void xuehaochazhao() {
    int w;
    cout<<"请输入要查询的学号:";
    cin>>w;
    if (w>count) {
        cout<<"查无此人"<<endl;
        return ;
    }
    cout<<"姓名："<<s[w].name<<endl;
    cout<<"数学成绩："<<s[w].shuxue<<endl;
    cout<<"英语成绩："<<s[w].yingyv<<endl;
    cout<<"语文成绩："<<s[w].yvwen<<endl;
}

void xingmingchazhaoxinxi() {
    string m;
    cout<<"请输入要查询的姓名:";
    cin>>m;
    int q=mp[m];
    if (q==0) {
        cout<<"查无此人"<<endl;
        return ;
    }
    cout<<"学号："<<q<<endl;
    cout<<"姓名："<<s[q].name<<endl;
    cout<<"数学成绩："<<s[q].shuxue<<endl;
    cout<<"英语成绩："<<s[q].yingyv<<endl;
    cout<<"语文成绩："<<s[q].yvwen<<endl;
}

int main() {
    system("chcp 65001");
    if (jiazai()) {
        cout<<"数据加载成功"<<endl;
    }else {
        cout<<"数据加载失败，将从零开始"<<endl;
    }
    printf("0是退出,1是添加信息,2是学号查找信息,3是姓名查找信息\n");
    printf("请输入：");
    int x;
    cin>>x;
    while (x) {
        if (x==1) {
            if (tianjia()) {
                printf("添加成功\n");
            }else {
                printf("添加失败\n");
            }
        }else if (x==2) {
            xuehaochazhao();
        }else if (x==3) {
            xingmingchazhaoxinxi();
        }
        printf("请继续输入：");
        cin>>x;
    }

    return 0;
}
