#include "../exercise.h"
#include <cstring>
#include <memory>
#include <string>
#include <vector>

// READ: `std::unique_ptr` <https://zh.cppreference.com/w/cpp/memory/unique_ptr>

std::vector<std::string> RECORDS;

class Resource {
    std::string _records;

public:
    void record(char record) {
        _records.push_back(record);
    }

    ~Resource() {
        RECORDS.push_back(_records);
    }
};

using Unique = std::unique_ptr<Resource>;
Unique reset(Unique ptr) {
    if (ptr) ptr->record('r');
    return std::make_unique<Resource>();
}
Unique drop(Unique ptr) {
    if (ptr) ptr->record('d');
    return nullptr;
}
Unique forward(Unique ptr) {
    if (ptr) ptr->record('f');
    return ptr;
}

int main(int argc, char **argv) {
    std::vector<std::string> problems[3];

    drop(forward(reset(nullptr)));
    problems[0] = std::move(RECORDS);

    forward(drop(reset(forward(forward(reset(nullptr))))));
    problems[1] = std::move(RECORDS);

    /**
    测试用例 3：drop(drop(reset(drop(reset(reset(nullptr))))))
    执行流程：
        最内层reset(nullptr)：创建Resource A（无记录），返回A。
        reset(A)：A记录 'r'（_records = "r"），创建Resource B，A被销毁（RECORDS暂存 "r"），返回B。
        drop(B)：B记录 'd'（_records = "d"），B被销毁（RECORDS暂存 "r", "d"），返回nullptr。
        reset(nullptr)：创建Resource C（无记录），返回C。
        drop(C)：C记录 'd'（_records = "d"），C被销毁（RECORDS暂存 "r", "d", "d"），返回nullptr。
        外层drop(nullptr)：无操作。
    problems[2]结果：
        因多对象析构顺序差异，macOS 和 Ubuntu 下为{"d", "d", "r"}。
     */
    drop(drop(reset(drop(reset(reset(nullptr))))));
    problems[2] = std::move(RECORDS);

    // ---- 不要修改以上代码 ----

    std::vector<const char *> answers[]{
        {"fd"},
        {"d", "ffr"},
        {"d", "d", "r"}
    };

    // ---- 不要修改以下代码 ----

    for (auto i = 0; i < 3; ++i) {
        ASSERT(problems[i].size() == answers[i].size(), "wrong size");
        for (auto j = 0; j < problems[i].size(); ++j) {
            ASSERT(std::strcmp(problems[i][j].c_str(), answers[i][j]) == 0, "wrong location");
        }
    }

    return 0;
}