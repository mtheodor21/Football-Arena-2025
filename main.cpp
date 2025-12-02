#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>

// Includem headerele conform structurii tale de fisiere (cu litere mici)
#include "Exceptii/Exceptii.h"
#include "Persoane/persoane.h"
#include "Persoane/jucator.h"
#include "Persoane/antrenor.h"
#include "Persoane/arbitru.h"
#include "Club/Club.h"

// ============================================================================
// CERINTA EXTRA: Clasa Medic definita local pentru a demonstra extensibilitatea
// fara a modifica fisierele existente ale proiectului.
// ============================================================================
class Medic : public Persoana {
    std::string specializare;
public:
    Medic(const std::string& nume, const std::string& prenume, const std::string& spec)
        : Persoana(nume, prenume), specializare(spec) {}

    // Constructorul virtual (Clone) - esential pentru polimorfismul din Club
    Medic* clone() const override { return new Medic(*this); }

    // Metoda virtuala pura din baza
    int calculeazaEficienta() const override {
        // Eficienta unui medic este vitala (default 100)
        return 100;
    }

protected:
    // Afisare prin NVI (Non-Virtual Interface)
    void afisareDetaliata(std::ostream& os) const override {
        os << "Rol: STAFF MEDICAL\n";
        os << "Specializare: " << specializare << "\n";
    }
};

int main() {
    try {
        std::cout << "=== PORNIRE SIMULARE CLUB DE FOTBAL (Date Hardcodate) ===\n\n";

        // 1. Creare Club (Containerul principal)
        Club club("FC Politehnica");

        // 2. Creare obiecte (instantiere directa)
        std::cout << ">> Creare si adaugare membri in club...\n";

        Jucator j1("Gica", "Hagi", 94, "CAM", 10);
        Jucator j2("Cristi", "Chivu", 88, "CB", 5);
        Antrenor a1("Mircea", "Lucescu", 40, 35); // 40 ani exp, 35 trofee
        Arbitru arb("Ion", "Craciunescu", 200, true); // 200 meciuri, ecuson FIFA

        // 3. Adaugare in club
        // Se demonstreaza Polimorfismul: metoda primeste 'Persoana&' dar stocheaza copii ale derivatelor
        club.adaugaMembru(j1);
        club.adaugaMembru(j2);
        club.adaugaMembru(a1);
        club.adaugaMembru(arb);

        // 4. Testare Cerinta Extra (Medic)
        // Adaugam o clasa definita local, necunoscuta de 'Club' la momentul compilarii lui
        Medic doc("Ionut", "Pavel", "Ortopedie");
        club.adaugaMembru(doc);

        // 5. Afisare Membri
        // Se foloseste mecanismul de functii virtuale pentru afisarea corecta a fiecarui tip
        club.afiseazaMembri();

        // 6. Analiza RTTI (Dynamic Cast)
        // Identifica tipurile reale ale obiectelor stocate
        club.analizeazaEchipa();

        // 7. Testare Copy-Constructor (Deep Copy)
        std::cout << "\n>> Testare Copy-Constructor (Deep Copy)...\n";
        {
            Club clubCopie = club; // Aici se apeleaza constructorul de copiere

            std::cout << ">> Modificam clubul original (adaugam un junior)...\n";
            club.adaugaMembru(Jucator("Junior", "Talentat", 60, "RW", 99));

            std::cout << "\n--- Afisare Club Copie (Trebuie sa fie FARA Junior) ---\n";
            clubCopie.afiseazaMembri();
        }
        // Aici clubCopie iese din scope si se apeleaza destructorul, stergand memoria clonelor sale.
        // Daca deep copy nu ar fi corect, am avea erori (double free) aici sau date corupte.

        std::cout << "\n--- Afisare Club Original (Cu Junior) ---\n";
        club.afiseazaMembri();

        // 8. Testare Exceptii (Date Invalide)
        std::cout << "\n>> Testare Exceptii (Date Invalide)...\n";
        try {
            std::cout << "Incercam sa cream un jucator invalid (rating negativ)...\n";
            Jucator jGresit("Test", "Eroare", -50, "GK", 1);
        } catch (const EroareDateInvalide& e) {
            std::cout << "[PRINS] Exceptie specifica: " << e.what() << "\n";
        }

        // 9. Statistica Static
        std::cout << "\n>> Statistica finala:\n";
        std::cout << "Numar total obiecte Persoana active in memorie: "
                  << Persoana::getNumarPersoane() << "\n";

    } catch (const std::exception& e) {
        std::cerr << "Eroare critica in main: " << e.what() << "\n";
    }

    std::cout << "\n=== SIMULARE INCHEIATA ===\n";
    return 0;
}