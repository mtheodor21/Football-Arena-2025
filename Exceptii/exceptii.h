#ifndef EXCEPTII_H
#define EXCEPTII_H

#include <exception>
#include <string>


class EroareSimulare : public std::exception {
protected:
    std::string mesaj;
public:
    explicit EroareSimulare(const std::string& msg) : mesaj(msg) {}

    virtual const char* what() const noexcept override {
        return mesaj.c_str();
    }
};

class EroareDateInvalide : public EroareSimulare {
public:
    explicit EroareDateInvalide(const std::string& msg)
        : EroareSimulare("[Date Invalide] " + msg) {}
};

class EroareFinanciara : public EroareSimulare {
public:
    explicit EroareFinanciara(const std::string& msg)
        : EroareSimulare("[Eroare Financiara] " + msg) {}
};


class EroareLogicaJoc : public EroareSimulare {
public:
    explicit EroareLogicaJoc(const std::string& msg)
        : EroareSimulare("[Eroare Logica Joc] " + msg) {}
};

#endif