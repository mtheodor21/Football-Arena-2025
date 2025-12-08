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
#include <memory>
#include <list>
#include <sstream>
#include <cmath>

#include "Exceptii/exceptii.h"
#include "Persoane/persoane.h"
#include "Persoane/jucator.h"
#include "Persoane/antrenor.h"
#include "Persoane/arbitru.h"
#include "Club/club.h"
#include "Utils/istoric.h"

class IObserver {
public:
    virtual ~IObserver() = default;
    virtual void onNotificare(const std::string& mesaj) = 0;
};

class ISubject {
private:
    std::list<IObserver*> observatori;
public:
    virtual ~ISubject() = default;
    void ataseaza(IObserver* obs) { observatori.push_back(obs); }
    void detaseaza(const IObserver* obs) {
        observatori.remove(const_cast<IObserver*>(obs));
    }
    void notifica(const std::string& mesaj) {
        for(auto* obs : observatori) obs->onNotificare(mesaj);
    }
};

class NewsFeed : public IObserver {
    std::deque<std::string> stiri;
public:
    void onNotificare(const std::string& mesaj) override {
        stiri.push_front("> [BREAKING] " + mesaj);
        if(stiri.size() > 12) stiri.pop_back();
    }
    void adaugaStire(const std::string& mesaj) {
        stiri.push_front("> " + mesaj);
        if(stiri.size() > 12) stiri.pop_back();
    }
    void afiseaza() const {
        std::cout << "\n=== NEWS FEED (Ultimele stiri din Liga 1) ===\n";
        if(stiri.empty()) std::cout << "Nicio stire recenta.\n";
        for(const auto& s : stiri) std::cout << s << "\n";
        std::cout << "=============================================\n";
    }
} ziar;

namespace Core {
    constexpr long long BUGET_INITIAL = 5000000;
    constexpr int BONUS_VICTORIE = 100000;
    constexpr int BONUS_EGAL = 30000;
    constexpr int MORAL_INITIAL = 75;
    constexpr int INCREDERE_INITIALA = 80;

    enum class StilJoc { POSESIE, CONTRAATAC, PARK_THE_BUS, GEGENPRESS, TIKI_TAKA };
    enum class Formatie { F442, F433, F352, F532, F343 };

    struct Tactica {
        Formatie formatie = Formatie::F433;
        StilJoc stil = StilJoc::POSESIE;
        int agresivitate = 50;
        int linieDefensiva = 50;

        std::string getNumeFormatie() const {
            switch(formatie) {
                case Formatie::F442: return "4-4-2 Classic";
                case Formatie::F433: return "4-3-3 Attack";
                case Formatie::F352: return "3-5-2 Modern";
                case Formatie::F532: return "5-3-2 Defensiv";
                default: return "3-4-3 Total Football";
            }
        }
        std::vector<int> getCerinte() const {
            switch(formatie) {
                case Formatie::F442: return {4, 4, 2};
                case Formatie::F433: return {4, 3, 3};
                case Formatie::F352: return {3, 5, 2};
                case Formatie::F532: return {5, 3, 2};
                default: return {3, 4, 3};
            }
        }
    };
}
using namespace Core;

namespace GameData {
    struct SeasonStats {
        int meciuriJucate = 0;
        int goluri = 0;
        int paseGol = 0;
        int cartonaseGalbene = 0;
        int cartonaseRosii = 0;
        double medieNota = 6.0;
        int meciuriFaraGolPrimit = 0;
    };

    struct PlayerMeta {
        int conditieFizica = 100;
        int moral = 80;
        int valoarePiata = 0;
        int salariuSaptamanal = 0;
        bool accidentat = false;
        int saptamaniIndisponibil = 0;
        int potential = 0;
        int varsta = 18;
        SeasonStats stats;

        std::string toString() const {
            return std::to_string(conditieFizica) + " " + std::to_string(moral) + " " +
                   std::to_string(valoarePiata) + " " + std::to_string(salariuSaptamanal) + " " +
                   std::to_string(stats.goluri);
        }
    };

