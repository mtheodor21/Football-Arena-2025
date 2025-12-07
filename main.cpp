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
#include <random>
#include <deque>

#include "Exceptii/exceptii.h"
#include "Persoane/persoane.h"
#include "Persoane/jucator.h"
#include "Persoane/antrenor.h"
#include "Persoane/arbitru.h"
#include "Club/club.h"
#include "Utils/istoric.h"

// ============================================================================
// CORE NAMESPACE
// ============================================================================
namespace Core {
    constexpr long long BUGET_INITIAL = 4000000;
    constexpr int MORAL_INITIAL = 75;
    constexpr int INCREDERE_INITIALA = 80;
    constexpr int BONUS_VICTORIE = 100000;
    constexpr int BONUS_EGAL = 30000;
    constexpr int COST_SCOUTING_EU = 50000;
    constexpr int COST_SCOUTING_SA = 120000;

    enum class StilJoc { POSESIE, CONTRAATAC, PARK_THE_BUS, GEGENPRESS, TIKI_TAKA };
    enum class Formatie { F442, F433, F352, F532, F343 };

    struct Tactica {
        Formatie formatie = Formatie::F433;
        StilJoc stil = StilJoc::POSESIE;
        int agresivitate = 50;

        std::string getNumeFormatie() const {
            switch(formatie) {
                case Formatie::F442: return "4-4-2";
                case Formatie::F433: return "4-3-3";
                case Formatie::F352: return "3-5-2";
                default: return "5-4-1";
            }
        }
        std::vector<int> getCerinte() const {
            switch(formatie) {
                case Formatie::F442: return {4, 4, 2};
                case Formatie::F433: return {4, 3, 3};
                case Formatie::F352: return {3, 5, 2};
                default: return {5, 4, 1};
            }
        }
    };

    class NewsFeed {
        std::deque<std::string> stiri;
    public:
        void adaugaStire(const std::string& stire) {
            stiri.push_front("> " + stire);
            if(stiri.size() > 8) stiri.pop_back();
        }
        void afiseaza() const {
            std::cout << "\n=== NEWS FEED ===\n";
            for(const auto& s : stiri) std::cout << s << "\n";
            std::cout << "=================\n";
        }
    } ziar;

    class Finante {
    private:
        long long balans;
        long long datorieBanca;
        long long venitSaptamanalSponsori;
        int pretBilet;
        std::vector<std::string> istoric;

    public:
        Finante(long long start = BUGET_INITIAL)
            : balans(start), datorieBanca(0), venitSaptamanalSponsori(0), pretBilet(20) {}

        void tranzactie(long long suma, std::string motiv) {
            balans += suma;
            std::string semn = (suma >= 0) ? "[+] " : "[-] ";
            istoric.push_back(semn + motiv + ": " + std::to_string(std::abs(suma)) + " EUR");
            if(istoric.size() > 5) istoric.erase(istoric.begin());
        }

        void platesteSalarii(const std::vector<Jucator*>& lot);

        void iaImprumut(long long suma) {
            datorieBanca += suma;
            balans += suma;
            ziar.adaugaStire("Clubul a luat imprumut: " + std::to_string(suma) + " EUR");
        }

        void platesteRata() {
            if(datorieBanca > 0) {
                long long rata = datorieBanca * 0.05;
                if(rata < 10000) rata = 10000;
                tranzactie(-rata, "Rata Banca");
                datorieBanca -= rata;
                if(datorieBanca <= 0) datorieBanca = 0;
            }
        }

        void seteazaPretBilet(int pret) { pretBilet = pret; }
        int getPretBilet() const { return pretBilet; }
        long long getBalans() const { return balans; }
        long long getDatorie() const { return datorieBanca; }
        void adaugaSponsor(long long suma) { venitSaptamanalSponsori += suma; }
        void incaseazaSponsori() { if(venitSaptamanalSponsori > 0) tranzactie(venitSaptamanalSponsori, "Sponsori"); }

