#ifndef JUCATOR_H
#define JUCATOR_H

#include "persoane.h"

class Jucator : public Persoana {
    int ratingGeneral;
    std::string pozitie;
    int numarTricou;

public:
    Jucator(const std::string& nume, const std::string& prenume, int rating, std::string pozitie, int nr);
    Jucator(const Jucator& other);


    Jucator* clone() const override;


    int calculeazaEficienta() const override;


    std::string getPozitie() const;

protected:

    void afisareDetaliata(std::ostream& os) const override;
};

#endif