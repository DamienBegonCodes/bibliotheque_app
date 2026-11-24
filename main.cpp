#include <iostream>
#include <string>
#include <limits>
#include <cstdlib>
#include <memory>

#include "OuvrageType.hpp"
#include "Bibliotheque.hpp"
#include "Auteur.hpp"
#include "Ouvrage.hpp"


static const std::string clear("\033[2J\033[H");
static const std::string entree_pour_continuer("\n--- Appuyer sur entree pour continuer ---");
static const std::string presentation(
    "Bonjour,\n\nBienvenue sur la bibliotheque app, une application console permettant de repertorier vos livres, BDs et auteurs.\n"
    "Un livre ou une BD est identifiable par un titre (sensible a la casse) et peut referencer son auteur.\n"
    "Un auteur est identifiable par son nom complet (sensible a la casse) et possede une bibliographie, qui reference ses ouvrages (BDs et/ou livres).\n"
    "La bibliographie d'un auteur est mis a jour automatiquement lors de l'ajout d'ouvrages.\n"
    "Il est donc recommande d'enregistrer un auteur dans la bibliotheque avant ses ouvrages pour que le referencement s'effectue correctement\n"
    "Sans referencement, un ouvrage sera considere sans auteur\n"
    "Le menu interactif vous permettra de modifier a votre guise le contenu de votre bibliotheque."
);

static const std::string menu_principal(
    "Menu :\n"
    "   1) Ajouter un element dans la bibliotheque\n"
    "   2) Rechercher l'auteur d'un ouvrage\n"
    "   3) Rechercher la bibliographie d'un auteur\n"
    "   4) Supprimer un element de la bibliotheque\n"
    "\n"
    "   5) Presentation de l'app\n"
    "\n"
    "   0) Quitter l'app"
);

static const std::string demande_choix_utilisateur(
    "\n--- Naviguez en inscrivant le chiffre correspondant a votre choix, puis appuyez sur entree ---"
);

// ################## FONCTIONS UTILITAIRES ##################