        void afiseazaRaportDetaliat() const {
            std::cout << "\n$$$ FINANTE $$$\nBalans: " << balans << " EUR | Datorie: " << datorieBanca << " EUR\n";
            std::cout << "Sponsori: " << venitSaptamanalSponsori << "/sapt | Bilet: " << pretBilet << " EUR\n";
            std::cout << "Ultimele tranzactii:\n";
            for (auto it = istoric.rbegin(); it != istoric.rend(); ++it) std::cout << *it << "\n";
        }
    };
}
using namespace Core;

// ============================================================================
// GAME DATA
// ============================================================================
namespace GameData {
    struct PlayerMeta {
        int conditieFizica = 100;
        int moral = 80;
        int goluriSezon = 0;
        int valoarePiata = 0;
        int salariuSaptamanal = 0;
        bool accidentat = false;
        int saptamaniIndisponibil = 0;
        int potential = 0;
    };
    std::map<Jucator*, PlayerMeta> metaData;

    void initializeazaMeta(Jucator* j) {
        if (metaData.find(j) == metaData.end()) {
            int rtg = j->getRating();
            metaData[j].valoarePiata = rtg * rtg * 180;
            metaData[j].salariuSaptamanal = rtg * 200;
            metaData[j].conditieFizica = 90 + rand() % 11;
            metaData[j].potential = rtg + (rand() % 15);
        }
    }
    PlayerMeta& getMeta(Jucator* j) {
        initializeazaMeta(j);
        return metaData[j];
    }
}

void Finante::platesteSalarii(const std::vector<Jucator*>& lot) {
    long long total = 0;
    for(auto* j : lot) total += GameData::getMeta(j).salariuSaptamanal;
    tranzactie(-total, "Salarii Jucatori");
}

class JucatorFactory {
public:
    static Jucator* creazaJucator(const std::string& n, const std::string& p, int r, const std::string& poz, int nr) {
        if (poz == "GK") return new Portar(n, p, r, poz, nr);
        if (poz == "CB" || poz == "RB" || poz == "LB") return new Fundas(n, p, r, poz, nr);
        if (poz.find("M") != std::string::npos) return new Mijlocas(n, p, r, poz, nr);
        if (poz == "ST" || poz == "LW" || poz == "RW") return new Atacant(n, p, r, poz, nr);
        return new Jucator(n, p, r, poz, nr);
    }
};

class Medic : public Persoana {
    std::string specializare;
    int skill;
public:
    Medic(const std::string& n, const std::string& p, const std::string& s, int k) : Persoana(n, p), specializare(s), skill(k) {}
    Medic* clone() const override { return new Medic(*this); }
    int calculeazaEficienta() const override { return skill * 10; }
    int trateazaJucator() const { return skill; }
protected:
    void afisareDetaliata(std::ostream& os) const override { os << "Medic: " << specializare << " (Skill: " << skill << ")\n"; }
};

struct EchipaAI {
    std::string nume;
    int rating;
    int puncte=0, mj=0, v=0, e=0, i=0, gm=0, gp=0;
};

// Variabile Globale
Finante finante;
Tactica tacticaCurenta;
int etapa = 1;
int moralEchipa = MORAL_INITIAL;
int incredereConducere = INCREDERE_INITIALA;
int numarFani = 15000;
std::string numeManager;
std::string numeClub = "FC Politehnica";
bool echipaModificataManual = false;

std::vector<Jucator*> lot;
std::vector<Jucator*> titulari;
std::vector<Jucator*> rezerve;
std::vector<EchipaAI> liga;
std::vector<Medic*> medici;
std::vector<Antrenor*> antrenori;

void clear() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
    for(int i=0; i<30; ++i) std::cout << "\n";
}
void sleepMs(int ms) { std::this_thread::sleep_for(std::chrono::milliseconds(ms)); }

void initLiga() {
    liga = {
        {numeClub, 0}, {"FCSB", 85}, {"CFR Cluj", 86}, {"Univ. Craiova", 84},
        {"Rapid", 83}, {"Dinamo", 79}, {"Farul", 81}, {"Sepsi", 78},
        {"U Cluj", 80}, {"Otelul", 77}, {"Petrolul", 76}, {"Poli Iasi", 74},
        {"UTA", 75}, {"Hermannstadt", 75}, {"FC Botosani", 72}, {"Unirea Slobozia", 70}
    };
}

