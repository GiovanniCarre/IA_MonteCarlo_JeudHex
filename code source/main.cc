#include <iostream>
#include "arbitre.hh"
#include "jeu.hh"
#include <ctime>
#include <cstdlib>
#include "assets/learner.hh"


int main(){
    Jeu j(11);
    
    bool learning = false;
    std::srand(std::time(nullptr));

    if (learning) {
        //JOUEUR 1
        Learner L1(1);
        L1.monteCarloLearning(j);

        //JOUEUR 2
        Learner L2(2);
        L2.monteCarloLearning(j);
    } else {
        //création de l'Arbitre (joueur jouant en 1er la premiere partie, joueur jouant en 2eme celle-ci , nombre de parties)
        Arbitre a (player::LPD1, player::RAND,3);
        //commence le challenge
        a.challenge();
    }

    return 0;
}
