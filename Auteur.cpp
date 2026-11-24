#include "Auteur.hpp"

Auteur::Auteur(std::string nom)
    :   nom_complet_(nom), 
        bibliographie_()
{}

std::string Auteur::getNom() const
{
    return nom_complet_;
}

std::list<std::weak_ptr<Ouvrage>> Auteur::getBibliographie() const
{
    return bibliographie_;
}

void Auteur::ajouterDansBibliographie(std::weak_ptr<Ouvrage> ouvrage)
{
    if (!ouvrage.expired())
    {
        bibliographie_.push_back(ouvrage);
    }
}