void sincronizeazaRezerve() {
    rezerve.clear();
    for(auto* j : lot) {
        bool eTitular = false;
        for(auto* t : titulari) {
            if(t == j) { eTitular = true; break; }
        }
        if(!eTitular) rezerve.push_back(j);
    }
}

void autoSelecteazaPrimul11() {
    std::vector<Jucator*> selectieTitulari;
    std::vector<Jucator*> selectieRezerve;
    std::vector<Jucator*> pool = lot;
    std::vector<int> cerinte = tacticaCurenta.getCerinte();

    std::sort(pool.begin(), pool.end(), [](Jucator* a, Jucator* b){
        return a->getRating() > b->getRating();
    });

    auto itGK = std::find_if(pool.begin(), pool.end(), [](Jucator* j){
        return j->getPozitie() == "GK" && !GameData::getMeta(j).accidentat;
    });
    if(itGK != pool.end()) {
        selectieTitulari.push_back(*itGK);
        pool.erase(itGK);
    }

    auto alege = [&](int count, const std::string& rol) {
        for(int k=0; k<count; ++k) {
            auto it = std::find_if(pool.begin(), pool.end(), [&](Jucator* j){
                if(GameData::getMeta(j).accidentat) return false;
                std::string p = j->getPozitie();
                if(rol=="DEF") return (p=="CB"||p=="RB"||p=="LB");
                if(rol=="MID") return (p.find("M") != std::string::npos);
                if(rol=="ATT") return (p=="ST"||p=="LW"||p=="RW");
                return false;
            });
            if(it != pool.end()) {
                selectieTitulari.push_back(*it);
                pool.erase(it);
            }
        }
    };

    alege(cerinte[0], "DEF");
    alege(cerinte[1], "MID");
    alege(cerinte[2], "ATT");

    while(selectieTitulari.size() < 11 && !pool.empty()) {
        selectieTitulari.push_back(pool.front());
        pool.erase(pool.begin());
    }

    selectieRezerve = pool;
    lot.clear();
    lot.insert(lot.end(), selectieTitulari.begin(), selectieTitulari.end());
    lot.insert(lot.end(), selectieRezerve.begin(), selectieRezerve.end());

    // Actualizam vectorii globali titulari/rezerve pe baza noii ordini din lot
    titulari.clear();
    rezerve.clear();
    for(size_t i=0; i<lot.size(); ++i) {
        if(i < 11) titulari.push_back(lot[i]);
        else rezerve.push_back(lot[i]);
    }

    echipaModificataManual = false;
}

int calculeazaPutereReala() {
    if(titulari.empty()) return 0;
    double suma = 0;
    double bonusTactic = 1.0;
    if(tacticaCurenta.stil == StilJoc::GEGENPRESS) bonusTactic = 1.15;
    if(tacticaCurenta.stil == StilJoc::PARK_THE_BUS) bonusTactic = 0.9;

    for(auto* j : titulari) {
        auto& meta = GameData::getMeta(j);
        if(meta.accidentat) continue;
        double eficienta = j->calculeazaEficienta();
        if(meta.conditieFizica < 70) eficienta *= 0.7;
        if(meta.moral > 90) eficienta *= 1.1;
        suma += eficienta;
    }
    if(!antrenori.empty()) suma += antrenori[0]->calculeazaEficienta() / 5;
    return (int)((suma / 11.0) * bonusTactic) + (moralEchipa - 50)/5;
}

std::string genereazaComentariu(const std::string& echipaAtac, bool gol, bool bara, bool aparare) {
    if (gol) return "GOOOOL " + echipaAtac + "! Executie magnifica!";
    if (bara) return "BARA! " + echipaAtac + " a fost la un pas de gol!";
    if (aparare) return "Portarul respinge sutul celor de la " + echipaAtac + "!";
    return "Actiune periculoasa...";
}

