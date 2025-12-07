#ifndef JUCATOR_H
#define JUCATOR_H

#include <iostream>
#include <string>
#include "persoane.h"

class Jucator : public Persoana {
protected:
    int ratingGeneral;
    std::string pozitie;
    int numarTricou;

public:
    Jucator(const std::string& nume, const std::string& prenume, int rating, std::string pozitie, int nr);
    Jucator(const Jucator& other);
    virtual ~Jucator() = default;

    Jucator* clone() const override;
    int calculeazaEficienta() const override;

    std::string getPozitie() const;
    int getRating() const { return ratingGeneral; }

protected:
    void afisareDetaliata(std::ostream& os) const override;
};

class Portar : public Jucator {
public:
    using Jucator::Jucator;
    Portar* clone() const override;
    int calculeazaEficienta() const override;
protected:
    void afisareDetaliata(std::ostream& os) const override;
};

class Fundas : public Jucator {
public:
    using Jucator::Jucator;
    Fundas* clone() const override;
    int calculeazaEficienta() const override;
protected:
    void afisareDetaliata(std::ostream& os) const override;
};

class Mijlocas : public Jucator {
public:
    using Jucator::Jucator;
    Mijlocas* clone() const override;
    int calculeazaEficienta() const override;
protected:
    void afisareDetaliata(std::ostream& os) const override;
};

class Atacant : public Jucator {
public:
    using Jucator::Jucator;
    Atacant* clone() const override;
    int calculeazaEficienta() const override;
protected:
    void afisareDetaliata(std::ostream& os) const override;
};

class FundasCentral : public Fundas {
public:
    using Fundas::Fundas;
    FundasCentral* clone() const override;
protected:
    void afisareDetaliata(std::ostream& os) const override;
};

class FundasStanga : public Fundas {
public:
    using Fundas::Fundas;
    FundasStanga* clone() const override;
protected:
    void afisareDetaliata(std::ostream& os) const override;
};

class FundasDreapta : public Fundas {
public:
    using Fundas::Fundas;
    FundasDreapta* clone() const override;
protected:
    void afisareDetaliata(std::ostream& os) const override;
};

class MijlocasCentral : public Mijlocas {
public:
    using Mijlocas::Mijlocas;
    MijlocasCentral* clone() const override;
protected:
    void afisareDetaliata(std::ostream& os) const override;
};

class MijlocasDefensiv : public Mijlocas {
public:
    using Mijlocas::Mijlocas;
    MijlocasDefensiv* clone() const override;
protected:
    void afisareDetaliata(std::ostream& os) const override;
};

class MijlocasOfensiv : public Mijlocas {
public:
    using Mijlocas::Mijlocas;
    MijlocasOfensiv* clone() const override;
protected:
    void afisareDetaliata(std::ostream& os) const override;
};

class MijlocasStanga : public Mijlocas {
public:
    using Mijlocas::Mijlocas;
    MijlocasStanga* clone() const override;
protected:
    void afisareDetaliata(std::ostream& os) const override;
};

class MijlocasDreapta : public Mijlocas {
public:
    using Mijlocas::Mijlocas;
    MijlocasDreapta* clone() const override;
protected:
    void afisareDetaliata(std::ostream& os) const override;
};

class AtacantCentral : public Atacant {
public:
    using Atacant::Atacant;
    AtacantCentral* clone() const override;
protected:
    void afisareDetaliata(std::ostream& os) const override;
};

class AtacantStanga : public Atacant {
public:
    using Atacant::Atacant;
    AtacantStanga* clone() const override;
protected:
    void afisareDetaliata(std::ostream& os) const override;
};

class AtacantDreapta : public Atacant {
public:
    using Atacant::Atacant;
    AtacantDreapta* clone() const override;
protected:
    void afisareDetaliata(std::ostream& os) const override;
};

#endif