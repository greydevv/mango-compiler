#include <iostream>

extern "C" {
    int add(int, int);
}

int main(int argc, char *argv[])
{
    std::cout << add(3, 4) << std::endl;
    return 0;
}
