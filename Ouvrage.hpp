#ifndef H_OUVRAGE
#define H_OUVRAGE

#include "OuvrageType.hpp"

#include <string>
#include <memory>

class Auteur;

class Ouvrage
{
private:
    std::string titre_;
    OuvrageType ouvrage_type_;
    std::weak_ptr<Auteur> auteur_;

public:

    Ouvrage() = delete;
    Ouvrage(std::string, OuvrageType, std::weak_ptr<Auteur>);

    std::string getTitre() const;
    OuvrageType getOuvrageType() const;
    std::weak_ptr<Auteur> getAuteur() const;
};


#endif