    std::map<Jucator*, PlayerMeta> metaData;

    void initializeazaMeta(Jucator* j) {
        if (metaData.find(j) == metaData.end()) {
            int rtg = j->getRating();
            metaData[j].valoarePiata = rtg * rtg * 220;
            metaData[j].salariuSaptamanal = rtg * 180;
            metaData[j].conditieFizica = 92 + rand() % 9;
            metaData[j].potential = std::min(99, rtg + (rand() % 15));
            metaData[j].varsta = 17 + rand() % 20;
        }
    }

    PlayerMeta& getMeta(Jucator* j) {
        initializeazaMeta(j);
        return metaData[j];
    }
}

class Finante : public ISubject {
private:
    long long balans;
    long long datorieBanca;
    long long rataSaptamanala;
    std::vector<std::string> istoric;
    int pretBilet;
    long long venitSponsori;
    int popularitateClub;

public:
    explicit Finante(long long start = BUGET_INITIAL)
        : balans(start), datorieBanca(0), rataSaptamanala(0), pretBilet(25), venitSponsori(0), popularitateClub(50) {}

    void tranzactie(long long suma, const std::string& motiv) {
        balans += suma;
        std::string semn = (suma >= 0) ? "[+] " : "[-] ";
        istoric.push_back(semn + motiv + ": " + std::to_string(std::abs(suma)) + " EUR");
        if(istoric.size() > 10) istoric.erase(istoric.begin());

        if(std::abs(suma) > 1000000) {
            notifica("Miscare financiara majora: " + motiv + " (" + std::to_string(suma) + " EUR)");
        }
    }

    void platesteSalarii(const std::vector<std::unique_ptr<Jucator>>& echipa) {
        long long total = 0;
        for(const auto& j_ptr : echipa) {
            total += GameData::getMeta(j_ptr.get()).salariuSaptamanal;
        }
        tranzactie(-total, "Salarii Jucatori & Staff");
    }

    void iaImprumut(long long suma) {
        if(datorieBanca > 15000000) {
            std::cout << "[BANCA] Refuzat. Grad de indatorare critic.\n";
            return;
        }
        datorieBanca += suma;
        balans += suma;
        long long dobanda = (long long)(suma * 0.12);
        long long totalDePlata = suma + dobanda;
        rataSaptamanala += (totalDePlata / 52);
        ziar.adaugaStire("Clubul s-a imprumutat cu " + std::to_string(suma) + " EUR");
    }

    void seteazaPretBilet(int pret) { pretBilet = pret; }
    int getPretBilet() const { return pretBilet; }
    long long getBalans() const { return balans; }
    long long getDatorie() const { return datorieBanca; }
    void adaugaSponsor(long long suma) { venitSponsori += suma; }

    void actualizeazaPopularitate(int delta) {
        popularitateClub = std::max(10, std::min(100, popularitateClub + delta));
    }

    int calculeazaSpectatori(int capacitateStadion) {
        double factorPret = 1.0 - ((pretBilet - 20) * 0.015);
        if (factorPret < 0.3) factorPret = 0.3;
        int baza = (int)(capacitateStadion * (popularitateClub / 100.0) * factorPret);
        int variatie = rand() % 3000 - 1500;
        return std::min(capacitateStadion, std::max(500, baza + variatie));
    }

    void afiseazaRaportDetaliat() const {
        std::cout << "\n$$$ BIROUL FINANCIAR $$$\n";
        std::cout << "Balans Lichid:    " << balans << " EUR\n";
        std::cout << "Datorie Totala:   " << datorieBanca << " EUR (Rata: " << rataSaptamanala << ")\n";
        std::cout << "Venit Sponsori:   " << venitSponsori << " EUR/sapt\n";
        std::cout << "Pret Bilet:       " << pretBilet << " EUR\n";
        std::cout << "Popularitate Brand: " << popularitateClub << "/100\n";
        std::cout << "\n[ISTORIC TRANZACTII]\n";
        for (auto it = istoric.rbegin(); it != istoric.rend(); ++it) std::cout << " " << *it << "\n";
        std::cout << "$$$$$$$$$$$$$$$$$$$$$$$$\n";
    }
};

