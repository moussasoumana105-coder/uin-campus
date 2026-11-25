#include <algorithm>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

// ---------- Saisie (chapitre 3) ----------

namespace {

struct EntierLu {
    bool valide{false};
    int valeur{0};
};

struct ReelLu {
    bool valide{false};
    double valeur{0.0};
};

std::string lireLigne(std::string_view invite)
{
    std::cout << invite;
    std::string ligne;
    std::getline(std::cin, ligne);
    return ligne;
}

[[nodiscard]] EntierLu versEntier(const std::string& texte)
{
    std::istringstream flux{texte};
    int valeur{};
    char reste{};
    const bool valide = (flux >> valeur) && !(flux >> reste);
    return {valide, valeur};
}

[[nodiscard]] ReelLu versReel(std::string texte)
{
    for (char& c : texte) {
        if (c == ',') {
            c = '.';
        }
    }
    std::istringstream flux{texte};
    double valeur{};
    char reste{};
    const bool valide = (flux >> valeur) && !(flux >> reste);
    return {valide, valeur};
}

} // namespace

int lireEntier(std::string_view invite, int min, int max)
{
    while (std::cin) {
        const auto [valide, valeur] = versEntier(lireLigne(invite));
        if (!valide) {
            std::cout << "   Saisie invalide : entrez un nombre entier.\n";
        } else if (valeur < min || valeur > max) {
            std::cout << "   La valeur doit être comprise entre " << min << " et " << max << ".\n";
        } else {
            return valeur;
        }
    }
    return min;
}

double lireNote(std::string_view invite)
{
    while (std::cin) {
        const auto [valide, note] = versReel(lireLigne(invite));
        if (!valide) {
            std::cout << "   Saisie invalide : entrez un nombre (ex. 12.5 ou 12,5).\n";
        } else if (note < 0.0 || note > 20.0) {
            std::cout << "   La note doit être comprise entre 0 et 20.\n";
        } else {
            return note;
        }
    }
    return 0.0;
}

// ---------- Carnet de notes ----------

namespace carnet {

enum class Ordre { Croissant, Decroissant };

struct Statistiques {
    double minimum{};
    double maximum{};
    double moyenne{};
    int nbValidees{};
};

// ---------- Calculs : aucune entrée-sortie ----------

/// @pre notes n'est pas vide
[[nodiscard]] Statistiques calculerStatistiques(const std::vector<double>& notes)
{
    const auto [mini, maxi] = std::ranges::minmax(notes);
    double somme{0.0};
    int validees{0};
    for (double n : notes) {
        somme += n;
        if (n >= 10.0) {
            ++validees;
        }
    }
    return {mini, maxi, somme / static_cast<double>(notes.size()), validees};
}

void appliquerBonus(std::vector<double>& notes, double bonus)
{
    std::ranges::for_each(notes, [bonus](double& n) { n = std::clamp(n + bonus, 0.0, 20.0); });
}

void trier(std::vector<double>& notes, Ordre ordre)
{
    if (ordre == Ordre::Croissant) {
        std::ranges::sort(notes);
    } else {
        std::ranges::sort(notes, [](double a, double b) { return a > b; });
    }
}

// ---------- Interface utilisateur ----------

void afficherNotes(const std::vector<double>& notes)
{
    if (notes.empty()) {
        std::cout << "Aucune note.\n";
        return;
    }
    std::cout << std::fixed << std::setprecision(2);
    for (std::size_t i{0}; i < notes.size(); ++i) {
        std::cout << std::setw(3) << i + 1 << ". " << std::setw(6) << notes[i] << '\n';
    }
}

void ajouter(std::vector<double>& notes)
{
    notes.push_back(lireNote("Note à ajouter : "));
}

void modifier(std::vector<double>& notes)
{
    if (notes.empty()) {
        std::cout << "Aucune note à modifier.\n";
        return;
    }
    afficherNotes(notes);
    const int rang = lireEntier("Numéro de la note : ", 1, static_cast<int>(notes.size()));
    notes[static_cast<std::size_t>(rang - 1)] = lireNote("Nouvelle valeur : ");
}

void supprimer(std::vector<double>& notes)
{
    if (notes.empty()) {
        std::cout << "Aucune note à supprimer.\n";
        return;
    }
    afficherNotes(notes);
    const int rang = lireEntier("Numéro de la note : ", 1, static_cast<int>(notes.size()));
    notes.erase(notes.begin() + (rang - 1));
}

void afficherStatistiques(const std::vector<double>& notes)
{
    if (notes.empty()) {
        std::cout << "Aucune note.\n";
        return;
    }
    const auto [mini, maxi, moyenne, validees] = calculerStatistiques(notes);
    std::cout << std::fixed << std::setprecision(2) << "Min " << mini << " | Max " << maxi
              << " | Moyenne " << moyenne << " | Validées " << validees << '/' << notes.size()
              << '\n';
}

[[nodiscard]] int afficherMenu()
{
    std::cout << "\n=== Carnet de notes ===\n";
    std::cout << "1. Ajouter une note\n2. Afficher les notes\n3. Modifier une note\n";
    std::cout << "4. Supprimer une note\n5. Trier les notes\n6. Statistiques\n";
    std::cout << "7. Appliquer un bonus\n0. Quitter\n";
    return lireEntier("Votre choix : ", 0, 7);
}

} // namespace carnet

int main()
{
    std::vector<double> notes;
    int choix{};
    do {
        choix = carnet::afficherMenu();
        switch (choix) {
        case 1: carnet::ajouter(notes); break;
        case 2: carnet::afficherNotes(notes); break;
        case 3: carnet::modifier(notes); break;
        case 4: carnet::supprimer(notes); break;
        case 5: {
            const int sens = lireEntier("1. Croissant     2. Décroissant : ", 1, 2);
            carnet::trier(notes, sens == 1 ? carnet::Ordre::Croissant : carnet::Ordre::Decroissant);
            break;
        }
        case 6: carnet::afficherStatistiques(notes); break;
        case 7: carnet::appliquerBonus(notes, lireNote("Bonus : ")); break;
        default: break;
        }
    } while (choix != 0);
}
