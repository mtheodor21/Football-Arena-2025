#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <stdexcept>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <chrono>
#include <algorithm> // Pentru sortare clasament
#include <iomanip>   // Pentru formatare tabel

// Includem headerele proiectului
#include "Exceptii/exceptii.h"
#include "Persoane/persoane.h"
#include "Persoane/jucator.h"
#include "Persoane/antrenor.h"
#include "Persoane/arbitru.h"
#include "Club/club.h"

// ============================================================================
// CERINTA EXTRA: Clasa Medic definita local
// ============================================================================
class Medic : public Persoana {
    std::string specializare;
public:
    Medic(const std::string& nume, const std::string& prenume, const std::string& spec)
        : Persoana(nume, prenume), specializare(spec) {}

    Medic* clone() const override { return new Medic(*this); }

    int calculeazaEficienta() const override { return 100; }

protected:
    void afisareDetaliata(std::ostream& os) const override {
        os << "Rol: STAFF MEDICAL\n";
        os << "Specializare: " << specializare << "\n";
    }
};

// ============================================================================
// STRUCTURI PENTRU LOGICA AVANSATA DE JOC
// ============================================================================

struct EchipaCampionat {
    std::string nume;
    int putere;
    int puncte;
    int goluriMarcate;
    int goluriPrimite;

    int golaveraj() const { return goluriMarcate - goluriPrimite; }
};

// Functie de sortare pentru clasament
bool comparaEchipe(const EchipaCampionat& a, const EchipaCampionat& b) {
    if (a.puncte != b.puncte) return a.puncte > b.puncte;
    return a.golaveraj() > b.golaveraj();
}

// Vector global (sau pasat prin referinta) pentru lotul de jucatori
// Avem nevoie de el aici pentru ca din clasa Club nu putem extrage jucatorii (incapsulare)
std::vector<Jucator> lotJucatori;
std::vector<Jucator*> primul11;
std::vector<EchipaCampionat> liga;

// ============================================================================
// UTILITARE JOC
// ============================================================================

void pauzaDramatica(int milisecunde = 500) {
    std::this_thread::sleep_for(std::chrono::milliseconds(milisecunde));
}

void curataEcran() {
    // Simulare curatare ecran (newline-uri multe)
    for(int i=0; i<50; i++) std::cout << "\n";
}

void initializeazaLiga(const std::string& numeEchipaTa) {
    liga.clear();
    liga.push_back({numeEchipaTa, 0, 0, 0, 0}); // Puterea se va calcula dinamic
    liga.push_back({"FCSB", 85, 0, 0, 0});
    liga.push_back({"Dinamo", 82, 0, 0, 0});
    liga.push_back({"CFR Cluj", 88, 0, 0, 0});
    liga.push_back({"Univ. Craiova", 84, 0, 0, 0});
    liga.push_back({"Rapid", 83, 0, 0, 0});
    liga.push_back({"Farul", 80, 0, 0, 0});
    liga.push_back({"Otelul", 75, 0, 0, 0});
}

// ============================================================================
// MENIURI SI LOGICA
// ============================================================================

void afiseazaClasament() {
    // Sortam echipele
    std::sort(liga.begin(), liga.end(), comparaEchipe);

    std::cout << "\n=== CLASAMENT LIGA 1 ===\n";
    std::cout << std::left << std::setw(4) << "Loc"
              << std::setw(20) << "Echipa"
              << std::setw(5) << "Pct"
              << std::setw(5) << "GM"
              << std::setw(5) << "GP"
              << "Golaveraj\n";
    std::cout << "--------------------------------------------------\n";

    for (size_t i = 0; i < liga.size(); ++i) {
        std::cout << std::left << std::setw(4) << (i + 1)
                  << std::setw(20) << liga[i].nume
                  << std::setw(5) << liga[i].puncte
                  << std::setw(5) << liga[i].goluriMarcate
                  << std::setw(5) << liga[i].goluriPrimite
                  << liga[i].golaveraj() << "\n";
    }
    std::cout << "\nApasa ENTER pentru a reveni la meniu...";
    std::cin.get();
}

