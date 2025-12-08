#include "persoane.h"

#include "../Exceptii/exceptii.h"

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
    os << "ID Persoana: " << this << "\n";
    os << "Nume complet: " << getNumeComplet() << "\n";
    // Apelam partea virtuala
    afisareDetaliata(os);
    os << "Eficienta calculata: " << calculeazaEficienta() << "/100\n";
    os << "--------------------------------\n";
}

std::string Persoana::getNumeComplet() const {
    return nume + " " + prenume;
}

// CORECTIE: Am sters numele variabilei 'os' pentru a scapa de eroarea "unused parameter"
void Persoana::afisareDetaliata(std::ostream&) const {
    // Implementare default (poate fi goala)
}



std::ostream& operator<<(std::ostream& os, const Persoana& p) {
    p.afiseazaInformatii(os);
    return os;
}