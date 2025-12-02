#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <stdexcept>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <chrono>
#include <algorithm>
#include <iomanip>

// Includem headerele proiectului (structura cu litere mici)
#include "Exceptii/exceptii.h"
#include "Persoane/persoane.h"
#include "Persoane/jucator.h"
#include "Persoane/antrenor.h"
#include "Persoane/arbitru.h"
#include "Club/club.h"

// ============================================================================
// CLASA LOCALA: MEDIC (Extensibilitate)
// ============================================================================
class Medic : public Persoana {
    std::string specializare;
public:
    Medic(const std::string& nume, const std::string& prenume, const std::string& spec)
        : Persoana(nume, prenume), specializare(spec) {}

    Medic* clone() const override { return new Medic(*this); }

    int calculeazaEficienta() const override { return 100; } // Medicii sunt vitali

    std::string getSpecializare() const { return specializare; }

protected:
    void afisareDetaliata(std::ostream& os) const override {
        os << "Rol: STAFF MEDICAL | Specializare: " << specializare << "\n";
    }
};

// ============================================================================
// STRUCTURI DE DATE AUXILIARE PENTRU LOGICA JOCULUI
// ============================================================================

struct EchipaAI {
    std::string nume;
    int rating;
    int puncte;
    int goluriM;
    int goluriP;
};

// Variabile Globale de Stare (Simulare Manager)
long long bugetClub = 500000; // 500k Euro start
int moralEchipa = 80;         // 0-100
int etapaCurenta = 1;
std::vector<EchipaAI> liga;
std::vector<Jucator> lotLocal; // O copie locala pentru a putea selecta primul 11
std::vector<Jucator*> primul11;

// ============================================================================
// UTILITARE VIZUALE SI LOGICE
// ============================================================================

void clearScreen() {
    // Hack simplu pentru "curatarea" consolei prin newline-uri
    for(int i=0; i<40; i++) std::cout << "\n";
}

