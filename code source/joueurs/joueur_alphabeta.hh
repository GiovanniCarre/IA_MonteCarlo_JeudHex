#pragma once


#include "joueur.hh"


class Joueur_AlphaBeta : public Joueur
{
public:
  Joueur_AlphaBeta(std::string nom, bool joueur);
  //char nom_abbrege() const override;

  void recherche_coup(Jeu j, couple & coup) override;
};
