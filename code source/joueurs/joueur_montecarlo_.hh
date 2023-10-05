#pragma once


#include "joueur.hh"
#include "algorithm"
#include "../assets/fileManager.hh"


/*

  Pour voir le joueur développé par notre groupe, aller voir 
    joueur_lesPetitsDiables.hh & joueur_lesPetitsDiables.cc

*/


class Joueur_MonteCarlo_ : public Joueur
{
public:
  Joueur_MonteCarlo_(std::string nom, bool joueur);
  //char nom_abbrege() const override;

  void recherche_coup(Jeu j, couple & coup) override;
};