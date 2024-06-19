#include <algorithm>
#include <array>
#include <cassert>
#include <cstdint>
#include <iostream>
#include <numeric>
#include <ranges>
#include <vector>

using std::array, std::size_t, std::vector;

enum class Engine {
    kZero,
    kFirst,
    kSecond,
    kThird,
    kFourth,
};

using std::size_t, std::uint64_t, std::uint32_t, std::int32_t, std::int64_t, std::array;

#define TEST_NUMBER 7
inline constexpr uint32_t kTestNumber = TEST_NUMBER;

#if TEST_NUMBER == 1
inline constexpr size_t kMaxN              = 5;
inline constexpr size_t kMaxM              = 5;
inline constexpr size_t kMaxW              = 85;
inline constexpr array<int32_t, 5> p_array = {1, 2, 3, 4, 5};
#elif TEST_NUMBER == 2
inline constexpr size_t kMaxN              = 30;
inline constexpr size_t kMaxM              = 30;
inline constexpr size_t kMaxW              = 3413;
inline constexpr array<int32_t, 5> p_array = {1, 2, 3, 4, 5};
#elif TEST_NUMBER == 6
inline constexpr size_t kMaxN              = 3;
inline constexpr size_t kMaxM              = 3;
inline constexpr uint32_t kMaxW            = 7633;
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

    static constexpr State invalid_state() noexcept {
        State s;
        s.set_invalid();
        return s;
    }

    constexpr void operator()(size_t i, size_t j, Engine e) noexcept {
        if constexpr (kEnableStateAsserts) {
            assert(can_place_cell(i, j, e));
        }
        power_ -= p[map_[i][j]];
        if constexpr (kEnableStateAsserts) {
            assert(power_ >= 0);
        }
        map_[i][j] = e;
        power_ += p[e];
        if constexpr (kEnableStateAsserts) {
            assert(power_ >= kDefaultPower);
        }
    }

    [[nodiscard]] constexpr bool can_place_cell(size_t i, size_t j, Engine e) const noexcept {
        if constexpr (kEnableStateAsserts) {
            assert(is_in_correct_state());
            assert(Engine::kZero <= e && e <= Engine::kFourth);
        }
        return is_valid() && e <= max_cell_engine(i, j);
    }

    [[nodiscard]] constexpr auto power_if_place(size_t i, size_t j, Engine e) const noexcept {
        return can_place_cell(i, j, e) ? power_ - p[map_[i][j]] + p[e] : kInvalidPower;
    }

    [[nodiscard]] constexpr auto power() const noexcept {
        if constexpr (kEnableStateAsserts) {
            assert(is_in_correct_state());
        }
        return power_;
    }

    [[nodiscard]] constexpr const auto &map() const noexcept {
        if constexpr (kEnableStateAsserts) {
            assert(is_in_correct_state());
        }
        return map_;
    }
    [[nodiscard]] constexpr Engine at(size_t i, size_t j) const noexcept {
        if constexpr (kEnableStateAsserts) {
            assert(i < kMaxN);
            assert(j < kMaxM);
        }
        return map_[i][j];
    }
    [[nodiscard]] constexpr Engine at_or(size_t i, size_t j, Engine e) const noexcept {
        return i < kMaxN && j < kMaxM ? map_[i][j] : e;
    }

    [[nodiscard]] constexpr bool has_engine_at_true_if_out_of_bounds(size_t i, size_t j,
                                                                     Engine e) const noexcept {
        if constexpr (kEnableStateAsserts) {
            assert(is_in_correct_state());
            assert(is_valid());
        }
        return i >= kMaxN || j >= kMaxM || map_[i][j] == e;
    }

    constexpr void set_invalid() noexcept {
        if constexpr (kEnableStateAsserts) {
            assert(is_in_correct_state());
        }
        power_ = kInvalidPower;
    }

    constexpr void set_valid() noexcept {
        if constexpr (kEnableStateAsserts) {
            assert(is_in_correct_state());
        }
        power_ = map_real_power();
    }

    [[nodiscard]] constexpr bool is_valid() const noexcept {
        if constexpr (kEnableStateAsserts) {
            assert(is_in_correct_state());
            if (power_ != kInvalidPower) {
                assert(power_ >= kDefaultPower);
            }
        }
        return power_ != kInvalidPower;
    }

    [[nodiscard]] constexpr bool is_invalid() const noexcept {
        return !is_valid();
    }

    [[nodiscard]] constexpr Engine max_cell_engine(size_t i, size_t j) const noexcept {
        if constexpr (kEnableStateAsserts) {
            assert(i < kMaxN);
            assert(j < kMaxM);
        }
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
        const auto max_e = Engine(std::countr_one(mask));
        if constexpr (kEnableStateAsserts) {
            assert(Engine::kZero <= max_e && max_e <= Engine::kFourth);
        }
        if (max_e < Engine::kZero || Engine::kFourth < max_e) {
            __builtin_unreachable();
        }
        return max_e;
    }

