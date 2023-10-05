#include "learningNode.hh"


float LearningNode::calculQUBC(float nbVisitFather)const{
    //
    /*
    Ici on fait un return directement pour gagner du temps de calcul mais par exemple
    exploitation = _score / _nbVisit
    exploration = sqrt(2*(log(nbCrossingFather)/_nbVisit))

    Donc QUBC = exploitation + exploration
        C est la constante d'exploration, égale à 2
    */
    return _score/_nbVisit + sqrt(2*(log(nbVisitFather)/_nbVisit));
}



couple LearningNode::getMaxQUBCChildCouple(std::vector<LearningNode> const &mcts)const{

    float maxQUBC = -5000000.0;
    couple c;
    float valQUBC;

    //it->first = couple : pair (X,Y), it->second => index de l'enfant dans le MCTS
    for (auto it = _children.begin(); it != _children.end(); ++it) {

        if (it->second>= mcts.size())std::cout << "ERROR :getMaxChild"<<std::endl;
        valQUBC = mcts.at(it->second).calculQUBC(_nbVisit);
        if (valQUBC > maxQUBC){
            c = it->first;
            
            maxQUBC = valQUBC;
        }
    }

    //on retourne l'indice de l'enfant avec le plus gros QUBC et le coup correspondant de l'enfant
    return c;

}

bool LearningNode::containsChild(couple const &c)const{
    return _children.find(c) != _children.end();
}


void LearningNode::putChild(couple const &coup, unsigned int indiceVal){
    _children.emplace(coup,indiceVal);
}

unsigned int LearningNode::getIndiceChild(couple const &c)const{
    return _children.find(c)->second;

}

void LearningNode::print() const {
    std::cout<<"Score : "<<_score<<"/"<<_nbVisit<<std::endl;
    for (auto it = _children.begin(); it != _children.end(); it++) {
        //it -> first = coup et it->first->first correspond à x ...
        std::cout<<"("<<it->first.first<<","<<it->first.second<<") : "<<it->second<<std::endl;
    }
    std::cout<<std::endl;
}

//Getters

float LearningNode::getNbVisit()const{
    return _nbVisit;
}


unsigned int LearningNode::nbChildren()const{
    return _children.size();
}
