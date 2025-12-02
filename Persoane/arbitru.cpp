#include "arbitru.h"

Arbitru::Arbitru(const std::string& nume, const std::string& prenume, int meciuri, bool fifa)
    : Persoana(nume, prenume), meciuriArbitrate(meciuri), areEcusonFIFA(fifa) {}

Arbitru* Arbitru::clone() const {
    return new Arbitru(*this);
}

int Arbitru::calculeazaEficienta() const {

    if (areEcusonFIFA) return 95;
    int calc = 50 + (meciuriArbitrate / 10);
    return (calc > 90) ? 90 : calc;
}

void Arbitru::afisareDetaliata(std::ostream& os) const {
    os << "Rol: ARBITRU\n";
    os << "Meciuri arbitrate: " << meciuriArbitrate << "\n";
    os << "Ecuson FIFA: " << (areEcusonFIFA ? "DA" : "NU") << "\n";
}