#pragma once

#include <fstream>
#include <string>
#include <vector>
#include <array>
#include <cmath>
#include <map>
#include "math.h"
#include "../jeu.hh"



class LearningNode{
private:
    float calculQUBC(float nbVisitFather)const;

public:
    //en public car dans le learner, c'est plus rapide d'y accéder
    float _score = 0;
    float _nbVisit = 0;

    //clé -> couple (correspond aux coups des enfants), valeur => index de l'enfant dans le MCTS
    std::map<couple, unsigned int> _children;

    LearningNode(){};
    LearningNode(float score, float nb, std::map<couple, unsigned int> children):
             _score(score), _nbVisit(nb), _children(children){}
             
    couple getMaxQUBCChildCouple(std::vector<LearningNode> const &mcts)const;

    //regarde si le coup est dans les enfants
    bool containsChild(couple const &c)const;

    //ajoute un enfant
    void putChild(couple const &coup, unsigned int keyN);  

    //renvoie l'indice de l'enfant en fonction du coup
    unsigned int getIndiceChild(couple const &c)const;

    //pour simplifier la correction des erreurs (n'est pas enregistré comme cela dans le fichier (VOIR FILEMANAGER))
    void print() const;

    //getters ---------------------


    unsigned int nbChildren()const;

    float getNbVisit()const;

};

