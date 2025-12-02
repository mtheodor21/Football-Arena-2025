#include <iostream>
#include <vector>
#include <fstream> // Necesara pentru lucrul cu fisiere

// CORECTIE: Includem fisierele folosind calea exacta si numele cu litere mici
// asa cum apar in structura ta de foldere (Persoane/persoane.h etc.)
#include "Exceptii/Exceptii.h"   // Presupunand ca exista folderul Exceptii
#include "Persoane/persoane.h"   // Numele fisierului este cu litera mica in poza
#include "Persoane/jucator.h"
#include "Persoane/antrenor.h"
#include "Persoane/arbitru.h"
#include "Club/Club.h"           // Presupunand ca exista folderul Club

// ============================================================================
// CERINTA EXTRA: Clasa Medic definita local pentru a demonstra extensibilitatea
// ============================================================================
class Medic : public Persoana {
    std::string specializare;
public:
    Medic(const std::string& nume, const std::string& prenume, const std::string& spec)
        : Persoana(nume, prenume), specializare(spec) {}

    Medic* clone() const override { return new Medic(*this); }

    int calculeazaEficienta() const override {
        return 100;
    }

protected:
    void afisareDetaliata(std::ostream& os) const override {
        os << "Rol: STAFF MEDICAL\n";
        os << "Specializare: " << specializare << "\n";
    }
};

int main() {
    try {
        std::cout << "=== PORNIRE SIMULARE CLUB DE FOTBAL ===\n\n";

        Club club("FC Politehnica");

        // 1. Deschidere fisier date
        std::ifstream fin("tastatura.txt");

        if (!fin.is_open()) {
            // Daca nu exista fisierul, cream unul demonstrativ pentru a nu crash-ui
            std::cout << "[INFO] Fisierul 'tastatura.txt' nu a fost gasit. Il cream acum...\n";
            std::ofstream fout("tastatura.txt");
            fout << "JUCATOR Gica Hagi 94 CAM 10\n";
            fout << "JUCATOR Cristi Chivu 88 CB 5\n";
            fout << "ANTRENOR Mircea Lucescu 40 35\n";
            fout << "ARBITRU Ion Craciunescu 200 1\n";
            fout << "MEDIC Ionut Pavel Ortopedie\n";
            fout.close();

            // Re-deschidem pentru citire
            fin.open("tastatura.txt");
        }

        std::cout << ">> Se incarca datele din fisierul 'tastatura.txt'...\n";

        std::string tip;
        while (fin >> tip) {
            // Citim atributele comune pentru orice Persoana
            std::string nume, prenume;
            fin >> nume >> prenume;

            if (tip == "JUCATOR") {
                int rating, numar;
                std::string pozitie;
                // Ordinea din fisier: Rating Pozitie Numar
                fin >> rating >> pozitie >> numar;
                club.adaugaMembru(Jucator(nume, prenume, rating, pozitie, numar));
            }
            else if (tip == "ANTRENOR") {
                int ani, trofee;
                fin >> ani >> trofee;
                club.adaugaMembru(Antrenor(nume, prenume, ani, trofee));
            }
            else if (tip == "ARBITRU") {
                int meciuri;
                bool fifa;
                fin >> meciuri >> fifa;
                club.adaugaMembru(Arbitru(nume, prenume, meciuri, fifa));
            }
            else if (tip == "MEDIC") {
                std::string spec;
                fin >> spec;
                club.adaugaMembru(Medic(nume, prenume, spec));
            }
            else {
                std::cout << "[INFO] Tip necunoscut in fisier: " << tip << ". Se ignora linia.\n";
                // Ignoram restul liniei
                std::string dummy;
                std::getline(fin, dummy);
            }
        }
        fin.close();
        std::cout << ">> Incarcare finalizata.\n";

        // 2. Afisare Date Incarcate
        club.afiseazaMembri();

        // 3. Analiza RTTI (Dynamic Cast)
        club.analizeazaEchipa();

        // 4. Testare Copy-Constructor (Deep Copy)
        std::cout << "\n>> Testare Copy-Constructor (Deep Copy)...\n";
        Club clubCopie = club;

        std::cout << ">> Modificam clubul original (stergem tot si adaugam un junior)...\n";
        // Nota: Pentru a demonstra independenta, ar fi ideal sa avem o metoda clear() sau remove,
        // dar momentan doar adaugam ceva ce nu va fi in copie.
        club.adaugaMembru(Jucator("Junior", "Talentat", 60, "RW", 99));

        std::cout << "\n--- Afisare Club Copie (Ar trebui sa aiba datele initiale, FARA Junior) ---\n";
        clubCopie.afiseazaMembri();

        std::cout << "\n--- Afisare Club Original (Cu Junior) ---\n";
        club.afiseazaMembri(); // Aici va aparea Juniorul

        // 5. Testare Exceptii (Date Invalide)
        std::cout << "\n>> Testare Exceptii (Date Invalide)...\n";
        try {
            std::cout << "Incercam sa cream un jucator invalid manual...\n";
            Jucator jGresit("Test", "Eroare", -50, "GK", 1);
        } catch (const EroareDateInvalide& e) {
            std::cout << "[PRINS] Exceptie specifica: " << e.what() << "\n";
        }

        std::cout << "\n>> Statistica finala:\n";
        std::cout << "Numar total obiecte Persoana active in memorie: "
                  << Persoana::getNumarPersoane() << "\n";

    } catch (const std::exception& e) {
        std::cerr << "Eroare critica: " << e.what() << "\n";
    }

    std::cout << "\n=== SIMULARE INCHEIATA ===\n";
    return 0;
}