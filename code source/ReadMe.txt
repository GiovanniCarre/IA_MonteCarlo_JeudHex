COMPETITION MONTECARLO-JEU DE HEX
Groupe "Les Petits Diables" : CARRE Giovanni, BIAGI Dorian, BERTHELOT Loïc

FICHIERS
- Nous avons ajouté les fichiers "joueur_lesPetitsDiables.cc" et "joueur_lesPetitsDiables.hh" dans le dossier joueurs.
- Nous avons créé le dossier assets qui contient tous nos autres fichiers, qui sont responsables de l'apprentissage, ainsi que de l'encodage, du stockage et du décodage de nos données d'apprentissage.
- Nous avons ajouté la ligne "add_subdirectory(assets)" au fichier CmakeLists.txt du dossier principal, afin de permettre l'inclusion de ces nouveaux fichiers.

STRUCTURE DE DONNEES
- Le fichier joueur_lesPetitsDiables.cc charge en mémoire deux arbres de recherche obtenus grâce à l'algorithme de Monte-Carlo (MCTS). Il est nécessaire d'avoir deux arbres pour traiter le cas où le joueur existentiel commence la partie, et le cas où il joue en deuxième. Les fichiers dont le nom commence par Intelligence1 et Intelligence2 dans le dossier assets/intelligence correspondent respectivement aux données de ces deux arbres.
- Nous avons séparé les données de ces deux arbres en plusieurs fichiers, pour pouvoir nous adapter au temps qui nous sera accordé au moment de la compétition.
- En mémoire, nos MCTS sont représentés par des vecteurs d'objets de la classe LearningNode (lors de l'apprentissage) ou PlayingNode (lors de la phase de jeu). Ces objets représentant les noeuds du MCTS contiennent eux-mêmes les indices de leurs noeuds enfants sous forme d'entiers stockés dans un vecteur. Ce système permet de nous déplacer dans les arbres de recherche sans utiliser de pointeurs, pour accélérer l'exécution de nos algorithmes.

ALGORITHMES
- Pour sélectionner le fils du noeud courant lors de la phase de descente, nous avons utilisé la méthode du QUBC, avec une constante C valant 2.
- Lors de la phase de roll out, nous avons choisi de faire jouer les deux joueurs jusqu'à ce que le plateau soit rempli de pions, pluôt que de les faire jouer jusqu'à ce que l'un d'eux gagne. Cela ne change rien au résultat obtenu en fin de roll out (une fois qu'un joueur a relié les deux bords, l'autre joueur ne peut plus faire de même), mais permet de ne pas appeler la fonction partie_finie(), qui est très couteuse en temps.
- Pour augmenter la vitesse d'apprentissage, nous avons fait le choix de n'appeler la fonction rand() qu'une fois par scénario, et de stocker cette valeur pour appliquer ensuite des modulos de valeurs toujours différentes dessus. La valeur retournée par rand() est suffisamment grande pour avoir une répartition quasi homogène des valeurs obtenues après calcul des modulos.
- L'arbre obtenu par la méthode de Monte Carlo ne couvre qu'une petite partie de l'arbre de recherche. Lorsque l'enchainement des coups joués n'est pas répertorié dans notre MCTS, notre algortihme de jeu, situé dans joueur_lesPetitsDiables.cc, applique une méthode en deux temps pour déterminer quelles cases sont les plus intéressantes : notre algorithme détermine d'abord quelles cases libres du plateau semblent former le chemin le plus court pour relier les deux bords, puis les cases obtenues sont départagées en calculant un score qui dépend des cases voisines (le score sera plus important si d'autres cases sélectionnées ou si des cases occupées par des pions adverses sont adjacentes).

ENCODAGE DANS LES FICHIERS
- Nos MCTS sont enregistrés dans des fichiers .txt. Les noeuds de notre MCTS d'apprentissage sous forme de vecteur y sont représentés dans le même ordre (le noeud racine au début). Nous utilisons un seul type de séparateur, qui sert à délimiter les différents noeuds dans le fichier. Ce séparateur est l'octet valant 127.
- Un noeud est représenté sur 2+5n octets (n étant un entier naturel). Les deux premiers octets forment une unique valeur qui correpond au score du noeud divisé par le nombre de passages. Les blocs de 5 octets suivants correspondent chacun à un fils du noeud. Le premier de ces 5 octets représente le coup qui conduit à ce fils, tandis que les 4 octets suivants forment ensemble l'indice du fils dans le MCTS.
- Le décodage est entièrement déterministe. Après avoir lu 2+5 octets d'un noeud, le prochain octet est soit un séparateur de valeur 127, soit l'indice du coup du prochain fils, qui vaut entre 0 et 120 (ce qui représente bien les 121 coups possibles sur le plateau). Un octet ne peut donc être interprété que d'une façon.
