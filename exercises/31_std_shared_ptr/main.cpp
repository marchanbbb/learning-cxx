#include "../exercise.h"
#include <memory>

// READ: `std::shared_ptr` <https://zh.cppreference.com/w/cpp/memory/shared_ptr>
// READ: `std::weak_ptr` <https://zh.cppreference.com/w/cpp/memory/weak_ptr>

// TODO: 将下列 `?` 替换为正确的值
int main(int argc, char **argv) {
    auto shared = std::make_shared<int>(10);
    std::shared_ptr<int> ptrs[]{shared, shared, shared};

    std::weak_ptr<int> observer = shared;
    ASSERT(observer.use_count() == 4, "初始时shared_ptr有4个：shared和ptrs中的3个");

    ptrs[0].reset();
    ASSERT(observer.use_count() == 3, "释放ptrs[0]后，剩余3个shared_ptr");

    ptrs[1] = nullptr;
    ASSERT(observer.use_count() == 2, "释放ptrs[1]后，剩余2个shared_ptr");

    ptrs[2] = std::make_shared<int>(*shared);
    ASSERT(observer.use_count() == 1, "ptrs[2]指向新对象后，原对象只剩shared");

    ptrs[0] = shared;
    ptrs[1] = shared;
    ptrs[2] = std::move(shared);
    ASSERT(observer.use_count() == 3, "ptrs[0]、ptrs[1]和ptrs[2]指向原对象，shared为空");

    std::ignore = std::move(ptrs[0]);
    ptrs[1] = std::move(ptrs[1]);
    ptrs[1] = std::move(ptrs[2]);
    ASSERT(observer.use_count() == 2, "ptrs[1]和ptrs[2]指向原对象");

    shared = observer.lock();
    ASSERT(observer.use_count() == 3, "shared和ptrs[1]和ptrs[2]指向原对象");

    shared = nullptr;
    for (auto &ptr : ptrs) ptr = nullptr;
    ASSERT(observer.use_count() == 0, "所有shared_ptr被释放，对象已销毁");

    shared = observer.lock();
    ASSERT(observer.use_count() == 0, "对象已销毁，use_count为0");

    return 0;
}