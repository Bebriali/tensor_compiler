#include <iostream>
#include <utility>
#include <vector>

int main() {
    int x = 5;
    int y = std::move(x);
    std::cout << "It works!" << std::endl;
    return 0;
}
