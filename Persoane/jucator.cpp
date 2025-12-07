#include "jucator.h"
#include "../Exceptii/exceptii.h"

Jucator::Jucator(const std::string& nume, const std::string& prenume, int rating, std::string pozitie, int nr)
    : Persoana(nume, prenume), ratingGeneral(rating), pozitie(pozitie), numarTricou(nr) {
    if (rating < 0 || rating > 100) throw EroareDateInvalide("Rating invalid (0-100).");
}
Jucator::Jucator(const Jucator& other)
    : Persoana(other), ratingGeneral(other.ratingGeneral), pozitie(other.pozitie), numarTricou(other.numarTricou) {}

Jucator* Jucator::clone() const { return new Jucator(*this); }
int Jucator::calculeazaEficienta() const { return ratingGeneral; }
std::string Jucator::getPozitie() const { return pozitie; }
void Jucator::afisareDetaliata(std::ostream& os) const {
    os << "Rol: JUCATOR GENERIC | Poz: " << pozitie << " | Nr: " << numarTricou << " | Rating: " << ratingGeneral << "\n";
}

Portar* Portar::clone() const { return new Portar(*this); }
int Portar::calculeazaEficienta() const { return ratingGeneral + 5; }
void Portar::afisareDetaliata(std::ostream& os) const {
    os << "Rol: [PORTAR] | Nr: " << numarTricou << " | Reflexe: " << ratingGeneral << "\n";
}

Fundas* Fundas::clone() const { return new Fundas(*this); }
int Fundas::calculeazaEficienta() const { return ratingGeneral + 2; }
void Fundas::afisareDetaliata(std::ostream& os) const {
    os << "Rol: [FUNDAS] | Poz: " << pozitie << " | Defensiva: " << ratingGeneral << "\n";
}

Mijlocas* Mijlocas::clone() const { return new Mijlocas(*this); }
int Mijlocas::calculeazaEficienta() const { return ratingGeneral + 3; }
void Mijlocas::afisareDetaliata(std::ostream& os) const {
    os << "Rol: [MIJLOCAS] | Poz: " << pozitie << " | Creativitate: " << ratingGeneral << "\n";
}

Atacant* Atacant::clone() const { return new Atacant(*this); }
int Atacant::calculeazaEficienta() const { return ratingGeneral + 4; }
void Atacant::afisareDetaliata(std::ostream& os) const {
    os << "Rol: [ATACANT] | Poz: " << pozitie << " | Finalizare: " << ratingGeneral << "\n";
}

FundasCentral* FundasCentral::clone() const { return new FundasCentral(*this); }
void FundasCentral::afisareDetaliata(std::ostream& os) const {
    os << ">> Fundas Central (CB) | Zid defensiv | Rating: " << ratingGeneral << "\n";
}
FundasStanga* FundasStanga::clone() const { return new FundasStanga(*this); }
void FundasStanga::afisareDetaliata(std::ostream& os) const {
    os << ">> Fundas Stanga (LB) | Viteza si Centrare | Rating: " << ratingGeneral << "\n";
}
FundasDreapta* FundasDreapta::clone() const { return new FundasDreapta(*this); }
void FundasDreapta::afisareDetaliata(std::ostream& os) const {
    os << ">> Fundas Dreapta (RB) | Viteza si Urcare | Rating: " << ratingGeneral << "\n";
}

MijlocasCentral* MijlocasCentral::clone() const { return new MijlocasCentral(*this); }
void MijlocasCentral::afisareDetaliata(std::ostream& os) const {
    os << ">> Mijlocas Central (CM) | Motorul echipei | Rating: " << ratingGeneral << "\n";
}
MijlocasDefensiv* MijlocasDefensiv::clone() const { return new MijlocasDefensiv(*this); }
void MijlocasDefensiv::afisareDetaliata(std::ostream& os) const {
    os << ">> Mijlocas Defensiv (CDM) | Inchizator | Rating: " << ratingGeneral << "\n";
}
MijlocasOfensiv* MijlocasOfensiv::clone() const { return new MijlocasOfensiv(*this); }
void MijlocasOfensiv::afisareDetaliata(std::ostream& os) const {
    os << ">> Mijlocas Ofensiv (CAM) | Decar | Rating: " << ratingGeneral << "\n";
}
MijlocasStanga* MijlocasStanga::clone() const { return new MijlocasStanga(*this); }
void MijlocasStanga::afisareDetaliata(std::ostream& os) const {
    os << ">> Mijlocas Stanga (LM) | Extrema clasica | Rating: " << ratingGeneral << "\n";
}
MijlocasDreapta* MijlocasDreapta::clone() const { return new MijlocasDreapta(*this); }
void MijlocasDreapta::afisareDetaliata(std::ostream& os) const {
    os << ">> Mijlocas Dreapta (RM) | Extrema clasica | Rating: " << ratingGeneral << "\n";
}

AtacantCentral* AtacantCentral::clone() const { return new AtacantCentral(*this); }
void AtacantCentral::afisareDetaliata(std::ostream& os) const {
    os << ">> Atacant Central (ST) | Golgheter pur | Rating: " << ratingGeneral << "\n";
}
AtacantStanga* AtacantStanga::clone() const { return new AtacantStanga(*this); }
void AtacantStanga::afisareDetaliata(std::ostream& os) const {
    os << ">> Atacant Stanga (LW) | Intrare in centru | Rating: " << ratingGeneral << "\n";
}
AtacantDreapta* AtacantDreapta::clone() const { return new AtacantDreapta(*this); }
void AtacantDreapta::afisareDetaliata(std::ostream& os) const {
    os << ">> Atacant Dreapta (RW) | Dribling si sut | Rating: " << ratingGeneral << "\n";
}