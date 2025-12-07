#ifndef ISTORIC_H
#define ISTORIC_H

#include <vector>
#include <iostream>
#include <algorithm>

template <typename T>
class Istoric {
private:
    std::vector<T> evenimente;
    int capacitateMaxima;

public:
    explicit Istoric(int capacitate = 100) : capacitateMaxima(capacitate) {}

    void adaugaEveniment(const T& eveniment) {
        if (evenimente.size() >= capacitateMaxima) {
            evenimente.erase(evenimente.begin());
        }
        evenimente.push_back(eveniment);
    }

    void afiseazaIstoric() const {
        std::cout << "--- ISTORIC ---\n";
        for (const auto& ev : evenimente) {
            std::cout << ev << "\n";
        }
    }

    T getUltimulEveniment() const {
        if (evenimente.empty()) throw std::runtime_error("Istoric gol!");
        return evenimente.back();
    }

    int getNumarEvenimente() const {
        return evenimente.size();
    }
};

#endif