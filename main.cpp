#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <limits>
#include <ctime>
#include <stdexcept>
#include <cstdlib>


class Stats_Jucator {

    int pace;
    int shooting;
    int passing;
    int dribbling;
    int defending;
    int physical;

public:
    Stats_Jucator(const int &pace, const int &shooting, const int &passing, const int &dribbling, const int &defending, const int &physical) {
        this->pace = pace;
        this-> shooting = shooting;
        this->passing = passing;
        this->dribbling = dribbling;
        this->defending = defending;
        this->physical = physical;

    }

    Stats_Jucator(const Stats_Jucator &other) {
        pace = other.pace;
        shooting = other.shooting;
        passing = other.passing;
        dribbling = other.dribbling;
        defending = other.defending;
        physical = other.physical;

    }

    Stats_Jucator &operator=(const Stats_Jucator &other) = default;

    friend std::ostream &operator<<( std::ostream &os, const Stats_Jucator &stats) {

        os << "Statistici jucator: "<< std::endl;
        os << "pace: " << stats.pace << std::endl;
        os << "shooting: "<<stats.shooting<< std::endl;
        os << "passing: "<<stats.passing<< std::endl;
        os << "dribbling: "<<stats.dribbling<< std::endl;
        os << "defending: "<<stats.defending<< std::endl;
        os << "physical: "<<stats.physical<< std::endl;
        return os;
    }

    ~Stats_Jucator() = default;


};

class informatii_jucator {
    std::string Nume;
    std::string Prenume;
    std::string Nationalitate;
    int Greutate;
    int Inaltime;
    std::string Pozitie_de_baza;
    std::string Pozitii_alternative;

    public:
    informatii_jucator(const std::string &Nume,const std::string &Prenume,const std::string &Nationalitate, const int &Greutate, const int &Inaltime, const std::string &Pozitie_de_baza, const std::string &Pozitii_alternative ) {
        this->Nume = Nume;
        this->Prenume = Prenume;
        this->Nationalitate = Nationalitate;
        this->Greutate = Greutate;
        this->Inaltime = Inaltime;
        this->Pozitie_de_baza = Pozitie_de_baza;
        this->Pozitii_alternative = Pozitii_alternative;

    }
    informatii_jucator(const informatii_jucator &other) {
        Nume = other.Nume;
        Prenume = other.Prenume;
        Nationalitate = other.Nationalitate;
        Greutate = other.Greutate;
        Inaltime = other.Inaltime;
        Pozitie_de_baza = other.Pozitie_de_baza;
        Pozitii_alternative = other.Pozitii_alternative;
    }
    informatii_jucator &operator=(const informatii_jucator &other) = default;

    friend std::ostream &operator<<( std::ostream &os, const informatii_jucator &stats) {
        os << "Nume: " << stats.Nume << std::endl;
        os << "Prenume: " << stats.Prenume << std::endl;
        os << "Nationalitate: " << stats.Nationalitate << std::endl;
        os << "Greutate: " << stats.Greutate << std::endl;
        os << "Inaltime: " << stats.Inaltime << std::endl;
        os << "Pozitie de baza: " << stats.Pozitie_de_baza << std::endl;
        os << "Pozitii alternative: " << stats.Pozitii_alternative << std::endl;
        return os;

    }
    ~informatii_jucator() = default;
};

class Antrenor {
    std::string Nume;
    std::string Prenume;
    std::string Nationalitate;
    int varsta;

    public:
    Antrenor(const std::string &Nume,const std::string &Prenume,const int &varsta, const std::string &Nationalitate) {
        this->Nume = Nume;
        this->Prenume = Prenume;
        this->varsta = varsta;
        this->Nationalitate = Nationalitate;

    }

    Antrenor(const Antrenor &other) {
        Nume = other.Nume;
        Prenume = other.Prenume;
        Nationalitate = other.Nationalitate;
        varsta = other.varsta;

    }
    Antrenor &operator=(const Antrenor &other) = default;
    friend std::ostream &operator<<( std::ostream &os, const Antrenor &stats) {
        os << "Nume: " << stats.Nume << std::endl;
        os << "Prenume: " << stats.Prenume << std::endl;
        os << "Nationalitate: " << stats.Nationalitate << std::endl;
        os << "Varsta: " << stats.varsta << std::endl;

        return os;
    }
    ~Antrenor() = default;
};

