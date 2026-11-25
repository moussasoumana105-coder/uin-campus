#include "uincampus/calculs.hpp"
#include "uincampus/regles.hpp"

#include <iostream>
#include <vector>

int main()
{
    const std::vector<double> notes{12.0, 15.0};
    const double m = uin::arrondir(uin::moyenne(notes));
    std::cout << "Moyenne " << m << " : " << uin::libelle(uin::mentionPour(m)) << '\n';
}
