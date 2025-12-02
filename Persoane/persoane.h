#ifndef PERSOANA_H
#define PERSOANA_H

#include <iostream>
#include <string>
#include <memory>

class Persoana {
protected:
    std::string nume;
    std::string prenume;

    static int contorPersoane;

public:

    Persoana(const std::string& nume, const std::string& prenume);

    Persoana(const Persoana& other);


    virtual ~Persoana();


    virtual Persoana* clone() const = 0;


    virtual int calculeazaEficienta() const = 0;


    void afiseazaInformatii(std::ostream& os) const;

    std::string getNumeComplet() const;


    static int getNumarPersoane();

protected:

    virtual void afisareDetaliata(std::ostream& os) const;
};


std::ostream& operator<<(std::ostream& os, const Persoana& p);

#endif