#pragma once

#include <iostream>
#include <vector>
#include <queue>

/** coordonne d'une case hexagonale */
/** first c'est l'ordonnee
 *  second c'est l'abscisse*/
using couple = std::pair<int, int>;

/** Pour chercher le gagnant */
struct triplet {
    couple provenance;
    int cout;
    couple sommet;
};

/**
 * @brief Classe Jeu
 * Classe gérant le jeu.
 * elle vous est passée en paramètres par la fonction jouer() de Joueur
 */
class Jeu {
  public:
    /**
     * @brief _grille
     * 0 : case vide, 1 : joueur qui joue les 1, 2 : joueur qui joue les 2
     */
    std::vector<std::vector<int>> _grille;
    int _taille;

  public:
    /**
     * @brief jeu
     * création d'un jeu de taille choisie dont chaque case est vide
     *
     * @param taille
     */
    Jeu(int taille);

    /**
     * @brief taille
     * @return la taille du jeu
     */
    int taille() const;

    /**
     * @brief case_libre
     * @param coup
     * @return vrai si le coup est présent sur le jeu et que la case est vide
     */
    bool case_libre(couple coup) const;

    /**
     * @brief coup_valide
     * @param coup
     * @return
     */
    bool coup_valide(couple coup) const;

    /**
     * @brief jouer_coup
     * @param coup
     * @param joueur
     * joue le coup pour un joueur
     */
    void jouer_coup(couple coup, int joueur);

    /**
     * @brief coups_possibles
     * @return la liste des coups possibles
     */
    std::vector<couple> coups_possibles() const;

    /**
     * @brief partie_finie
     * @return vrai si la partie est finie
     */
    bool partie_finie() const;

    /**
     * @brief vider_jeu
     * vide le jeu
     */
    void vider_jeu();

    /**
     * @brief grille
     * @return une copie du jeu
     */
    std::vector<std::vector<int>> grille() const;

    /**
     * @brief pion
     * @param sommet
     * @return  renvoi la valeur de la case/pion sur le couple sommet, -1 si hors jeu
     *
     */
    int pion(couple sommet) const;

    /**
     * @brief voisins
     * @param sommet
     * @return la liste des voisins au couple passé en paramètre
     */
    std::vector<couple> voisins(couple sommet) const;

    /**
     * @brief cotes_relies
     * @return  1 si les 1 relient les bords haut et bas,
     *          2 si les 2 relient les bords droite et gauche,
     *          0 sinon
     */
    int cotes_relies() const;

    /**
     * @brief dijkstra
     * @param sommet
     * @param joueur
     * @return vector des sommets reliés au couple sommet passé en paramètre selon un
     * joueur
     */
    std::vector<triplet> dijkstra(couple sommet, int joueur) const;

    /**
     * @brief supprimer_coup
     * @param coup
     * retire le pion sur la case coup
     */
    void supprimer_coup(couple coup);
};

//attention n'affiche en fait que le plateau du jeu
std::ostream& operator<<( std::ostream &flux, Jeu const& j);