void simuleazaEtapa() {
    clear();

    if (!echipaModificataManual) {
        std::cout << "[INFO] Nu ai setat manual echipa. Antrenorul secund alege cel mai bun 11 pentru "
                  << tacticaCurenta.getNumeFormatie() << "...\n";
        autoSelecteazaPrimul11();
        sleepMs(1500);
    } else {
        std::cout << "[INFO] Joci cu echipa selectata de tine.\n";
        sleepMs(1000);
    }

    int advIdx = (etapa % (liga.size()-1)) + 1;
    EchipaAI& adv = liga[advIdx];
    EchipaAI* noi = &liga[0];

    int putereNoi = calculeazaPutereReala();
    int putereAdv = adv.rating + (rand()%10 - 5);

    std::cout << "\nSTART MECI: " << noi->nume << " (" << putereNoi << ") vs " << adv.nume << " (" << putereAdv << ")\n";
    sleepMs(1000);

    int scorNoi=0, scorAdv=0;
    int minute=0;

    while(minute < 90) {
        minute += rand() % 5 + 3;
        if(minute > 90) minute = 90;

        int zar = rand() % 100;
        int diff = putereNoi - putereAdv;

        if(zar < 20) {
            std::cout << "Min " << std::setw(2) << minute << ": ";
            if(rand()%100 < 50 + diff) {
                if(rand()%100 < 35) {
                    std::cout << genereazaComentariu(noi->nume, true, false, false) << "\n";
                    scorNoi++;
                } else if (rand()%100 < 20) {
                    std::cout << genereazaComentariu(noi->nume, false, true, false) << "\n";
                } else {
                    std::cout << genereazaComentariu(noi->nume, false, false, true) << "\n";
                }
            } else {
                if(rand()%100 < 35) {
                    std::cout << genereazaComentariu(adv.nume, true, false, false) << "\n";
                    scorAdv++;
                } else {
                    std::cout << "Ocazie " << adv.nume << "! Apararea respinge.\n";
                }
            }
            sleepMs(800);
        } else if (minute % 20 == 0) {
             std::cout << "Min " << std::setw(2) << minute << ": Joc echilibrat la mijlocul terenului...\n";
             sleepMs(200);
        }
    }

    std::cout << "\nFINAL: " << scorNoi << " - " << scorAdv << "\n";

    noi->mj++; adv.mj++; noi->gm+=scorNoi; noi->gp+=scorAdv; adv.gm+=scorAdv; adv.gp+=scorNoi;

    for(auto* j : titulari) {
        GameData::getMeta(j).conditieFizica = std::max(0, GameData::getMeta(j).conditieFizica - 10);
    }

    if(scorNoi > scorAdv) {
        std::cout << "VICTORIE! (+3p)\n";
        finante.tranzactie(BONUS_VICTORIE, "Premiu Victorie");
        noi->puncte+=3; noi->v++; adv.i++;
        moralEchipa = std::min(100, moralEchipa + 5);
        ziar.adaugaStire(noi->nume + " castiga meciul etapei!");
    } else if(scorNoi == scorAdv) {
        std::cout << "EGAL.\n";
        finante.tranzactie(BONUS_EGAL, "Premiu Egal");
        noi->puncte++; noi->e++; adv.e++; adv.puncte++;
    } else {
        std::cout << "INFRANGERE.\n";
        noi->i++; adv.v++; adv.puncte+=3;
        moralEchipa = std::max(0, moralEchipa - 5);
        ziar.adaugaStire(noi->nume + " pierde puncte pretioase.");
    }

    for(size_t i=1; i<liga.size(); ++i) {
        if((int)i != advIdx) {
            int gA = rand()%4, gB = rand()%4;
            liga[i].mj++; liga[i].gm+=gA; liga[i].gp+=gB;
            if(gA>gB) { liga[i].puncte+=3; liga[i].v++; }
            else if(gA==gB) { liga[i].puncte++; liga[i].e++; }
            else liga[i].i++;
        }
    }

    finante.platesteSalarii(lot);
    finante.platesteRata();
    finante.incaseazaSponsori();

    etapa++;
    std::cout << "\nApasa ENTER..."; std::cin.ignore(); std::cin.get();
}