void gestioneazaPrimul11() {
    curataEcran();
    std::cout << "=== GESTIONARE LOT SI PRIMUL 11 ===\n";
    std::cout << "Selecteaza exact 11 jucatori pentru meciul urmator.\n\n";

    primul11.clear();

    // Iteram prin toti jucatorii din lot
    for (size_t i = 0; i < lotJucatori.size(); ++i) {
        // Daca am atins deja 11, restul sunt rezerve automat
        if (primul11.size() >= 11) {
            std::cout << "[REZERVA] " << lotJucatori[i].getNumeComplet()
                      << " (" << lotJucatori[i].getPozitie() << ") - Rating: "
                      << lotJucatori[i].calculeazaEficienta() << "\n";
            continue;
        }

        std::cout << "Jucator: " << lotJucatori[i].getNumeComplet()
                  << " | Poz: " << lotJucatori[i].getPozitie()
                  << " | Rating: " << lotJucatori[i].calculeazaEficienta() << "\n";
        std::cout << "Il incluzi in primul 11? (d/n): ";
        char optiune;
        std::cin >> optiune;
        std::cin.ignore(); // curata buffer

        if (optiune == 'd' || optiune == 'D') {
            primul11.push_back(&lotJucatori[i]);
            std::cout << ">>> Adaugat in titulari.\n";
        } else {
            std::cout << ">>> Ramane rezerva.\n";
        }
        std::cout << "-----------------------\n";
    }

    // Validare - CORECTAT WARNING: cast la size_t sau int pentru comparatii sigure
    if (primul11.size() != 11) {
        std::cout << "\n[ATENTIE] Ai selectat " << primul11.size() << " jucatori!\n";
        std::cout << "Jocul va completa automat sau va taia din lista pentru a avea 11.\n";

        // Daca sunt prea putini, adaugam din rezerve
        size_t index = 0;
        // Folosim 11u pentru unsigned int literal
        while (primul11.size() < 11 && index < lotJucatori.size()) {
            // Verificam daca e deja in primul 11 (pointer comparison)
            bool eDeja = false;
            for(auto* p : primul11) if(p == &lotJucatori[index]) eDeja = true;

            if(!eDeja) primul11.push_back(&lotJucatori[index]);
            index++;
        }
    }

    std::cout << "\nPrimul 11 a fost salvat! Apasa ENTER...";
    std::cin.get();
}

// Returneaza puterea medie a primului 11 selectat
int calculeazaRatingTitulari() {
    if (primul11.empty()) return 50; // default
    int suma = 0;
    for (auto* j : primul11) {
        suma += j->calculeazaEficienta();
    }
    // Cast la int pentru a evita warning
    return suma / static_cast<int>(primul11.size());
}

void joacaMeci(const std::string& numeEchipaTa) {
    // 1. Alege adversar (random dintre celelalte echipe)
    int indexAdversar = rand() % liga.size();
    while (liga[indexAdversar].nume == numeEchipaTa) {
        indexAdversar = rand() % liga.size();
    }
    EchipaCampionat& adversar = liga[indexAdversar];

    // Gaseste echipa noastra in liga
    EchipaCampionat* noi = nullptr;
    for(auto& e : liga) if(e.nume == numeEchipaTa) noi = &e;

    // Calcul puteri
    int putereNoi = calculeazaRatingTitulari();
    int putereAdversar = adversar.putere + (rand() % 10 - 5); // Variatie mica

    curataEcran();
    std::cout << "========================================\n";
    std::cout << "MATCH DAY: " << numeEchipaTa << " vs " << adversar.nume << "\n";
    std::cout << "Rating Noi: " << putereNoi << " | Rating Ei: " << putereAdversar << "\n";
    std::cout << "========================================\n";
    pauzaDramatica(1500);

    int goluriNoi = 0;
    int goluriEi = 0;

    // Simulare Textuala
    for (int min = 0; min <= 90; min += 15) {
        std::cout << "Minutul " << min << "... ";
        pauzaDramatica(300);

        int zar = rand() % 100;
        // Logica simpla de posesie bazata pe diferenta de rating
        int sansaNoastra = 50 + (putereNoi - putereAdversar);

        if (zar < 15) { // Ocazie de gol
            std::cout << "Ocazie mare! ";
            if (rand() % 100 < sansaNoastra) {
                std::cout << "Atacam noi... Sut... ";
                pauzaDramatica(400);
                if (rand() % 100 < 40) {
                    std::cout << "GOOOL " << numeEchipaTa << "!\n";
                    goluriNoi++;
                } else std::cout << "Ratare!\n";
            } else {
                std::cout << "Ataca " << adversar.nume << "... Sut... ";
                pauzaDramatica(400);
                if (rand() % 100 < 40) {
                    std::cout << "GOL " << adversar.nume << "!\n";
                    goluriEi++;
                } else std::cout << "Apara portarul nostru!\n";
            }
        } else {
            std::cout << "Joc echilibrat la mijlocul terenului.\n";
        }
    }

    std::cout << "\n>>> SCOR FINAL: " << numeEchipaTa << " " << goluriNoi << " - " << goluriEi << " " << adversar.nume << "\n";

    // Actualizare clasament Noi
    if (noi) {
        noi->goluriMarcate += goluriNoi;
        noi->goluriPrimite += goluriEi;
        if (goluriNoi > goluriEi) { noi->puncte += 3; std::cout << "Victorie! (+3 pct)\n"; }
        else if (goluriNoi == goluriEi) { noi->puncte += 1; std::cout << "Egal! (+1 pct)\n"; }
        else std::cout << "Infrangere...\n";
    }

    // Actualizare clasament Adversar
    adversar.goluriMarcate += goluriEi;
    adversar.goluriPrimite += goluriNoi;
    if (goluriEi > goluriNoi) adversar.puncte += 3;
    else if (goluriEi == goluriNoi) adversar.puncte += 1;

    // Simulare rezultate pentru restul ligii (ca sa nu ramana la 0)
    for(auto& echipa : liga) {
        if(echipa.nume != numeEchipaTa && echipa.nume != adversar.nume) {
            // Rezultat random
            int gM = rand() % 3;
            int gP = rand() % 3;
            echipa.goluriMarcate += gM;
            echipa.goluriPrimite += gP;
            if(gM > gP) echipa.puncte += 3;
            else if (gM == gP) echipa.puncte += 1;
        }
    }

    std::cout << "\nApasa ENTER pentru a continua...";
    std::cin.ignore();
    std::cin.get();
}


