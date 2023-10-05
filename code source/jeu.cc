#include "jeu.hh"

#include <iostream>

Jeu::Jeu(int taille)
    : _grille(taille, std::vector<int>(taille, 0)), _taille(taille) {
}

int Jeu::taille() const {
    return _taille;
}


bool Jeu::case_libre(couple coup) const {
    return (coup_valide(coup) && (_grille[coup.first][coup.second] == 0));
}

bool Jeu::coup_valide(couple coup) const {
    return ((!((coup.first >= _taille) || (coup.first < 0)) &&
             !((coup.second >= _taille) || (coup.second < 0))));
}

void Jeu::jouer_coup(couple coup, int joueur) {
    if (case_libre(coup))
        _grille[coup.first][coup.second] = joueur;
    else
        std::cerr << "coup invalide dans jouer_coup abs : "<<coup.second<<" ,ord :"<<coup.first;
}

std::vector<couple> Jeu::coups_possibles() const {
    std::vector<couple> liste;
    for (int x(0); x < _taille; x++) {
        for (int y(0); y < _taille; y++) {
            if (pion(couple(x,y)) == 0)
                liste.push_back(couple(x, y));
        }
    }
    return liste;
}

bool Jeu::partie_finie() const {
    return ((coups_possibles().empty()) || (cotes_relies()!=0)) ;
}

void Jeu::vider_jeu() {
    for (int x(0); x < _taille; x++) {
        for (int y(0); y < _taille; y++) {
            _grille[x][y] = 0;
        }
    }
}

std::vector<std::vector<int>> Jeu::grille() const {
    return _grille;
}

int Jeu::pion(couple sommet) const {
    return coup_valide(sommet) ? _grille[sommet.first][sommet.second] : -1;
}

std::vector<couple> Jeu::voisins(couple sommet) const {
    std::vector<couple> voisins;
    for (int x(-1); x <= 1; x++) {
        for (int y(-1); y <= 1; y++) {
            if (x != y) {
                if (pion(couple(sommet.first + x, sommet.second + y)) != -1)
                    voisins.push_back(couple(sommet.first + x, sommet.second + y));
            }
        }
    }
    return voisins;
}

int Jeu::cotes_relies() const {

    //Ma version
    std::vector<bool> visited(_taille * _taille, false);
    std::queue<couple> q;
    for (int j = 0; j < _taille; j++) {
        if (pion(couple(0, j)) == 1) {
            visited[0 * _taille + j] = true;
            q.push(couple(0, j));
        }
    }
    while (!q.empty()) {
        couple sommet = q.front();
        q.pop();
        if (sommet.first == _taille - 1) {
            return 1;
        }
        for (auto voisin : voisins(sommet)) {
            int i = voisin.first;
            int j = voisin.second;
            if (!visited[i * _taille + j] && pion(couple(i, j)) == 1) {
                visited[i * _taille + j] = true;
                q.push(couple(i, j));
            }
        }
    }

    // Vérification si la colonne de gauche est reliée à celle de droite
    visited.assign(_taille * _taille, false);
    for (int i = 0; i < _taille; i++) {
        if (pion(couple(i, 0)) == 2) {
            visited[i * _taille + 0] = true;
            q.push(couple(i, 0));
        }
    }
    while (!q.empty()) {
        couple sommet = q.front();
        q.pop();
        if (sommet.second == _taille - 1) {
            return 2;
        }
        for (auto voisin : voisins(sommet)) {
            int i = voisin.first;
            int j = voisin.second;
            if (!visited[i * _taille + j] && pion(couple(i, j)) == 2) {
                visited[i * _taille + j] = true;
                q.push(couple(i, j));
            }
        }
    }

    return 0;


    /* VERSION DE LA PROF
    // il y a un chemin de 2 si la gauche rejoint la droite
     for (int x(0); x < _taille; x++)
        if (pion(couple(x, 0)) == 2)
            for (auto t : dijkstra(couple(x, 0), 2))
                if (t.sommet.second == _taille - 1)
                {
                    return 2;
                }


    // 1l y a un chemin de 1 si le haut rejoint le bas
    for (int y(0); y < _taille; y++)
        if (pion(couple(0, y)) == 1)
            for (auto t : dijkstra(couple(0, y), 1))
                if (t.sommet.first == _taille - 1)
                {
                    return 1;
                }

    return 0;
    */
}

std::vector<triplet> Jeu::dijkstra(couple depart, int joueur) const {
    std::vector<triplet> liste;
    std::vector<triplet> file_de_priorite;
    if (pion(depart) == joueur) {
        liste.push_back(triplet{couple(-1, -1), 0, depart});
        for (auto voisin : voisins(depart))
            if (pion(voisin) == joueur)
                file_de_priorite.push_back(triplet{depart, 1, voisin});

        bool present(true);

        while (file_de_priorite.begin() != file_de_priorite.end()) {
            auto c = file_de_priorite.back();
            file_de_priorite.pop_back();
            present = false;
            for (auto b : liste) {
                if (b.sommet == c.sommet)
                    present = true;
            }
            if (!present) {
                liste.push_back(c);
                for (auto voisin : voisins(c.sommet)) {
                    if (pion(voisin) == joueur) {
                        present = false;
                        for (auto b : liste)
                            if (b.sommet == voisin)
                                present = true;
                        if (!present)
                            file_de_priorite.push_back(
                                triplet{c.sommet, c.cout + 1, voisin});
                    }
                }
            }
        }
    }
    return liste;
}

void Jeu::supprimer_coup(couple coup) {
    if (coup_valide(coup))
        _grille[coup.first][coup.second] = 0;
    else
        std::cerr << "coup invalide (dans supprimer_coup) abs : "<<coup.second <<", ord"<<coup.first<<std::endl;
}



std::ostream& operator<<( std::ostream &flux, Jeu const& j ){
    flux<<" ";
    for (int x(0); x < j.taille(); x++) {
        flux<<" "<<x;
    }
    flux<<std::endl;
    flux<<"  ";
    for (int x(0); x < j.taille(); x++) {
        flux<<"/\\";
    }
    flux<<std::endl;
    int nb = 0;
    for (int x(0); x < j.taille(); x++) {
        nb++;
        for (int a(0); a < x; a++)
            flux<<" ";
        flux<<x;
        for (int y(0); y < j.taille(); y++) {
            flux<<"|";
            if (j.grille()[x][y] == 0)
                flux<<" ";
            else flux<<j.grille()[x][y];
                    }
       flux<<"|"<<std::endl;
        for (int a(0); a < x + 2; a++)
            flux<<" ";
        for (int i(0); i < j.taille(); i++) {
            flux<<"\\/";
        }
        if (nb != 0 && nb != j.taille()) {
            flux<<"\\";
        }
       flux<<std::endl;
    }
return flux;
}
