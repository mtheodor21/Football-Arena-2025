#ifndef ANTRENOR_H
#define ANTRENOR_H

#include "persoane.h"

class Antrenor : public Persoana {
    int aniExperienta;
    int trofeeCastigate;

public:
    Antrenor(const std::string& nume, const std::string& prenume, int ani, int trofee);

    Antrenor* clone() const override;
    int calculeazaEficienta() const override;

protected:
    void afisareDetaliata(std::ostream& os) const override;
};

#endif