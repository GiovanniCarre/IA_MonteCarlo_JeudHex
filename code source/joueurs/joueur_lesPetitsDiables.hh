#pragma once


#include "joueur.hh"
#include "algorithm"
#include "../assets/fileManager.hh"

class Joueur_LPD_ : public Joueur
{
protected: 
  std::vector<std::vector<int>> grillePrecedente;   //permet de calculer quels étaient les coups précédents
  std::vector<couple> coupsPrecedents;              //permet de se déplacer dans le MCTS

  static std::vector<PlayingNode> MCTS;
  static std::vector<PlayingNode> MCTS1;
  static std::vector<PlayingNode> MCTS2;

public:
  Joueur_LPD_(std::string nom, bool joueur);

  void calculerCoupsPrecedents(Jeu& j);

  void recherche_coup(Jeu j, couple & coup) override;

  void meilleurCoupSansMC(Jeu& j, couple & coup);

  void afficherMCTS();
};