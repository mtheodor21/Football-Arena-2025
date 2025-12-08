#include "jucator.h"

Jucator::Jucator(const std::string& n, const std::string& p, int r, const std::string& poz, int nr)
    : Persoana(n, p), rating(r), pozitie(poz), numar(nr) {}

int Jucator::calculeazaEficienta() const {
    return rating;
}

Persoana* Jucator::clone() const {
    return new Jucator(*this);
}

const std::string& Jucator::getPozitie() const {
    return pozitie;
}

int Jucator::getRating() const {
    return rating;
}

void Jucator::afisareDetaliata(std::ostream& os) const {
    os << "Jucator (" << pozitie << ") #" << numar << " Rating: " << rating << "\n";
}

int Portar::calculeazaEficienta() const { return rating * 1.1; }
Persoana* Portar::clone() const { return new Portar(*this); }

Persoana* Fundas::clone() const { return new Fundas(*this); }

Persoana* Mijlocas::clone() const { return new Mijlocas(*this); }

Persoana* Atacant::clone() const { return new Atacant(*this); }