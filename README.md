# Octopoulpe - Oceans

Projet de réseaux réalisé à l'ENSEIRB-MATMECA réalisé par :
- Laurie-Anne Parant
- Louise Mouret
- Paul Breton
- Paul Gaulier
- Nicolas Vidal

Les sources que vous pourrez trouver ici se découpent en deux projets :
- Poseidon : Projet du serveur de l'Aquarium, écrit en C
- Aquarium : Projet de la vue de l'aquarium, écrit en Java et utilisant la bibliothèque JavaFX.

## Poseidon

Pour compiler, vous devez posséder CMake.
Comme pour tout projet réalisé avec cmake, créez un répertoire build/ à la racine principale du projet. 
Exécutez la commande `cmake ../`.
Entrez dans `build/` et entrez la commande `make all`.
Il ne vous reste plus qu'à exécuter `build/Poseidon/src/Server` pour lancer le serveur principal !
Un script réalise toutes ces actions pour vous...
Il ne vous reste qu'à lancer `./Oceans.sh` pour réaliser tout ceci !

Attention, le fichier `controller.cfg` doit obligatoirement se trouver dans le même répertoire que l'exécutable du serveur. (géré automatiquement par CMake)

## Aquarium

Pour compiler, vous devez posséder l'extension Java de CMake (normalement packagée par défaut dans votre installation de CMake).
Si malheureusement CMake refuse de compiler correctement, vous pourrez trouver à la racine du projet Aquarium/, un script qui vous
permettra tout de même de compiler et d'exécuter le programme en Java (même s'il est tout de même plus "agréable" d'utiliser le CMake).

## Note

Les deux scripts fournis disposent d'une fonction `usage`, que vous pouvez appeller à l'aide de l'option -h ou --help, n'hésitez pas à 
vous en servir...
