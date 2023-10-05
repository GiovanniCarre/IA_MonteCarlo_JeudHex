#include "joueur.hh"

Joueur::Joueur(std::string nom, bool joueur)
    :_nom(nom),_joueur(joueur)
{}

void Joueur::jouer(Jeu j, couple & coup, std::mutex &coup_mutex)
{
    coup_mutex.lock();
    recherche_coup(j,coup);
    coup_mutex.unlock();
}

std::string Joueur::nom() const
{
    return _nom;
}

bool Joueur::joueur() const
{
    return _joueur;
}

void Joueur::setJoueur(bool joueur)
{
    _joueur = joueur;
}
