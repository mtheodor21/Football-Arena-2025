#include "persoane.h"

Persoana::Persoana(const std::string& n, const std::string& p) : nume(n), prenume(p) {}

Persoana::~Persoana() = default;

std::string Persoana::getNumeComplet() const {
    return nume + " " + prenume;
}

std::ostream& operator<<(std::ostream& os, const Persoana& p) {
    os << p.getNumeComplet() << " ";
    p.afisareDetaliata(os);
    return os;
}