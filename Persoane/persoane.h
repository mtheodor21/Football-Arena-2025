#ifndef PERSOANE_H
#define PERSOANE_H

#include <string>
#include <iostream>

class Persoana {
protected:
    std::string nume;
    std::string prenume;

public:
    Persoana(const std::string& n, const std::string& p);
    virtual ~Persoana();

    std::string getNumeComplet() const;

    virtual int calculeazaEficienta() const = 0;
    virtual Persoana* clone() const = 0;

    friend std::ostream& operator<<(std::ostream& os, const Persoana& p);

protected:
    virtual void afisareDetaliata(std::ostream& os) const = 0;
};

#endif