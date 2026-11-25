#include "uincampus/calculs.hpp"

#include <algorithm>
#include <cassert>
#include <cmath>
#include <numeric>

namespace uin {

double somme(std::span<const double> valeurs)
{
    return std::accumulate(valeurs.begin(), valeurs.end(), 0.0);
}

double moyenne(std::span<const double> notes)
{
    assert(!notes.empty() && "moyenne() : aucune note");
    return somme(notes) / static_cast<double>(notes.size());
}

double moyenne(std::span<const double> notes, std::span<const double> coefficients)
{
    assert(!notes.empty() && "moyenne() : aucune note");
    assert(notes.size() == coefficients.size() && "moyenne() : tailles différentes");
    double total{0.0};
    for (std::size_t i{0}; i < notes.size(); ++i) {
        total += notes[i] * coefficients[i];
    }
    const double sommeCoefficients = somme(coefficients);
    assert(sommeCoefficients > 0.0 && "moyenne() : somme des coefficients nulle");
    return total / sommeCoefficients;
}

Statistiques calculerStatistiques(std::span<const double> notes)
{
    assert(!notes.empty() && "calculerStatistiques() : aucune note");
    const auto [mini, maxi] = std::minmax_element(notes.begin(), notes.end());
    const double moy = moyenne(notes);
    double sommeEcarts{0.0};
    for (double n : notes) {
        sommeEcarts += (n - moy) * (n - moy);
    }
    const double ecart = std::sqrt(sommeEcarts / static_cast<double>(notes.size()));
    return Statistiques{*mini, *maxi, moy, ecart};
}

int compterValidees(std::span<const double> notes, double seuil)
{
    return static_cast<int>(
        std::count_if(notes.begin(), notes.end(), [seuil](double n) { return n >= seuil; }));
}

double arrondir(double valeur, int decimales)
{
    const double facteur = std::pow(10.0, decimales);
    return std::round(valeur * facteur) / facteur;
}

} // namespace uin
