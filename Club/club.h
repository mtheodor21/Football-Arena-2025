#ifndef CLUB_H
#define CLUB_H

#include <vector>
#include <iostream>
#include <algorithm>
#include "../Persoane/persoane.h"

class Club {
    std::string numeClub;
    std::vector<Persoana*> membri;

public:
    explicit Club(const std::string& nume);

    ~Club();
    Club(const Club& other);
    Club& operator=(Club other);

    friend void swap(Club& first, Club& second) noexcept;

    void adaugaMembru(const Persoana& p);
    void afiseazaMembri() const;

    void analizeazaEchipa() const;

    int getNumarJucatori() const;
};

#endif