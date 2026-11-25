#include <iomanip>
#include <iostream>
#include <vector>

int main()
{
    std::vector<double> notes;
    for (int i{0}; i < 10; ++i) {
        notes.push_back(10.0 + i);
        std::cout << "taille " << std::setw(2) << notes.size() << ", capacité " << std::setw(2)
                  << notes.capacity() << '\n';
    }
    std::vector<double> copie = notes; // copie profonde, correcte
}
