#include <iostream>
#include <fstream>
#include <vector>
#include "playingNode.hh"
#include "learningNode.hh"

void readFile(std::string const & addr, std::vector<PlayingNode> & tree); //Le vecteur passé par référence est rempli grâce au fichier spécifié

void printTree(std::vector<PlayingNode> const & tree); //Fonction utile pour le débuggage uniquement

std::string toString(LearningNode const & l); //On pourrait remplacer ça par une méthode toString dans la classe LearningNode

void writeTreeInFile(std::string const & addr, std::vector<LearningNode> const & tree,
unsigned int intervalBegin, unsigned int intervalEnd); //Fonction appelée depuis le learner pour sauvegarder les recherches dans un fichier