#include "fileManager.hh"
#include <fstream>
#include <iostream>
#include <string>

void readFile(std::string const & addr, std::vector<PlayingNode> & tree) {
    
    std::ifstream file(addr, std::ios::binary);
    if (!file.is_open()) {
        std::cout << "Impossible d'ouvrir le fichier : "<<addr<<std::endl;
    } else {
        std::string word;
        std::string fileContent((std::istreambuf_iterator<char>(file)), (std::istreambuf_iterator<char>()));
        for (unsigned int i = 0; i < fileContent.length(); i++) {
            if (fileContent[i] == static_cast<unsigned char>(127) && word.length()%5==2) {
                tree.push_back(PlayingNode(word));
                word = "";
            }
            else word += fileContent[i];
        }
    }
}

void printTree(std::vector<PlayingNode> const & tree) {
    for (unsigned int i = 0; i < tree.size(); i++) tree[i].print();
}

std::string toString(LearningNode const & l) { 
    /* Pour obtenir le ratio de succès sous forme de uint16_t :
    - on calcule le ratio _score/nbCrossing, compris entre -1 et 1
    - on ajoute 1, pour avoir une valeur entre 0 et 2
    - on multiplie par 2 puissance 15 -1 (32767) pour avoir une valeur entre 0 et 2 puissance 16-2
    - on fait un cast en uint16_t */
    if(l._nbVisit == 0) std::cout<<"Division par 0 dans toString"<<std::endl;
    uint16_t successRatio = static_cast<uint16_t> ((l._score/l._nbVisit+1)*32767);    
    /* On applique ensuite un masque au ratio de succès pour le stocker dans deux char, c1 et c2 :
    - c2 s'obtient en appliquant le masque 0000_0000_1111_1111, soit 2 puissance 8 moins 1 = 255
    - c1 s'obtient de la même manière, après avoir décalé le ratio de succès de 8 bits vers la droite */
    char c2 = static_cast<unsigned char>(successRatio & 255);
    successRatio >>= 8;
    char c1 = static_cast<unsigned char>(successRatio & 255);
    std::string result = "";
    result.push_back(c1);
    result.push_back(c2);
    unsigned int childIndex; //indice du fils courant, stocké sur 4 octets
    std::string childIndexString; //indice du fils courant converti en string
    for (auto it = l._children.begin(); it != l._children.end(); it++) {
        result += static_cast<unsigned char>(it->first.first+it->first.second*11); //concaténation du coup, représenté par l'une des 121 premières valeurs d'un char
        childIndex = it->second;
        childIndexString = "";
        for (unsigned int j = 0; j < 4 ; j++) { // on parcourt l'indice de l'enfant de gauche à droite
            childIndexString.insert(0,1,static_cast<unsigned char>(childIndex & 255)); //ajout par la gauche de l'octet 'de poids faible' de childIndex, converti en char
            childIndex >>= 8; //on passe à l'octet suivant
        }
        result+=childIndexString;
    }
    return result;
}

void writeTreeInFile(std::string const & addr, std::vector<LearningNode> const & tree, 
unsigned int intervalBegin, unsigned int intervalEnd) {
    std::string result = "";
    for (unsigned int i = intervalBegin; i < intervalEnd; i++) {
        result += toString(tree[i]); 
        result += static_cast<unsigned char>(127); // char séparant les différents noeuds
    }
    std::ofstream file(addr, std::ios::binary);
    if (! file.is_open()) {
        std::cout << "Impossible d'ouvrir le fichier en écriture"<<std::endl;
    } else {
        file << result;
    }
}
