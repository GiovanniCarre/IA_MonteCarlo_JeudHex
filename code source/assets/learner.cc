#include "learner.hh"


//constructeur
Learner::Learner(int playerArg) {
    _player = playerArg;
    
    //Varie le temps de calcul, représente le nombre de scénarios pour entraîner l'algorithme
    _nbScenario = 500000;
    
}


//Méthode d'apprentissage (La méthode principale)
void Learner::monteCarloLearning(Jeu &j){

    //initialisation ------------------------------------


     std::srand(std::time(nullptr));
    std::cout << "Début apprentissage pour le joueur : "<< _player <<"------------------------\n";
    //normalement on aura le nombre de noeuds = nombre de scénarios + racine donc on alloue avant
    _MCTS.reserve(_nbScenario+1);

   

    //création de la racine
    LearningNode root;
    _MCTS.push_back(root);
    
    
    //POUR FAIRE DES SAUVEGARDES du mcts sur plusieurs fichiers,
    unsigned int nbSave = 0;

    //ici on met tous les checkpoints qu'on veut en fonction du nombre de noeuds
    std::vector<unsigned int> savePoints;
    
    
    savePoints.push_back(1000);
    savePoints.push_back(10000);
    savePoints.push_back(500000);
    savePoints.push_back(1000000);
    savePoints.push_back(5000000);
    savePoints.push_back(10000000);
    savePoints.push_back(25000000);
    //déclaration différentes variables
    unsigned int indexCurrent = 0;
    std::vector<unsigned int> nodeVisitedIndex; //utile pour la remontée
    
    int currentPlayer = _player;

    unsigned int randomNumber;
    couple coupQUBC;//coup choisi pour lors du calcul du meilleur QUBC
    unsigned int indexMaxChild;
    
    //boucle sur chaque scénario
    for (unsigned int i = 0;i < _nbScenario;++i){
        //réinitialisation
        printProgress(20000, i);
        randomNumber = rand();
        
        nodeVisitedIndex.clear();
        indexCurrent = 0;
        j.vider_jeu();        
        nodeVisitedIndex.push_back(0);//on recommence par la racine


        //DESCENTE QUBC -------------------------------------------------------------------------
        std::vector<couple> coupsPossibles = j.coups_possibles();
        
        //tant que le nombre de coups = nombre d'enfant(s) du noeud courant =
        //On vérifie si tous les enfants ont été traversés au moins une fois
        while(_MCTS[indexCurrent].nbChildren() == coupsPossibles.size()){
            alternePlayer(currentPlayer);

            //on obtient le meilleur coup
            coupQUBC = _MCTS[indexCurrent].getMaxQUBCChildCouple(_MCTS);
            indexMaxChild = _MCTS[indexCurrent]._children[coupQUBC];

            //correspond à jouer le coup, sachant que nous n'avons pas utilisé la méthode jouer_coup pour gagner en termes de performances
            j._grille[coupQUBC.first][coupQUBC.second] = currentPlayer;
            
            //on dit que le noeud courant est celui qu'on a choisi
            indexCurrent = indexMaxChild;

            //rajoute les noeuds visités
            nodeVisitedIndex.push_back(indexMaxChild);
            
            coupsPossibles = j.coups_possibles();
        }
        
        
        //accroissement (on ne choisit pas un noeud qui est final même si normalement ca n'arrive jamais) ---------------------------------------
        int result = j.cotes_relies();

        //le if pour ne pas faire l'accroissement sur un coup déjà gagnant / perdant (en gros on fait en sorte que la partie ne soit pas déjà finie)
        if (!result){
            //le randomNumber est un nombre aléatoire mais calculé à chaque début de scénario
            int irand = randomNumber%coupsPossibles.size();
            
            int nbCoupsPossibles = coupsPossibles.size();
            couple coupChoose = coupsPossibles.at(irand);
            //la boucle est là pour ne pas refaire un coup qu'on à déjà fait
            while(_MCTS.at(indexCurrent).containsChild(coupChoose)){
                ++irand;
                if (irand >= nbCoupsPossibles) irand = 0;
                coupChoose = coupsPossibles.at(irand);

            }

            alternePlayer(currentPlayer);
            
            //correspond à jouer_coup
            j._grille[coupChoose.first][coupChoose.second] = currentPlayer;

            _MCTS.at(indexCurrent).putChild(coupChoose, i+1);

            indexCurrent = i+1;//Le noeud courant devient le nouveau noeud ajouté au MCTS
            _MCTS.emplace_back();//on rajoute un noeud vide (sans argument)
            
            
            nodeVisitedIndex.push_back(i+1);
            
            //roll out --------------------------------------
            
            coupsPossibles = j.coups_possibles();
            unsigned int indexChoose;
            

            //tant qu'il reste des coups possibles on continue
            //permet d'être rapide (pas d'appels à partie terminée qui est long)
            //car dans le jeu d'hex meme si on continue jusqu'à la fin de remplir la grille,
            //le résultat restera le même
            while(!coupsPossibles.empty()){//on fait tous les coups possibles de manière aléatoire
                alternePlayer(currentPlayer);

                indexChoose = randomNumber%coupsPossibles.size();
                
                coupChoose = coupsPossibles[indexChoose];
                //on supprime le coup choisi
                coupsPossibles.erase(coupsPossibles.begin()+indexChoose);

                //correspond encore à joueur_coup
                j._grille.at(coupChoose.first).at(coupChoose.second) = currentPlayer;
                
                // on a besoin que la grille soit publique pour gagner du temps
            }
           
             
        }
            
        //on a modifié dans notre code la fonction cotes_reliés car elle prenait du temps (250 micro secondes)
        //notre version améliorée s'exécute en moyenne en 150μs.
        result = j.cotes_relies();
            
        //remontée ------------------------------------
        //fait directement l'update, du haut vers le bas.
        //(de la racine vers le noeud finale)
        for (unsigned int k = 0; k < nodeVisitedIndex.size();++k){
            _MCTS[nodeVisitedIndex[k]]._score+=result*2-3;//si result = 1 renvoie -1 et si result = 2 renvoie 1
            ++_MCTS[nodeVisitedIndex[k]]._nbVisit;//+1 au nombre de visite
        }

        //-ALGO MONTECARLO FINI- ---------------
        
        
     
        //La remontée se fait du haut vers le bas
           
    }
    //fin apprentissage
    //save en fonction du nombre de scénarios de manière arbitraire
    for (unsigned int i = 0;i < _nbScenario;++i){
        for (unsigned int k = 0; k < savePoints.size();k++){
            if (savePoints.at(k) == i){
                if (k == 0){
                    save("assets/", _MCTS, 0, savePoints.at(0),0);
                }else{
                    save("assets/", _MCTS, savePoints.at(k-1),savePoints.at(k),nbSave);
                }
                nbSave++;
            }
        }
    }

    //affiche stats sur nombre de passages
    printVisit();


    //sauvegarde finale des derniers éléments
    save("assets/", _MCTS, savePoints.back(), _MCTS.size(),nbSave);
    
    //taille calcul (approximative)
    unsigned int size = getRealSize();
    std::cout << "Apprentissage FINI"<<", Taille (pas très précis) : "<< std::to_string(size) <<" octets."<<std::endl;

}



