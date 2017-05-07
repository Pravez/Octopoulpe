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

## Résumé

Pour simplifier au plus le système de build, nous avons mis à disposition un script qui réalise toutes les commandes nécessaires à la 
compilation et exécution des programmes.
Vous pouvez donc utiliser le script situé à la racine du projet principal, nommé `Oceans.sh`. Il est muni d'un menu d'aide appellable via
l'option -h ou --help.

Si vous souhaitez préparer le répertoire de build, et compiler toutes les sources, utilisez : `./Oceans.sh -p -c`.
Si vous souhaitez lancer un des deux programmes produits, utilisez : `./Oceans.sh -r [EXEC]` où EXEC est soit Server, soit Aquarium.

## Poseidon

Pour compiler, vous devez posséder CMake.
Comme pour tout projet réalisé avec cmake, créez un répertoire build/ à la racine principale du projet. 
Exécutez la commande `cmake ../`.
Entrez dans `build/` et entrez la commande `make all`.
Il ne vous reste plus qu'à exécuter `build/Poseidon/src/Server` pour lancer le serveur principal !
Attention, le fichier `controller.cfg` doit obligatoirement se trouver dans le même répertoire que l'exécutable du serveur. (géré automatiquement par CMake)

## Aquarium

Pour compiler, vous devez posséder l'extension Java de CMake (normalement packagée par défaut dans votre installation de CMake).
Si malheureusement CMake refuse de compiler correctement, vous pourrez trouver à la racine du projet Aquarium/, un script qui vous
permettra tout de même de compiler et d'exécuter le programme en Java (même s'il est tout de même plus "agréable" d'utiliser le CMake).
Attention, le répertoire Images/ doit se trouver au même niveau que les .class de java. De plus, le repertoire resources/ doit se trouver au
même endroit que le classpath global (pas besoin de s'en soucier dans le cadre par défaut, tout est normalement géré).

## A propos des tests

Nous avons réalisé des tests sur le serveur uniquement, permettant de vérifier le bon fonctionnement d'après notre implémentation
du protocole de communication entre les clients et le serveur, et le bon parsage des données en entrée d'un fichier .cfg donné.
Pour lancer ces tests, utilisez le script `Oceans.sh` via l'option -t ou --test. Si vous souhaitez exécuter à la main, ces 
exécutables se situent dans le répertoire tst/ de la solution (et donc dans un répertoire du même nom dans le build).