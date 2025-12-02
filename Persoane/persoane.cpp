#include "persoane.h"
#include "../Exceptii/Exceptii.h"

int Persoana::contorPersoane = 0;

Persoana::Persoana(const std::string& nume, const std::string& prenume)
    : nume(nume), prenume(prenume) {

    if (nume.empty() || prenume.empty()) {
        throw EroareDateInvalide("Numele sau prenumele nu poate fi gol!");
    }
    contorPersoane++;
}

Persoana::Persoana(const Persoana& other)
    : nume(other.nume), prenume(other.prenume) {
    contorPersoane++;
}

Persoana::~Persoana() {
    contorPersoane--;
}

void Persoana::afiseazaInformatii(std::ostream& os) const {
    os << "--------------------------------\n";
    os << "ID Persoana: " << this << "\n"; // Afisam adresa doar ca debug
    os << "Nume complet: " << getNumeComplet() << "\n";

    afisareDetaliata(os);
    os << "Eficienta calculata: " << calculeazaEficienta() << "/100\n";
    os << "--------------------------------\n";
}

std::string Persoana::getNumeComplet() const {
    return nume + " " + prenume;
}

void Persoana::afisareDetaliata(std::ostream& os) const {

}

int Persoana::getNumarPersoane() {
    return contorPersoane;
}

std::ostream& operator<<(std::ostream& os, const Persoana& p) {
    p.afiseazaInformatii(os);
    return os;
}