private:
    static constexpr bool kEnableStateAsserts = false;

    [[nodiscard]] constexpr bool is_in_correct_state() const noexcept {
        return power_ == kInvalidPower || power_ == map_real_power();
    }
    [[nodiscard]] constexpr int64_t map_real_power() const noexcept {
        int64_t real_power = 0;
        for (const auto &row : map_) {
            for (const Engine e : row) {
                real_power += p[e];
            }
        }
        return real_power;
    }

    Map map_{};
    static_assert(std::is_same_v<decltype(map_), array<array<Engine, kMaxM>, kMaxN>>);
    static constexpr int64_t kDefaultPower = kMaxN * kMaxM * p[Engine::kZero];
    int64_t power_                         = kDefaultPower;
};

std::ostream &operator<<(std::ostream &out, const State &dp) {
    out << "power = " << dp.power() << '\n';
    for (const auto &row : dp.map()) {
        for (const Engine e : row) {
            out << uint32_t(e) << ' ';
        }
        out << '\n';
    }
    return out;
}

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
    //    if (setpriority(PRIO_PROCESS, 0, -20)) {
    //        perror("setpriority");
    //        return EXIT_FAILURE;
    //    }
    //    rlimit rl{};
    //    if (getrlimit(RLIMIT_STACK, &rl)) {
    //        perror("getrlimit");
    //        return EXIT_FAILURE;
    //    }
    //    rl.rlim_cur = std::max(rl.rlim_cur, rlim_t(256) << 20);
    //    if (setrlimit(RLIMIT_STACK, &rl)) {
    //        perror("setrlimit");
    //        return EXIT_FAILURE;
    //    }

    return EXIT_SUCCESS;
}

class Solver final {
public:
    Solver() noexcept {
        for (size_t i = 0; i < kMaxN; i++) {
            for (size_t j = 0; j < kMaxM; j++) {
                for (Engine e1 = Engine::kZero; e1 <= Engine::kFourth; ++e1) {
                    for (Engine e2 = Engine::kZero; e2 <= Engine::kFourth; ++e2) {
                        for (Engine e3 = Engine::kZero; e3 <= Engine::kFourth; ++e3) {
                            for (Engine e4 = Engine::kZero; e4 <= Engine::kFourth; ++e4) {
                                for (Engine e5 = Engine::kZero; e5 <= Engine::kFourth; ++e5) {
                                    auto &st = dp_current_[i][j][size_t(e1)][size_t(e2)][size_t(e3)]
                                                          [size_t(e4)][size_t(e5)];
                                    st.set_invalid();
                                }
                            }
                        }
                    }
                }

                for (Engine e5 = Engine::kZero; e5 <= Engine::kFirst; ++e5) {
                    auto &st =
                        dp_current_[i][j][size_t(Engine::kZero)][size_t(Engine::kZero)]
                                   [size_t(Engine::kZero)][size_t(Engine::kZero)][size_t(e5)];
                    assert(st.is_invalid());
                    st.set_valid();
                    st(i, j, e5);
                    assert(st.is_valid());
                }
            }
        }

        for (size_t i = 0; i < kMaxN; i++) {
            for (Engine any_e = Engine::kZero; any_e <= Engine::kFourth; ++any_e) {
                for (Engine e5 = Engine::kZero; e5 <= Engine::kFirst; ++e5) {
                    {
                        auto &st =
                            dp_current_[i][0][size_t(any_e)][size_t(Engine::kZero)]
                                       [size_t(Engine::kZero)][size_t(Engine::kZero)][size_t(e5)];
                        if (st.is_invalid()) {
                            st.set_valid();
                        }
                        st(i, 0, e5);
                        assert(st.is_valid());
                    }
                    {
                        auto &st =
                            dp_current_[i][kMaxM - 1][size_t(Engine::kZero)][size_t(Engine::kZero)]
                                       [size_t(any_e)][size_t(Engine::kZero)][size_t(e5)];
                        if (st.is_invalid()) {
                            st.set_valid();
                        }
                        st(i, kMaxM - 1, e5);
                        assert(st.is_valid());
                    }
                }
            }
        }
        for (size_t j = 0; j < kMaxM; j++) {
            for (Engine any_e = Engine::kZero; any_e <= Engine::kFourth; ++any_e) {
                for (Engine e5 = Engine::kZero; e5 <= Engine::kFirst; ++e5) {
                    {
                        auto &st =
                            dp_current_[0][j][size_t(Engine::kZero)][size_t(any_e)]
                                       [size_t(Engine::kZero)][size_t(Engine::kZero)][size_t(e5)];
                        if (st.is_invalid()) {
                            st.set_valid();
                        }
                        st(0, j, e5);
                        assert(st.is_valid());
                    }
                    {
                        auto &st =
                            dp_current_[kMaxN - 1][j][size_t(Engine::kZero)][size_t(Engine::kZero)]
                                       [size_t(Engine::kZero)][size_t(any_e)][size_t(e5)];
                        if (st.is_invalid()) {
                            st.set_valid();
                        }
                        st(kMaxN - 1, j, e5);
                        assert(st.is_valid());
                    }
                }
            }
        }

        update_max_seen_power(1);
    }

