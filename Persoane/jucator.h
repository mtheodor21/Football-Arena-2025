#ifndef JUCATOR_H
#define JUCATOR_H

#include "persoane.h"
#include <string>

class Jucator : public Persoana {
protected:
    int rating;
    std::string pozitie;
    int numar;

public:
    Jucator(const std::string& n, const std::string& p, int r, const std::string& poz, int nr);

    ~Jucator() override = default;

    virtual int calculeazaEficienta() const override;
    Persoana* clone() const override;

    const std::string& getPozitie() const;

    int getRating() const;

protected:
    void afisareDetaliata(std::ostream& os) const override;
};

class Portar : public Jucator {
public:
    using Jucator::Jucator;

    int calculeazaEficienta() const override;
    Persoana* clone() const override;
};

class Fundas : public Jucator {
public:
    using Jucator::Jucator;

    Persoana* clone() const override;
};

class Mijlocas : public Jucator {
public:
    using Jucator::Jucator;

    Persoana* clone() const override;
};

class Atacant : public Jucator {
public:
    using Jucator::Jucator;
    Persoana* clone() const override;
};

#endif