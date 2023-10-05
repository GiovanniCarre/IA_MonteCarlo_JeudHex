#include "joueur_lesPetitsDiables.hh"


//initialisation des MCTS en static
std::vector<PlayingNode> Joueur_LPD_::MCTS1 = {};
std::vector<PlayingNode> Joueur_LPD_::MCTS2 = {};
std::vector<PlayingNode> Joueur_LPD_::MCTS = {};


Joueur_LPD_::Joueur_LPD_(std::string nom, bool joueur)
    :Joueur(nom,joueur)
{
    //chargement 1 seule fois à la constuction du 1er joueur LPD
    if (MCTS.empty()){
        auto t1 = std::chrono::high_resolution_clock::now();
        unsigned int tempsChargement = 7;// correspond au nombre de fichiers à charger, donc moins il y en a, moins ca prendra de temps
        //Mettre aussi le nom de fichier intelligence (on part du dossier parent de joueur)
        std::string src = "assets/intelligence/Intelligence";
        for (unsigned int i = 0; i < tempsChargement;i++){
            std::cout << "Chargement LPD : Fichier numéro : "<<i<<" / "<<tempsChargement-1<<std::endl;
            //lecture j1
            readFile(src+"1_"+std::to_string(i)+".txt", this->MCTS1);//fait appel à la méthode de fileManager
            //lecture j2
            readFile(src+"2_"+std::to_string(i)+".txt", this->MCTS2);
        }
        auto t2 = std::chrono::high_resolution_clock::now();
        auto duree = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
        std::cout << "\nChargement Fini pour les petits diables:  Nombre de scénarios : " << std::to_string(MCTS1.size())<< " , Nombre de ms : "<<std::to_string(duree/1000)<< std::endl;
    }
    
    //changement du mcts en fonction de la partie
    if (joueur)
        MCTS = MCTS1;
    else
        MCTS = MCTS2;


    //initialisation du terrain
    grillePrecedente.clear();
    std::vector<int> v;
    for (unsigned int i = 0; i < 11; i++){
        for (unsigned int j = 0; j < 11; j++){
            v.push_back(0);
        }
        grillePrecedente.push_back(v);
        v.clear();
    }

    coupsPrecedents.clear();
}


void Joueur_LPD_::calculerCoupsPrecedents(Jeu& j){

    //nb cases utilisées
    couple c1 = std::make_pair(-1, -1);
    couple c2 = std::make_pair(-1, -1);
    
    std::vector<std::vector<int>> grille = j.grille();
    for (unsigned int i = 0; i < grille.size(); i++){
        for (unsigned int j = 0; j < grille[i].size(); j++){ // j est la position en x, i en y
            if(grille[i][j] != grillePrecedente[i][j]){
                if(grille[i][j] == 1){
                    c1 = std::make_pair(j, i); 
                } else {
                    c2 = std::make_pair(j, i);
                }
            }
        }    
    }

    if (c1.first != -1)    coupsPrecedents.push_back(c1);
    if (c2.first != -1)    coupsPrecedents.push_back(c2);


    grillePrecedente = grille;
}



