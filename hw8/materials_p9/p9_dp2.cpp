#include <algorithm>
#include <array>
#include <cassert>
#include <cstdint>
#include <iostream>
#include <numeric>
#include <vector>
#include <ranges>
#include <execution>

using std::array, std::size_t;

enum class Engine {
    kZero,
    kFirst,
    kSecond,
    kThird,
    kFourth,
};

using std::size_t, std::uint64_t, std::uint32_t, std::int32_t, std::int64_t, std::array;

#define TEST_NUMBER 1
inline constexpr uint32_t kTestNumber = TEST_NUMBER;

#if TEST_NUMBER == 1
inline constexpr size_t kMaxN = 5;
inline constexpr size_t kMaxM = 5;
inline constexpr size_t kMaxW = 85;
inline constexpr array<int32_t, 5> p_array = {1, 2, 3, 4, 5};
#elif TEST_NUMBER == 2
inline constexpr size_t kMaxN              = 30;
inline constexpr size_t kMaxM              = 30;
inline constexpr size_t kMaxW              = 3413;
inline constexpr array<int32_t, 5> p_array = {1, 2, 3, 4, 5};
#elif TEST_NUMBER == 6
inline constexpr size_t kMaxN = 3;
inline constexpr size_t kMaxM = 3;
inline constexpr uint32_t kMaxW = 7633;
inline constexpr array<int32_t, 5> p_array = {1, 9, 81, 729, 6561};
#elif TEST_NUMBER == 7
inline constexpr size_t kMaxN              = 4;
inline constexpr size_t kMaxM              = 4;
inline constexpr uint32_t kMaxW            = 149041;
inline constexpr array<int32_t, 5> p_array = {1, 16, 256, 4096, 65536};
#elif TEST_NUMBER == 8
inline constexpr size_t kMaxN              = 8;
inline constexpr size_t kMaxM              = 9;
inline constexpr size_t kMaxW              = 231012;
inline constexpr array<int32_t, 5> p_array = {1, 10, 100, 1000, 10000};
#elif TEST_NUMBER == 9;
inline constexpr size_t kMaxN              = 167;
inline constexpr size_t kMaxM              = 284;
inline constexpr size_t kMaxW              = 54813600;
inline constexpr array<int32_t, 5> p_array = {100, 250, 550, 1000, 2000};
#elif TEST_NUMBER == 10
inline constexpr size_t kMaxN              = 256;
inline constexpr size_t kMaxM              = 256;
inline constexpr size_t kMaxW              = 3732549;
inline constexpr array<int32_t, 5> p_array = {13, 21, 34, 55, 89};
#else
#error "Unknown TEST_NUMBER"
#endif

using Map = array<array<Engine, kMaxM>, kMaxN>;

constexpr Engine &operator--(Engine &e) noexcept {
    return e = static_cast<Engine>(static_cast<uint32_t>(e) - 1);
}

constexpr Engine &operator++(Engine &e) noexcept {
    return e = static_cast<Engine>(static_cast<uint32_t>(e) + 1);
}

inline constexpr struct {
    static constexpr auto operator[](Engine e) noexcept {
        static_assert(sizeof(e) == sizeof(uint32_t));
        return p_array[size_t(uint32_t(e))];
    }
} p;

struct State final {
    static constexpr int64_t kInvalidPower = -1;

    constexpr State &operator()(size_t i, size_t j, Engine e) noexcept {
        assert(is_in_correct_state());
        assert(can_place_cell(i, j, e));
        power_ -= p[map_[i][j]];
        map_[i][j] = e;
        power_ += p[e];
        return *this;
    }

    [[nodiscard]] constexpr bool can_place_cell(size_t i, size_t j, Engine e) const noexcept {
        assert(is_in_correct_state());
        assert(i < kMaxN);
        assert(j < kMaxM);
        return is_valid() && e <= max_cell_engine(i, j);
    }

    [[nodiscard]] constexpr auto power_if_place(size_t i, size_t j, Engine e) const noexcept {
        assert(is_in_correct_state());
        return can_place_cell(i, j, e) ? power_ - p[map_[i][j]] + p[e] : kInvalidPower;
    }

    [[nodiscard]] constexpr auto power() const noexcept {
        assert(is_in_correct_state());
        return power_;
    }

    [[nodiscard]] constexpr const auto &map() const noexcept {
        assert(is_in_correct_state());
        return map_;
    }

    constexpr void set_invalid() noexcept {
        assert(is_in_correct_state());
        power_ = kInvalidPower;
    }

    [[nodiscard]] constexpr bool is_valid() const noexcept {
        assert(is_in_correct_state());
        return power_ > 0;
    }

    [[nodiscard]] friend constexpr bool operator==(const State &lhs, const State &rhs) noexcept = default;

    [[nodiscard]] friend constexpr std::weak_ordering operator<=>(const State &lhs, const State &rhs) noexcept {
        return lhs.power_ <=> rhs.power_;
    }

