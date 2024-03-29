/*************************************************************************
    Analyzer  -  Analyse un ensemble d'objets 'Log' selon certains critères pour générer un top 10
    des pages les plus visitées, et accessoirement, un graphe de tous les hits au format GraphViz (.dot)
                             -------------------
    début                : 16/01/2024
    copyright            : (C) 2024 par Habba, Ifkirne, Jeanne, Perret, Pierre
    e-mail               : ___
*************************************************************************/

//---------- Interface de la classe <Analyzer> (fichier Analyzer.h) ----------------
#if !defined(ANALYZER_H)
#define ANALYZER_H

//--------------------------------------------------- Interfaces utilisées
using namespace std;
#include <unordered_map>
#include <vector>
#include <iostream>

//------------------------------------------------------------------------
// Analyse un ensemble d'objets 'Log' selon certains critères pour générer un top 10 des pages
// les plus visitées, et accessoirement, un graphe de tous les hits au format GraphViz (.dot)
//------------------------------------------------------------------------
class Analyzer
{
    //----------------------------------------------------------------- PUBLIC
public:
    //----------------------------------------------------- Méthodes publiques
    void Graphique(const char *fichierDot) const;
    // Mode d'emploi :
    //      Ecrit dans un fichier le code nécessaire à la génération ultérieure
    //      d'un graphe au format GraphViz.
    // Contrat :
    //      Aucun

    void AfficheTop10() const;
    // Mode d'emploi :
    //      Affiche dans la sortie standard la liste des 10 pages les plus visitées
    //      en renseignant leur nom et leur nombre total de hits.
    // Contrat :
    //      Aucun

    vector<pair<string, int>> TopN(unsigned int n) const;
    // Mode d'emploi :
    //      Génère une collection (nom page, nombre de hits) correspondant aux n
    //      pages les plus visitées.
    // Contrat :
    //      Aucun

    //-------------------------------------------- Constructeurs - destructeur
    Analyzer(const Analyzer &ana) = delete;
    // Il n'est pas possible de créer un objet Analyzer par copie

    Analyzer(const char *nomfichier, const char *optS, bool optE, char optT, bool optA);
    // Mode d'emploi :
    //      Constructeur par défaut de la classe Analyzer. A partir d'un d'un fichier .log
    //      et d'une série d'options (décrites dans le manuel d'utilisation), crée et sélectionne
    //      les objets Log à analyser.
    // Contrat :
    //      Aucun
    virtual ~Analyzer();

    //------------------------------------------------------------------ PRIVE
protected:
    //----------------------------------------------------- Méthodes protégées
    int getOrAddPageNumber(string page);
    // Mode d'emploi :
    //      Renvoie le numéro correspondant à un nom de page existant demandé, ou
    //      intègre la page en lui associant un numéro si la page n'est pas déjà
    //      connue.
    // Contrat :
    //      Aucun

    //----------------------------------------------------- Attributs protégés

    // Unordered map associant un nom de page à un indice numérique
    unordered_map<string, int> page_indices;

    // Vecteur permettant de trouver le nom d'une page à partir d'un indice entier
    // (qui correspond à la position dans le vecteur)
    vector<string *> page_reverse;

    // Vecteur permettant de trouver le dictionnaire des hits d'une page
    // à partir de l'indice de celle-ci.
    // Principe : hits[indice cible][indice referrer] = nombre hits referrer -> cible
    vector<unordered_map<int, int>> hits;

    const char *nomfichier;
    const char *urlbase;
    bool exclureMedias;
    char heure;
    bool noAnsi;
};

#endif // ANALYZER_H
