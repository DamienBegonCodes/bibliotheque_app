#include "Ouvrage.hpp"

Ouvrage::Ouvrage(std::string titre, OuvrageType type, std::weak_ptr<Auteur> auteur) 
    :   titre_(titre),
        ouvrage_type_(type),
        auteur_(auteur)
{}

std::string Ouvrage::getTitre() const
{
    return titre_;
}

OuvrageType Ouvrage::getOuvrageType() const
{
    return ouvrage_type_;
}


std::weak_ptr<Auteur> Ouvrage::getAuteur() const
{
    return auteur_;
}

