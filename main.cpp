#include "common.h"

// mkdir build && cd build && cmake .. && cmake --build . --config Release

namespace acme
{
    auto test(std::span<char const * const> args) -> std::optional<std::string>;

}


auto main(int argc, char const * const argv) -> int
{
    auto const args{std::span{argv, argv + argc}};

    if (auto res = acme::test(args))
    {
        print("{}\n", res.value());
    }

    return 0;
}