namespace methodesLPD {
//voisins d'un couple sur une grille hexagonale
//résultats sur https://onlinegdb.com/rNApArQQ6
void trouverVoisins(couple& c, std::vector<couple>& vois){
    couple tmp = c;

    bool g = c.first != 0;
    bool d = c.first != 10;
    bool b = c.second != 10;
    bool h = c.second != 0;

    //gauche et dessous
    if (g){ // gauche
        tmp.first--;
        vois.push_back(tmp);
        tmp.first++;
    }
    if (b){ // bas
        tmp.second++;
        vois.push_back(tmp); // en dessous
        if (g){  // en dessous à gauche
            tmp.first--;
            vois.push_back(tmp);
        }
    }
    
    tmp = c;
    if (d){ // droite
        tmp.first++;
        vois.push_back(tmp);
        tmp.first--;
    }
    if (h){ // haut
        tmp.second -= 1;
        vois.push_back(tmp); // au dessus
        if (d){  // au dessus à droite
            tmp.first += 1;
            vois.push_back(tmp);
        }
    }
}



// donne une grille avec la distance à un point sur chaque case
void calcDistGrille(std::vector<std::vector<int>>& grilleJeu, std::vector<std::vector<int>>& grilleDist, bool joueur){
    const int SZ = grilleJeu.size();

    couple c;
    std::vector<couple> voisins;

    int valJ = (joueur-1)*-1+1;

    //on propage les valeurs SZ fois pour qu'elles atteignent tout le plateau
    for (int o = 0; o < SZ; o++){
    
    for (int x = 0; x < SZ; x++){
        for (int y = 0; y < SZ; y++){ 
            if (grilleDist[x][y] == 1) continue; //a déjà la valeur minimale
            voisins.clear();
            if (grilleJeu[x][y] == 0) { //si la case est vide
                c = std::make_pair(x, y);
                methodesLPD::trouverVoisins(c, voisins);
                
                int minP = grilleDist[x][y];
                for (unsigned int id = 0; id < voisins.size(); id++){
                    minP = std::min(minP, grilleDist[voisins[id].first][voisins[id].second]+1);
                }

                grilleDist[x][y] = minP;
                
            } else if (grilleJeu[x][y] == valJ){
                //si on a déjà joué sur une case, alors son prix est le prix min de ses voisins
                c = std::make_pair(x, y);
                methodesLPD::trouverVoisins(c, voisins);
                
                int min = grilleDist[x][y];
                for (unsigned int id = 0; id < voisins.size(); id++){
                    min = std::min(min, grilleDist[voisins[id].first][voisins[id].second]);
                }
                grilleDist[x][y] = min;


            } else { //pion adverse
                grilleDist[x][y] = 99;
            }
        }
    }
    }


    //on remplace juste les cases déjà jouées à la fin pour ne pas rejouer dessus dans le futur
    for (int x = 0; x < SZ; x++){
        for (int y = 0; y < SZ; y++){ 
            if (grilleJeu[x][y] != 0)
                grilleDist[x][y] = 99;
        }
    }


}



float calcScorePos(std::vector<std::vector<int>>& grilleJeu, std::vector<std::vector<int>>& grilleDist, int x, int y, int minVal){
    couple c = std::make_pair(x, y);
    std::vector<couple> voisins;
    methodesLPD::trouverVoisins(c, voisins);
               
    //nb de voisins vides sur le chemin optimal + 1.2*(nbEnnemis)
    float sc = 0, nbEnn = 0;
    for (unsigned int id = 0; id < voisins.size(); id++){
        if (grilleDist[voisins[id].first][voisins[id].second] == minVal){
            sc++;
        } else if (grilleJeu[voisins[id].first][voisins[id].second] != grilleJeu[x][y]){
            nbEnn++;
        }
    }

    return sc+nbEnn*1.2;
}

}



void Joueur_LPD_::meilleurCoupSansMC(Jeu& j, couple &coup){
    // on va utiliser un algorithme personnalisé similaire à Dikjstra
    bool display_grid = false;
    const unsigned int SZ = j.taille();

    std::vector <std::vector<int>> grilleJeu = j.grille();
    std::vector <std::vector<int>> grillePrixD1, grillePrixD2, grillePrix; //grille représentant le coût de chaque case (distance min des 2 côtés)
    //les lignes du dessous sont peu élégantes mais permettent d'éviter des copies de tableaux, coûteuses en temps
    grillePrixD1 = {{99,99,99,99,99,99,99,99,99,99,99},{99,99,99,99,99,99,99,99,99,99,99},{99,99,99,99,99,99,99,99,99,99,99},{99,99,99,99,99,99,99,99,99,99,99},{99,99,99,99,99,99,99,99,99,99,99},{99,99,99,99,99,99,99,99,99,99,99},{99,99,99,99,99,99,99,99,99,99,99},{99,99,99,99,99,99,99,99,99,99,99},{99,99,99,99,99,99,99,99,99,99,99},{99,99,99,99,99,99,99,99,99,99,99},{99,99,99,99,99,99,99,99,99,99,99}};
    grillePrixD2 = {{99,99,99,99,99,99,99,99,99,99,99},{99,99,99,99,99,99,99,99,99,99,99},{99,99,99,99,99,99,99,99,99,99,99},{99,99,99,99,99,99,99,99,99,99,99},{99,99,99,99,99,99,99,99,99,99,99},{99,99,99,99,99,99,99,99,99,99,99},{99,99,99,99,99,99,99,99,99,99,99},{99,99,99,99,99,99,99,99,99,99,99},{99,99,99,99,99,99,99,99,99,99,99},{99,99,99,99,99,99,99,99,99,99,99},{99,99,99,99,99,99,99,99,99,99,99}};
    grillePrix = {{99,99,99,99,99,99,99,99,99,99,99},{99,99,99,99,99,99,99,99,99,99,99},{99,99,99,99,99,99,99,99,99,99,99},{99,99,99,99,99,99,99,99,99,99,99},{99,99,99,99,99,99,99,99,99,99,99},{99,99,99,99,99,99,99,99,99,99,99},{99,99,99,99,99,99,99,99,99,99,99},{99,99,99,99,99,99,99,99,99,99,99},{99,99,99,99,99,99,99,99,99,99,99},{99,99,99,99,99,99,99,99,99,99,99},{99,99,99,99,99,99,99,99,99,99,99}};

    //on définit les points de départ et d'arrivée (valeur 1 dans les grilles)
    for (unsigned int i = 0; i < SZ; i++){
        if (!this->joueur()){ //le joueur doit faire une ligne verticale
            if (grilleJeu[i][0] != 1){
                if (grilleJeu[i][0] == 2) grillePrixD1[i][0] = 0;
                else grillePrixD1[i][0] = 1;
            }
            if (grilleJeu[i][10] != 1){
                if (grilleJeu[i][10] == 2) grillePrixD2[i][10] = 0;
                else grillePrixD2[i][10] = 0;
            }
        } else { //le joueur doit faire une ligne horizontale
            if (grilleJeu[0][i] != 2){
                if (grilleJeu[0][i] == 1) grillePrixD1[0][i] = 0;
                else grillePrixD1[0][i] = 1;
            }
            if (grilleJeu[10][i] != 2){
                if (grilleJeu[10][i] == 1) grillePrixD2[10][i] = 0;
                else grillePrixD2[10][i] = 1;
            }
        }
    }



    methodesLPD::calcDistGrille(grilleJeu, grillePrixD1, this->joueur());
    methodesLPD::calcDistGrille(grilleJeu, grillePrixD2, this->joueur());

    //calculer prix min (avec addition des deux grilles)
    int prixMin = (99*99)*2;
    for (unsigned int x = 0; x < SZ; x++){
        for (unsigned int y = 0; y < SZ; y++){
            int v = grillePrixD1[x][y] + grillePrixD2[x][y];
            grillePrix[x][y] = v;
            if (v < prixMin){
                coup.first = x; 
                coup.second = y;
                prixMin = v;
            } 
        }
    }


    //calculer le meilleur coup à jouer (avec le score max)
    float meilleurScore = 0, localScore = 0;
    for (unsigned int x = 0; x < SZ; x++){
        for (unsigned int y = 0; y < SZ; y++){
            if (grillePrix[x][y] == prixMin){
                localScore = methodesLPD::calcScorePos(grilleJeu, grillePrix, x, y, prixMin);
                if ((localScore > meilleurScore) || (localScore == meilleurScore && rand()%2)){
                    coup.first = x; 
                    coup.second = y;
                    meilleurScore = localScore; 
                }
            }
        }
    }



    //affichage de la grille
    if (display_grid){
        for (unsigned int i = 0; i < SZ; i++){
            std::cout << "\n";
            for (unsigned int e = 0; e < i; e++){
                std::cout << " ";
            }

            for (unsigned int j = 0; j < SZ; j++){
                std::cout << grillePrix[i][j] << " ";
            }
        }
    }
}



