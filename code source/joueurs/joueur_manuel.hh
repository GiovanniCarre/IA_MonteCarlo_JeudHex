#pragma once


#include "joueur.hh"


class Joueur_Manuel : public Joueur
{
private:

public:
  Joueur_Manuel(std::string nom, bool joueur);

  void recherche_coup(Jeu j, couple & coup) override;

};
