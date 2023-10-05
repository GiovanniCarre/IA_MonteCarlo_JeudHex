#include "playingNode.hh"

PlayingNode::PlayingNode(std::string const & s) {
    /* Construction du ratio de succès (_score), grâce aux deux premiers caracères de la chaine reçue en paramètre*/
    _score = static_cast<uint16_t>(static_cast<unsigned char>(s[0]));
    _score <<= 8;
    _score |= static_cast<uint16_t>(static_cast<unsigned char>(s[1]));
    /* Construction de la partie de la chaine relative aux enfants*/
    for (unsigned int i = 2; i < s.length(); i+=5) {
        /* Construction du coup menant à l'enfant courant*/
        int8_t integerKey = static_cast<int8_t>(s[i]);
        std::pair<int, int> key = {integerKey%11, integerKey/11};

        /* Construction de l'indice de l'enfant courant*/
        unsigned int value = static_cast<unsigned int>(static_cast<unsigned char>(s[i+1]));        
        for (unsigned int j = 2; j < 5; j++) {
            value <<= 8;
            value |= static_cast<unsigned int>(static_cast<unsigned char>(s[i+j]));
        }

        /* Ajout de la valeur associée à la clé nouvellement construite */
        _children[key] = value;
    }
}

void PlayingNode::print() const {
    std::cout<<"Score : "<<_score<<std::endl;
    for (auto it = _children.begin(); it != _children.end(); it++) {
        std::cout<<"("<<it->first.first<<","<<it->first.second<<") : "<<it->second<<std::endl;
    }
    std::cout<<std::endl;
}