void Joueur_LPD_::recherche_coup(Jeu j, couple &coup){
    std::vector<couple> coupsPossibles = j.coups_possibles();


    //valeurs de base au cas où on n'a pas le temps de tout calculer
    coup.first = coupsPossibles[0].first; 
    coup.second = coupsPossibles[0].second; 
    calculerCoupsPrecedents(j);

    //on descend dans le MCTS  

    //MCTS[0] = racine
    PlayingNode noeudActuel = MCTS[0];
    for (unsigned int i = 0; i < (unsigned int) coupsPrecedents.size(); i++){
        // regarde si la clé apparait dans _children 
        // (sinon elle est rajoutée automatiquement, ce qui crée des erreurs plus loin)
        if (noeudActuel._children.find(coupsPrecedents[i]) == noeudActuel._children.end()) {
            meilleurCoupSansMC(j, coup);
            return;
        } else if (noeudActuel._children[coupsPrecedents[i]] < MCTS.size()){
            // On se déplace vers le fils du noeud représentant le coup qui a été joué
            noeudActuel = MCTS[ noeudActuel._children[coupsPrecedents[i]] ];
        } else {
            meilleurCoupSansMC(j, coup); // n'apparait pas dans notre arbre
            return;
        }
    }  


    

    // position dans les enfants de noeud actuel, 
    // trié dans le même ordre que coupsPossibles    
    couple meilleurPos = noeudActuel._children.begin()->first;
    unsigned int meilleur = noeudActuel._children.begin()->second; 
    
    uint16_t meilleurScore = 0; //un noeud que l'on n'a pas exploré aura un score faible pour qu'on tende à rester sur des chemins que l'on connaît
    if (MCTS.size() > meilleur){   //le noeud est défini dans notre arbre
        meilleurScore = MCTS[meilleur]._score;
    }


    
    for (unsigned int i = 1; i < coupsPossibles.size(); i++){
        unsigned int idCoupEtudie = noeudActuel._children[coupsPossibles[i]];
        uint16_t sc = -0.5; // valeur de base si noeud non exploré
        if (MCTS.size() > idCoupEtudie){   //le noeud est défini dans notre arbre
            sc = MCTS[idCoupEtudie]._score; // calcul du % de victoire depuis le coup étudié
            if (sc > meilleurScore){ // si plus avantageux que le meilleur précédents, maj
                meilleur = idCoupEtudie;
                meilleurPos = coupsPossibles[i];
                meilleurScore = sc;
            }
        }

    }

    

    //on joue le meilleur coup trouvé
    coup.first = meilleurPos.first;
    coup.second = meilleurPos.second;

    return; 
    
}



void Joueur_LPD_::afficherMCTS(){
    for (unsigned int i = 0; i < this->MCTS.size(); i++){
        MCTS[i].print();
    }

}