void meniuTactica() {
    while(true) {
        clear();
        std::cout << "=== VESTIAR & TACTICA ===\n";
        std::cout << "Formatie Curenta: " << tacticaCurenta.getNumeFormatie() << "\n";
        std::cout << "Stil Joc: " << (tacticaCurenta.stil==StilJoc::POSESIE ? "Posesie" : "Echilibrat") << "\n";
        std::cout << "Mod Selectie: " << (echipaModificataManual ? "MANUAL (Schimbarile tale)" : "AUTO (Cel mai bun 11)") << "\n";
        std::cout << "Rating Estimat: " << calculeazaPutereReala() << "\n\n";

        std::cout << "1. Alege 4-4-2\n2. Alege 4-3-3\n3. Alege 3-5-2\n";
        std::cout << "4. Auto-Selecteaza cei mai buni jucatori (Reset Manual)\n";
        std::cout << "5. Schimba Jucatori (Manual)\n";
        std::cout << "0. Iesire\nCmd: ";

        int c; std::cin >> c;
        if(c==0) break;

        if(c==1) tacticaCurenta.formatie = Formatie::F442;
        else if(c==2) tacticaCurenta.formatie = Formatie::F433;
        else if(c==3) tacticaCurenta.formatie = Formatie::F352;

        if(c==4) {
            autoSelecteazaPrimul11();
        }
        else if(c==5) {
            std::cout << "\nTITULARI:\n";
            for(size_t i=0; i<titulari.size(); ++i)
                std::cout << i+1 << ". " << titulari[i]->getNumeComplet() << " (" << titulari[i]->getPozitie() << ")\n";
            std::cout << "REZERVE:\n";
            for(size_t i=0; i<rezerve.size(); ++i)
                std::cout << i+12 << ". " << rezerve[i]->getNumeComplet() << " (" << rezerve[i]->getPozitie() << ")\n";

            int t, r; std::cout << "Schimba (Titular Rezerva): "; std::cin >> t >> r;
            if(t>0 && r>11 && t<=(int)titulari.size() && r<=(int)(11+rezerve.size())) {
                std::iter_swap(lot.begin()+t-1, lot.begin()+r-1);

                // Recalculam manual vectorii de pointeri titulari/rezerve
                titulari.clear(); rezerve.clear();
                for(size_t i=0; i<lot.size(); ++i) {
                    if(i < 11) titulari.push_back(lot[i]);
                    else rezerve.push_back(lot[i]);
                }

                echipaModificataManual = true;
                std::cout << "Schimbare efectuata! Modul a devenit MANUAL.\n";
                sleepMs(1000);
            }
        }
    }
}

void meniuFinante() {
    clear();
    finante.afiseazaRaportDetaliat();
    std::cout << "\n1. Imprumut (1M)\n0. Back\n";
    int c; std::cin >> c;
    if(c==1) finante.iaImprumut(1000000);
}

void meniuTransferuri() {
    clear();
    std::cout << "=== TRANSFERURI ===\nBuget: " << finante.getBalans() << "\n";
    std::cout << "1. Cauta Portar (500k)\n2. Cauta Atacant (800k)\n3. Vinde un jucator random (+300k)\n0. Back\n";
    int c; std::cin >> c;
    if(c==1 && finante.getBalans() > 500000) {
        finante.tranzactie(-500000, "Cumparare GK");
        Jucator* j = JucatorFactory::creazaJucator("Helmuth", "Duckhadam", 78, "GK", 1);
        lot.push_back(j);
        sincronizeazaRezerve();
        std::cout << "Portar transferat!\n";
    }
    else if(c==2 && finante.getBalans() > 800000) {
        finante.tranzactie(-800000, "Cumparare ST");
        Jucator* j = JucatorFactory::creazaJucator("Marius", "Lacatus", 82, "ST", 9);
        lot.push_back(j);
        sincronizeazaRezerve();
        std::cout << "Atacant transferat!\n";
    }
    else if(c==3 && rezerve.size() > 0) {
        finante.tranzactie(300000, "Vanzare Jucator");
        Jucator* out = rezerve.back();
        // Stergere din lot
        auto it = std::find(lot.begin(), lot.end(), out);
        if(it != lot.end()) {
            lot.erase(it);
            sincronizeazaRezerve();
            ziar.adaugaStire("Clubul l-a vandut pe " + out->getNumeComplet());
            std::cout << "Jucator vandut.\n";
        }
    }
    sleepMs(1000);
}

