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
#include <optional>
#include <random> // Necesara pentru std::shuffle

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
            if (suma > balans) throw std::runtime_error("Fonduri insuficiente: " + destinatie);
            balans -= suma;
            istoricTranzactii.push_back("[PLATA] " + destinatie + ": -" + std::to_string(suma));
        }
        long long getBalans() const { return balans; }
        void afiseazaRaportFinanciar() const {
            std::cout << "\n--- RAPORT FINANCIAR ---\n";
            std::cout << "Balans: " << balans << " EUR\n";
            for (auto it = istoricTranzactii.rbegin(); it != istoricTranzactii.rend(); ++it) {
                std::cout << *it << "\n";
                if (std::distance(istoricTranzactii.rbegin(), it) > 4) break;
            }
        }
    };

    struct Performanta {
        int goluri = 0;
        int meciuri = 0;
    };

    class Statistica {
    private:
        std::map<std::string, Performanta> date;
    public:
        void inregistreazaMeci(const std::string& nume) { date[nume].meciuri++; }
        void inregistreazaGol(const std::string& nume) { date[nume].goluri++; }
        void afiseazaTopScoreri() const {
            std::vector<std::pair<std::string, int>> top;
            for (const auto& [n, p] : date) if (p.goluri > 0) top.push_back({n, p.goluri});
            std::sort(top.begin(), top.end(), [](const auto& a, const auto& b) { return a.second > b.second; });
            std::cout << "\n--- TOP GOLGHETERI ---\n";
            for (const auto& p : top) std::cout << std::left << std::setw(20) << p.first << ": " << p.second << " goluri\n";
        }
    };

    class ScoutingNetwork {
    private:
        std::vector<Jucator*> piata;
    public:
        ~ScoutingNetwork() { for (auto* p : piata) delete p; }
        void adaugaJucator(Jucator* j) { piata.push_back(j); }
        std::vector<Jucator*> cautaJucatori(std::function<bool(Jucator*)> f) {
            std::vector<Jucator*> rez;
            std::copy_if(piata.begin(), piata.end(), std::back_inserter(rez), f);
            return rez;
        }
        void eliminaJucator(Jucator* j) {
            auto it = std::find(piata.begin(), piata.end(), j);
            if (it != piata.end()) piata.erase(it);
        }
        void genereazaRandom() {
            std::vector<std::string> n = {"Santos", "Kowalski", "Muller", "Popa"};
            std::vector<std::string> p = {"ST", "CB", "GK"};
            for(int i=0; i<3; ++i) piata.push_back(new Jucator(n[rand()%4], "Scout", 70+rand()%20, p[rand()%3], 0));
        }
    };
}
using namespace Core;

class JucatorFactory {
public:
    static Jucator* creazaJucator(const std::string& n, const std::string& p, int r, const std::string& poz, int nr) {
        if (poz == "GK") return new Portar(n, p, r, poz, nr);
        if (poz == "CB") return new FundasCentral(n, p, r, poz, nr);
        if (poz == "LB") return new FundasStanga(n, p, r, poz, nr);
        if (poz == "RB") return new FundasDreapta(n, p, r, poz, nr);
        if (poz == "LWB" || poz == "RWB") return new Fundas(n, p, r, poz, nr);
        if (poz == "CM") return new MijlocasCentral(n, p, r, poz, nr);
        if (poz == "CDM") return new MijlocasDefensiv(n, p, r, poz, nr);
        if (poz == "CAM") return new MijlocasOfensiv(n, p, r, poz, nr);
        if (poz == "LM") return new MijlocasStanga(n, p, r, poz, nr);
        if (poz == "RM") return new MijlocasDreapta(n, p, r, poz, nr);
        if (poz == "ST") return new AtacantCentral(n, p, r, poz, nr);
        if (poz == "LW") return new AtacantStanga(n, p, r, poz, nr);
        if (poz == "RW") return new AtacantDreapta(n, p, r, poz, nr);
        return new Jucator(n, p, r, poz, nr);
    }
};

class Medic : public Persoana {
    std::string specializare;
    int aniExperienta;
public:
    Medic(const std::string& n, const std::string& p, const std::string& s, int e)
        : Persoana(n, p), specializare(s), aniExperienta(e) {}
    Medic* clone() const override { return new Medic(*this); }
    int calculeazaEficienta() const override { return 100; }
protected:
    void afisareDetaliata(std::ostream& os) const override {
        os << "Medic: " << specializare << " (" << aniExperienta << " ani exp)\n";
    }
};