void sleepMs(int ms) {
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

void afiseazaHeader(const std::string& titlu) {
    std::cout << "\n==============================================================\n";
    std::cout << "   " << titlu << "\n";
    std::cout << "==============================================================\n";
}

void initLiga() {
    liga = {
        {"FC Politehnica", 0, 0, 0, 0}, // Noi
        {"FCSB", 85, 0, 0, 0},
        {"CFR Cluj", 88, 0, 0, 0},
        {"Univ. Craiova", 84, 0, 0, 0},
        {"Rapid", 82, 0, 0, 0},
        {"Dinamo", 78, 0, 0, 0}
    };
}

// ============================================================================
// MODULE JOC
// ============================================================================

// --- MODUL 1: GESTIONARE PRIMUL 11 ---
void meniuPrimul11() {
    clearScreen();
    afiseazaHeader("VESTIAR - SELECTIE PRIMUL 11");
    std::cout << "Selecteaza jucatorii pe care te bazezi in etapa " << etapaCurenta << ".\n\n";

    primul11.clear();

    // Selectie simplificata: parcurgem lotul si intrebam
    for (size_t i = 0; i < lotLocal.size(); ++i) {
        if (primul11.size() == 11) break;

        std::cout << "Jucator: " << lotLocal[i].getNumeComplet()
                  << " (" << lotLocal[i].getPozitie() << ") Rating: "
                  << lotLocal[i].calculeazaEficienta() << "\n";
        std::cout << "Titular? [d/n]: ";
        char rasp;
        std::cin >> rasp;
        if (rasp == 'd' || rasp == 'D') {
            primul11.push_back(&lotLocal[i]);
            std::cout << ">>> Adaugat in echipa de start.\n";
        }
        std::cout << "--------------------------------\n";
    }

    // Auto-fill daca userul s-a plictisit
    while (primul11.size() < 11 && primul11.size() < lotLocal.size()) {
        // Adaugam pur si simplu urmatorii disponibili care nu sunt deja in lista
        // (Simplificare logica pentru demo)
        bool exista = false;
        for(auto* p : primul11) {
            // Comparatie adrese, dar aici e riscant. Simplificam adaugand de la inceput.
        }
        // Resetam si luam primii 11 automat daca nu a selectat destui
        if(primul11.size() < 11) {
             std::cout << "\n[INFO] Nu ai selectat 11 jucatori. Antrenorul secund completeaza echipa automat.\n";
             primul11.clear();
             for(size_t i=0; i<lotLocal.size() && i<11; ++i) {
                 primul11.push_back(&lotLocal[i]);
             }
             break;
        }
    }
    sleepMs(1000);
}

// --- MODUL 2: SCOUTING & TRANSFERURI ---
void meniuTransferuri(Club& club) {
    clearScreen();
    afiseazaHeader("PIATA DE TRANSFERURI");
    std::cout << "Buget actual: " << bugetClub << " EUR\n\n";

    // Generam 3 jucatori random
    std::vector<std::string> numeScout = {"Neymar", "Mbappe", "Ronaldo", "Messi", "Hagi Jr.", "Popescu", "Radu"};
    std::vector<std::string> pozitii = {"ST", "CAM", "CB", "GK", "CM"};

    std::cout << "Scoutii au gasit urmatorii jucatori liberi de contract:\n";
    for(int i=0; i<3; ++i) {
        std::string n = numeScout[rand() % numeScout.size()];
        std::string p = pozitii[rand() % pozitii.size()];
        int r = 70 + (rand() % 25); // Rating 70-95
        int pret = r * 1000; // Pret estimativ (salariu la semnatura)

        std::cout << i+1 << ". " << n << " (" << p << ") - Rating: " << r << " | Cost: " << pret << " EUR\n";
    }

    std::cout << "\n4. Nu transfera pe nimeni\n";
    std::cout << "Alege optiunea: ";
    int opt;
    std::cin >> opt;

    if (opt >= 1 && opt <= 3) {
        // Re-generam datele (simulat) pentru a crea obiectul
        int index = opt - 1;
        // In realitate am salva datele generate mai sus intr-un vector temporar,
        // dar pentru demo regeneram similar:
        int cost = 50000; // Cost fix demo

        if (bugetClub >= cost) {
            bugetClub -= cost;
            std::cout << "Negocieri in curs...";
            sleepMs(1000);
            std::cout << " TRANSFER REUSIT! Jucatorul s-a alaturat clubului.\n";

            // Adaugam in Club (OOP) si in Lotul Local (Logic)
            Jucator nou("JucatorNou", "Transferat", 85, "RW", 99);
            club.adaugaMembru(nou);
            lotLocal.push_back(nou);
        } else {
            std::cout << "[EROARE] Fonduri insuficiente!\n";
        }
    }
    sleepMs(1500);
}

// --- MODUL 3: MECIUL ---
void joacaMeci(const std::string& numeClub) {
    clearScreen();

    // Alegem adversar
    int idxAdv = rand() % liga.size();
    while(liga[idxAdv].nume == numeClub) idxAdv = rand() % liga.size();
    EchipaAI& adversar = liga[idxAdv];
    EchipaAI* noi = &liga[0]; // Presupunem ca suntem prima

    // Calculam puterea noastra (media primului 11)
    int putereNoi = 0;
    if(primul11.empty()) putereNoi = 60; // Penalizare daca nu ai facut echipa
    else {
        for(auto* j : primul11) putereNoi += j->calculeazaEficienta();
        putereNoi /= primul11.size();
    }
    // Bonus de moral
    putereNoi += (moralEchipa - 50) / 10;

    afiseazaHeader("ETAPA " + std::to_string(etapaCurenta) + ": " + numeClub + " vs " + adversar.nume);
    std::cout << "Rating Noi: " << putereNoi << " vs Rating Ei: " << adversar.rating << "\n\n";
    std::cout << "Arbitrul fluiera inceputul partidei...\n";
    sleepMs(1000);

    int scorNoi = 0, scorEi = 0;

    // Simulare 90 minute
    for(int min=0; min<=90; min+=10) {
        std::cout << "Min " << std::setw(2) << min << ": ";
        int sansa = rand() % 100;
        int dif = putereNoi - adversar.rating; // Pozitiv = suntem mai buni

        if (sansa < 15) { // Eveniment
            if (rand() % 100 < (50 + dif)) {
                std::cout << "GOOOOL " << numeClub << "! ";
                // Alegem un marcator random din primul 11
                if(!primul11.empty())
                    std::cout << primul11[rand() % primul11.size()]->getNumeComplet() << " inscrie!\n";
                else std::cout << "Inscriem printr-o actiune colectiva!\n";
                scorNoi++;
            } else {
                std::cout << "Gol " << adversar.nume << ". Apararea noastra a visat.\n";
                scorEi++;
            }
            sleepMs(800);
        } else if (sansa > 90) {
            std::cout << "Cartonas galben acordat pentru un fault dur.\n";
        } else {
            std::cout << "Joc de posesie la mijlocul terenului...\n";
        }
        sleepMs(200);
    }

    std::cout << "\n--------------------------------------------------------------\n";
    std::cout << "SCOR FINAL: " << scorNoi << " - " << scorEi << "\n";
    std::cout << "--------------------------------------------------------------\n";

    // Procesare rezultat
    if (scorNoi > scorEi) {
        std::cout << "VICTORIE! (+3 Puncte, +50.000 EUR)\n";
        bugetClub += 50000;
        moralEchipa = std::min(100, moralEchipa + 5);
        noi->puncte += 3;
    } else if (scorNoi == scorEi) {
        std::cout << "EGALITATE. (+1 Punct, +10.000 EUR)\n";
        bugetClub += 10000;
        noi->puncte += 1;
    } else {
        std::cout << "INFRANGERE. Moralul scade.\n";
        moralEchipa = std::max(0, moralEchipa - 10);
    }

    // Update golaveraj
    noi->goluriM += scorNoi; noi->goluriP += scorEi;
    adversar.goluriM += scorEi; adversar.goluriP += scorNoi;
    // Simulare restul ligii
    for(auto& ech : liga) {
        if(ech.nume != numeClub && ech.nume != adversar.nume) {
            ech.puncte += rand() % 4; // 0, 1 sau 3 pct (simplificat)
            ech.goluriM += rand() % 3;
        }
    }

    etapaCurenta++;
    std::cout << "\nApasa ENTER pentru a continua...";
    std::cin.ignore();
    std::cin.get();
}

// --- MODUL 4: CLASAMENT ---
void afiseazaClasament() {
    clearScreen();
    afiseazaHeader("CLASAMENT LIGA 1");

    // Sortare folosind lambda
    std::sort(liga.begin(), liga.end(), [](const EchipaAI& a, const EchipaAI& b) {
        return a.puncte > b.puncte;
    });

    std::cout << std::left << std::setw(4) << "Loc"
              << std::setw(20) << "Echipa"
              << std::setw(5) << "Pct"
              << "GM-GP\n";
    std::cout << "--------------------------------\n";

    for(size_t i=0; i<liga.size(); ++i) {
        std::cout << std::left << std::setw(4) << i+1
                  << std::setw(20) << liga[i].nume
                  << std::setw(5) << liga[i].puncte
                  << liga[i].goluriM << "-" << liga[i].goluriP << "\n";
    }
    std::cout << "\nApasa ENTER...";
    std::cin.ignore();
    std::cin.get();
}

// --- MODUL 5: ANTRENAMENT ---
void meniuAntrenament() {
    clearScreen();
    afiseazaHeader("BAZA DE ANTRENAMENT");
    std::cout << "1. Antrenament Tactic (Creste Moralul)\n";
    std::cout << "2. Antrenament Fizic (Scade Moralul, dar creste eficienta - Simulat)\n";
    std::cout << "3. Vizita Medicala (Foloseste clasa Medic)\n";
    std::cout << "Alege: ";
    int opt; std::cin >> opt;

    switch(opt) {
        case 1:
            std::cout << "Echipa a facut teambuilding...\n";
            moralEchipa = std::min(100, moralEchipa + 10);
            std::cout << "Moral actual: " << moralEchipa << "\n";
            break;
        case 3:
            std::cout << "Medicii verifica lotul...\n";
            // Aici am putea folosi dynamic_cast in main daca am avea acces la membri
            std::cout << "Toti jucatorii sunt apti de joc.\n";
            break;
        default:
            std::cout << "Antrenament efectuat.\n";
    }
    sleepMs(1000);
}

// ============================================================================
// MAIN
// ============================================================================

int main() {
    srand(time(0));

    try {
        // 1. Setup Initial
        Club club("FC Politehnica");
        initLiga();

        // 2. Incarcare din Fisier
        std::ifstream fin("tastatura.txt");
        if (!fin.is_open()) {
            // Creare automata fisier demo daca nu exista
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
                 << "JUCATOR Prunea Florin 80 GK 12\n"
                 << "ANTRENOR Iordanescu Anghel 50 10\n"
                 << "MEDIC Popescu Ion Recuperare\n";
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
                club.adaugaMembru(j);
                lotLocal.push_back(j); // Salvam si local pentru a putea manipula usor
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

        // Initializare primul 11 default
        for(size_t i=0; i<lotLocal.size() && i<11; ++i) primul11.push_back(&lotLocal[i]);

        // 3. GAME LOOP (Bucla Principala a Jocului)
        bool ruleaza = true;
        while (ruleaza) {
            clearScreen();
            std::cout << "==============================================================\n";
            std::cout << "   MANAGER DE FOTBAL 2025 - FC POLITEHNICA\n";
            std::cout << "==============================================================\n";
            std::cout << "Buget: " << bugetClub << " EUR  |  Moral: " << moralEchipa << "%  |  Etapa: " << etapaCurenta << "\n";
            std::cout << "--------------------------------------------------------------\n";
            std::cout << "1. [MECI] Joaca urmatoarea etapa\n";
            std::cout << "2. [ECHIPA] Alege primul 11\n";
            std::cout << "3. [LIGA] Vezi clasament\n";
            std::cout << "4. [TRANSFERURI] Cauta jucatori noi\n";
            std::cout << "5. [CLUB] Raport complet (Afisare Polimorfica)\n";
            std::cout << "6. [ANTRENAMENT] Sesiune pregatire\n";
            std::cout << "7. [ANALIZA] Raport RTTI (Tipuri membri)\n";
            std::cout << "8. [ADMIN] Salveaza si Iesi\n";
            std::cout << "--------------------------------------------------------------\n";
            std::cout << "Comanda ta: ";

            int cmd;
            std::cin >> cmd;

            // Input validation basic
            if(std::cin.fail()) {
                std::cin.clear();
                std::cin.ignore(1000, '\n');
                continue;
            }

            switch(cmd) {
                case 1: joacaMeci("FC Politehnica"); break;
                case 2: meniuPrimul11(); break;
                case 3: afiseazaClasament(); break;
                case 4: meniuTransferuri(club); break;
                case 5:
                    clearScreen();
                    club.afiseazaMembri();
                    std::cout << "\nApasa ENTER..."; std::cin.ignore(); std::cin.get();
                    break;
                case 6: meniuAntrenament(); break;
                case 7:
                    clearScreen();
                    club.analizeazaEchipa();
                    std::cout << "\nApasa ENTER..."; std::cin.ignore(); std::cin.get();
                    break;
                case 8:
                    club.salveazaInFisier();
                    std::cout << "Joc salvat. La revedere!\n";
                    ruleaza = false;
                    break;
                default:
                    std::cout << "Comanda invalida!\n"; sleepMs(500);
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "[CRASH JOC] " << e.what() << "\n";
        return 1;
    }

    return 0;
}