unsigned int Learner::getRealSize() const {
    //on utilise une pile avec accumulate
    return std::accumulate(_MCTS.begin(), _MCTS.end(), 0,
        [](unsigned int acc, const LearningNode& noeud) {
            return acc + 8 + 24 * noeud.nbChildren();
        }
    );
}


//fait appel à fileManager et sinon juste de l'affichage d'info et calcul du temps
void Learner::save(std::string path, std::vector<LearningNode> const & tree,
unsigned int intervalBegin,unsigned int intervalEnd, unsigned int nbSave)const{

    auto t1 = std::chrono::high_resolution_clock::now();


    std::cout << "Début de la sauvegarde jusqu'à : "<<std::to_string(intervalEnd)<<std::endl;
    if (intervalBegin >= intervalEnd){
        std::cout << "ATTENTION IL Y A UN PB L'intervalle begin est supérieur, dans la save."<<std::endl;
    }
    writeTreeInFile(path+"intelligence/Intelligence"+std::to_string(_player)+"_"+std::to_string(nbSave)+".txt", tree, 
    intervalBegin, intervalEnd);


    auto t2 = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

    std::cout << "Duree sauvegarde : "<< duration<< " ms.\n";
}


//statistiques
void Learner::printVisit() const {
    //accumulateur - idem que la fonction plus haut
    unsigned int total_Visits = std::accumulate(_MCTS.begin(), _MCTS.end(), 0,
        [](int acc, const LearningNode& n) { return acc + n.getNbVisit(); });

    std::cout << "Moyenne de passage par noeuds : " << total_Visits / _MCTS.size() << "--------" << std::endl;
    std::cout << "Taille du MCTS : "<<_MCTS.size() << "\n";
}



void Learner::printProgress(unsigned int interval, unsigned int nbScenarioDone)const{
    if (nbScenarioDone%interval == 0)
        std::cout << "Progression : "<<nbScenarioDone<<" / "<<_nbScenario<<"\n";
}


void Learner::alternePlayer(int & j){
    j = 3-j; //if j== 2 -> j=1 else j=2   
}
//---GETTERS

std::vector<LearningNode> Learner::getMCTS()const{
        return _MCTS;
}