struct EchipaAI {
    std::string nume;
    int rating;
    int puncte;
    int mj, v, e, i, gm, gp;
};

Finante managerFinanciar;
Statistica stats;
ScoutingNetwork scouts;
int moral = MORAL_INITIAL;
int etapa = 1;

std::vector<Jucator*> lotJucatori;
std::vector<Jucator*> titulari;
std::vector<Jucator*> rezerve;
std::vector<EchipaAI> liga;
std::vector<Antrenor*> staffTehnic;
std::vector<Medic*> staffMedical;
Istoric<std::string> istoricMeciuri;

void clear() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
    for(int i=0; i<50; ++i) std::cout << "\n";
}
void sleepMs(int ms) { std::this_thread::sleep_for(std::chrono::milliseconds(ms)); }

class ClubBuilder {
private:
    Club* club;
    std::vector<Jucator*>& destinatieLot;
public:
    ClubBuilder(const std::string& nume, std::vector<Jucator*>& lotRef) : destinatieLot(lotRef) {
        club = new Club(nume);
    }

    ClubBuilder& adaugaJucator(Jucator* j) {
        club->adaugaMembru(*j);
        destinatieLot.push_back(j);
        return *this;
    }

    ClubBuilder& adaugaStaff(Persoana* p) {
        club->adaugaMembru(*p);
        if(auto* a = dynamic_cast<Antrenor*>(p)) staffTehnic.push_back(a);
        if(auto* m = dynamic_cast<Medic*>(p)) staffMedical.push_back(m);
        return *this;
    }

    Club* build() { return club; }
};

std::optional<Jucator*> cautaJucatorInPool(const std::vector<Jucator*>& pool, int index) {
    if(index >= 0 && index < pool.size()) return pool[index];
    return std::nullopt;
}

void initLiga(const std::string& numeEchipaMea) {
    liga = {
        {numeEchipaMea, 0, 0, 0, 0, 0, 0, 0, 0},
        {"FCSB", 85, 0, 0, 0, 0, 0, 0, 0},
        {"CFR Cluj", 87, 0, 0, 0, 0, 0, 0, 0},
        {"Univ. Craiova", 84, 0, 0, 0, 0, 0, 0, 0},
        {"Rapid", 83, 0, 0, 0, 0, 0, 0, 0},
        {"Dinamo", 79, 0, 0, 0, 0, 0, 0, 0},
        {"Farul", 81, 0, 0, 0, 0, 0, 0, 0},
        {"Sepsi", 78, 0, 0, 0, 0, 0, 0, 0}
    };
    scouts.genereazaRandom();
}

void sincronizeazaLot() {
    titulari.clear();
    rezerve.clear();
    for(size_t i=0; i<lotJucatori.size(); ++i) {
        if(i < 11) titulari.push_back(lotJucatori[i]);
        else rezerve.push_back(lotJucatori[i]);
    }
}

int calculeazaRating() {
    if(titulari.empty()) return 50;
    int s = std::accumulate(titulari.begin(), titulari.end(), 0, [](int a, Jucator* b){ return a + b->calculeazaEficienta(); });
    return s / titulari.size() + (moral - 50)/10;
}

