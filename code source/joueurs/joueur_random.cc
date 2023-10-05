#include "joueur_random.hh"

Joueur_Random::Joueur_Random(std::string nom, bool joueur)
    :Joueur(nom,joueur)
{}



/*char Joueur_Random::nom_abbrege() const
{
    return 'R';
}
*/

void Joueur_Random::recherche_coup(Jeu j, couple &coup)
{
    int taille = j.coups_possibles().size();
    int num = rand()%(taille);
     coup.first=j.coups_possibles()[num].first;
     coup.second=j.coups_possibles()[num].second;


}