class Echipa {
    std::string Nume;
    std::string Stadion;
    std::string Localitate;
    int buget;
    std::string Nume_Antrenor;
    int rating;

public:
    Echipa(const std::string &Nume,const std::string &Stadion,const int &buget, const int &rating, const std::string &Nume_Antrenor) {
        this->Nume = Nume;
        this->Stadion = Stadion;
        this->buget = buget;
        this->Nume_Antrenor = Nume_Antrenor;
        this->rating = rating;
    }
    Echipa(const Echipa &other) {
        Nume = other.Nume;
        Stadion = other.Stadion;
        Localitate = other.Localitate;
        buget = other.buget;
        Nume_Antrenor = other.Nume_Antrenor;
        rating = other.rating;

    }
     const std::string& getNume() const {
        return Nume;
    }

     int getRating() const {
        return rating;
    }


    Echipa &operator=(const Echipa &other) = default;
    friend std::ostream &operator<<( std::ostream &os, const Echipa &stats) {
        os << "Nume: " << stats.Nume << std::endl;
        os << "Stadion: " << stats.Stadion << std::endl;
        os << "Localitate: " << stats.Localitate << std::endl;
        os << "buget: " << stats.buget << std::endl;
        os << "Nume_Antrenor: " << stats.Nume_Antrenor << std::endl;
        os << "Rating echipa: " << stats.rating << std::endl;
        return os;
    }
    ~Echipa() = default;

};

class Staff_Medical {
    std::string Nume;
    std::string Prenume;
    std::string Specializare;

    public:
    Staff_Medical(const std::string &Nume,const std::string &Prenume,const std::string &Specializare) {
        this->Nume = Nume;
        this->Prenume = Prenume;
        this->Specializare = Specializare;

    }
    Staff_Medical(const Staff_Medical &other) {
        Nume = other.Nume;
        Prenume = other.Prenume;
        Specializare = other.Specializare;

    }
    Staff_Medical &operator=(const Staff_Medical &other) = default;
    friend std::ostream &operator<<( std::ostream &os, const Staff_Medical &stats) {
        os << "Nume: " << stats.Nume << std::endl;
        os << "Prenume: " << stats.Prenume << std::endl;
        os << "Specializare: " << stats.Specializare << std::endl;

        return os;
    }
    ~Staff_Medical() = default;

};

class Arbitru {
    std::string Nume;
    std::string Prenume;
    std::string Localitate;

    public:
    Arbitru(const std::string &Nume,const std::string &Prenume,const std::string &Localitate) {
        this->Nume = Nume;
        this->Prenume = Prenume;
        this->Localitate = Localitate;

    }
    Arbitru(const Arbitru &other) {
        Nume = other.Nume;
        Prenume = other.Prenume;
        Localitate = other.Localitate;

    }

    std::string getNumeComplet() const {
        return Nume + " " + Prenume;
    }
    Arbitru &operator=(const Arbitru &other) = default;
    friend std::ostream &operator<<( std::ostream &os, const Arbitru &stats) {
        os << "Nume: " << stats.Nume << std::endl;
        os << "Prenume: " << stats.Prenume << std::endl;
        os << "Localitate: " << stats.Localitate << std::endl;

        return os;
    }
    ~Arbitru() = default;
};

class Competitie {
    std::string Nume;
    std::string Echipe;
    std::string Reguli;
    public:
    Competitie(const std::string &Nume,const std::string &Echipe,const std::string &Reguli) {
        this->Nume = Nume;
        this->Echipe = Echipe;
        this->Reguli = Reguli;

    }
    Competitie(const Competitie &other) {
        Nume = other.Nume;
        Echipe = other.Echipe;
        Reguli = other.Reguli;

    }
    Competitie &operator=(const Competitie &other) = default;
    friend std::ostream &operator<<( std::ostream &os, const Competitie &stats) {
        os << "Nume: " << stats.Nume << std::endl;
        os << "Echipe: " << stats.Echipe << std::endl;
        os << "Reguli: " << stats.Reguli << std::endl;

        return os;
    }
    ~Competitie() = default;

};

