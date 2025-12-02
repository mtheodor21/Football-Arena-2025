// CORECTIE: Toate headerele cu litere mici
#include "club.h"
#include "../Persoane/jucator.h"
#include "../Persoane/antrenor.h"
#include "../Persoane/arbitru.h"
#include "../Exceptii/exceptii.h"

Club::Club(const std::string& nume) : numeClub(nume) {
    if (nume.empty()) throw EroareDateInvalide("Numele clubului nu poate fi gol");
}

Club::~Club() {
    for (Persoana* p : membri) {
        delete p;
    }
    membri.clear();
}

Club::Club(const Club& other) : numeClub(other.numeClub) {
    for (const auto* p : other.membri) {
        membri.push_back(p->clone());
    }
}

Club& Club::operator=(Club other) {
    swap(*this, other);
    return *this;
}

void swap(Club& first, Club& second) noexcept {
    using std::swap;
    swap(first.numeClub, second.numeClub);
    swap(first.membri, second.membri);
}

void Club::adaugaMembru(const Persoana& p) {
    membri.push_back(p.clone());
}

void Club::afiseazaMembri() const {
    std::cout << "\n=== Membrii Clubului " << numeClub << " ===\n";
    for (const auto* p : membri) {
        std::cout << *p;
    }
}

void Club::analizeazaEchipa() const {
    std::cout << "\n--- Analiza Structura Echipa ---\n";
    for (const auto* p : membri) {
        if (const Jucator* j = dynamic_cast<const Jucator*>(p)) {
            std::cout << "[Jucator gasit] " << j->getNumeComplet()
                      << " joaca pe pozitia " << j->getPozitie() << "\n";
        }
        else if (const Antrenor* a = dynamic_cast<const Antrenor*>(p)) {
            std::cout << "[STAFF TEHNIC] " << a->getNumeComplet() << " coordoneaza echipa.\n";
        }
        else if (dynamic_cast<const Arbitru*>(p)) {
            std::cout << "[OFICIAL] Avem un arbitru in incinta.\n";
        }
        else {
            std::cout << "[NECUNOSCUT] Persoana neidentificata.\n";
        }
    }
}

int Club::getNumarJucatori() const {
    int count = 0;
    for (const auto* p : membri) {
        if (dynamic_cast<const Jucator*>(p) != nullptr) {
            count++;
        }
    }
    return count;
}