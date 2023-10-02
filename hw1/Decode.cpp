#include <iostream>
#include <cassert>
#include <cstdint>
#include <string>
#include <stack>
#include <string>
#include <vector>

static constexpr bool IsDigit(int32_t c) {
    return static_cast<uint32_t>(c) - '0' <= '9' - '0';
}

std::string Decode(std::string& input) {
    std::string s;
    std::vector<std::string> stack;
    std::vector<uint32_t> digits_stack;

    for (uint32_t i = 0; i < input.size(); i++) {
        int32_t c = input[i];
        if (IsDigit(c)) {
            stack.emplace_back(std::move(s));
            digits_stack.push_back(uint32_t(c) - '0');
            s.clear();
            i++;
            assert(input.at(i) == '[');
        }
        else if (c == ']') {
            assert(!stack.empty() && stack.size() == digits_stack.size());
            uint32_t digit = digits_stack.back();
            digits_stack.pop_back();

            s.reserve(s.size() * digit);
            std::string_view view(s);
            switch(digit) {
                case 0:
                    s.clear();
                    break;
                case 9:
                    s += view;
                    [[fallthrough]];
                case 8:
                    s += view;
                    [[fallthrough]];
                case 7:
                    s += view;
                    [[fallthrough]];
                case 6:
                    s += view;
                    [[fallthrough]];
                case 5:
                    s += view;
                    [[fallthrough]];
                case 4:
                    s += view;
                    [[fallthrough]];
                case 3:
                    s += view;
                    [[fallthrough]];
                case 2:
                    s += view;
                    break;
            }

            std::string s_prev = std::move(stack.back());
            stack.pop_back();
            s_prev += s;
            s = std::move(s_prev);
        }
        else {
            s += char(c);
        }
    }

    return s;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    std::string input;
    getline(std::cin, input);

    std::cout << Decode(input);
    std::cout.flush();
    return 0;
}
