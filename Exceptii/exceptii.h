#ifndef EXCEPTII_H
#define EXCEPTII_H

#include <exception>
#include <string>

// Baza ierarhiei proprii de exceptii
class EroareSimulare : public std::exception {
protected:
    std::string mesaj;
public:
    explicit EroareSimulare(const std::string& msg) : mesaj(msg) {}

    virtual const char* what() const noexcept override {
        return mesaj.c_str();
    }
};

// Exceptie 1: Date invalide (ex: nume gol, stats negative)
class EroareDateInvalide : public EroareSimulare {
public:
    explicit EroareDateInvalide(const std::string& msg)
        : EroareSimulare("[Date Invalide] " + msg) {}
};

// Exceptie 2: Probleme financiare (ex: buget depasit)
class EroareFinanciara : public EroareSimulare {
public:
    explicit EroareFinanciara(const std::string& msg)
        : EroareSimulare("[Eroare Financiara] " + msg) {}
};

// Exceptie 3: Logica joc (ex: nu sunt destui jucatori)
class EroareLogicaJoc : public EroareSimulare {
public:
    explicit EroareLogicaJoc(const std::string& msg)
        : EroareSimulare("[Eroare Logica Joc] " + msg) {}
};

#endif