    friend std::ostream &operator<<(std::ostream &out, const State &dp) {
        out << "power = " << dp.power() << '\n';
        for (const auto &row: dp.map_) {
            for (const Engine e: row) {
                out << uint32_t(e) << ' ';
            }
            out << '\n';
        }
        return out;
    }

    [[nodiscard]] constexpr Engine max_cell_engine(size_t i, size_t j) const noexcept {
        uint32_t mask = 0;
        if (i > 0) {
            mask |= 1u << uint32_t(map_[i - 1][j]);
        }
        if (i + 1 < kMaxN) {
            mask |= 1u << uint32_t(map_[i + 1][j]);
        }
        if (j > 0) {
            mask |= 1u << uint32_t(map_[i][j - 1]);
        }
        if (j + 1 < kMaxM) {
            mask |= 1u << uint32_t(map_[i][j + 1]);
        }
        return Engine(std::countr_one(mask));
    }

private:
    [[nodiscard]] constexpr bool is_in_correct_state() const noexcept {
        int64_t real_power = 0;
        for (const auto &row: map_) {
            for (const Engine e: row) {
                real_power += p[e];
            }
        }
        return power_ == kInvalidPower || power_ == real_power;
    }

    Map map_{};
    static_assert(std::is_same_v<decltype(map_), array<array<Engine, kMaxM>, kMaxN>>);
    static constexpr int64_t kDefaultPower = kMaxN * kMaxM * p[Engine::kZero];
    int64_t power_ = kDefaultPower;
};

static_assert(State().power());

#include <sys/resource.h>

[[nodiscard("c-style error code")]] static int setup() noexcept {
    // #define GET_OUTPUT_FILENAME(tn) "result_" #tn
    // #define OUTPUT_FILENAME(tn)     GET_OUTPUT_FILENAME(tn)
    //     if (freopen(OUTPUT_FILENAME(TEST_NUMBER), "a+", stdout) == nullptr) {
    //         perror("freopen");
    //         return EXIT_FAILURE;
    //     }
    // #undef OUTPUT_FILENAME
    // #undef GET_OUTPUT_FILENAME
    if (setpriority(PRIO_PROCESS, 0, -20)) {
        perror("setpriority");
        return EXIT_FAILURE;
    }
    // rlimit rl{};
    // if (getrlimit(RLIMIT_STACK, &rl)) {
    //     perror("getrlimit");
    //     return EXIT_FAILURE;
    // }
    // rl.rlim_cur = std::max(rl.rlim_cur, rlim_t(256) << 20);
    // if (setrlimit(RLIMIT_STACK, &rl)) {
    //     perror("setrlimit");
    //     return EXIT_FAILURE;
    // }

    return EXIT_SUCCESS;
}

constexpr uint64_t operator-(const timespec &t2, const timespec &t1) noexcept {
    const auto sec_passed = static_cast<uint64_t>(t2.tv_sec - t1.tv_sec);
    auto nanoseconds_passed = sec_passed * 1'000'000'000;
    using unsigned_nanoseconds_t = std::make_unsigned_t<decltype(timespec::tv_nsec)>;
    nanoseconds_passed += static_cast<unsigned_nanoseconds_t>(t2.tv_nsec);
    nanoseconds_passed -= static_cast<unsigned_nanoseconds_t>(t1.tv_nsec);
    return nanoseconds_passed;
}

inline constexpr size_t kMaxSteps = 62;
inline constexpr size_t kMaxStates = size_t(5e9) / (kMaxN * kMaxM * kMaxSteps * 5);

static int64_t solve() {
    std::vector<State> current_states;
    std::vector<State> next_states;
    current_states.reserve(kMaxStates);
    next_states.reserve(kMaxStates);
    current_states.emplace_back();

    for (size_t rev_step = kMaxSteps; rev_step > 0; rev_step--) {
        for (const State &st: current_states) {
            for (size_t i = 0; i < kMaxN; i++) {
                for (size_t j = 0; j < kMaxM; j++) {
                    const Engine max_engine = st.max_cell_engine(i, j);
                    assert(Engine::kZero <= max_engine && max_engine <= Engine::kFourth);
                    if (max_engine < Engine::kZero || max_engine > Engine::kFourth) {
                        __builtin_unreachable();
                    }
                    for (Engine e = Engine::kZero; e <= max_engine; ++e) {
                        if (st.map()[i][j] != e) {
                            next_states.emplace_back(st)(i, j, e);
                        }
                    }
                }
            }
        }

        if (next_states.size() > kMaxStates) {
            std::partial_sort(
                    std::execution::par,
                    next_states.begin(),
                    next_states.begin() + static_cast<std::ptrdiff_t>(std::min(kMaxStates, next_states.size())),
                    next_states.end(),
                    std::greater{});
            next_states.resize(kMaxStates);
        } else {
            std::sort(next_states.begin(), next_states.end(), std::greater{});
        }

        current_states.swap(next_states);
        next_states.clear();
    }

    return current_states.front().power();
}

int main() {
    if (int ret = setup(); ret) {
        return ret;
    }

    std::cout << solve() << '\n';
}
