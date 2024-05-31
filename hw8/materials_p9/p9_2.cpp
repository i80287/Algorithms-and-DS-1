#include <algorithm>
#include <array>
#include <bit>
#include <cassert>
#include <cmath>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <valarray>
#include <vector>
#include "config_macros.hpp"

using namespace std;

int main() {
    constexpr uint64_t n           = 256;
    constexpr uint64_t m           = 256;
    constexpr uint64_t N = n * m;
    constexpr uint64_t w           = 3732549;
    constexpr array<uint32_t, 5> p = {13, 21, 34, 55, 89};

    constexpr auto updiv = [](int64_t a, uint32_t b) constexpr noexcept {
        return (a + b - 1) / b;
    };

    int64_t ans = 0;

    constexpr uint64_t lowerbound4 = uint64_t(max(0l, updiv(int64_t(w - N * p[3]), p[4] - p[3])));
    constexpr uint64_t upperbound4 = min(N, w / p[4]);
    for (uint64_t k4 = lowerbound4; k4 <= upperbound4; k4++) {
        const uint64_t s4 = k4 * p[4];
        assert(s4 <= w);
        ATTRIBUTE_ASSUME(s4 <= w);

        const uint64_t lowebound3 = uint64_t(max(0l, updiv(int64_t(w - s4 - (N - k4) * p[2]), p[3] - p[2])));
        const uint64_t upperbound3 = min(N - k4, (w - s4) / p[3]);
        for (uint64_t k3 = lowebound3; k3 <= upperbound3; k3++) {
            const uint64_t s3 = s4 + k3 * p[3];
            assert(s3 <= w);
            ATTRIBUTE_ASSUME(s3 <= w);

            const uint64_t lowerbound2 = max(k3 / 2, uint64_t(max(0l, updiv(int64_t(w - s3 - (N - (k4 + k3)) * p[1]), p[2] - p[1]))));
            const uint64_t upperbound2 = min(N - (k4 + k3), (w - s3) / p[2]);
            for (uint64_t k2 = lowerbound2; k2 <= upperbound2; k2++) {
                const uint64_t s2 = s3 + k2 * p[2];
                assert(s2 <= w);
                ATTRIBUTE_ASSUME(s2 <= w);

                const uint64_t lowerbound1 = max(k2 / 8, uint64_t(max(0l, updiv(int64_t(w - s2 - (N - (k4 + k3 + k2)) * p[0]), p[1] - p[0]))));
                const uint64_t upperbound1 = min(N - (k4 + k3 + k2), (w - s2) / p[1]);
                for (uint64_t k1 = lowerbound1; k1 <= upperbound1; k1++) {
                    const uint64_t s1 = s2 + k1 * p[1];
                    assert(s1 <= w);
                    ATTRIBUTE_ASSUME(s1 <= w);

                    const uint64_t k0 = N - (k4 + k3 + k2 + k1);
                    const uint64_t s0 = s1 + k0 * p[0];
                    if (s0 == w) [[unlikely]] {
                        printf("%lu %lu %lu %lu %lu\n", k0, k1, k2, k3, k4);
                    }
                }
            }
        }
    }

    cout << ans;
    // for (uint64_t k0 = 1; k0 <= nm; k0++) {
    //     if (k0 * p[0] > w) [[unlikely]] {
    //         break;
    //     }
    //     for (uint64_t k1 = 0; k1 <= nm - k0; k1++) {
    //         if (k0 * p[0] + k1 * p[1] > w) [[unlikely]] {
    //             break;
    //         }
    //         for (uint64_t k2 = 0; k2 <= nm - (k0 + k1); k2++) {
    //             if (k0 * p[0] + k1 * p[1] + k2 * p[2] > w) [[unlikely]] {
    //                 break;
    //             }
    //             for (uint64_t k3 = 0; k3 <= nm - (k0 + k1 + k2); k3++) {
    //                 if (k0 * p[0] + k1 * p[1] + k2 * p[2] + k3 * p[3] > w) [[unlikely]] {
    //                     break;
    //                 }
    //                 assert(k0 + k1 + k2 + k3 <= nm);
    //                 const uint64_t k4 = nm - (k0 + k1 + k2 + k3);
    //                 const uint64_t ww = k0 * p[0] + k1 * p[1] + k2 * p[2] + k3 * p[3] + k4 * p[4];
    //                 if (ww == w) [[unlikely]] {
    //                     printf("%lu %lu %lu %lu %lu\n", k0, k1, k2, k3, k4);
    //                 }
    //             }
    //         }
    //     }
    // }
}
