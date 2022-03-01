#include <iostream>

extern "C" {
    double add(double, double);
}

int main(int argc, char *argv[])
{
    std::cout << add(3, 4) << std::endl;
    return 0;
}
