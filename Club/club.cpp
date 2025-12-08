#include "club.h"
#include "../Persoane/jucator.h"
#include "../Persoane/antrenor.h"
#include "../Persoane/arbitru.h"
#include "../Exceptii/exceptii.h"
#include <fstream>
#include <algorithm>

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




void Club::salveazaInFisier() const {
    std::ofstream fout("informatii_club.txt");

    if (!fout.is_open()) {
        std::cerr << "[EROARE] Nu s-a putut deschide fisierul pentru scriere!\n";
        return;
    }

    fout << "=== RAPORT CLUB: " << numeClub << " ===\n";
    fout << "Total Membri: " << membri.size() << "\n\n";

    for (const auto* p : membri) {
        fout << *p;
    }

    fout.close();
    std::cout << "[INFO] Datele au fost salvate in 'informatii_club.txt'.\n";
}


void Club::analizeazaEchipa() const {
    std::cout << "\n--- Analiza Structura Echipa ---\n";
    for (const auto* p : membri) {
        if (const Jucator* j = dynamic_cast<const Jucator*>(p)) {
            std::cout << "[Jucator] " << j->getNumeComplet() << " (" << j->getPozitie() << ")\n";
        }
        else if (const Antrenor* a = dynamic_cast<const Antrenor*>(p)) {
            std::cout << "[Staff] " << a->getNumeComplet() << " (Antrenor)\n";
        }
        else if (dynamic_cast<const Arbitru*>(p)) {
            std::cout << "[Oficial] Arbitru delegat\n";
        }
    }
}

int Club::getNumarJucatori() const {
    return std::count_if(membri.begin(), membri.end(), [](const Persoana* p) {
        return dynamic_cast<const Jucator*>(p) != nullptr;
    });
}