#pragma once

#include <mutex>
#include <iostream>
#include "../jeu.hh"

/*****************************************************************************************************
 *      Toutes les classes implantant les divers joueurs doivent hériter de cette classe             *
 *      et donc contenir ces methodes                                                                *
 *****************************************************************************************************/

/**
 * @brief The Joueur class
 * classe mère des joueurs
 * chaque joueur doit implémenter la méthode jouer
 */
class Joueur
{
    std::string _nom;   //nom du joueur
    bool _joueur;       //joueur 1 : true (qui commence) , joueur 2 : false (qui joue en deuxieme)



public:
    /**
     * @brief Joueur
     * @param nom
     * @param joueur
     * création du joueur, joueur correspond au numero du joueur ( joueur 1 : true commence, joueur 2 : false joue en deuxieme)
     */
    Joueur(std::string nom, bool joueur);

    /**
     * @brief jouer
     * @param jeu 
     * @param coup : le coup doit etre modifié par le joueur avant la fin du temps imparti
     * @param coup_mutex : le mutex doit etre pris par le joueur au début de la partie puis rendu avant le temps imparti
     */
    virtual void jouer(Jeu jeu, couple & coup , std::mutex & coup_mutex) final;

    /**
     * @brief recherche_coup
     * @param jeu
     * @param coup : le coup doit etre modifié par le joueur avant la fin du temps imparti
     */
    virtual void recherche_coup(Jeu j, couple & coup) =0;


    /**
     * @brief nom
     * @return le nom du joueur
     */
    std::string nom() const;

    /**
     * @brief joueur
     * @return le numero du joueur ( joueur 1 : true, joueur 2 : false )
     */
    bool joueur() const;

    /**
     * @brief setJoueur
     * @param joueur ( joueur 1 : true, joueur 2 : false )
     */
    void setJoueur(bool joueur);

    /*char nom_abbrege() const{
        return nom()[0];
    } ;*/

};