    std::pair<State, size_t> run() {
        constexpr std::tuple<int, int, int> actions[] = {
            {1, 1, 0}, {1, 2, 0}, {1, 3, 0}, {1, 4, 0}, {2, 1, 0}, {2, 2, 0}, {2, 3, 0}, {2, 4, 0},
            {3, 1, 0}, {3, 2, 0}, {3, 3, 0}, {3, 4, 0}, {4, 1, 0}, {4, 2, 0}, {4, 3, 0}, {4, 4, 0},
            {1, 3, 1}, {1, 4, 2}, {1, 2, 1}, {3, 4, 1}, {2, 2, 1}, {1, 1, 2}, {1, 3, 2}, {4, 2, 1},
            {4, 1, 1}, {4, 4, 2}, {2, 4, 2}, {2, 1, 2}, {2, 4, 3}, {2, 3, 3}, {2, 2, 4}, {3, 1, 3},
            {3, 2, 2}, {3, 3, 4}, {4, 2, 2}, {4, 2, 3},
        };
        State checker;
        assert((actions[0] == std::tuple<int, int, int>{0 + 1, 0 + 1, int(Engine::kZero)}));
        checker(0, 0, Engine::kZero);
        for (size_t dp_step = 2; dp_step <= kMaxSteps; dp_step++) {
            update_current_dp();
            update_max_seen_power(dp_step);
            std::cout << "dp_step = " << dp_step << '\n';

            static_assert(kMaxSteps - 1 < std::size(actions));
            const auto [ir, jr, er] = actions[dp_step - 1];
            const auto i            = size_t(uint32_t(ir) - 1);
            const auto j            = size_t(uint32_t(jr) - 1);
            const auto e            = Engine(er);
            checker(i, j, e);

            const Engine e1 = checker.at_or(i, j - 1, Engine::kZero);
            const Engine e2 = checker.at_or(i - 1, j, Engine::kZero);
            const Engine e3 = checker.at_or(i, j + 1, Engine::kZero);
            const Engine e4 = checker.at_or(i + 1, j, Engine::kZero);
            const Engine e5 = checker.at(i, j);
            const State &st =
                dp_current_[i][j][size_t(e1)][size_t(e2)][size_t(e3)][size_t(e4)][size_t(e5)];
            if (st.power() < checker.power()) {
                std::cout << "step = " << dp_step << " | i = " << i << " | j = " << j
                          << " | e1 = " << size_t(e1) << " | e2 = " << size_t(e2)
                          << " | e3 = " << size_t(e3) << " | e4 = " << size_t(e4)
                          << " | e5 = " << size_t(e5) << '\n'
                          << checker << '\n'
                          << st << std::endl;
                throw std::runtime_error("dp.power() < checker.power()");
            }
            // if (dp_step == 32) {
            //     const auto &dst = dp_current_[2][0][0][2][0][1][3];
            //     std::cout << "At dp_step " << dp_step << ":\n"
            //               << "(i, j) = (2, 0) | (e1, e2, e3, e4, e5) = (0, 2, 0, 1, 3)" << '\n'
            //               << dst
            //               << std::flush;
            //     assert(dst.has_engine_at_true_if_out_of_bounds(1, 0, Engine::kSecond));
            //     assert(dst.has_engine_at_true_if_out_of_bounds(2, 1, Engine::kZero));
            //     assert(dst.has_engine_at_true_if_out_of_bounds(3, 0, Engine::kFirst));
            //     assert(dst.has_engine_at_true_if_out_of_bounds(2, 0, Engine::kThird));
            //     continue;
            // }
        }

        return {state_with_max_seen_power, state_with_max_seen_power_step};
    }

private:
    using DPType =
        array<array<array<array<array<array<array<State, 5>, 5>, 5>, 5>, 5>, kMaxM>, kMaxN>;

