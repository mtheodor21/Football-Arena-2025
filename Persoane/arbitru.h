#ifndef ARBITRU_H
#define ARBITRU_H

#include "persoane.h"

class Arbitru : public Persoana {
    int meciuriArbitrate;
    bool areEcusonFIFA;

public:
    Arbitru(const std::string& nume, const std::string& prenume, int meciuri, bool fifa);

    Arbitru* clone() const override;
    int calculeazaEficienta() const override;

protected:
    void afisareDetaliata(std::ostream& os) const override;
};

#endif