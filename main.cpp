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
#include <numeric>
#include <functional>

#include "Exceptii/exceptii.h"
#include "Persoane/persoane.h"
#include "Persoane/jucator.h"
#include "Persoane/antrenor.h"
#include "Persoane/arbitru.h"
#include "Club/club.h"
#include "Utils/istoric.h"

namespace Core {

    constexpr long long BUGET_INITIAL = 2500000;
    constexpr int MORAL_INITIAL = 80;
    constexpr int BONUS_VICTORIE = 50000;
    constexpr int BONUS_EGAL = 20000;

    class Finante {
    private:
        long long balans;
        std::vector<std::string> istoricTranzactii;

    public:
        Finante(long long start = BUGET_INITIAL) : balans(start) {}

        void incaseaza(long long suma, const std::string& sursa) {
            balans += suma;
            istoricTranzactii.push_back("[INCASARE] " + sursa + ": +" + std::to_string(suma));
        }

        void plateste(long long suma, const std::string& destinatie) {
            if (suma > balans) {
                throw std::runtime_error("Fonduri insuficiente pentru: " + destinatie);
            }
            balans -= suma;
            istoricTranzactii.push_back("[PLATA] " + destinatie + ": -" + std::to_string(suma));
        }

        long long getBalans() const { return balans; }

        void afiseazaRaportFinanciar() const {
            std::cout << "\n--- RAPORT FINANCIAR ---\n";
            std::cout << "Balans curent: " << balans << " EUR\n";
            std::cout << "Ultimele tranzactii:\n";
            for (auto it = istoricTranzactii.rbegin(); it != istoricTranzactii.rend(); ++it) {
                std::cout << *it << "\n";
                if (std::distance(istoricTranzactii.rbegin(), it) > 4) break;
            }
            std::cout << "------------------------\n";
        }
    };

    struct Performanta {
        int goluri = 0;
        int meciuri = 0;
        int cartonase = 0;
    };

    class Statistica {
    private:
        std::map<std::string, Performanta> date;

    public:
        void inregistreazaMeci(const std::string& numeJucator) {
            date[numeJucator].meciuri++;
        }

        void inregistreazaGol(const std::string& numeJucator) {
            date[numeJucator].goluri++;
        }

        void afiseazaTopScoreri() const {
            std::vector<std::pair<std::string, int>> top;
            for (const auto& [nume, perf] : date) {
                if (perf.goluri > 0) top.push_back({nume, perf.goluri});
            }

            std::sort(top.begin(), top.end(), [](const auto& a, const auto& b) {
                return a.second > b.second;
            });

            std::cout << "\n--- TOP GOLGHETERI ---\n";
            if (top.empty()) std::cout << "Niciun gol marcat inca.\n";
            else {
                for (const auto& p : top) {
                    std::cout << std::left << std::setw(20) << p.first << ": " << p.second << " goluri\n";
                }
            }
        }
    };

    class ScoutingNetwork {
    private:
        std::vector<Jucator*> piataTransferuri;

    public:
        ~ScoutingNetwork() {
            for (auto* p : piataTransferuri) delete p;
        }

        void adaugaJucator(Jucator* j) {
            piataTransferuri.push_back(j);
        }

        void genereazaPiataRandom() {
            for (auto* p : piataTransferuri) delete p;
            piataTransferuri.clear();

            std::vector<std::string> nume = {"Santos", "Kowalski", "Muller", "Popa", "Ionescu", "Smith", "Yamal"};
            std::vector<std::string> pozitii = {"ST", "CAM", "CDM", "CB", "GK"};

            for (int i = 0; i < 5; ++i) {
                std::string n = nume[rand() % nume.size()];
                std::string p = "Scouted";
                std::string poz = pozitii[rand() % pozitii.size()];
                int rtg = 65 + rand() % 30;

                piataTransferuri.push_back(new Jucator(n, p, rtg, poz, 0));
            }
        }

        std::vector<Jucator*> cautaJucatori(std::function<bool(Jucator*)> criteriu) {
            std::vector<Jucator*> rezultate;
            std::copy_if(piataTransferuri.begin(), piataTransferuri.end(), std::back_inserter(rezultate), criteriu);
            return rezultate;
        }

        void eliminaJucator(Jucator* j) {
            auto it = std::find(piataTransferuri.begin(), piataTransferuri.end(), j);
            if (it != piataTransferuri.end()) {
                piataTransferuri.erase(it);
            }
        }
    };
}

using namespace Core;

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

Finante managerFinanciar;
Statistica statsJucatori;
ScoutingNetwork scouts;
int moral = MORAL_INITIAL;
int etapa = 1;