    void update_current_dp() noexcept {
        for (size_t i = 0; i < kMaxN; i++) {
            for (size_t j = 0; j < kMaxM; j++) {
                for (Engine e1 = Engine::kZero; e1 <= Engine::kFourth; ++e1) {
                    for (Engine e2 = Engine::kZero; e2 <= Engine::kFourth; ++e2) {
                        for (Engine e3 = Engine::kZero; e3 <= Engine::kFourth; ++e3) {
                            for (Engine e4 = Engine::kZero; e4 <= Engine::kFourth; ++e4) {
                                for (Engine e5 = Engine::kZero; e5 <= Engine::kFourth; ++e5) {
                                    dp_next_[i][j][size_t(e1)][size_t(e2)][size_t(e3)][size_t(e4)]
                                            [size_t(e5)] =
                                                find_max_for_dp_next(i, j, e1, e2, e3, e4, e5);
                                }
                            }
                        }
                    }
                }
            }
        }

        for (size_t i = 0; i < kMaxN; i++) {
            for (size_t j = 0; j < kMaxM; j++) {
                assert(dp_next_[i][j][size_t(Engine::kZero)][size_t(Engine::kZero)]
                               [size_t(Engine::kZero)][size_t(Engine::kZero)][size_t(Engine::kZero)]
                                   .is_valid());
                assert(dp_next_[i][j][size_t(Engine::kZero)][size_t(Engine::kZero)][size_t(
                    Engine::kZero)][size_t(Engine::kZero)][size_t(Engine::kFirst)]
                           .is_valid());
            }
        }
        dp_current_ = dp_next_;
    }

