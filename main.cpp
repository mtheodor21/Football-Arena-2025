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
#include <map>

#include "Exceptii/exceptii.h"
#include "Persoane/persoane.h"
#include "Persoane/jucator.h"
#include "Persoane/antrenor.h"
#include "Persoane/arbitru.h"
#include "Club/club.h"

class JucatorFactory {
public:
    static Jucator* creazaJucator(const std::string& nume, const std::string& prenume, int rating, const std::string& pozitie, int nr) {
        if (pozitie == "GK") return new Portar(nume, prenume, rating, pozitie, nr);

        if (pozitie == "CB") return new FundasCentral(nume, prenume, rating, pozitie, nr);
        if (pozitie == "LB") return new FundasStanga(nume, prenume, rating, pozitie, nr);
        if (pozitie == "RB") return new FundasDreapta(nume, prenume, rating, pozitie, nr);
        if (pozitie == "LWB" || pozitie == "RWB") return new Fundas(nume, prenume, rating, pozitie, nr);

        if (pozitie == "CM") return new MijlocasCentral(nume, prenume, rating, pozitie, nr);
        if (pozitie == "CDM") return new MijlocasDefensiv(nume, prenume, rating, pozitie, nr);
        if (pozitie == "CAM") return new MijlocasOfensiv(nume, prenume, rating, pozitie, nr);
        if (pozitie == "LM") return new MijlocasStanga(nume, prenume, rating, pozitie, nr);
        if (pozitie == "RM") return new MijlocasDreapta(nume, prenume, rating, pozitie, nr);

        if (pozitie == "ST" || pozitie == "CF") return new AtacantCentral(nume, prenume, rating, pozitie, nr);
        if (pozitie == "LW") return new AtacantStanga(nume, prenume, rating, pozitie, nr);
        if (pozitie == "RW") return new AtacantDreapta(nume, prenume, rating, pozitie, nr);

        return new Jucator(nume, prenume, rating, pozitie, nr);
    }
};

class Medic : public Persoana {
    std::string specializare;
    int aniExperienta;
public:
    Medic(const std::string& nume, const std::string& prenume, const std::string& spec, int exp)
        : Persoana(nume, prenume), specializare(spec), aniExperienta(exp) {}

    Medic* clone() const override { return new Medic(*this); }
    int calculeazaEficienta() const override { return std::min(100, 50 + aniExperienta * 5); }
    std::string getSpecializare() const { return specializare; }
protected:
    void afisareDetaliata(std::ostream& os) const override {
        os << "Rol: STAFF MEDICAL\n";
        os << "Specializare: " << specializare << " | Exp: " << aniExperienta << " ani\n";
    }
};

struct EchipaAI {
    std::string nume;
    int rating;
    int puncte;
    int meciuriJucate;
    int victorii;
    int egaluri;
    int infrangeri;
    int gm;
    int gp;
    int golaveraj() const { return gm - gp; }
};

struct DetaliiClub {
    std::string numeStadion;
    int capacitateStadion;
    int anInfiintare;
    int numarTrofee;
    std::string rival;
};

long long buget = 2500000;
int moral = 80;
int etapa = 1;

std::vector<Jucator*> lotJucatori;
std::vector<Jucator*> titulari;
std::vector<Jucator*> rezerve;
std::vector<EchipaAI> liga;
DetaliiClub detaliiMele;
std::vector<Antrenor*> staffTehnic;
std::vector<Medic*> staffMedical;