void joacaMeci() {
    clear();
    // 1. Stabilim adversarul pentru echipa jucatorului
    int advIndex = (etapa % (liga.size() - 1)) + 1;
    EchipaAI& adv = liga[advIndex];
    EchipaAI* noi = &liga[0]; // Noi suntem mereu la index 0

    int rNoi = calculeazaRating();
    int rAdv = adv.rating + (rand()%10 - 5);

    // --- MECIUL PROPRIU ---
    std::cout << "\n=============================================\n";
    std::cout << "   MATCH DAY: " << noi->nume << " vs " << adv.nume << "\n";
    std::cout << "   Rating estimat: " << rNoi << " vs " << rAdv << "\n";
    std::cout << "=============================================\n";
    sleepMs(1500);

    int s1 = 0, s2 = 0;

    // Simulare pe minute (0-90)
    for(int min = 0; min <= 90; min += (rand()%10 + 5)) {
        std::cout << "Min " << std::setw(2) << min << ": ";

        int sansa = 50 + (rNoi - rAdv)/2; // Sansa ajustata
        int zar = rand() % 100;

        if (zar < 15) { // 15% sansa de faza periculoasa
            if (rand() % 100 < 40) { // 40% sansa sa fie gol daca e faza periculoasa
                if (rand() % 100 < sansa) {
                    std::string marcator = "Un jucator";
                    if (!titulari.empty()) marcator = titulari[rand() % titulari.size()]->getNumeComplet();
                    std::cout << "GOOOOL " << noi->nume << "! Inscrie " << marcator << "!\n";
                    s1++;
                    stats.inregistreazaGol(marcator);
                } else {
                    std::cout << "GOL " << adv.nume << "! Contraatac letal.\n";
                    s2++;
                }
            } else {
                if (rand() % 100 < sansa) std::cout << "Ocazie imensa " << noi->nume << "! BARA!\n";
                else std::cout << "Ocazie " << adv.nume << "! Portarul respinge.\n";
            }
        } else if (zar < 35) {
            if (rand() % 100 < sansa) std::cout << noi->nume << " controleaza posesia.\n";
            else std::cout << adv.nume << " preseaza agresiv.\n";
        } else if (zar < 50) {
            std::cout << "Joc echilibrat la mijlocul terenului.\n";
        } else {
            std::string atmo[] = {"Ce meci intens!", "Publicul este in picioare.", "Ritm bun de joc.", "Suporterii canta."};
            std::cout << atmo[rand() % 4] << "\n";
        }
        sleepMs(400);
    }

    std::cout << "\n---------------------------------------------\n";
    std::cout << "FLUIER FINAL: " << s1 << " - " << s2 << "\n";
    std::cout << "---------------------------------------------\n";

    istoricMeciuri.adaugaEveniment("Etapa " + std::to_string(etapa) + ": " + std::to_string(s1) + "-" + std::to_string(s2) + " vs " + adv.nume);

    noi->mj++; adv.mj++; noi->gm+=s1; noi->gp+=s2; adv.gm+=s2; adv.gp+=s1;
    if(s1 > s2) {
        noi->puncte+=3; noi->v++; adv.i++; managerFinanciar.incaseaza(BONUS_VICTORIE, "Victorie"); moral = std::min(100, moral + 5);
        std::cout << "VICTORIE! (+3p)\n";
    } else if(s1 == s2) {
        noi->puncte+=1; noi->e++; adv.e++; managerFinanciar.incaseaza(BONUS_EGAL, "Egal");
        std::cout << "EGAL. (+1p)\n";
    } else {
        noi->i++; adv.v++; adv.puncte+=3; moral = std::max(0, moral - 10);
        std::cout << "INFRANGERE.\n";
    }

    // --- SIMULARE RESTUL MECIURILOR ---
    std::cout << "\n[REZULTATE ALTE MECIURI]\n";

    // Identificam echipele care nu au jucat inca
    std::vector<int> echipeLibere;
    for(size_t i = 1; i < liga.size(); ++i) {
        if((int)i != advIndex) echipeLibere.push_back(i);
    }

    // Le amestecam pentru diversitate
    std::shuffle(echipeLibere.begin(), echipeLibere.end(), std::default_random_engine(rand()));

    // Le imperechem
    for(size_t i = 0; i < echipeLibere.size(); i += 2) {
        if(i + 1 >= echipeLibere.size()) break; // Siguranta

        int idxA = echipeLibere[i];
        int idxB = echipeLibere[i+1];
        EchipaAI& teamA = liga[idxA];
        EchipaAI& teamB = liga[idxB];

        // Generam scor bazat pe rating
        int diff = teamA.rating - teamB.rating;
        int sansaA = 50 + diff;
        int goluriA = 0, goluriB = 0;

        // Simulam cateva sanse
        for(int k=0; k<5; ++k) {
            if(rand()%100 < 35) { // sansa de gol in meci
                if(rand()%100 < sansaA) goluriA++;
                else goluriB++;
            }
        }

        std::cout << std::left << std::setw(15) << teamA.nume << goluriA << " - " << goluriB << " " << teamB.nume << "\n";

        // Actualizare Clasament
        teamA.mj++; teamB.mj++;
        teamA.gm += goluriA; teamA.gp += goluriB;
        teamB.gm += goluriB; teamB.gp += goluriA;

        if(goluriA > goluriB) { teamA.v++; teamB.i++; teamA.puncte += 3; }
        else if(goluriA == goluriB) { teamA.e++; teamB.e++; teamA.puncte += 1; teamB.puncte += 1; }
        else { teamA.i++; teamB.v++; teamB.puncte += 3; }
    }

    etapa++;
    std::cout << "\nApasa ENTER pentru a continua..."; std::cin.ignore(); std::cin.get();
}

