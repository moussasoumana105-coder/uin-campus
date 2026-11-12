#include "uincampus/version.hpp"
#include <iostream>

int main()
{
    std::cout << "UIN-Campus — Système de gestion académique\n";
    std::cout << "Version " << uin::version() << "\n";
}
