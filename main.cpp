#include <iostream>
#include <string>


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
    Antrenor(const std::string &Nume,const std::string &Prenume,const int &varsta) {
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

public:
    Echipa(const std::string &Nume,const std::string &Stadion,const int &buget) {
        this->Nume = Nume;
        this->Stadion = Stadion;
        this->buget = buget;
        this->Nume_Antrenor = Nume_Antrenor;

    }
    Echipa(const Echipa &other) {
        Nume = other.Nume;
        Stadion = other.Stadion;
        Localitate = other.Localitate;
        buget = other.buget;
        Nume_Antrenor = other.Nume_Antrenor;

    }
    Echipa &operator=(const Echipa &other) = default;
    friend std::ostream &operator<<( std::ostream &os, const Echipa &stats) {
        os << "Nume: " << stats.Nume << std::endl;
        os << "Stadion: " << stats.Stadion << std::endl;
        os << "Localitate: " << stats.Localitate << std::endl;
        os << "buget: " << stats.buget << std::endl;
        os << "Nume_Antrenor: " << stats.Nume_Antrenor << std::endl;

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
int main() {

    Stats_Jucator jucator(12 , 23 , 43 , 43 , 43, 43);
    std::cout << jucator;
    return 0;
}