// ============================================================================
// MAIN
// ============================================================================

int main() {
    srand(static_cast<unsigned int>(time(0)));

    try {
        Club club("FC Politehnica"); // Pentru cerinta OOP
        std::string numeEchipaMea = "FC Politehnica";

        // 1. Initializare Liga
        initializeazaLiga(numeEchipaMea);

        // 2. Citire Date
        std::ifstream fin("tastatura.txt");
        if (!fin.is_open()) {
            // Creare fisier dummy daca nu exista
             std::ofstream fout("tastatura.txt");
             fout << "JUCATOR Hagi Gheorghe 94 CAM 10\n"
                  << "JUCATOR Popescu Gica 89 CB 6\n"
                  << "JUCATOR Mutu Adrian 91 ST 10\n"
                  << "JUCATOR Chivu Cristi 88 LB 5\n"
                  << "JUCATOR Petrescu Dan 87 RB 2\n"
                  << "JUCATOR Belodedici Miodrag 86 CB 4\n"
                  << "JUCATOR Dumitrescu Ilie 85 LM 11\n"
                  << "JUCATOR Lacatus Marius 84 RM 7\n"
                  << "JUCATOR Raducioiu Florin 83 ST 9\n"
                  << "JUCATOR Lupescu Ionut 84 CM 5\n"
                  << "JUCATOR Stelea Bogdan 82 GK 1\n"
                  << "JUCATOR Prunea Florin 80 GK 12\n" // Rezerva 1
                  << "JUCATOR Galca Costel 81 CDM 8\n" // Rezerva 2
                  << "ANTRENOR Iordanescu Anghel 50 10\n";
             fout.close();
             fin.open("tastatura.txt");
        }

        std::string tip;
        while (fin >> tip) {
            std::string nume, prenume;
            fin >> nume >> prenume;

            if (tip == "JUCATOR") {
                int rating, numar;
                std::string pozitie;
                fin >> rating >> pozitie >> numar;

                Jucator j(nume, prenume, rating, pozitie, numar);
                club.adaugaMembru(j); // Pentru OOP
                lotJucatori.push_back(j); // Pentru logica jocului
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
                std::string dummy; std::getline(fin, dummy);
            }
        }
        fin.close();

        // Initializare Primul 11 Default (primii 11 cititi sau cati avem)
        // CORECTAT WARNING: size_t pentru index
        for(size_t i=0; i<lotJucatori.size() && i<11; ++i) {
            primul11.push_back(&lotJucatori[i]);
        }

        // 3. MENIU PRINCIPAL (Game Loop)
        bool ruleaza = true;
        while (ruleaza) {
            curataEcran();
            std::cout << "========================================\n";
            std::cout << "   MANAGER LIGA 1 - " << numeEchipaMea << "\n";
            std::cout << "========================================\n";
            std::cout << "1. Joaca Urmatorul Meci\n";
            std::cout << "2. Gestioneaza Primul 11\n";
            std::cout << "3. Vezi Clasament\n";
            std::cout << "4. Raport Club (Afisare membri)\n";
            std::cout << "5. Salvare Date & Iesire\n";
            std::cout << "========================================\n";
            std::cout << "Alegerea ta: ";

            int optiune;
            std::cin >> optiune;
            std::cin.ignore(); // curata newline

            switch (optiune) {
                case 1:
                    joacaMeci(numeEchipaMea);
                    break;
                case 2:
                    gestioneazaPrimul11();
                    break;
                case 3:
                    afiseazaClasament();
                    break;
                case 4:
                    club.afiseazaMembri();
                    std::cout << "\nApasa ENTER..."; std::cin.get();
                    break;
                case 5:
                    club.salveazaInFisier();
                    std::cout << "La revedere, Mister!\n";
                    ruleaza = false;
                    break;
                default:
                    std::cout << "Optiune invalida!\n";
            }
        }

    } catch (const std::exception& e) {
        std::cerr << "[CRASH] " << e.what() << "\n";
        return 1;
    }

    return 0;
}