void meniuAcademie() {
    clear();
    std::cout << "Academie Juniori\n1. Scout Europa (" << COST_SCOUTING_EU << ")\n0. Back\n";
    int c; std::cin >> c;
    if(c==1) {
        if(finante.getBalans() > COST_SCOUTING_EU) {
            finante.tranzactie(-COST_SCOUTING_EU, "Scouting");
            lot.push_back(new Jucator("Junior", "Nou", 60, "CAM", 99));
            sincronizeazaRezerve();
            std::cout << "Junior gasit!\n";
        } else std::cout << "Fara bani.\n";
        sleepMs(1000);
    }
}

int main() {
    srand(time(0));
    Club club("FC Politehnica");

    std::ifstream fin("tastatura.txt");
    if (!fin.is_open()) {
        std::cerr << "[EROARE FATALA] Fisierul 'tastatura.txt' nu a fost gasit! Programul se va inchide.\n";
        return 1;
    }

    std::string tip;
    while(fin >> tip) {
        std::string n, p; fin >> n >> p;
        if(tip == "JUCATOR") {
            int r, nr; std::string poz; fin >> r >> poz >> nr;
            Jucator* j = JucatorFactory::creazaJucator(n, p, r, poz, nr);
            lot.push_back(j);
            club.adaugaMembru(*j);
        } else if(tip == "MEDIC") {
            std::string s; int e; fin >> s >> e; medici.push_back(new Medic(n, p, s, e));
        } else if(tip == "ANTRENOR") {
            int a, t; fin >> a >> t; antrenori.push_back(new Antrenor(n, p, a, t));
        } else {
            std::string d; std::getline(fin, d);
        }
    }
    fin.close();

    // Initializare
    // Titularii sunt primii 11 cititi din fisier, restul rezerve
    for(size_t i=0; i<lot.size(); ++i) {
        if(i < 11) titulari.push_back(lot[i]);
        else rezerve.push_back(lot[i]);
    }

    initLiga();
    finante.adaugaSponsor(30000);

    std::cout << "Nume Manager: "; std::getline(std::cin, numeManager);

    bool running = true;
    while(running) {
        clear();
        std::cout << "=== MANAGER PRO 2025 ===\n";
        std::cout << "Club: " << numeClub << " | Manager: " << numeManager << "\n";
        finante.afiseazaRaportDetaliat();
        std::cout << "Echipa: " << (echipaModificataManual ? "MANUAL" : "AUTO") << " | Rating: " << calculeazaPutereReala() << "\n";

        ziar.afiseaza();

        std::cout << "\n1. JOACA MECI\n2. TACTICA & ECHIPA\n3. TRANSFERURI\n4. FINANTE\n5. ACADEMIE\n6. CLASAMENT\n7. SAVE & IESI\nCmd: ";
        int cmd; std::cin >> cmd;

        switch(cmd) {
            case 1: simuleazaEtapa(); break;
            case 2: meniuTactica(); break;
            case 3: meniuTransferuri(); break;
            case 4: meniuFinante(); break;
            case 5: meniuAcademie(); break;
            case 6:
                clear();
                std::sort(liga.begin(), liga.end(), [](auto a, auto b){ return a.puncte > b.puncte; });
                for(int i=0; i<(int)liga.size(); ++i) std::cout << i+1 << ". " << liga[i].nume << " " << liga[i].puncte << "p\n";
                std::cin.ignore(); std::cin.get();
                break;
            case 7:
                club.salveazaInFisier();
                running = false;
                break;
        }
    }
    return 0;
}