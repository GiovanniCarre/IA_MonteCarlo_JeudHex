#include "joueur_manuel.hh"


Joueur_Manuel::Joueur_Manuel(std::string nom, bool joueur)
    :Joueur(nom,joueur)
{}

void Joueur_Manuel::recherche_coup(Jeu j, couple &coup)
{
  int a, o;
  std::cout<<nom()<<" donnez l'abscisse et l'ordonnee :";
  std::cin>>a>>o;
  coup.first = o;
  coup.second = a;

}
