#include "cheatengine/core/application.hpp"

#include <iostream>
#include <iomanip>

int main()
{
    cheatengine::Application app;
    (void)app;
    std::cout << "CheatEngine prototype initialized." << std::endl;

    std::cout << "Testing program..." << std::endl;

    auto sample = cheatengine::SearchValue::fromFloat32(3.14f);
    std::cout << "bytes: ";
    for (auto b : sample.data()) {
        std::cout << std::hex << std::setfill('0') << std::setw(2) << static_cast<int>(b) << ' ';
    }
    std::cout << std::dec << std::setfill(' ') << '\n';

    return 0;
}