void clear() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void sleepMs(int ms) {
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

void antet(std::string titlu) {
    std::cout << "\n==============================================================\n";
    std::cout << "   " << titlu << "\n";
    std::cout << "==============================================================\n";
}

void initJoc() {
    detaliiMele = {"Arena Politehnica", 30000, 1921, 10, "UTA Arad"};
    liga = {
        {"FC Politehnica", 0, 0, 0, 0, 0, 0, 0, 0},
        {"FCSB", 85, 0, 0, 0, 0, 0, 0, 0},
        {"CFR Cluj", 87, 0, 0, 0, 0, 0, 0, 0},
        {"Univ. Craiova", 84, 0, 0, 0, 0, 0, 0, 0},
        {"Rapid", 83, 0, 0, 0, 0, 0, 0, 0},
        {"Dinamo", 79, 0, 0, 0, 0, 0, 0, 0},
        {"Farul", 81, 0, 0, 0, 0, 0, 0, 0},
        {"Sepsi OSK", 78, 0, 0, 0, 0, 0, 0, 0}
    };
}

void sincronizeazaLot() {
    titulari.clear();
    rezerve.clear();
    for(size_t i=0; i<lotJucatori.size(); ++i) {
        if(i < 11) titulari.push_back(lotJucatori[i]);
        else rezerve.push_back(lotJucatori[i]);
    }
}

int calculeazaRatingEchipa() {
    if(titulari.empty()) return 50;
    int s = 0;
    for(auto* p : titulari) s += p->calculeazaEficienta();
    int media = s / titulari.size();
    if(!staffTehnic.empty()) media += (staffTehnic[0]->calculeazaEficienta() / 20);
    media += (moral - 50) / 10;
    return media;
}

void meniuTactica() {
    bool inMeniu = true;
    while(inMeniu) {
        clear();
        antet("VESTIAR - GESTIONARE LOT");

        std::cout << "TITULARI (" << titulari.size() << "):\n";
        for(size_t i=0; i<titulari.size(); ++i) {
            std::cout << " " << std::setw(2) << i+1 << ". ";
            std::cout << std::left << std::setw(20) << titulari[i]->getNumeComplet();
            std::cout << " -> "; titulari[i]->afiseazaInformatii(std::cout);
        }

        std::cout << "\nREZERVE (" << rezerve.size() << "):\n";
        for(size_t i=0; i<rezerve.size(); ++i) {
             std::cout << " " << std::setw(2) << i+1 + 11 << ". " << rezerve[i]->getNumeComplet() << "\n";
        }

        std::cout << "\n[OPTIUNI]:\n";
        std::cout << " Format: 'x y' pentru a schimba titularul X cu rezerva Y (ex: 1 12)\n";
        std::cout << " 0 0 pentru a iesi din vestiar\n";
        std::cout << "Comanda ta: ";

        int tIdx, rIdx;
        if (!(std::cin >> tIdx >> rIdx)) {
            std::cin.clear(); std::cin.ignore(1000, '\n'); continue;
        }

        if (tIdx == 0 && rIdx == 0) {
            inMeniu = false;
        } else {
            int realT = tIdx - 1;
            int realR = rIdx - 12;

            if (realT >= 0 && realT < (int)titulari.size() && realR >= 0 && realR < (int)rezerve.size()) {
                std::cout << "Se face schimbarea: " << titulari[realT]->getNumeComplet()
                          << " <-> " << rezerve[realR]->getNumeComplet() << "...\n";
                sleepMs(800);

                Jucator* temp = titulari[realT];

                auto itT = std::find(lotJucatori.begin(), lotJucatori.end(), titulari[realT]);
                auto itR = std::find(lotJucatori.begin(), lotJucatori.end(), rezerve[realR]);

                if (itT != lotJucatori.end() && itR != lotJucatori.end()) {
                    std::iter_swap(itT, itR);
                    sincronizeazaLot();
                }
            } else {
                std::cout << "Indecsi invalizi!\n"; sleepMs(1000);
            }
        }
    }
}

void meniuTransferuri(Club& club) {
    clear();
    antet("PIATA DE TRANSFERURI");
    std::cout << "Buget disponibil: " << buget << " EUR\n\n";

    std::vector<std::string> numePool = {"Santos", "Kowalski", "Muller", "Popa", "Ionescu", "Smith", "Yamal", "Toure"};
    std::vector<std::string> pozPool = {"ST", "CAM", "CDM", "CB", "GK", "RW", "LW"};

    std::cout << "Scoutii au identificat urmatorii jucatori:\n";
    std::vector<Jucator*> market;
    std::vector<int> preturi;

    for(int i=0; i<5; ++i) {
        std::string n = numePool[rand() % numePool.size()];
        std::string poz = pozPool[rand() % pozPool.size()];
        int rtg = 65 + rand() % 30;
        int pret = rtg * rtg * 150;

        Jucator* j = JucatorFactory::creazaJucator(n, "Scout", rtg, poz, 99);
        market.push_back(j);
        preturi.push_back(pret);

        std::cout << i+1 << ". " << n << " (" << poz << ") | Rtg: " << rtg << " | Pret: " << pret << " EUR\n";
    }

    std::cout << "\n6. Inapoi la meniu\n";
    std::cout << "Alege jucatorul de cumparat: ";
    int opt; std::cin >> opt;

    if (opt >= 1 && opt <= 5) {
        int idx = opt - 1;
        if (buget >= preturi[idx]) {
            buget -= preturi[idx];
            std::cout << "Negocieri reusite! " << market[idx]->getNumeComplet() << " a semnat.\n";

            club.adaugaMembru(*market[idx]);
            lotJucatori.push_back(market[idx]);

            for(size_t k=0; k<market.size(); ++k) {
                if((int)k != idx) delete market[k];
            }
            sincronizeazaLot();
            sleepMs(1500);
        } else {
            std::cout << "Fonduri insuficiente!\n"; sleepMs(2000);
            for(auto* p : market) delete p;
        }
    } else {
        for(auto* p : market) delete p;
    }
}

void meniuDetaliiClub(const Club& club) {
    clear();
    antet("DETALII CLUB & INFRASTRUCTURA");
    std::cout << "Nume Stadion: " << detaliiMele.numeStadion << " (Capacitate: " << detaliiMele.capacitateStadion << ")\n";
    std::cout << "An infiintare: " << detaliiMele.anInfiintare << "\n";
    std::cout << "Palmares: " << detaliiMele.numarTrofee << " Trofee majore\n";
    std::cout << "Situatie financiara: " << buget << " EUR\n";
    std::cout << "------------------------------------------\n";

    std::cout << "STAFF MEDICAL:\n";
    for(auto* m : staffMedical) std::cout << " - Dr. " << m->getNumeComplet() << " (" << m->getSpecializare() << ")\n";

    std::cout << "STAFF TEHNIC:\n";
    for(auto* a : staffTehnic) std::cout << " - " << a->getNumeComplet() << " (Antrenor Principal)\n";

    std::cout << "\nApasa ENTER pentru raportul complet al membrilor...";
    std::cin.ignore(); std::cin.get();
    std::cout << "\n--- RAPORT COMPLET (Din clasa Club) ---\n";
    club.afiseazaMembri();
    std::cout << "\nApasa ENTER pentru a reveni...";
    std::cin.get();
}

void joacaEtapa() {
    clear();
    EchipaAI& adversar = liga[etapa % (liga.size()-1) + 1];
    EchipaAI* noi = &liga[0];
    int ratingNoi = calculeazaRatingEchipa();
    int ratingAdv = adversar.rating + (rand() % 10 - 5);

    antet("ETAPA " + std::to_string(etapa) + " - MATCH DAY");
    std::cout << "FC Politehnica (Rtg: " << ratingNoi << ") vs " << adversar.nume << " (Rtg: " << ratingAdv << ")\n";
    sleepMs(2000);

    int scorNoi = 0, scorAdv = 0;
    int minuteEvenimente[] = {12, 25, 38, 55, 67, 82, 89};

    for (int min : minuteEvenimente) {
        std::cout << "Min " << min << ": ";
        int sansa = rand() % 100;
        int avantaj = ratingNoi - ratingAdv;

        if (sansa < 40) std::cout << "Joc calm la mijlocul terenului...\n";
        else if (sansa < 70) {
            if (rand() % 100 < (40 + avantaj)) {
                std::cout << "GOOOOL POLITEHNICA! ";
                if(!titulari.empty())
                    std::cout << titulari[rand() % titulari.size()]->getNumeComplet() << " inscrie!\n";
                else std::cout << "Inscriem!\n";
                scorNoi++;
            } else std::cout << "Ratare imensa pentru noi! Bara!\n";
        } else {
            if (rand() % 100 < (40 - avantaj)) {
                std::cout << "GOL " << adversar.nume << ". Apararea noastra a cedat.\n";
                scorAdv++;
            } else std::cout << "Interventie salvatoare a portarului nostru!\n";
        }
        sleepMs(1000);
    }

    std::cout << "\n----------------------------------------\n";
    std::cout << "SCOR FINAL: " << scorNoi << " - " << scorAdv << "\n";
    std::cout << "----------------------------------------\n";

    noi->meciuriJucate++; adversar.meciuriJucate++;
    noi->gm += scorNoi; noi->gp += scorAdv;
    adversar.gm += scorAdv; adversar.gp += scorNoi;

    if (scorNoi > scorAdv) {
        std::cout << "VICTORIE! (+3p, +50k EUR)\n";
        buget += 50000; moral += 5;
        noi->puncte += 3; noi->victorii++; adversar.infrangeri++;
    } else if (scorNoi == scorAdv) {
        std::cout << "EGAL. (+1p, +20k EUR)\n";
        buget += 20000;
        noi->puncte += 1; noi->egaluri++; adversar.puncte += 1; adversar.egaluri++;
    } else {
        std::cout << "INFRANGERE.\n";
        moral -= 10;
        noi->infrangeri++; adversar.victorii++; adversar.puncte += 3;
    }

    for(size_t i=1; i<liga.size(); ++i) {
        if((int)i != (etapa % (liga.size()-1) + 1)) {
            liga[i].meciuriJucate++;
            liga[i].puncte += rand() % 4;
            liga[i].gm += rand() % 3;
            liga[i].gp += rand() % 3;
        }
    }
    etapa++;
    std::cout << "\nApasa ENTER pentru a continua...";
    std::cin.ignore(); std::cin.get();
}

void afiseazaClasament() {
    clear();
    antet("CLASAMENT LIGA 1 - ETAPA " + std::to_string(etapa-1));
    std::sort(liga.begin(), liga.end(), [](const EchipaAI& a, const EchipaAI& b) {
        if(a.puncte != b.puncte) return a.puncte > b.puncte;
        return a.golaveraj() > b.golaveraj();
    });

    std::cout << std::left << std::setw(3) << "Loc" << std::setw(20) << "Echipa"
              << std::setw(4) << "MJ" << std::setw(4) << "V" << std::setw(4) << "E"
              << std::setw(4) << "I" << std::setw(6) << "Gol" << "Pct\n";
    std::cout << "--------------------------------------------------------------\n";

    for(size_t i=0; i<liga.size(); ++i) {
        std::cout << std::left << std::setw(3) << i+1
                  << std::setw(20) << liga[i].nume
                  << std::setw(4) << liga[i].meciuriJucate
                  << std::setw(4) << liga[i].victorii
                  << std::setw(4) << liga[i].egaluri
                  << std::setw(4) << liga[i].infrangeri
                  << std::setw(2) << liga[i].gm << "-" << std::setw(3) << liga[i].gp
                  << liga[i].puncte << "\n";
    }
    std::cout << "\nApasa ENTER...";
    std::cin.ignore(); std::cin.get();
}

int main() {
    srand(time(0));
    initJoc();

    try {
        Club club("FC Politehnica");

        std::ifstream fin("tastatura.txt");
        if (!fin.is_open()) {
            throw std::runtime_error("Fisierul 'tastatura.txt' nu a fost gasit! Asigura-te ca exista si contine datele.");
        }

        std::string tip;
        while(fin >> tip) {
            // Ignoram liniile de comentarii care incep cu #
            if (tip[0] == '#') {
                std::string dummy;
                std::getline(fin, dummy);
                continue;
            }

            std::string nume, prenume;
            fin >> nume >> prenume;
            if(tip == "JUCATOR") {
                int r, nr; std::string p;
                fin >> r >> p >> nr;

                Jucator* j = JucatorFactory::creazaJucator(nume, prenume, r, p, nr);

                club.adaugaMembru(*j);
                lotJucatori.push_back(j);
            }
            else if(tip == "ANTRENOR") {
                int ani, tr; fin >> ani >> tr;
                Antrenor* a = new Antrenor(nume, prenume, ani, tr);
                club.adaugaMembru(*a);
                staffTehnic.push_back(a);
            }
            else if(tip == "MEDIC") {
                std::string spec; int exp;
                fin >> spec >> exp;
                Medic* m = new Medic(nume, prenume, spec, exp);
                club.adaugaMembru(*m);
                staffMedical.push_back(m);
            }
            else if(tip == "ARBITRU") {
                int m; bool f; fin >> m >> f;
                club.adaugaMembru(Arbitru(nume, prenume, m, f));
            }
            else { std::string d; std::getline(fin, d); }
        }
        fin.close();

        sincronizeazaLot();

        bool ruleaza = true;
        while(ruleaza) {
            clear();
            std::cout << "##############################################################\n";
            std::cout << "         FOOTBALL MANAGER 2025 - ULTIMATE EDITION\n";
            std::cout << "##############################################################\n";
            std::cout << " MANAGER: Tu | BUGET: " << buget << " EUR | MORAL: " << moral << "% | ETAPA: " << etapa << "\n";
            std::cout << "--------------------------------------------------------------\n";
            std::cout << " [1] JOACA MECI (Etapa curenta)\n";
            std::cout << " [2] VESTIAR (Modifica primul 11)\n";
            std::cout << " [3] TRANSFERURI (Cumpara jucatori)\n";
            std::cout << " [4] CLASAMENT LIGA 1\n";
            std::cout << " [5] BIROU (Detalii Club, Palmares, Staff, Medic)\n";
            std::cout << " [6] SALVEAZA & IESI\n";
            std::cout << "--------------------------------------------------------------\n";
            std::cout << "Selecteaza actiunea: ";

            int cmd; std::cin >> cmd;

            switch(cmd) {
                case 1: joacaEtapa(); break;
                case 2: meniuTactica(); break;
                case 3: meniuTransferuri(club); sincronizeazaLot(); break;
                case 4: afiseazaClasament(); break;
                case 5: meniuDetaliiClub(club); break;
                case 6:
                    club.salveazaInFisier();
                    std::cout << "Salvare realizata. La revedere!\n";
                    ruleaza = false;
                    break;
                default:
                    std::cout << "Comanda invalida!\n"; sleepMs(500);
            }
        }

        for(auto* p : lotJucatori) delete p;

    } catch (const std::exception& e) {
        std::cerr << "[CRASH] " << e.what() << "\n";
        return 1;
    }

    return 0;
}