    [[nodiscard]] constexpr State find_max_for_dp_next(size_t i, size_t j, Engine e1, Engine e2,
                                                       Engine e3, Engine e4,
                                                       Engine e5) const noexcept {
        int64_t max_power = State::kInvalidPower;
        size_t max_power_i{};
        size_t max_power_j{};
        Engine max_power_e1{};
        Engine max_power_e2{};
        Engine max_power_e3{};
        Engine max_power_e4{};
        Engine max_power_e5{};
        for (size_t i1 = 0; i1 < kMaxN; i1++) {
            for (size_t j1 = 0; j1 < kMaxM; j1++) {
                for (Engine e11 = Engine::kZero; e11 <= Engine::kFourth; ++e11) {
                    for (Engine e12 = Engine::kZero; e12 <= Engine::kFourth; ++e12) {
                        for (Engine e13 = Engine::kZero; e13 <= Engine::kFourth; ++e13) {
                            for (Engine e14 = Engine::kZero; e14 <= Engine::kFourth; ++e14) {
                                for (Engine e15 = Engine::kZero; e15 <= Engine::kFourth; ++e15) {
                                    const State &st1 =
                                        dp_current_[i1][j1][size_t(e11)][size_t(e12)][size_t(e13)]
                                                   [size_t(e14)][size_t(e15)];
                                    if (st1.is_invalid()) {
                                        continue;
                                    }
                                    assert(
                                        st1.has_engine_at_true_if_out_of_bounds(i1, j1 - 1, e11));
                                    assert(
                                        st1.has_engine_at_true_if_out_of_bounds(i1 - 1, j1, e12));
                                    assert(
                                        st1.has_engine_at_true_if_out_of_bounds(i1, j1 + 1, e13));
                                    assert(
                                        st1.has_engine_at_true_if_out_of_bounds(i1 + 1, j1, e14));
                                    assert(st1.has_engine_at_true_if_out_of_bounds(i1, j1, e15));

                                    if (st1.has_engine_at_true_if_out_of_bounds(i, j - 1, e1) &&
                                        st1.has_engine_at_true_if_out_of_bounds(i - 1, j, e2) &&
                                        st1.has_engine_at_true_if_out_of_bounds(i, j + 1, e3) &&
                                        st1.has_engine_at_true_if_out_of_bounds(i + 1, j, e4)) {
                                        const auto pip = st1.power_if_place(i, j, e5);
                                        if (pip > max_power) {
                                            max_power    = pip;
                                            max_power_i  = i1;
                                            max_power_j  = j1;
                                            max_power_e1 = e11;
                                            max_power_e2 = e12;
                                            max_power_e3 = e13;
                                            max_power_e4 = e14;
                                            max_power_e5 = e15;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        if (max_power == State::kInvalidPower) {
            return State::invalid_state();
        }

        auto state =
            dp_current_[max_power_i][max_power_j][size_t(max_power_e1)][size_t(max_power_e2)]
                       [size_t(max_power_e3)][size_t(max_power_e4)][size_t(max_power_e5)];
        assert(state.has_engine_at_true_if_out_of_bounds(i, j - 1, e1));
        assert(state.has_engine_at_true_if_out_of_bounds(i - 1, j, e2));
        assert(state.has_engine_at_true_if_out_of_bounds(i, j + 1, e3));
        assert(state.has_engine_at_true_if_out_of_bounds(i + 1, j, e4));
        state(state, i, j, e1, e2, e3, e4, );
        assert(state.power() == max_power);
        return state;
    }

    void update_max_seen_power(size_t step) noexcept {
        auto max_power = state_with_max_seen_power.power();
        size_t max_power_i{};
        size_t max_power_j{};
        Engine max_power_e1{};
        Engine max_power_e2{};
        Engine max_power_e3{};
        Engine max_power_e4{};
        Engine max_power_e5{};
        for (size_t i = 0; i < kMaxN; i++) {
            for (size_t j = 0; j < kMaxM; j++) {
                for (Engine e1 = Engine::kZero; e1 <= Engine::kFourth; ++e1) {
                    for (Engine e2 = Engine::kZero; e2 <= Engine::kFourth; ++e2) {
                        for (Engine e3 = Engine::kZero; e3 <= Engine::kFourth; ++e3) {
                            for (Engine e4 = Engine::kZero; e4 <= Engine::kFourth; ++e4) {
                                for (Engine e5 = Engine::kZero; e5 <= Engine::kFourth; ++e5) {
                                    const State &st =
                                        dp_current_[i][j][size_t(e1)][size_t(e2)][size_t(e3)]
                                                   [size_t(e4)][size_t(e5)];
                                    if (!st.is_valid()) {
                                        continue;
                                    }
                                    check_dp(i, j, e1, e2, e3, e4, e5);
                                    if (st.power() > max_power) {
                                        max_power    = st.power();
                                        max_power_i  = i;
                                        max_power_j  = j;
                                        max_power_e1 = e1;
                                        max_power_e2 = e2;
                                        max_power_e3 = e3;
                                        max_power_e4 = e4;
                                        max_power_e5 = e5;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        if (max_power > state_with_max_seen_power.power()) {
            std::cout << "Update max power " << state_with_max_seen_power.power() << " -> "
                      << max_power << " on step " << step << '\n';
            state_with_max_seen_power =
                dp_current_[max_power_i][max_power_j][size_t(max_power_e1)][size_t(max_power_e2)]
                           [size_t(max_power_e3)][size_t(max_power_e4)][size_t(max_power_e5)];
            assert(state_with_max_seen_power.power() == max_power);
            state_with_max_seen_power_step = step;
        }
    }

    void check_dp(size_t i, size_t j, Engine e1, Engine e2, Engine e3, Engine e4,
                  Engine e5) const noexcept {
        check_dp(dp_current_[i][j][size_t(e1)][size_t(e2)][size_t(e3)][size_t(e4)][size_t(e5)], i,
                 j, e1, e2, e3, e4, e5);
    }

    void check_dp([[maybe_unused]] const State &st, [[maybe_unused]] size_t i,
                  [[maybe_unused]] size_t j, [[maybe_unused]] Engine e1, [[maybe_unused]] Engine e2,
                  [[maybe_unused]] Engine e3, [[maybe_unused]] Engine e4,
                  [[maybe_unused]] Engine e5) const noexcept {
        assert(st.is_valid());
        assert(st.has_engine_at_true_if_out_of_bounds(i, j - 1, e1));
        assert(st.has_engine_at_true_if_out_of_bounds(i - 1, j, e2));
        assert(st.has_engine_at_true_if_out_of_bounds(i, j + 1, e3));
        assert(st.has_engine_at_true_if_out_of_bounds(i + 1, j, e4));
        assert(st.has_engine_at_true_if_out_of_bounds(i, j, e5));
    }

    static constexpr size_t kMaxSteps = 36;
    DPType dp_current_{};
    DPType dp_next_{};

    State state_with_max_seen_power{};
    size_t state_with_max_seen_power_step{};
};

int main() {
    if (int ret = setup(); ret) {
        return ret;
    }
    const auto [state, step] = Solver{}.run();
    std::cout << "step = " << step << '\n' << state << '\n';
}
