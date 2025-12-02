#include "antrenor.h"
// CORECTIE: Litere mici
#include "../Exceptii/exceptii.h"

Antrenor::Antrenor(const std::string& nume, const std::string& prenume, int ani, int trofee)
    : Persoana(nume, prenume), aniExperienta(ani), trofeeCastigate(trofee) {

    if (ani < 0) throw EroareDateInvalide("Anii de experienta nu pot fi negativi.");
}

Antrenor* Antrenor::clone() const {
    return new Antrenor(*this);
}

int Antrenor::calculeazaEficienta() const {
    int score = (aniExperienta * 2) + (trofeeCastigate * 5);
    return (score > 100) ? 100 : score;
}

void Antrenor::afisareDetaliata(std::ostream& os) const {
    os << "Rol: ANTRENOR\n";
    os << "Experienta: " << aniExperienta << " ani\n";
    os << "Palmares: " << trofeeCastigate << " trofee\n";
}