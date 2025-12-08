#ifndef ISTORIC_H
#define ISTORIC_H

#include <vector>
#include <stdexcept>
#include <iostream> // Necesara pentru std::cout in afiseazaIstoric

template <typename T>
class Istoric {
private:
    std::vector<T> evenimente;
    size_t capacitateMaxima;

public:

    Istoric(size_t capacitate = 20) : capacitateMaxima(capacitate) {}

    void adaugaEveniment(const T& eveniment) {
        if (evenimente.size() >= capacitateMaxima) {
            evenimente.erase(evenimente.begin());
        }
        evenimente.push_back(eveniment);
    }

    T getUltimulEveniment() const {
        if (evenimente.empty()) {
            return T();
        }
        return evenimente.back();
    }

    size_t getNumarEvenimente() const {
        return evenimente.size();
    }

    const std::vector<T>& getEvenimente() const {
        return evenimente;
    }


    void afiseazaIstoric() const {
        if (evenimente.empty()) {
            std::cout << "Nu exista evenimente in istoric." << std::endl;
            return;
        }

        for (const auto& ev : evenimente) {
            std::cout << ev << std::endl;
        }
    }
};

#endif