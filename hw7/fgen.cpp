#include <cstdio>
#include <array>
#include <cstring>
#include <cstddef>
#include <cstdint>
#include <cassert>

using namespace std;

int main() {
    static constinit array<char, size_t(1e5) + 2> fs{};

    FILE* f = fopen("f.in", "w");
    if (!f) {
        perror("fopen");
        return 1;
    }

    int32_t cnt = 7;
    fprintf(f, "%d\n", cnt);
    auto flush = [&]() {
        cnt--;
        assert(cnt >= 0);
        fs[fs.size() - 2] = '\n';
        fs[fs.size() - 1] = '\0';
        int ret = fputs(fs.data(), f);
        assert(ret >= 0);
    };

    fs.fill('0');
    flush();

    fs.fill('0');
    for (size_t i = 0; i < fs.size(); i += 2) {
        fs[i] = '1';
    }
    flush();

    fs.fill('0');
    for (size_t i = 1; i < fs.size(); i += 2) {
        fs[i] = '1';
    }
    flush();

    fs.fill('0');
    for (size_t i = 0; i < fs.size(); i += 3) {
        fs[i] = '1';
    }
    flush();

    fs.fill('0');
    for (size_t i = 1; i < fs.size(); i += 3) {
        fs[i] = '1';
    }
    flush();

    fs.fill('0');
    for (size_t i = 2; i < fs.size(); i += 3) {
        fs[i] = '1';
    }
    flush();

    fs.fill('1');
    flush();

    assert(cnt == 0);
}