std::vector<Jucator*> lotJucatori;
std::vector<Jucator*> titulari;
std::vector<Jucator*> rezerve;
std::vector<EchipaAI> liga;
DetaliiClub detaliiMele;
std::vector<Antrenor*> staffTehnic;
std::vector<Medic*> staffMedical;
Istoric<std::string> istoricMeciuri(50);

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
    scouts.genereazaPiataRandom();
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

    int suma = std::accumulate(titulari.begin(), titulari.end(), 0,
        [](int acc, Jucator* p) { return acc + p->calculeazaEficienta(); });

    int media = suma / static_cast<int>(titulari.size());
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
    antet("SCOUTING NETWORK");
    std::cout << "Buget disponibil: " << managerFinanciar.getBalans() << " EUR\n\n";

    auto jucatoriGasiti = scouts.cautaJucatori([](Jucator* j) {
        return j->getRating() > 0;
    });

    std::cout << "Scoutii au identificat urmatorii jucatori:\n";
    for(size_t i=0; i<jucatoriGasiti.size(); ++i) {
        int pret = jucatoriGasiti[i]->getRating() * jucatoriGasiti[i]->getRating() * 150;
        std::cout << i+1 << ". " << jucatoriGasiti[i]->getNumeComplet()
                  << " (" << jucatoriGasiti[i]->getPozitie() << ") | Rtg: "
                  << jucatoriGasiti[i]->getRating() << " | Pret: " << pret << " EUR\n";
    }

    std::cout << "\n0. Inapoi la meniu\n";
    std::cout << "9. Reimprospateaza Piata (Trimite scouti in alte tari)\n";
    std::cout << "Alege jucatorul de cumparat: ";
    int opt; std::cin >> opt;

    if (opt == 9) {
        scouts.genereazaPiataRandom();
        std::cout << "Piata a fost actualizata!\n";
        sleepMs(1000);
        return;
    }

    if (opt >= 1 && opt <= (int)jucatoriGasiti.size()) {
        int idx = opt - 1;
        Jucator* tinta = jucatoriGasiti[idx];
        int pret = tinta->getRating() * tinta->getRating() * 150;

        try {
            managerFinanciar.plateste(pret, "Transfer " + tinta->getNumeComplet());

            std::cout << "Negocieri reusite! " << tinta->getNumeComplet() << " a semnat.\n";

            Jucator* copie = tinta->clone();
            club.adaugaMembru(*copie);
            lotJucatori.push_back(copie);

            scouts.eliminaJucator(tinta);

            sincronizeazaLot();
            sleepMs(1500);

        } catch (const std::runtime_error& e) {
            std::cout << "Eroare Transfer: " << e.what() << "\n";
            sleepMs(2000);
        }
    }
}