class Finante_Club {
    int Fonduri_Totale;
    int buget_Transferuri;
    int buget_Salarii;
    public:
    Finante_Club() {
        Fonduri_Totale = 0;
        buget_Transferuri = 0;
        buget_Salarii = 0;

    }
    Finante_Club(const Finante_Club &other) {
        Fonduri_Totale = other.Fonduri_Totale;
        buget_Transferuri = other.buget_Transferuri;
        buget_Salarii = other.buget_Salarii;


    }
    Finante_Club &operator=(const Finante_Club &other) = default;
    friend std::ostream &operator<<( std::ostream &os, const Finante_Club &stats) {
        os << "Fonduri: " << stats.Fonduri_Totale << std::endl;
        os << "Buget_Transferuri: " << stats.buget_Transferuri << std::endl;
        os << "Buget_Salarii: " << stats.buget_Salarii << std::endl;

        return os;
    }
    ~Finante_Club() = default;
};

class Contract {
    int Salariu_Persoana;
    std::string Data_Expirare;
    int Clauza_Reziliere;
    public:
    Contract() {
        Salariu_Persoana = 0;
        Data_Expirare = "";
        Clauza_Reziliere = 0;

    }
    Contract(const Contract &other) {
        Salariu_Persoana = other.Salariu_Persoana;
        Data_Expirare = other.Data_Expirare;
        Clauza_Reziliere = other.Clauza_Reziliere;

    }
    Contract &operator=(const Contract &other) = default;
    friend std::ostream &operator<<( std::ostream &os, const Contract &stats) {
        os << " Salariu_Persoana: " <<stats.Salariu_Persoana << std::endl;
        os<< "Data_Expirare: " <<stats.Data_Expirare << std::endl;
        os<<" Clauza_Reziliere: " <<stats.Clauza_Reziliere << std::endl;

        return os;

    }
    ~Contract() = default;
};

class Meci {
    const Echipa& EchipaAcasa;
    const Echipa& EchipaDeplasare;
    const Arbitru& ArbitruMeci;
    int ScorAcasa;
    int ScorDeplasare;
    bool simulat;

    public:
    Meci(const Echipa& Acasa, const Echipa& Deplasare, const Arbitru& ArbitruMeci)
        : EchipaAcasa(Acasa),
        EchipaDeplasare(Deplasare),
        ArbitruMeci(ArbitruMeci),
        ScorAcasa(0),
        ScorDeplasare(0),
        simulat(false)
    {

    }
    void seteazaRezultat(int GoluriAcasa, int GoluriDeplasare) {
        this->ScorAcasa = GoluriAcasa;
        this->ScorDeplasare = GoluriDeplasare;
        this->simulat = true;
    }

    const Echipa& getEchipaAcasa() const {
        return EchipaAcasa;
    }
    const Echipa& getEchipaDeplasare() const {
        return EchipaDeplasare;
    }

    int getScorAcasa() const {
        return ScorAcasa;
    }
    int getScorDeplasare() const {
        return ScorDeplasare;
    }
    bool getsimulat() const {
        return simulat;
    }

    friend std::ostream &operator<<( std::ostream &os, const Meci &meci) {
        os << " ~~~~~~~ SCOR MECI ~~~~~~" << std::endl;
        os << "Acasa" << meci.EchipaAcasa.getNume() << std::endl;
        os << "Deplasare" << meci.EchipaDeplasare.getNume() << std::endl;
        os << "Arbitru" << meci.ArbitruMeci.getNumeComplet() << std::endl;

        if (meci.getsimulat()) {
            os << "Rezultat" << meci.ScorAcasa << "-" << meci.ScorDeplasare << std::endl;
        }
        else
        {
         os << "Meciul nu s-a jucat inca!"<< std::endl;
        }
        return os;
    }
};