void afiseazaClasament() {
    clear();
    std::sort(liga.begin(), liga.end(), [](const auto& a, const auto& b){
        return a.puncte != b.puncte ? a.puncte > b.puncte : (a.gm-a.gp) > (b.gm-b.gp);
    });
    std::cout << "LOC ECHIPA PCT GM-GP\n";
    for(size_t i=0; i<liga.size(); ++i)
        std::cout << i+1 << ". " << std::left << std::setw(15) << liga[i].nume << liga[i].puncte << " " << liga[i].gm << "-" << liga[i].gp << "\n";
    std::cout << "\nApasa ENTER..."; std::cin.ignore(); std::cin.get();
}

void meniuTactica() {
    clear();
    std::cout << "TITULARI:\n";
    for(size_t i=0; i<titulari.size(); ++i) std::cout << i+1 << ". " << titulari[i]->getNumeComplet() << "\n";
    std::cout << "\nREZERVE:\n";
    for(size_t i=0; i<rezerve.size(); ++i) std::cout << i+1+11 << ". " << rezerve[i]->getNumeComplet() << "\n";

    int t, r;
    std::cout << "\nSchimba (Titular Rezerva, ex: 1 12): ";
    if(std::cin >> t >> r) {
        if(t > 0 && t <= titulari.size() && r > 11 && r <= 11+rezerve.size()) {
            std::iter_swap(lotJucatori.begin() + t - 1, lotJucatori.begin() + r - 1);
            sincronizeazaLot();
        }
    }
}

void meniuTransfer() {
    clear();
    std::cout << "Buget: " << managerFinanciar.getBalans() << "\n";
    auto gasiti = scouts.cautaJucatori([](auto*){ return true; });
    for(size_t i=0; i<gasiti.size(); ++i)
        std::cout << i+1 << ". " << gasiti[i]->getNumeComplet() << " (" << gasiti[i]->getRating() << ") - " << gasiti[i]->getRating()*1000 << " EUR\n";

    int opt; std::cout << "Cumpara: "; std::cin >> opt;
    if(opt > 0 && opt <= gasiti.size()) {
        Jucator* t = gasiti[opt-1];
        int pret = t->getRating()*1000;
        try {
            managerFinanciar.plateste(pret, "Transfer");
            lotJucatori.push_back(t->clone());
            scouts.eliminaJucator(t);
            sincronizeazaLot();
            std::cout << "Transfer reusit!\n";
        } catch(...) { std::cout << "Fonduri insuficiente!\n"; }
        sleepMs(1000);
    }
}

