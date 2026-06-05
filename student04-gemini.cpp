#include <iostream>
#include <string>
#include <unordered_map>
#include <fstream>
#include <ctime>
#include <limits>
#include <iomanip>

using namespace std;

// ================== 数据结构定义 ==================
struct Student {
    string name;       // 姓名
    string id;         // 学号 (统一用string，防止超出整型范围或输入字母导致崩溃)
    string gender;     // 性别
    unordered_map<string, int> scores; // 学科成绩 (科目 -> 分数)
};

// 核心数据库：学号 -> 学生实体
unordered_map<string, Student> db;
// 辅助索引：姓名 -> 学号 (原版逻辑限制了姓名不能重复)
unordered_map<string, string> name_to_id;

// ================== 防呆工具函数 ==================

// 防呆机制 1：安全清空输入缓冲区，防止输入字母导致无限死循环
void clear_input_buffer() {
    cin.clear(); // 清除 cin 的错误状态标志
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // 丢弃缓冲区里的残留字符
}

// 防呆机制 2：安全获取整数输入
int get_safe_int(const string& prompt) {
    int value;
    while (true) {
        cout << prompt;
        if (cin >> value) {
            return value;
        } else {
            cout << "【系统警告】输入无效！请输入纯数字。\n";
            clear_input_buffer();
        }
    }
}

// ================== 核心功能函数 ==================

void log_operation(const string& action) {
    ofstream log_file("caozuorizhi", ios::app);
    if (!log_file.is_open()) return;
    time_t now = time(0);
    char buf[80];
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", localtime(&now));
    log_file << buf << "  " << action << endl;
    log_file.close();
}

void print_menu() {
    cout << "\n===============================\n"
         << " 0. 安全退出系统\n"
         << " 1. 录入新生信息\n"
         << " 2. 按姓名删除学生\n"
         << " 3. 按学号删除学生\n"
         << " 4. 学科成绩全面统计\n"
         << " 5. 查看系统操作日志\n"
         << " 6. 按姓名查询信息\n"
         << " 7. 按学号查询信息\n"
         << "===============================\n";
}

void save_data() {
    ofstream out_file("xueshengxinxi");
    for (const auto& pair : db) {
        const Student& s = pair.second;
        out_file << s.name << endl << s.id << endl << s.gender << endl;
        for (const auto& score_pair : s.scores) {
            out_file << score_pair.first << endl << score_pair.second << endl;
        }
        out_file << "#" << endl; // 单个学生数据结束标志
    }
    out_file.close();
}

bool load_data() {
    ifstream in_file("xueshengxinxi");
    if (!in_file.is_open()) return false;

    string n, id, gender, subject;
    int score;
    while (in_file >> n >> id >> gender) {
        Student s;
        s.name = n;
        s.id = id;
        s.gender = gender;
        
        while (in_file >> subject && subject != "#") {
            in_file >> score;
            s.scores[subject] = score;
        }
        db[id] = s;
        name_to_id[n] = id;
    }
    in_file.close();
    return true;
}

void add_student() {
    string n, id, gender;
    
    // 防呆：检查姓名冲突
    cout << "请输入姓名：";
    cin >> n;
    while (name_to_id.count(n)) {
        cout << "【冲突】系统中已存在姓名为 '" << n << "' 的学生，请重新输入（或加上编号）：\n姓名：";
        cin >> n;
    }

    // 防呆：检查学号冲突
    cout << "请输入学号：";
    cin >> id;
    while (db.count(id)) {
        cout << "【冲突】学号 '" << id << "' 已被占用，请重新输入：\n学号：";
        cin >> id;
    }

    cout << "请输入性别：";
    cin >> gender;

    Student s{n, id, gender, {}};
    string log_info = "录入新生：姓名[" + n + "] 学号[" + id + "] 性别[" + gender + "]";

    int is_continue = get_safe_int("\n录入成绩按 1，暂不录入/停止录入按 0\n请输入：");
    while (is_continue != 0) {
        string subject;
        cout << "请输入学科名称：";
        cin >> subject;
        
        // 防呆：强制分数必须是有效的整数
        int score = get_safe_int("请输入 [" + subject + "] 的成绩：");
        
        s.scores[subject] = score;
        log_info += "  " + subject + ":" + to_string(score);
        
        is_continue = get_safe_int("继续录入成绩按 1，完成录入按 0\n请输入：");
    }

    db[id] = s;
    name_to_id[n] = id;
    save_data(); // 立即持久化
    log_operation(log_info);
    cout << "【成功】学生信息已保存！\n";
}