void meniuDetaliiClub(const Club& club) {
    clear();
    antet("DETALII CLUB & INFRASTRUCTURA");
    std::cout << "Nume Stadion: " << detaliiMele.numeStadion << " (Capacitate: " << detaliiMele.capacitateStadion << ")\n";
    std::cout << "An infiintare: " << detaliiMele.anInfiintare << "\n";

    managerFinanciar.afiseazaRaportFinanciar();

    std::cout << "STAFF TEHNIC:\n";
    for(auto* a : staffTehnic) std::cout << " - " << a->getNumeComplet() << "\n";

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

    for(auto* j : titulari) statsJucatori.inregistreazaMeci(j->getNumeComplet());

    int scorNoi = 0, scorAdv = 0;
    int minuteEvenimente[] = {12, 25, 38, 55, 67, 82, 89};

    for (int min : minuteEvenimente) {
        std::cout << "Min " << min << ": ";
        int sansa = rand() % 100;
        int avantaj = ratingNoi - ratingAdv;

        if (sansa < 40) std::cout << "Joc calm la mijlocul terenului...\n";
        else if (sansa < 70) {
            if (rand() % 100 < (40 + avantaj)) {
                std::string marcator = "Jucator Necunoscut";
                if(!titulari.empty()) marcator = titulari[rand() % titulari.size()]->getNumeComplet();

                std::cout << "GOOOOL POLITEHNICA! Inscrie " << marcator << "!\n";
                scorNoi++;
                statsJucatori.inregistreazaGol(marcator);

            } else std::cout << "Ratare imensa pentru noi! Bara!\n";
        } else {
            if (rand() % 100 < (40 - avantaj)) {
                std::cout << "GOL " << adversar.nume << ". Apararea noastra a cedat.\n";
                scorAdv++;
            } else std::cout << "Interventie salvatoare a portarului nostru!\n";
        }
        sleepMs(1500);

        if (rand() % 100 < 5 && !staffMedical.empty()) {
            std::cout << "   [!] Un jucator s-a accidentat. Dr. " << staffMedical[0]->getNumeComplet()
                      << " intra pe teren pentru ingrijiri.\n";
            sleepMs(1000);
            std::cout << "   [OK] Jucatorul revine in joc.\n";
        }
    }

    std::cout << "\n----------------------------------------\n";
    std::cout << "SCOR FINAL: " << scorNoi << " - " << scorAdv << "\n";
    std::cout << "----------------------------------------\n";

    std::string rezultatMeci = "Etapa " + std::to_string(etapa) + ": " + std::to_string(scorNoi) + "-" + std::to_string(scorAdv) + " vs " + adversar.nume;
    istoricMeciuri.adaugaEveniment(rezultatMeci);

    noi->meciuriJucate++; adversar.meciuriJucate++;
    noi->gm += scorNoi; noi->gp += scorAdv;
    adversar.gm += scorAdv; adversar.gp += scorNoi;

    if (scorNoi > scorAdv) {
        std::cout << "VICTORIE! (+3p, +" << BONUS_VICTORIE << " EUR)\n";
        managerFinanciar.incaseaza(BONUS_VICTORIE, "Premiu Victorie");
        moral += 5;
        noi->puncte += 3; noi->victorii++; adversar.infrangeri++;
    } else if (scorNoi == scorAdv) {
        std::cout << "EGAL. (+1p, +" << BONUS_EGAL << " EUR)\n";
        managerFinanciar.incaseaza(BONUS_EGAL, "Premiu Egal");
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

void meniuStatistici() {
    clear();
    antet("CENTRUL DE STATISTICA");
    std::cout << "1. Top Golgheteri\n";
    std::cout << "2. Istoric Rezultate\n";
    std::cout << "Alege: ";
    int opt; std::cin >> opt;

    if(opt == 1) statsJucatori.afiseazaTopScoreri();
    else if(opt == 2) istoricMeciuri.afiseazaIstoric();

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
            std::ofstream fout("tastatura.txt");
            fout << "JUCATOR Nita Florin 82 GK 1\n"
                 << "JUCATOR Ratiu Andrei 80 RB 2\n"
                 << "JUCATOR Dragusin Radu 84 CB 3\n"
                 << "JUCATOR Burca Andrei 78 CB 4\n"
                 << "JUCATOR Bancu Nicusor 79 LB 11\n"
                 << "JUCATOR Marin Razvan 81 CM 6\n"
                 << "JUCATOR Stanciu Nicolae 83 CAM 10\n"
                 << "JUCATOR Man Dennis 82 RW 20\n"
                 << "JUCATOR Mihaila Valentin 80 LW 22\n"
                 << "JUCATOR Dragus Denis 79 ST 9\n"
                 << "JUCATOR Hagi Ianis 80 CAM 14\n"
                 << "JUCATOR Moldovan Horatiu 78 GK 12\n"
                 << "JUCATOR Puscas George 75 ST 25\n"
                 << "ANTRENOR Iordanescu Edward 45 3\n"
                 << "MEDIC Popescu Ion Ortopedie 15\n"
                 << "MEDIC Ionescu Maria Cardiologie 10\n"
                 << "ARBITRU Hategan Ovidiu 200 1\n";
            fout.close();
            fin.open("tastatura.txt");
        }

        std::string tip;
        while(fin >> tip) {
            if(tip[0] == '#') { std::string d; std::getline(fin, d); continue; }

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
            std::cout << " MANAGER: Tu | BUGET: " << managerFinanciar.getBalans() << " EUR | MORAL: " << moral << "% | ETAPA: " << etapa << "\n";
            std::cout << "--------------------------------------------------------------\n";
            std::cout << " [1] JOACA MECI (Etapa curenta)\n";
            std::cout << " [2] VESTIAR (Modifica primul 11)\n";
            std::cout << " [3] SCOUTING & TRANSFERURI\n";
            std::cout << " [4] CLASAMENT LIGA 1\n";
            std::cout << " [5] BIROU (Detalii Club, Finante, Staff)\n";
            std::cout << " [6] STATISTICI (Golgheteri & Istoric)\n";
            std::cout << " [7] SALVEAZA & IESI\n";
            std::cout << "--------------------------------------------------------------\n";
            std::cout << "Selecteaza actiunea: ";

            int cmd; std::cin >> cmd;

            switch(cmd) {
                case 1: joacaEtapa(); break;
                case 2: meniuTactica(); break;
                case 3: meniuTransferuri(club); sincronizeazaLot(); break;
                case 4: afiseazaClasament(); break;
                case 5: meniuDetaliiClub(club); break;
                case 6: meniuStatistici(); break;
                case 7:
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