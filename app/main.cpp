#include "uincampus/regles.hpp"
#include "uincampus/saisie.hpp"
#include "uincampus/version.hpp"

#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

namespace {

// Affiche « libellé ... valeur » sur 40 colonnes (compte les caractères UTF-8).
void ligne(std::string_view etiquette, const std::string& valeur)
{
    std::size_t n{0};
    for (char c : etiquette) {
        if ((static_cast<unsigned char>(c) & 0xC0U) != 0x80U) {
            ++n;
        }
    }
    for (char c : valeur) {
        if ((static_cast<unsigned char>(c) & 0xC0U) != 0x80U) {
            ++n;
        }
    }
    const std::size_t vides = n < 40 ? 40 - n : 1;
    std::cout << etiquette << std::string(vides, ' ') << valeur << '\n';
}

} // namespace

int main()
{
    std::cout << "UIN-Campus " << uin::version() << " — Calcul de la moyenne d'un étudiant\n\n";

    const std::string nom = uin::lireTexte("Nom de l'étudiant : ");
    const int nbNotes = uin::lireEntier("Nombre de notes (1 à 30) : ", 1, 30);

    std::vector<double> notes;
    for (int i{1}; i <= nbNotes; ++i) {
        notes.push_back(uin::lireNote("   Note " + std::to_string(i) + " : "));
    }

    double somme{0.0};
    for (double n : notes) {
        somme += n;
    }
    const double moyenne = somme / static_cast<double>(notes.size());
    const uin::Mention mention = uin::mentionPour(moyenne);

    std::ostringstream moy;
    moy << std::fixed << std::setprecision(2) << moyenne;

    std::cout << '\n' << std::string(40, '-') << '\n';
    ligne("Étudiant", nom);
    ligne("Nombre de notes", std::to_string(notes.size()));
    ligne("Moyenne", moy.str());
    ligne("Mention", std::string{uin::libelle(mention)});
    ligne("Décision", moyenne >= uin::seuilValidation ? "Validé" : "Non validé");
    std::cout << std::string(40, '-') << '\n';
}
