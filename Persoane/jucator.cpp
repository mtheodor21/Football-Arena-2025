#include "jucator.h"
// CORECTIE: Litere mici la fisier
#include "../Exceptii/exceptii.h"

Jucator::Jucator(const std::string& nume, const std::string& prenume, int rating, std::string pozitie, int nr)
    : Persoana(nume, prenume), ratingGeneral(rating), pozitie(pozitie), numarTricou(nr) {

    if (rating < 0 || rating > 100) {
        throw EroareDateInvalide("Rating-ul jucatorului trebuie sa fie intre 0 si 100.");
    }
}

Jucator::Jucator(const Jucator& other)
    : Persoana(other), ratingGeneral(other.ratingGeneral), pozitie(other.pozitie), numarTricou(other.numarTricou) {
}

Jucator* Jucator::clone() const {
    return new Jucator(*this);
}

int Jucator::calculeazaEficienta() const {
    return ratingGeneral;
}

std::string Jucator::getPozitie() const {
    return pozitie;
}

void Jucator::afisareDetaliata(std::ostream& os) const {
    os << "Rol: JUCATOR\n";
    os << "Pozitie: " << pozitie << " | Numar: #" << numarTricou << "\n";
    os << "Rating FIFA: " << ratingGeneral << "\n";
}