class MotorSimulareMeci
{
public:
    MotorSimulareMeci() = default;


    static void SimuleazaMeci(Meci& meci) {
        const Echipa& EchipaAcasa = meci.getEchipaAcasa();
        const Echipa& EchipaDeplasare  = meci.getEchipaDeplasare();

        int PutereAcasa = EchipaAcasa.getRating();
        int PutereDeplasare = EchipaDeplasare.getRating();

        int ScorAcasa = 0;
        int ScorDeplasare = 0;

        PutereAcasa += 5;

        int SanseAcasa = PutereAcasa/10;
        int SanseDeplasare = PutereDeplasare/10;

        for (int i=0;i< SanseAcasa;i++) {
            if (rand() % 100 < 30) {
                ScorAcasa++;
            }
        }

        for (int i=0;i<SanseDeplasare;i++) {
            if (rand() % 100 < 30) {
                ScorDeplasare++;
            }
        }
        if (rand() % 100 < 10 ) {
            if (rand() % 2 == 0)
                ScorAcasa++;
            else
                ScorDeplasare++;
        }
    meci.seteazaRezultat(ScorAcasa, ScorDeplasare);
    }
};
int main() {
    srand(static_cast<unsigned int>(time(nullptr)));

    std::vector<Echipa> echipe;
    std::vector<Arbitru> arbitri;

    std::ifstream fisier("tastatura.txt");
    std::string identificator;

    if (!fisier.is_open()) {
        std::cout << "[EROARE CRITICA] Nu am putut deschide fisierul cu informatii!"<< std::endl;
        return 1;
    }

    std::cout << "Se incarca datele din fisier..."<<std::endl;

    // citim fisierul linie cu linie
    while (std::getline(fisier, identificator)) {

        // ignoram liniile goale sau comentariile
        if (identificator.empty() || identificator[0] == '#') {
            continue;
        }

        // oprim citirea la semnalul de sfarsit
        if (identificator == "END") {
            break;
        }

        // folosim un try-catch pentru a preveni erorile de conversie (ex daca bugetul e text)
        try {
            if (identificator == "ECHIPA") {
                // citim cele 5 linii de date
                std::string nume, stadion, bugetStr, ratingStr, antrenor;
                std::getline(fisier, nume);
                std::getline(fisier, stadion);
                std::getline(fisier, bugetStr);
                std::getline(fisier, ratingStr);
                std::getline(fisier, antrenor);

                // convertim textul in numere
                int buget = std::stoi(bugetStr);
                int rating = std::stoi(ratingStr);

                // am creeat obiectul si il adaugam in vector
                echipe.push_back(Echipa(nume, stadion, buget, rating, antrenor));

            } else if (identificator == "ARBITRU") {
                // citim cele 3 linii de date
                std::string nume, prenume, localitate;
                std::getline(fisier, nume);
                std::getline(fisier, prenume);
                std::getline(fisier, localitate);

                // am creeat obiectul si il adaugam in vector
                arbitri.push_back(Arbitru(nume, prenume, localitate));

            }
            else if (identificator == "ANTRENOR") {
                std::string l1, l2, l3, l4;
                std::getline(fisier, l1);
                std::getline(fisier, l2);
                std::getline(fisier, l3);
                std::getline(fisier, l4);
            } else if (identificator == "STAFF") {
                std::string l1, l2, l3;
                std::getline(fisier, l1);
                std::getline(fisier, l2);
                std::getline(fisier, l3);
            } else if (identificator == "JUCATOR_INFO") {
                std::string l;
                for(int i=0; i<7; ++i) std::getline(fisier, l);
            } else if (identificator == "JUCATOR_STATS") {
                std::string l;
                for(int i=0; i<6; ++i) std::getline(fisier, l);
            }


        } catch (const std::invalid_argument&) {
            std::cout << "[EROARE FISIER] O linie nu a putut fi convertita in numar. Sar peste intrare."<<std::endl;
        }
    }

    fisier.close(); // inchidem fisierul
    std::cout << "Date incarcate: " << echipe.size() << " echipe si "
              << arbitri.size() << " arbitri."<< std::endl;

    // bloc pt rezolvarea erorilor github actions
    if (std::getenv("CI") != nullptr)
    {
        // suntem in github actions
        std::cout << std::endl<< "Suntem in github actions.Testam programul..."<<std::endl;

        if (echipe.size() < 2 || arbitri.empty()) {
            std::cout << "[EROARE] Nu exista suficiente date (minim 2 echipe si 1 arbitru)!"<<std::endl;
            return 1; // eșueaza build ul dacă datele nu s au încarcat
        }

        Meci meciDemo(echipe[0], echipe[1], arbitri[0]);
        std::cout << "~~~~~~ Inainte de simulare ~~~~~~"<<std::endl;
        std::cout << meciDemo;

        MotorSimulareMeci::SimuleazaMeci(meciDemo);

        std::cout << std::endl<<" ~~~~~~Dupa simulare ~~~~~~"<<std::endl;
        std::cout << meciDemo;

        std::cout <<std::endl<< "Test complet."<<std::endl;
        return 0;
    }

else {
    std::cout << std::endl<< "Bun venit in Football Arena 2025 - Varianta Beta" << std::endl;
    bool ruleaza = true;
    int optiune;

    while (ruleaza) {
        // afisam optiunile
        std::cout << std::endl<<"~~~~~~~~ MENIU PRINCIPAL ~~~~~~~~"<<std::endl;
        std::cout << "1. Vezi lista echipelor din campionat"<<std::endl;
        std::cout << "2. Simuleaza un meci demonstrativ (primele 2 echipe din fisier)"<< std::endl;
        std::cout << "0. Iesire din joc"<<std::endl;
        std::cout << "~~~~~~~~~~~~~~~~~~~~~~"<<std::endl;
        std::cout << "Alegerea ta: ";

        // citim optiunea
        std::cin >> optiune;

        // verificam daca input ul este valid
        if (std::cin.fail()) {
            std::cout << std::endl<< "[EROARE] Te rog introdu un numar valid."<<std::endl;
            std::cin.clear(); // Reseteaza flag-ul de eroare al cin
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        switch (optiune) {
            case 1:
                // optiunea 1, afiseaza echipele
                std::cout << std::endl<< "~~~~~~ Echipele din Campionat  ~~~~~~"<<std::endl;
                if(echipe.empty()) {
                    std::cout << "Nicio echipa nu a fost incarcata.\n";
                }
                for (const Echipa& echipa : echipe) {
                    std::cout << "~~~~~~~~~~" <<std::endl<< echipa;
                }
                break;

            case 2:
                // optiunea 2,simulam meciul
                std::cout << std::endl <<"~~~~~~ Meci Demonstrativ ~~~~~~"<<std::endl;

                // Verificam daca avem suficiente echipe si arbitri
                if (echipe.size() < 2 || arbitri.empty()) {
                    std::cout << "[Eroare] Nu exista suficiente date (minim 2 echipe si 1 arbitru)!"<<std::endl;
                } else {
                    // Cream meciul folosind primele echipe si primul arbitru din fisier
                    Meci meciDemo(echipe[0], echipe[1], arbitri[0]);

                    std::cout << "~~~~~~ Inainte de simulare ~~~~~~"<<std::endl;
                    std::cout << meciDemo; // Afiseaza meciul (nejucat)

                    // Simulam meciul
                    MotorSimulareMeci::SimuleazaMeci(meciDemo);

                    std::cout << std::endl<<" ~~~~~~Dupa simulare ~~~~~~"<<std::endl;
                    std::cout << meciDemo; // Afiseaza meciul (jucat)
                }
                break;

            case 0:
                // --- Optiunea 0: Iesire ---
                std::cout << "Multumim pentru testare. La revedere! " << std::endl;
                ruleaza = false; // Opreste bucla while
                break;

            default:
                // --- Orice alta optiune ---
                std::cout <<std::endl<< "[EROARE] Optiune invalida. Te rog alege 0, 1 sau 2."<<std::endl;
                break;
        } // Sfarsit switch
    } // Sfarsit while(ruleaza)
}
    return 0;
}