#pragma once

#include <numeric>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>
#include <iostream>
#include <random>
#include <array>
#include <cmath>
#include <map>
#include "math.h"
#include "fileManager.hh"
#include "../jeu.hh"
#include <chrono>

class Learner{
    // ====>=========>  POUR CHANGER LE NOMBRE DE SCENARIO CONSTRUCTEUR DU .CC <=======<======

private:
    std::vector<LearningNode> _MCTS;
    unsigned int _nbScenario;
    int _player;

public:
    
    //player correspond aux joueurs auxquels on veut avoir le MCTS // 
    Learner(int player);

    //FONCTION PRINCIPALE QUI VA FAIRE l'apprentissage pour 1 joueur, sachant que le joueur sera 

    //on a fais en sorte de ne pas dépasser les 11*11 grilles
    void monteCarloLearning(Jeu &j);

    //sauvegarde le MCTS sur un interval précis, et fais appel à fileManager
    void save(std::string path, std::vector<LearningNode> const & tree, 
    unsigned int intervalBegin,unsigned int intervalEnd, unsigned int nbSave)const;

    
    //si j== 2 alors j=1 sinon j=2
    //fais changer le joueur

    void alternePlayer(int & j);
    
    //analyse ---------------------------

    //affiche quelques infos sur la moyenne des visites des noeuds, pour ajuster la constante d'exploration
    void printVisit()const;

    //lors de l'apprentissage, s'exécute pour savoir où on est
    void printProgress(unsigned int interval, unsigned int nbScenarioDone)const;

    //fais l'approximation du vecteur(MCTS) en octets
    unsigned int getRealSize()const;


    //getters ---------------
    std::vector<LearningNode> getMCTS()const;

};