int choix_utilisateur_entier(int debut_plage, int fin_plage)
{
    int choix(0);
    if (!(std::cin >> choix) || std::cin.peek() != '\n' || choix > fin_plage || choix < debut_plage)
    {
        std::cout << clear << std::flush;

        std::cout << "Erreur : la saisie est invalide" << std::endl;
        std::cout << "Aide : Veuillez saisir un chiffre parmis ceux disponible dans le menu, sans introduire d'autres caracteres" << std::endl;

        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        std::cout << entree_pour_continuer << std::endl;
        std::cin.ignore();
        return -1;
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return choix;
}

std::string choix_utilisateur_string()
{
    std::string s;
    if (!std::getline(std::cin, s))
    {
        std::cerr << "Erreur lors de la lecture d'une chaine de caractere, arret du programme";
        std::exit(EXIT_FAILURE);
    }
    return s;
}

void affichage_presentation()
{
    std::cout << clear << std::flush;

    std::cout << presentation << std::endl;
    std::cout << entree_pour_continuer << std::endl;
    std::cin.ignore();
}

void affichage_auteur(std::weak_ptr<Auteur> auteur)
{
    std::shared_ptr<Auteur> auteur_ref_forte = auteur.lock();
    if (!auteur_ref_forte)
    {
        return;
    }
    std::cout <<    "Auteur :" << std::endl;
    std::cout <<    "   Nom complet : " << auteur_ref_forte->getNom() << std::endl;
    std::cout <<    "   Bibliographie :" << std::endl;

    for (std::weak_ptr<Ouvrage> ouvrage : auteur_ref_forte->getBibliographie())
    {
        std::shared_ptr<Ouvrage> ouvrage_ref_forte = ouvrage.lock();
        if (ouvrage_ref_forte)
        {
            std::cout <<    "       - " << ouvrage_ref_forte->getTitre() << std::endl;
        }
    }
}

void affichage_ouvrage(std::shared_ptr<Ouvrage> ouvrage)
{
    if (ouvrage == nullptr)
    {
        return;
    }

    std::string type_ouvrage;
    if (ouvrage->getOuvrageType() == OuvrageType::LIVRE)
    {
        type_ouvrage = "Livre";
    }
    else
    {
        type_ouvrage = "BD";
    }

    std::string nom_auteur("");
    std::shared_ptr<Auteur> auteur = ouvrage->getAuteur().lock();
    if (auteur != nullptr)
    {
        nom_auteur = auteur->getNom();
    }

    std::cout <<    "Ouvrage :" << std::endl;
    std::cout <<    "   Titre : " << ouvrage->getTitre() << std::endl;
    std::cout <<    "   Type : " << type_ouvrage << std::endl;
    std::cout <<    "   Auteur : " << nom_auteur << std::endl;
}

// ################## (FONCTIONS UTILITAIRES) ##################


// ################## FONCTIONS DE NAVIGATION ##################

void ajout_auteur()
{
    std::cout << clear << std::flush;

    std::cout << "Veuillez saisir le nom complet de l'auteur a enregistrer :" << std::endl;

    std::string nom = choix_utilisateur_string();

    std::cout << std::endl;
    if (nom.empty())
    {
        std::cout << "Le nom saisi est vide, l'auteur ne sera pas ajoute a la bibliotheque\n" << std::endl;
        
        std::cout << entree_pour_continuer << std::endl;
        std::cin.ignore();
        return;
    }

    if (Bibliotheque::getInstance().getAuteur(nom) != nullptr)
    {
        std::cout << "Le nom saisi existe deja dans la bibliotheque ! L'auteur ne sera pas ajoute a la bibliotheque\n" << std::endl;
        
        std::cout << entree_pour_continuer << std::endl;
        std::cin.ignore();
        return;
    }
    
    Bibliotheque::getInstance().ajoutAuteur(nom);

    std::cout << "L'auteur a ete ajoute dans la bibliotheque !\n" << std::endl;
        
    std::cout << entree_pour_continuer << std::endl;
    std::cin.ignore();
}

void ajout_ouvrage(OuvrageType type)
{
    std::cout << clear << std::flush;

    std::cout << "Veuillez saisir le titre de l'ouvrage :" << std::endl;

    std::string titre = choix_utilisateur_string();

    std::cout << std::endl;

    if (titre.empty())
    {
        std::cout << "Le titre saisi est vide, l'ouvrage ne sera pas ajoute a la bibliotheque\n" << std::endl;
        
        std::cout << entree_pour_continuer << std::endl;
        std::cin.ignore();
        return;
    }

    if (Bibliotheque::getInstance().getOuvrage(titre) != nullptr)
    {
        std::cout << "Le titre saisi existe deja dans la bibliotheque ! L'ouvrage ne sera pas ajoute a la bibliotheque\n" << std::endl;
        
        std::cout << entree_pour_continuer << std::endl;
        std::cin.ignore();
        return;
    }

    std::cout << "Veuillez saisir le nom de l'auteur de l'ouvrage :" << std::endl;

    std::string nom = choix_utilisateur_string();

    std::cout << std::endl;

    std::shared_ptr<Auteur> ref_auteur = Bibliotheque::getInstance().getAuteur(nom);
    if (ref_auteur == nullptr)
    {
        std::cout << "La reference auteur n'a pas ete trouve dans la bibliotheque" << std::endl;
    }

    Bibliotheque::getInstance().ajoutOuvrage(titre, type, ref_auteur);

    std::cout << "L'ouvrage a ete ajoute dans la bibliotheque !\n" << std::endl;
        
    std::cout << entree_pour_continuer << std::endl;
    std::cin.ignore();
}

void suppression_auteur()
{
    std::cout << clear << std::flush;
    std::cout << "Veuillez renseigner le nom de l'auteur a supprimer :" << std::endl;

    std::string nom = choix_utilisateur_string();

    std::shared_ptr<Auteur> auteur = Bibliotheque::getInstance().getAuteur(nom);

    if (auteur == nullptr)
    {
        std::cout << clear << std::flush;
        std::cout << "L'auteur renseigne n'existe pas dans la bibliotheque\n" << std::endl;
        
        std::cout << entree_pour_continuer << std::endl;
        std::cin.ignore();
        return;
    }

    Bibliotheque::getInstance().supprimerAuteur(nom);

    std::cout << clear << std::flush;
    std::cout << "L'auteur a bien ete supprime\n" << std::endl;
 
    std::cout << entree_pour_continuer << std::endl;
    std::cin.ignore();
}

void suppression_ouvrage()
{
    std::cout << clear << std::flush;
    std::cout << "Veuillez renseigner le titre de l'ouvrage a supprimer :" << std::endl;

    std::string titre = choix_utilisateur_string();

    std::shared_ptr<Ouvrage> ouvrage = Bibliotheque::getInstance().getOuvrage(titre);

    if (ouvrage == nullptr)
    {
        std::cout << clear << std::flush;
        std::cout << "L'ouvrage renseigne n'existe pas dans la bibliotheque\n" << std::endl;
        
        std::cout << entree_pour_continuer << std::endl;
        std::cin.ignore();
        return;
    }

    Bibliotheque::getInstance().supprimerOuvrage(titre);

    std::cout << clear << std::flush;
    std::cout << "L'ouvrage a bien ete supprime\n" << std::endl;
 
    std::cout << entree_pour_continuer << std::endl;
    std::cin.ignore();
}

void ajout_element()
{
    std::cout << clear << std::flush;
    std::cout <<    "Quel type d'elements souhaitez vous ajouter a la bibliotheque : " << std::endl;
    std::cout <<    "   1) Auteur" << std::endl;
    std::cout <<    "   2) Livre" << std::endl;
    std::cout <<    "   3) BD" << std::endl;
    std::cout <<    "   " << std::endl;
    std::cout <<    "   0) Revenir au menu principal" << std::endl;
    
    
    std::cout << demande_choix_utilisateur << std::endl;

    int choix = choix_utilisateur_entier(0, 3);

    switch(choix)
    {
        case 1:
            ajout_auteur();
            break;

        case 2:
            ajout_ouvrage(OuvrageType::LIVRE);
            break;

        case 3:
            ajout_ouvrage(OuvrageType::BD);
            break;

        case 0:
            return;
    }
}

void recherche_auteur_ouvrage()
{
    std::cout << clear << std::flush;

    std::cout << "Veuillez saisir le titre de l'ouvrage dont vous cherchez l'auteur :" << std::endl;

    std::string titre = choix_utilisateur_string();

    std::cout << std::endl;

    std::shared_ptr<Ouvrage> ref_ouvrage = Bibliotheque::getInstance().getOuvrage(titre);

    if (ref_ouvrage == nullptr)
    {
        std::cout << "La bibliotheque ne contient pas un tel ouvrage" << std::endl;

        std::cout << entree_pour_continuer << std::endl;
        std::cin.ignore();
        return;
    }

    std::shared_ptr<Auteur> auteur = ref_ouvrage->getAuteur().lock();
    if (auteur == nullptr)
    {
        std::cout << "L'auteur de l'ouvrage n'est pas reference" << std::endl;

        std::cout << entree_pour_continuer << std::endl;
        std::cin.ignore();        
        return;
    }

    std::cout << clear << std::flush;
    affichage_auteur(auteur);
    
    std::cout << entree_pour_continuer << std::endl;
    std::cin.ignore();
}

void recherche_bibliographie()
{
    std::cout << clear << std::flush;

    std::cout << "Veuillez saisir le nom de l'auteur dont vous cherchez la bibliographie :" << std::endl;

    std::string nom = choix_utilisateur_string();

    std::cout << std::endl;

    std::shared_ptr<Auteur> ref_auteur = Bibliotheque::getInstance().getAuteur(nom);

    if (ref_auteur == nullptr)
    {
        std::cout << "La bibliotheque ne contient pas un tel auteur" << std::endl;

        std::cout << entree_pour_continuer << std::endl;
        std::cin.ignore();
        return;
    }

    std::cout << clear << std::flush;

    if (ref_auteur->getBibliographie().empty())
    {
        std::cout << "L'auteur a une bibliographie vide !" << std::endl;
    }
    else
    {
        for (std::weak_ptr<Ouvrage> ouvrage : ref_auteur->getBibliographie())
        {
            std::shared_ptr<Ouvrage> ouvrage_ref_forte = ouvrage.lock();
            if (ouvrage_ref_forte != nullptr)
            {
                affichage_ouvrage(ouvrage_ref_forte);
                std::cout << std::endl;
            }
        }
    }

    std::cout << entree_pour_continuer << std::endl;
    std::cin.ignore();
}

void suppression_element()
{
    std::cout << clear << std::flush;
    std::cout <<    "Quel type d'elements souhaitez vous supprimer de la bibliotheque : " << std::endl;
    std::cout <<    "   1) Auteur" << std::endl;
    std::cout <<    "   2) Ouvrage" << std::endl;
    std::cout <<    "   " << std::endl;
    std::cout <<    "   0) Revenir au menu principal" << std::endl;
    
    std::cout << demande_choix_utilisateur << std::endl;

    int choix = choix_utilisateur_entier(0, 2);

    switch(choix)
    {
        case 1:
            suppression_auteur();
            break;

        case 2:
            suppression_ouvrage();
            break;

        case 0:
            return;
    }
}

void navigation()
{
    while (true)
    {
        std::cout << clear << std::flush;

        std::cout << menu_principal << std::endl;
        std::cout << demande_choix_utilisateur << std::endl;
        
        int choix = choix_utilisateur_entier(0, 5);

        switch(choix)
        {
            case 1:
                ajout_element();
                break;
            
            case 2:
                recherche_auteur_ouvrage();
                break;
            
            case 3:
                recherche_bibliographie();
                break;
            
            case 4:
                suppression_element();
                break;
            
            case 5:
                affichage_presentation();
                break;
            
            case 0:
                return;
        }
    }
}

// ################## (FONCTIONS DE NAVIGATION) ##################


int main()
{
    // Affichage de la presentation au lancement de l'app
    affichage_presentation();
    navigation();

    return 0;
}