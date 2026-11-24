#include "Bibliotheque.hpp"

Bibliotheque & Bibliotheque::getInstance()
{
    static Bibliotheque instance;
    return instance;
}

std::shared_ptr<Auteur> Bibliotheque::getAuteur(const std::string & nom) const
{
    if (!auteurs_.contains(nom))
    {
        return std::shared_ptr<Auteur>();
    }
    return auteurs_.at(nom);
}

std::shared_ptr<Ouvrage> Bibliotheque::getOuvrage(const std::string & titre) const
{
    if (!ouvrages_.contains(titre))
    {
        return std::shared_ptr<Ouvrage>();
    }
    return ouvrages_.at(titre);
}


void Bibliotheque::ajoutAuteur(const std::string & nom)
{
    if (!auteurs_.contains(nom))
    {
        auteurs_[nom] = std::make_shared<Auteur>(nom);
    }
}

void Bibliotheque::ajoutOuvrage(const std::string & titre, OuvrageType type, std::weak_ptr<Auteur> auteur)
{
    if (!ouvrages_.contains(titre))
    {
        ouvrages_[titre] = std::make_shared<Ouvrage>(titre, type, auteur);
        std::shared_ptr<Auteur> auteur_ref_forte = auteur.lock();
        if (auteur_ref_forte != nullptr && auteurs_.contains(auteur_ref_forte->getNom()))
        {
            auteur_ref_forte->ajouterDansBibliographie(ouvrages_[titre]);
        }
    }
}

std::weak_ptr<Auteur> Bibliotheque::getAuteurDeOuvrage(const std::string & titre) const
{
    if (ouvrages_.contains(titre))
    {
        return ouvrages_.at(titre)->getAuteur();
    }

    return std::weak_ptr<Auteur>();
}

std::list<std::weak_ptr<Ouvrage>> Bibliotheque::getOuvragesDeAuteur(const std::string & nom) const
{
    if (auteurs_.contains(nom))
    {
        return auteurs_.at(nom)->getBibliographie();
    }
    return {};
}

void Bibliotheque::supprimerAuteur(const std::string & nom)
{
    if (auteurs_.contains(nom))
    {
        auteurs_.erase(nom);
    }
}

void Bibliotheque::supprimerOuvrage(const std::string & titre)
{
    if (ouvrages_.contains(titre))
    {
        ouvrages_.erase(titre);
    }
}
