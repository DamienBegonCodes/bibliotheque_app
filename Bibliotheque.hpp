#ifndef H_BIBLIOTHEQUE
#define H_BIBLIOTHEQUE

#include "OuvrageType.hpp"
#include "Ouvrage.hpp"
#include "Auteur.hpp"

#include <memory>
#include <list>
#include <map>

// Bibliotheque est implementé selon le design pattern du Singleton
class Bibliotheque
{
private:
    Bibliotheque(Bibliotheque &) = delete;
    Bibliotheque & operator=(const Bibliotheque &) = delete;
    
    Bibliotheque() = default;

    std::map<std::string, std::shared_ptr<Ouvrage>> ouvrages_;
    std::map<std::string, std::shared_ptr<Auteur>> auteurs_; 

public:
    static Bibliotheque & getInstance();

    // Utilitaires
    std::shared_ptr<Auteur> getAuteur(const std::string &) const;
    std::shared_ptr<Ouvrage> getOuvrage(const std::string &) const;

    // Méthodes spécifiées par les exigences du test
    void ajoutAuteur(const std::string &);
    void ajoutOuvrage(const std::string &, OuvrageType, std::weak_ptr<Auteur>);
    std::weak_ptr<Auteur> getAuteurDeOuvrage(const std::string &) const;
    std::list<std::weak_ptr<Ouvrage>> getOuvragesDeAuteur(const std::string &) const;
    void supprimerAuteur(const std::string &);
    void supprimerOuvrage(const std::string &);
};


#endif