int main() {
    srand(time(0));
    std::vector<Jucator*> databaseJucatori;
    std::vector<Persoana*> databaseStaff;

    std::ifstream fin("tastatura.txt");
    if (!fin.is_open()) {
        std::cerr << "Lipsa fisier tastatura.txt! Asigura-te ca este in acelasi folder cu executabilul.\n";
        return 1;
    }

    std::string tip;
    while(fin >> tip) {
        if(tip.empty() || tip[0]=='#') {
            std::string d; std::getline(fin, d);
            continue;
        }

        try {
            std::string n, p;
            if (!(fin >> n >> p)) break;

            if(tip=="JUCATOR") {
                int r, nr; std::string poz;
                if (!(fin >> r >> poz >> nr)) throw std::runtime_error("Format jucator invalid");
                databaseJucatori.push_back(JucatorFactory::creazaJucator(n, p, r, poz, nr));
            } else if(tip=="ANTRENOR") {
                int a, t;
                if (!(fin >> a >> t)) throw std::runtime_error("Format antrenor invalid");
                databaseStaff.push_back(new Antrenor(n, p, a, t));
            } else if(tip=="MEDIC") {
                std::string s; int e;
                if (!(fin >> s >> e)) throw std::runtime_error("Format medic invalid");
                databaseStaff.push_back(new Medic(n, p, s, e));
            } else if(tip=="ARBITRU") {
                int m; bool f;
                if (!(fin >> m >> f)) throw std::runtime_error("Format arbitru invalid");
                databaseStaff.push_back(new Arbitru(n, p, m, f));
            } else {
                std::string d; std::getline(fin, d);
            }
        } catch (const std::exception& e) {
            std::cerr << "[ATENTIE] Linie corupta ignorata (" << tip << "): " << e.what() << "\n";
            fin.clear();
            std::string d; std::getline(fin, d);
        }
    }
    fin.close();

    std::cout << "1. Start Rapid (FC Politehnica)\n2. Creeaza Club Propriu (Draft Mode)\nAlege: ";
    int modJoc;
    if(!(std::cin >> modJoc)) modJoc = 1;

    std::string numeClub = "FC Politehnica";
    ClubBuilder* builder = nullptr;

    if (modJoc == 2) {
        clear();
        std::cout << "Numele noului club: ";
        std::cin.ignore(); std::getline(std::cin, numeClub);
        builder = new ClubBuilder(numeClub, lotJucatori);

        int filtru = 0;
        while(true) {
            clear();
            std::string numeFiltru = (filtru==0?"Toti":(filtru==1?"Portari":(filtru==2?"Fundasi":(filtru==3?"Mijlocasi":"Atacanti"))));
            std::cout << "\n--- DRAFT JUCATORI (" << lotJucatori.size() << "/15) ---\n";
            std::cout << "Se afiseaza doar: " << numeFiltru << "\n";
            std::cout << "-------------------------------------------\n";

            int contor = 0;
            for(size_t i=0; i<databaseJucatori.size(); ++i) {
                if(!databaseJucatori[i]) continue;

                std::string poz = databaseJucatori[i]->getPozitie();
                bool afiseaza = false;
                if(filtru == 0) afiseaza = true;
                else if(filtru == 1 && poz == "GK") afiseaza = true;
                else if(filtru == 2 && (poz == "CB" || poz == "LB" || poz == "RB" || poz == "LWB" || poz == "RWB")) afiseaza = true;
                else if(filtru == 3 && (poz == "CM" || poz == "CDM" || poz == "CAM" || poz == "LM" || poz == "RM")) afiseaza = true;
                else if(filtru == 4 && (poz == "ST" || poz == "CF" || poz == "LW" || poz == "RW")) afiseaza = true;

                if(afiseaza) {
                    std::cout << i+1 << ". " << databaseJucatori[i]->getNumeComplet()
                              << " [" << poz << "] Rtg:" << databaseJucatori[i]->getRating() << "\n";
                    contor++;
                }
            }
            if(contor == 0) std::cout << "(Niciun jucator gasit pentru acest filtru)\n";

            std::cout << "-------------------------------------------\n";
            if(lotJucatori.size() >= 11) std::cout << "0. START SEZON (Termina selectia)\n";
            std::cout << "99. SCHIMBA FILTRU\n";

            int idx; std::cout << "\nAlege ID (sau 99 pt Filtru): ";
            if(!(std::cin >> idx)) {
                std::cin.clear(); std::cin.ignore(1000, '\n');
                continue;
            }

            if(idx == 0 && lotJucatori.size() >= 11) break;
            if(idx == 99) {
                std::cout << "\nFiltre disponibile:\n1. Portari (GK)\n2. Fundasi (CB, LB, RB, WB)\n3. Mijlocasi (CM, CDM, CAM, LM, RM)\n4. Atacanti (ST, LW, RW, CF)\n0. Toti Jucatorii\nAlege Filtru: ";
                std::cin >> filtru;
                continue;
            }

            auto jucatorOpt = cautaJucatorInPool(databaseJucatori, idx-1);
            if(jucatorOpt.has_value() && jucatorOpt.value() != nullptr) {
                builder->adaugaJucator(jucatorOpt.value());
                databaseJucatori[idx-1] = nullptr;
                std::cout << ">> Jucator adaugat!\n"; sleepMs(500);
            }
        }
        for(auto* s : databaseStaff) builder->adaugaStaff(s);

    } else {
        builder = new ClubBuilder(numeClub, lotJucatori);
        for(auto* j : databaseJucatori) if(j) builder->adaugaJucator(j);
        for(auto* s : databaseStaff) if(s) builder->adaugaStaff(s);
    }

    Club* club = builder->build();
    initLiga(numeClub);
    sincronizeazaLot();

    bool ruleaza = true;
    while(ruleaza) {
        clear();
        std::cout << "MANAGER: " << numeClub << " | ETAPA: " << etapa << " | BUGET: " << managerFinanciar.getBalans() << "\n";
        std::cout << "1. Joaca\n2. Tactica\n3. Transfer\n4. Clasament\n5. Detalii\n6. Exit\nCmd: ";
        int c;
        if(!(std::cin >> c)) {
            std::cin.clear(); std::cin.ignore(1000, '\n'); continue;
        }

        switch(c) {
            case 1: joacaMeci(); break;
            case 2: meniuTactica(); break;
            case 3: meniuTransfer(); break;
            case 4: afiseazaClasament(); break;
            case 5: club->afiseazaMembri(); std::cout << "\nWait..."; std::cin.ignore(); std::cin.get(); break;
            case 6: ruleaza=false; break;
        }
    }

    delete builder;
    delete club;
    for(auto* p : databaseJucatori) if(p) delete p;

    return 0;
}