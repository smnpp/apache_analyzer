# Projet d'Analyse de Logs

## Description

Ce dossier contient les éléments suivants :

- **Compte-Rendu.pdf** : Le compte rendu du projet.
- **umlTP4.pdf** & **umlTP4.pnj** : Le diagramme UML du projet.
- **Tests** : Un dossier contenant tous les tests.
- **Code source** : Les fichiers sources du projet.

## Manuel d'Utilisation

Un extrait du compte rendu est fourni ci-dessous pour guider l'utilisation du programme :

### 1. Spécifications détaillées

Les options peuvent être utilisées dans n’importe quel ordre. Dans le cas d’une répétition d’option, seuls les arguments du dernier appel seront pris en compte, les précédents seront négligés. En cas de conflit entre plusieurs options, la priorité est également accordée à celle appelée en dernier. Enfin, il n’est pas possible de concaténer les options ; par exemple, la séquence "-egts" ne sera pas reconnue.

#### 1.1 Description des différentes options possibles

- **-h** : Afficher ce message d’aide (exécuter "Analog -h").
- **-a** : Obtenir un affichage statique (requis pour les tests).
- **-g <graphique>** : Cette option permet de produire un fichier au format GraphViz (.dot) du graphe analysé nommé `<graphique>`. Chaque document apparaîtra sous la forme d’un nœud et chaque arc indiquera le nombre de parcours associés.
- **-e** : Cette option permet d’exclure tous les documents qui ont une extension de type image (jpeg, png, ...), css ou javascript.
- **-t <heure>** : Cette option permet de ne prendre en compte que les hits qui sont dans le créneau horaire correspondant à l’intervalle `[<heure>, <heure>+1[`. Notons que si l’heure dépasse 24, un modulo(24) est appliqué pour obtenir une heure cohérente.
- **-s <URL>** : URL racine du site web (par défaut, "intranet-if.insa-lyon.fr").

### Création de l'exécutable

Pour créer l'exécutable, exécutez la commande suivante :

`make`

---

Ce README devrait vous fournir les informations nécessaires pour utiliser et comprendre notre application. En cas de questions ou de problèmes, n'hésitez pas à nous contacter.
