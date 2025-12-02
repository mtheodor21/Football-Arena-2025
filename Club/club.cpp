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

void Club::afiseazaMembri() const {
    std::cout << "\n=== Membrii Clubului " << numeClub << " ===\n";
    std::for_each(membri.begin(), membri.end(), [](const Persoana* p) {
        std::cout << *p;
    });
}

void Club::salveazaInFisier() const {
    std::ofstream fout("informatii_club.txt");

    if (!fout.is_open()) {
        std::cerr << "[EROARE] Nu s-a putut deschide fisierul 'informatii_club.txt' pentru scriere!\n";
        return;
    }

    fout << "=== RAPORT DETALIAT CLUB: " << numeClub << " ===\n\n";
    fout << "Numar total membri: " << membri.size() << "\n";
    fout << "--------------------------------\n";

    for (const auto* p : membri) {
        fout << *p;
    }

    fout.close();
    std::cout << "[INFO] Datele au fost salvate cu succes in fisierul 'informatii_club.txt'.\n";
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
    return std::count_if(membri.begin(), membri.end(), [](const Persoana* p) {
        return dynamic_cast<const Jucator*>(p) != nullptr;
    });
}