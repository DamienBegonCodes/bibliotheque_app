#ifndef H_AUTEUR
#define H_AUTEUR

#include <memory>
#include <list>
#include <string>

class Ouvrage;

class Auteur
{
private:
    std::string nom_complet_;
    std::list<std::weak_ptr<Ouvrage>> bibliographie_;
    
public:
    Auteur() = delete;
    Auteur(std::string);

    std::string getNom() const;
    std::list<std::weak_ptr<Ouvrage>> getBibliographie() const;
    void ajouterDansBibliographie(std::weak_ptr<Ouvrage>);
};

#endif