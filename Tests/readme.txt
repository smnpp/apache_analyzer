Programme de tests automatiques - mode d'emploi


-------------------------------------------------------------------------------
test.sh repertoireExe [ repertoireTest [ fichier.csv ] ]

- repertoireExe indique le répertoire dans lequel se trouve l'executable a 
tester
- repertoireTest indique le répertoire dans lequel se trouvent les fichiers de 
configuration du test
- repertoire indique le répertoire dans lequel se trouvent les fichiers de 
configuration du test
- fichier.csv indique un fichier dans lequel les résultats du tests seront 
ajoutés

En l'absence d'arguments, le script traitera le répertoire courant

Fichiers de configuration :
- run : fichier texte indiquant la ligne de commande à éxécuter (obligatoire)
- std.in : fichier texte indiquant une entrée clavier à simuler (facultatif)
- std.out : fichier texte indiquant la sortie devant être produite 
(facultatif/validation*)
- stderr.out : fichier texte indiquant la sortie d'erreur devant être produite 
(facultatif/validation*)
- description : fichier texte donnant la description du test (facultatif)
- returncode : fichier texte contenant l'entier code retour attendu 
(facultatif/validation*)
- *.outfile : un ou plusieurs fichiers devant être produits par le programme 
(facultatif/validation*)
  si le fichier s'appelle exemple.txt.outfile, il sera comparé à exemple.txt

*validation indique que le script va tester la conformité, si cette conformité 
n'est pas 
satisfaite, il l'indiquera

-------------------------------------------------------------------------------
mktest.sh [ repertoireExe ]

- repertoireExe indique le répertoire dans lequel se trouve l'executable a 
tester

En l'absence d'arguments, le script traitera le répertoire parent

Cherche à valider tous les tests contenus dans les sous répertoires