void process_delete_or_query(int choice) {
    bool is_query = (choice == 6 || choice == 7);
    bool is_name = (choice == 2 || choice == 6);
    string keyword;
    
    cout << "请输入要" << (is_query ? "查询" : "删除") << "的" << (is_name ? "姓名：" : "学号：");
    cin >> keyword;

    string target_id = "";
    if (is_name) {
        if (name_to_id.count(keyword)) target_id = name_to_id[keyword];
    } else {
        if (db.count(keyword)) target_id = keyword;
    }

    if (target_id == "") {
        cout << "【系统提示】未找到相关学生数据！\n";
        log_operation((is_query ? "查询失败：" : "删除失败：") + keyword + " (未找到)");
        return;
    }

    Student& s = db[target_id];

    if (!is_query) {
        // 执行删除
        string del_name = s.name;
        name_to_id.erase(del_name);
        db.erase(target_id);
        save_data(); // 立即持久化
        cout << "【成功】已彻底删除该学生及其成绩数据。\n";
        log_operation("删除学生：姓名[" + del_name + "] 学号[" + target_id + "]");
    } else {
        // 执行查询
        cout << "\n--- 个人信息面板 ---\n"
             << "姓名：" << s.name << "\n"
             << "学号：" << s.id << "\n"
             << "性别：" << s.gender << "\n"
             << "--------------------\n";
        if (s.scores.empty()) {
            cout << "【状态】暂无学科成绩记录。\n";
        } else {
            cout << "成绩单：\n";
            for (const auto& score_pair : s.scores) {
                cout << " > " << score_pair.first << " : " << score_pair.second << "\n";
            }
        }
        log_operation("查询学生：姓名[" + s.name + "] 学号[" + s.id + "]");
    }
}

void show_statistics() {
    if (db.empty()) {
        cout << "【系统提示】系统中暂无任何学生数据，无法统计。\n";
        log_operation("执行成绩统计失败：无数据");
        return;
    }

    // 结构体封装每个学科的统计信息
    struct SubjectStat {
        long long sum = 0;
        int count = 0;
        int max_score = -1;
        int min_score = 999999;
        string max_name;
        string max_id;
        string min_name;
        string min_id;
    };
    
    unordered_map<string, SubjectStat> stats;

    for (const auto& pair : db) {
        const Student& s = pair.second;
        for (const auto& score_pair : s.scores) {
            const string& subject = score_pair.first;
            int score = score_pair.second;
            
            SubjectStat& st = stats[subject];
            st.sum += score;
            st.count++;
            
            if (score > st.max_score) {
                st.max_score = score;
                st.max_name = s.name;
                st.max_id = s.id;
            }
            if (score < st.min_score) {
                st.min_score = score;
                st.min_name = s.name;
                st.min_id = s.id;
            }
        }
    }

    if (stats.empty()) {
        cout << "【系统提示】暂未录入任何成绩数据。\n";
        log_operation("执行成绩统计失败：无成绩数据");
        return;
    }

    cout << "\n======= 学科成绩总览 =======\n";
    for (const auto& pair : stats) {
        const string& subject = pair.first;
        const SubjectStat& st = pair.second;
        
        cout << "【" << subject << "】" << " (共 " << st.count << " 人参考)\n"
             << " > 最高分：" << setw(3) << st.max_score << " | 姓名：" << st.max_name << " (学号:" << st.max_id << ")\n"
             << " > 最低分：" << setw(3) << st.min_score << " | 姓名：" << st.min_name << " (学号:" << st.min_id << ")\n"
             << " > 平均分：" << fixed << setprecision(2) << (double)st.sum / st.count << "\n"
             << "----------------------------\n";
    }
    log_operation("执行了全系统学科成绩统计");
}

void read_logs() {
    ifstream log_file("caozuorizhi");
    if (!log_file.is_open()) {
        cout << "【系统提示】未找到日志文件或日志为空。\n";
        return;
    }
    
    string line;
    bool has_content = false;
    cout << "\n======= 系统操作日志 =======\n";
    while (getline(log_file, line)) {
        cout << line << endl;
        has_content = true;
    }
    cout << "============================\n";
    
    log_file.close();
    if (!has_content) {
        cout << "日志文件为空。\n";
    }
    log_operation("查阅了系统操作日志");
}

// ================== 主程序 ==================

int main() {
    cout << "正在初始化系统数据...\n";
    if (load_data()) {
        cout << "【启动成功】共加载了 " << db.size() << " 名学生的档案。\n";
        log_operation("系统安全启动，成功加载本地数据库");
    } else {
        cout << "【启动提示】未检测到本地历史数据，将创建全新数据库。\n";
        log_operation("系统初次启动，创建新数据库");
    }

    while (true) {
        print_menu();
        int choice = get_safe_int("请键入指令数字 (0-7)：");

        if (choice == 0) {
            cout << "正在安全退出系统... 再见！\n";
            log_operation("操作员安全退出系统");
            break;
        }

        switch (choice) {
            case 1: add_student(); break;
            case 2: // 删姓名
            case 3: // 删学号
            case 6: // 查姓名
            case 7: // 查学号
                process_delete_or_query(choice); 
                break;
            case 4: show_statistics(); break;
            case 5: read_logs(); break;
            default: 
                cout << "【警告】未知的指令！请输入 0 到 7 之间的数字。\n"; 
                break;
        }
    }
    return 0;
}
