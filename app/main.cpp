#include "uincampus/calculs.hpp"
#include "uincampus/regles.hpp"
#include "uincampus/saisie.hpp"
#include "uincampus/version.hpp"

#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <span>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

namespace {

enum class Suite { Continuer, Terminer, Erreur };

void afficherAide()
{
    std::cout << "Usage : uin-campus [--version] [--aide]\n"
              << " --version    affiche la version\n"
              << " --aide       affiche cette aide\n";
}

[[nodiscard]] Suite traiterOptions(std::span<char*> arguments)
{
    if (arguments.size() <= 1) {
        return Suite::Continuer;
    }
    for (const std::string_view option : arguments.subspan(1)) {
        if (option == "--version") {
            std::cout << "uin-campus " << uin::version() << '\n';
            return Suite::Terminer;
        }
        if (option == "--aide") {
            afficherAide();
            return Suite::Terminer;
        }
        std::cerr << "Option inconnue : " << option << '\n';
        afficherAide();
        return Suite::Erreur;
    }
    return Suite::Continuer;
}

// Nombre de caractères UTF-8 (et non d'octets) d'un texte.
std::size_t largeurAffichee(std::string_view texte)
{
    std::size_t n{0};
    for (char c : texte) {
        if ((static_cast<unsigned char>(c) & 0xC0U) != 0x80U) {
            ++n;
        }
    }
    return n;
}

// Équivalent de std::format("{:<24}{:>20}") compatible avec les accents.
void afficherLigne(std::string_view libelle, std::string_view valeur)
{
    const std::size_t g = largeurAffichee(libelle);
    const std::size_t d = largeurAffichee(valeur);
    std::cout << libelle << std::string(g < 24 ? 24 - g : 0, ' ')
              << std::string(d < 20 ? 20 - d : 0, ' ') << valeur << '\n';
}

std::string deuxDecimales(double valeur)
{
    std::ostringstream flux;
    flux << std::fixed << std::setprecision(2) << valeur;
    return flux.str();
}

} // namespace

int main(int argc, char* argv[])
{
    switch (traiterOptions(std::span{argv, static_cast<std::size_t>(argc)})) {
    case Suite::Terminer:
        return EXIT_SUCCESS;
    case Suite::Erreur:
        return EXIT_FAILURE;
    case Suite::Continuer:
        break;
    }

    std::cout << "UIN-Campus " << uin::version() << " — Relevé d'un étudiant\n\n";

    const std::string nom = uin::lireTexte("Nom de l'étudiant : ");
    const int nbUE = uin::lireEntier("Nombre d'UE (1 à 30) : ", 1, 30);

    std::vector<double> notes;
    std::vector<double> coefficients;
    notes.reserve(static_cast<std::size_t>(nbUE));
    coefficients.reserve(static_cast<std::size_t>(nbUE));

    for (int i{1}; i <= nbUE; ++i) {
        notes.push_back(uin::lireNote(" Note de l'UE " + std::to_string(i) + " : "));
        coefficients.push_back(static_cast<double>(
            uin::lireEntier(" Coefficient de l'UE " + std::to_string(i) + " : ", 1, 10)));
    }

    const double moyenneGenerale = uin::arrondir(uin::moyenne(notes, coefficients));
    const auto [mini, maxi, moyenneSimple, ecartType] = uin::calculerStatistiques(notes);
    const int validees = uin::compterValidees(notes);

    std::cout << '\n' << std::string(44, '-') << '\n';
    afficherLigne("Étudiant", nom);
    afficherLigne("Moyenne pondérée", deuxDecimales(moyenneGenerale));
    afficherLigne("Moyenne simple", deuxDecimales(moyenneSimple));
    afficherLigne("Note min / max", deuxDecimales(mini) + " / " + deuxDecimales(maxi));
    afficherLigne("Écart-type", deuxDecimales(ecartType));
    afficherLigne("UE validées", std::to_string(validees) + " / " + std::to_string(nbUE));
    afficherLigne("Mention", uin::libelle(uin::mentionPour(moyenneGenerale)));
    std::cout << std::string(44, '-') << '\n';

    return EXIT_SUCCESS;
}