class JucatorFactory {
private:
    JucatorFactory() {}
public:
    static JucatorFactory& getInstance() {
        static JucatorFactory instance;
        return instance;
    }
    JucatorFactory(JucatorFactory const&) = delete;
    void operator=(JucatorFactory const&) = delete;

    Jucator* creazaJucator(const std::string& n, const std::string& p, int r, const std::string& poz, int nr) {
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

Finante finante;
Tactica tacticaCurenta;
Istoric<std::string> istoricMeciuri;

int etapa = 1;
int moralEchipa = MORAL_INITIAL;
int incredereConducere = INCREDERE_INITIALA;
int numarFani = 15000;
int capacitateStadion = 25000;
std::string numeManager;
std::string numeClub = "FC Politehnica";
bool echipaModificataManual = false;

std::vector<std::unique_ptr<Jucator>> lot;
std::vector<std::unique_ptr<Jucator>> academia;
std::vector<std::unique_ptr<Medic>> medici;
std::vector<std::unique_ptr<Antrenor>> antrenori;

std::vector<Jucator*> titulari;
std::vector<Jucator*> rezerve;
std::vector<EchipaAI> liga;

void clear() {
#ifdef _WIN32
    system("cls");
#else
    if (std::getenv("TERM")) {
        system("clear");
    }
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

void sincronizeazaLot() {
    titulari.clear();
    rezerve.clear();
    for(size_t i=0; i<lot.size(); ++i) {
        if(i < 11) titulari.push_back(lot[i].get());
        else rezerve.push_back(lot[i].get());
    }
}

void sincronizeazaRezerve() {
    rezerve.clear();
    for(auto& j_ptr : lot) {
        Jucator* j = j_ptr.get();
        bool eTitular = false;
        for(const auto* t : titulari) {
            if(t == j) { eTitular = true; break; }
        }
        if(!eTitular) rezerve.push_back(j);
    }
}

int calculeazaPutereReala() {
    if(titulari.empty()) return 0;
    double suma = 0;
    double bonusTactic = 1.0;

    if(tacticaCurenta.stil == StilJoc::GEGENPRESS) bonusTactic = 1.15;
    else if(tacticaCurenta.stil == StilJoc::PARK_THE_BUS) bonusTactic = 0.9;

    for(auto* j : titulari) {
        const auto& meta = GameData::getMeta(j);
        if(meta.accidentat) continue;
        double eficienta = j->calculeazaEficienta();

        if(meta.conditieFizica < 50) eficienta *= 0.5;
        else if(meta.conditieFizica < 80) eficienta *= 0.8;

        if(meta.moral > 90) eficienta *= 1.1;

        suma += eficienta;
    }

    if(!antrenori.empty()) suma += antrenori[0]->calculeazaEficienta() / 5;
    return (int)((suma / 11.0) * bonusTactic) + (moralEchipa - 50)/5;
}

std::string genereazaComentariu(const std::string& echipaAtac, int tip) {
    static const std::vector<std::string> goluri = {
        "GOOOOL " + echipaAtac + "! Sut fabulos direct in vinclu!",
        "GOOOOL! " + echipaAtac + " deschide apararea adversa ca pe o conserva!",
        "Incredibil! " + echipaAtac + " inscrie dupa o greseala uriasa!",
        "GOOOOL " + echipaAtac + "! Lovitura de cap perfecta din corner!"
    };
    static const std::vector<std::string> bare = {
        "BARA! " + echipaAtac + " a fost la un pas de gol!",
        "Sut violent al celor de la " + echipaAtac + " care loveste transversala!"
    };
    static const std::vector<std::string> aparari = {
        "Portarul respinge eroic sutul celor de la " + echipaAtac + "!",
        "Interventie salvatoare a fundasului pe linia portii!"
    };
    static const std::vector<std::string> var = {
        "Arbitrul consulta VAR... Golul este VALIDAT!",
        "Moment tensionat... Arbitrul merge la monitor... PENALTY pentru " + echipaAtac + "!"
    };

    if(tip == 0) return goluri[rand() % goluri.size()];
    if(tip == 1) return bare[rand() % bare.size()];
    if(tip == 2) return aparari[rand() % aparari.size()];
    if(tip == 3) return var[rand() % var.size()];
    return "Faza confuza...";
}

void autoSelecteazaPrimul11() {
    titulari.clear();
    std::vector<int> cerinte = tacticaCurenta.getCerinte();

    std::sort(lot.begin(), lot.end(), [](const std::unique_ptr<Jucator>& a, const std::unique_ptr<Jucator>& b){
        return a->getRating() > b->getRating();
    });

    for(auto& j : lot) {
        if(j->getPozitie() == "GK" && !GameData::getMeta(j.get()).accidentat) {
            titulari.push_back(j.get());
            break;
        }
    }

    int nevoieDef = cerinte[0];
    for(auto& j : lot) {
        if(nevoieDef == 0) break;
        std::string p = j->getPozitie();
        bool dejaSelectat = (std::find(titulari.begin(), titulari.end(), j.get()) != titulari.end());
        if(!dejaSelectat && !GameData::getMeta(j.get()).accidentat && (p=="CB" || p=="RB" || p=="LB")) {
            titulari.push_back(j.get());
            nevoieDef--;
        }
    }

    int nevoieMid = cerinte[1];
    for(auto& j : lot) {
        if(nevoieMid == 0) break;
        std::string p = j->getPozitie();
        bool dejaSelectat = (std::find(titulari.begin(), titulari.end(), j.get()) != titulari.end());
        if(!dejaSelectat && !GameData::getMeta(j.get()).accidentat && (p.find("M") != std::string::npos)) {
            titulari.push_back(j.get());
            nevoieMid--;
        }
    }

    int nevoieAtt = cerinte[2];
    for(auto& j : lot) {
        if(nevoieAtt == 0) break;
        std::string p = j->getPozitie();
        bool dejaSelectat = (std::find(titulari.begin(), titulari.end(), j.get()) != titulari.end());
        if(!dejaSelectat && !GameData::getMeta(j.get()).accidentat && (p=="ST" || p=="LW" || p=="RW")) {
            titulari.push_back(j.get());
            nevoieAtt--;
        }
    }

    while(titulari.size() < 11) {
        for(auto& j : lot) {
            bool dejaSelectat = (std::find(titulari.begin(), titulari.end(), j.get()) != titulari.end());
            if(!dejaSelectat && !GameData::getMeta(j.get()).accidentat) {
                titulari.push_back(j.get());
                if(titulari.size() == 11) break;
            }
        }
        break;
    }

    sincronizeazaRezerve();
    echipaModificataManual = false;
}

void joacaMeciOficial() {
    clear();
    int advIdx = (etapa % (liga.size()-1)) + 1;
    EchipaAI& adv = liga[advIdx];
    EchipaAI* noi = &liga[0];

    int putereNoi = calculeazaPutereReala();
    int putereAdv = adv.rating + (rand()%10 - 5);
    int spectatori = finante.calculeazaSpectatori(capacitateStadion);
    bool acasa = (etapa % 2 != 0);

    if(acasa) {
        putereNoi += 5;
        std::cout << "[STADION] " << spectatori << " de fani canta imnul!\n";
    }

    std::cout << "\nSTART MECI: " << noi->nume << " (" << putereNoi << ") vs " << adv.nume << " (" << putereAdv << ")\n";
    sleepMs(1500);

    int scorNoi=0, scorAdv=0;
    int minute=0;

    while(minute < 90) {
        minute += rand() % 4 + 2;
        if(minute > 90) minute = 90;

        int zar = rand() % 100;
        int diff = putereNoi - putereAdv;

        if(zar < 22) {
            std::cout << "Min " << std::setw(2) << minute << ": ";

            if(rand()%100 < 2 && !titulari.empty()) {
                Jucator* acc = titulari[rand()%titulari.size()];
                GameData::getMeta(acc).accidentat = true;
                GameData::getMeta(acc).saptamaniIndisponibil = 3;
                std::cout << "ACCIDENTARE! " << acc->getNumeComplet() << " cere schimbare.\n";

                if(!medici.empty()) {
                   int skill = medici[0]->trateazaJucator();
                   std::cout << " (Medic skill " << skill << ": Jucatorul va fi tratat rapid)\n";
                }

                putereNoi -= 10;
                sleepMs(1000);
                continue;
            }

            if(rand()%100 < 50 + diff) {
                if(rand()%100 < 35) {
                    std::cout << genereazaComentariu(noi->nume, 0) << "\n";
                    scorNoi++;
                    if(!titulari.empty()) GameData::getMeta(titulari[rand()%titulari.size()]).stats.goluri++;
                } else if (rand()%100 < 20) std::cout << genereazaComentariu(noi->nume, 1) << "\n";
                else if (rand()%100 < 5) {
                    std::cout << "Verificare VAR...\n";
                    sleepMs(1000);
                    std::cout << genereazaComentariu(noi->nume, 3) << "\n";
                    scorNoi++;
                }
                else std::cout << genereazaComentariu(noi->nume, 2) << "\n";
            } else {
                if(rand()%100 < 35) {
                    std::cout << genereazaComentariu(adv.nume, 0) << "\n";
                    scorAdv++;
                } else std::cout << genereazaComentariu(adv.nume, 2) << "\n";
            }
            sleepMs(800);
        } else if (minute % 15 == 0) {
             std::cout << "Min " << std::setw(2) << minute << ": ";
             if(tacticaCurenta.stil == StilJoc::POSESIE) std::cout << "Dominam posesia si construim calm.\n";
             else std::cout << "Joc fizic la mijlocul terenului.\n";
             sleepMs(200);
        }
    }

    std::cout << "\nFINAL: " << scorNoi << " - " << scorAdv << "\n";

    istoricMeciuri.adaugaEveniment("Etapa " + std::to_string(etapa) + ": " + std::to_string(scorNoi) + "-" + std::to_string(scorAdv));

    noi->mj++; adv.mj++; noi->gm+=scorNoi; noi->gp+=scorAdv; adv.gm+=scorAdv; adv.gp+=scorNoi;
    for(auto* j : titulari) {
        auto& m = GameData::getMeta(j);
        m.conditieFizica = std::max(0, m.conditieFizica - 15);
        m.stats.meciuriJucate++;
    }

    if(scorNoi > scorAdv) {
        std::cout << "VICTORIE!\n";
        finante.tranzactie(BONUS_VICTORIE, "Premiu Victorie");
        if(acasa) finante.tranzactie(spectatori * finante.getPretBilet(), "Bilete");
        noi->puncte+=3; noi->v++; adv.i++;
        moralEchipa = std::min(100, moralEchipa + 5);
        incredereConducere = std::min(100, incredereConducere + 3);
        ziar.adaugaStire(noi->nume + " castiga spectaculos!");
        finante.actualizeazaPopularitate(2);
    } else if(scorNoi == scorAdv) {
        std::cout << "EGAL.\n";
        finante.tranzactie(BONUS_EGAL, "Premiu Egal");
        if(acasa) finante.tranzactie(spectatori * finante.getPretBilet(), "Bilete");
        noi->puncte++; noi->e++; adv.e++; adv.puncte++;
    } else {
        std::cout << "INFRANGERE.\n";
        if(acasa) finante.tranzactie(spectatori * finante.getPretBilet(), "Bilete");
        noi->i++; adv.v++; adv.puncte+=3;
        moralEchipa = std::max(0, moralEchipa - 8);
        incredereConducere -= 5;
        ziar.adaugaStire("Esec dureros pentru " + noi->nume);
        finante.actualizeazaPopularitate(-3);
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

    etapa++;
    std::cout << "\nApasa ENTER pentru a reveni la meniu..."; std::cin.ignore(); std::cin.get();
}

void meniuTactica() {
    clear();
    std::cout << "=== VESTIAR & TACTICA ===\n";
    std::cout << "Formatie Curenta: " << tacticaCurenta.getNumeFormatie() << "\n";
    std::cout << "Stil: " << (int)tacticaCurenta.stil << " | Agresivitate: " << tacticaCurenta.agresivitate << "\n";
    std::cout << "Mod Selectie: " << (echipaModificataManual ? "MANUAL" : "AUTO") << "\n";
    std::cout << "Rating Estimat: " << calculeazaPutereReala() << "\n\n";

    std::cout << "\n1. Alege 4-4-2\n2. Alege 4-3-3\n3. Alege 3-5-2\n";
    std::cout << "4. Auto-Selecteaza jucatori\n";
    std::cout << "5. Schimba Jucatori (Manual)\n";
    std::cout << "0. Iesire\nCmd: ";

    int c; std::cin >> c;
    if(c==0) return;

    if(c==1) tacticaCurenta.formatie = Formatie::F442;
    else if(c==2) tacticaCurenta.formatie = Formatie::F433;
    else if(c==3) tacticaCurenta.formatie = Formatie::F352;

    if(c==4) {
        autoSelecteazaPrimul11();
    }
    else if(c==5) {
        std::cout << "\nTITULARI:\n";
        for(size_t i=0; i<titulari.size(); ++i)
            std::cout << i+1 << ". " << titulari[i]->getNumeComplet() << " (" << GameData::getMeta(titulari[i]).conditieFizica << "%)\n";
        std::cout << "REZERVE:\n";
        for(size_t i=0; i<rezerve.size(); ++i)
            std::cout << i+12 << ". " << rezerve[i]->getNumeComplet() << "\n";
        int t, r; std::cout << "Schimba (T R): "; std::cin >> t >> r;
        if(t>0 && r>11 && t<=(int)titulari.size() && r<=(int)(11+rezerve.size())) {
            std::iter_swap(lot.begin()+t-1, lot.begin()+r-1);
            sincronizeazaLot();
            echipaModificataManual = true;
        }
    }
}

void meniuTransferuri() {
    clear();
    std::cout << "=== PIATA TRANSFERURI ===\nBuget: " << finante.getBalans() << "\n";
    std::vector<std::string> names = {"Neymar", "Mbappe", "Hagi", "Popescu", "Ronaldo"};
    for(int i=0; i<5; ++i) {
        int rtg = 70 + rand()%25;
        int pret = rtg * 100000;
        std::cout << i+1 << ". " << names[i] << " (Rtg: " << rtg << ") - Pret cerut: " << pret << "\n";
    }
    std::cout << "0. Back\nCmd: ";
    int opt; std::cin >> opt;
    if(opt>=1 && opt<=5) {
        int rtg = 70 + rand()%25;
        int pretCerut = rtg * 100000;
        int pretMinim = pretCerut * 0.9;

        std::cout << "Clubul cere " << pretCerut << " EUR. Cat oferi?\n";
        long long oferta; std::cin >> oferta;

        if(oferta >= pretMinim) {
            if(finante.getBalans() >= oferta) {
                int salariuCerut = rtg * 200;
                std::cout << "Clubul a acceptat! Jucatorul cere salariu " << salariuCerut << " EUR/sapt.\n";
                std::cout << "Oferi? (1=DA, 0=NU): ";
                int salOpt; std::cin >> salOpt;
                if(salOpt == 1) {
                    finante.tranzactie(-oferta, "Transfer");
                    Jucator* j = JucatorFactory::getInstance().creazaJucator(names[opt-1], "Nou", rtg, "ST", 99);
                    lot.push_back(std::unique_ptr<Jucator>(j));
                    GameData::getMeta(j).salariuSaptamanal = salariuCerut;
                    sincronizeazaLot();
                    std::cout << "Transfer Reusit!\n";
                    ziar.adaugaStire(numeClub + " l-a transferat pe " + names[opt-1]);
                } else std::cout << "Negocierile au esuat.\n";
            } else std::cout << "Nu ai fonduri.\n";
        } else {
            std::cout << "Clubul a refuzat oferta. 'Este o jignire!'\n";
        }
    }
    sleepMs(1500);
}

void meniuAcademie() {
    clear();
    std::cout << "=== ACADEMIA U19 ===\n";
    std::cout << "Jucatori in Academie: " << academia.size() << "\n";
    if(!academia.empty()) {
        for(size_t i=0; i<academia.size(); ++i) {
            const auto& m = GameData::getMeta(academia[i].get());
            std::cout << i+1 << ". " << academia[i]->getNumeComplet()
                      << " (" << academia[i]->getPozitie() << ") Rtg:" << academia[i]->getRating()
                      << " Pot:" << m.potential << "\n";
        }
        std::cout << "\n1. Promoveaza Junior\n0. Back\n";
        int c; std::cin >> c;
        if(c==1 && !academia.empty()) {
            std::cout << "ID: "; int id; std::cin >> id;
            if(id>0 && id<=(int)academia.size()) {
                lot.push_back(std::move(academia[id-1]));
                academia.erase(academia.begin() + id - 1);
                sincronizeazaLot();
                std::cout << "Promovat!\n";
            }
        }
    } else {
        std::cout << "1. Organizeaza Trial (Cost: 10k EUR)\n0. Back\n";
        int c; std::cin >> c;
        if(c==1 && finante.getBalans() > 10000) {
            finante.tranzactie(-10000, "Trial Juniori");
            Jucator* j = new Jucator("Junior", "Talent", 55+rand()%10, "CAM", 0);
            GameData::getMeta(j).potential = 80 + rand()%20;
            academia.push_back(std::unique_ptr<Jucator>(j));
            std::cout << "Am gasit un talent!\n";
        }
    }
    sleepMs(1500);
}

void meniuFinante() {
    clear();
    finante.afiseazaRaportDetaliat();
    std::cout << "\n1. Imprumut (1M)\n2. Setare pret bilet\n3. Vezi datorii\n0. Back\n";
    int c; std::cin >> c;
    if(c==1) finante.iaImprumut(1000000);
    else if(c==2) {
        std::cout << "Pret nou: "; int p; std::cin >> p;
        finante.seteazaPretBilet(p);
    }
    else if(c==3) std::cout << "Datorie curenta: " << finante.getDatorie() << " EUR\n";
}

void meniuStaff() {
    clear();
    std::cout << "=== STAFF ===\n";
    std::cout << "Antrenori Secunzi: " << antrenori.size() << "\n";
    std::cout << "Medici: " << medici.size() << "\n";
    std::cout << "\n1. Listeaza Antrenori\n2. Listeaza Medici\n0. Back\n";
    int c; std::cin >> c;
    if(c==1) for(const auto& a: antrenori) std::cout << a->getNumeComplet() << "\n";
    if(c==2) for(const auto& m: medici) std::cout << m->getNumeComplet() << "\n";
    sleepMs(2000);
}

void meniuDetaliiClub(const Club& club) {
    clear();
    std::cout << "=== RAPORT CLUB ===\n";
    club.analizeazaEchipa();
    std::cout << "Total membri: " << club.getNumarJucatori() << "\n";

    if (!lot.empty()) {
        std::cout << "Exemplu Meta Data Capitan: " << GameData::getMeta(lot[0].get()).toString() << "\n";
    }

    std::cout << "\nApasa ENTER..."; std::cin.ignore(); std::cin.get();
}

void meniuStatistici() {
    clear();
    std::cout << "=== STATISTICI ===\n";
    std::cout << "1. Istoric Meciuri\n2. Back\n";
    int c; std::cin >> c;
    if(c==1) istoricMeciuri.afiseazaIstoric();
}

int main() {
    srand(time(0));

    numeClub = "Club Necunoscut";

    finante.ataseaza(&ziar);

    std::ifstream fin("tastatura.txt");
    if (!fin.is_open()) {
        std::ofstream fout("tastatura.txt");
        fout << "CLUB FC_Politehnica\n";
        fout << "MANAGER Hagi Gheorghe\n";
        fout << "JUCATOR Nita Florin 82 GK 1\nJUCATOR Ratiu Andrei 80 RB 2\nJUCATOR Dragusin Radu 84 CB 3\n"
             << "JUCATOR Burca Andrei 78 CB 4\nJUCATOR Bancu Nicusor 79 LB 11\nJUCATOR Marin Razvan 81 CM 6\n"
             << "JUCATOR Stanciu Nicolae 83 CAM 10\nJUCATOR Man Dennis 82 RW 20\nJUCATOR Mihaila Valentin 80 LW 22\n"
             << "JUCATOR Dragus Denis 79 ST 9\nJUCATOR Hagi Ianis 80 CAM 14\nJUCATOR Moldovan Horatiu 78 GK 12\n"
             << "MEDIC Popescu Ion Trauma 10\nANTRENOR Iordanescu Edi 45 2\nARBITRU Hategan Ovidiu 200 1\n";
        fout.close(); fin.open("tastatura.txt");
    }

    Club club("Initializare...");

    std::string tip;
    while(fin >> tip) {
        if (tip == "CLUB") {
            std::string temp;
            std::getline(fin, temp);
            if(!temp.empty() && temp[0] == ' ') temp.erase(0, 1);
            std::replace(temp.begin(), temp.end(), '_', ' ');
            numeClub = temp;
        }
        else if (tip == "MANAGER") {
            std::string n, p;
            fin >> n >> p;
            numeManager = n + " " + p;
        }
        else if(tip == "JUCATOR") {
            std::string n, p; fin >> n >> p;
            int r, nr; std::string poz; fin >> r >> poz >> nr;
            Jucator* j = JucatorFactory::getInstance().creazaJucator(n, p, r, poz, nr);
            lot.push_back(std::unique_ptr<Jucator>(j));
            club.adaugaMembru(*j);
        } else if(tip == "MEDIC") {
            std::string n, p; fin >> n >> p;
            std::string s; int e; fin >> s >> e; medici.push_back(std::make_unique<Medic>(n, p, s, e));
        } else if(tip == "ANTRENOR") {
            std::string n, p; fin >> n >> p;
            int a, t; fin >> a >> t; antrenori.push_back(std::make_unique<Antrenor>(n, p, a, t));
        } else {
            std::string d; std::getline(fin, d);
        }
    }
    fin.close();

    autoSelecteazaPrimul11();
    initLiga();
    finante.adaugaSponsor(30000);

    bool running = true;
    while(running) {
        clear();
        std::cout << "=== MANAGER PRO 2025 ===\n";
        std::cout << "Club: " << numeClub << " | Manager: " << numeManager << "\n";
        finante.afiseazaRaportDetaliat();
        std::cout << "Echipa: " << (echipaModificataManual ? "MANUAL" : "AUTO") << " | Rating: " << calculeazaPutereReala() << "\n";

        ziar.afiseaza();

        std::cout << "\n1. JOACA MECI\n2. TACTICA & ECHIPA\n3. TRANSFERURI\n4. FINANTE\n5. ACADEMIE\n6. CLASAMENT\n7. STAFF\n8. DETALII CLUB & ISTORIC\n9. SAVE & IESI\nCmd: ";
        int cmd; std::cin >> cmd;

        switch(cmd) {
            case 1: joacaMeciOficial(); break;
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
            case 7: meniuStaff(); break;
            case 8:
                std::cout << "1. Detalii Club\n2. Istoric Meciuri\n";
                int k; std::cin >> k;
                if(k==1) meniuDetaliiClub(club);
                else meniuStatistici();
                break;
            case 9:
                club.salveazaInFisier();
                finante.detaseaza(&ziar);
                running = false;
                break;
        }
    }

    return 0;
}