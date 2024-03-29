/*************************************************************************
    Analyzer  -  Analyse un ensemble d'objets 'Log' selon certains critères pour générer un top 10
    des pages les plus visitées, et accessoirement, un graphe de tous les hits au format GraphViz (.dot)
                             -------------------
    début                : 16/01/2024
    copyright            : (C) 2024 par Habba, Ifkirne, Jeanne, Perret, Pierre
    e-mail               : ___
*************************************************************************/

//---------- Réalisation de la classe <Analyzer> (fichier Analyzer.cpp) ------------

//---------------------------------------------------------------- INCLUDE

//-------------------------------------------------------- Include système
#include <iostream>
#include <string>
#include <cstring>
#include <unordered_map>
#include <vector>

//------------------------------------------------------ Include personnel
#include "LogStream.h"
#include "Analyzer.h"

//------------------------------------------------------ FONCTIONS LOCALES
bool starts_with(const string &s, const char *suffix, unsigned int offset = 0)
// Mode d'emploi :
//      Vérifie si une subdivision d'une chaîne de caractères commençant à un certain
//      offset commence par un certain suffixe
// Contrat :
//      Aucun
{
    unsigned int len = strlen(suffix);
    if (len + offset > s.size())
    {
        return false;
    }
    for (unsigned int i = 0; i < len; i++)
    {
        if (s[i + offset] != suffix[i])
        {
            return false;
        }
    }

    return true;
}

bool ends_with(const string &s, const char *suffix)
// Mode d'emploi :
//      Vérifie si une subdivision d'une chaîne de caractères commençant à un certain
//      offset se termien par un certain suffixe
// Contrat :
//      Aucun
{
    unsigned int len = strlen(suffix);
    if (len > s.size())
    {
        return false;
    }
    for (unsigned int i = 0; i < len; i++)
    {
        if (s[s.size() - i - 1] != suffix[len - i - 1])
        {
            return false;
        }
    }
    return true;
}

//----------------------------------------------------------------- PUBLIC

//----------------------------------------------------- Méthodes publiques
void Analyzer::Graphique(const char *fichierDot) const
// Algorithme :
//      Parcours séquentiel des collections  "page_indices" et "hits"
{
    cout << "Generating \"" << fichierDot << "...\"";
    if (noAnsi)
    {
        cout << endl;
    }
    else
    {
        cout << "(1/2, "
             << "\0337";
    }

    ofstream file(fichierDot);
    file << "digraph {" << endl;

    unsigned int nodes = page_indices.size(); // Ajouts des sommets
    unsigned int node = 0;
    for (auto paire : page_indices)
    {
        file << "\t"
             << "node" << paire.second << "[label=\"" << paire.first << "\"];" << endl;
        node++;
        if (!noAnsi)
        {
            cout << "\0338" << node << "/" << nodes << " nodes)";
        }
    }

    if (!noAnsi)
    {
        cout << "\033[2K\r"
             << "Generating \"" << fichierDot << "\"... (2/2, "
             << "\0337";
    }

    unsigned int links = 0; // Ajout des arêtes
    for (unsigned int i = 0; i < hits.size(); i++)
    {
        for (auto paire : hits[i])
        {
            file << "\t"
                 << "node" << paire.first << "->node" << i << "[label=\"" << paire.second << "\"];" << endl;
            links++;
            if (!noAnsi)
            {
                cout << "\0338" << links << ")";
            }
        }
    }
    file << "}";
    file.close();

    if (!noAnsi)
    {
        cout << "\033[2K\r";
    }
    cout << "Generated \"" << fichierDot << "\" (" << nodes << " nodes, " << links << " links)" << endl;
}

void Analyzer::AfficheTop10() const
// Algorithme :
//     Aucun.
{
    vector<pair<string, int>> topPages = TopN(10);

    cout << "Top 10 most visited pages of this website:" << endl;
    for (size_t i = 0; i < topPages.size(); i++)
    {
        cout << i + 1 << ": " << topPages[i].first << " (" << topPages[i].second << (topPages[i].second == 1 ? " entry" : " entries") << ")" << endl;
    }
}

vector<pair<string, int>> Analyzer::TopN(unsigned int n) const
// Algorithme :
//     Parcours séquentiel de la collection "hits" incluant, pour chaque cible d'indice i:
//     - le parcours séquentiel de chaque sous-collection située à hits[i]
//     pour calculer le nombre total de hits pour cette cible
//     - l'insertion de la cible et de son nombre de hits dans le vecteur à renvoyer
//     à la position adéquate
{
    cout << "Extracting top " << n << " urls...";
    if (noAnsi)
    {
        cout << endl;
    }
    else
    {
        cout << " ("
             << "\0337";
    }

    unsigned long urls = hits.size();
    vector<pair<string, int>> topPages;

    for (size_t i = 0; i < urls; i++)
    {
        const auto hit = hits[i];
        int total = 0;
        for (const auto &refered : hit)
        {
            total += refered.second;
        }

        if (total == 0)
        {
            continue;
        }

        bool inserted = false;
        for (unsigned int j = 0; j < topPages.size(); j++)
        {
            if (topPages[j].second < total)
            {
                topPages.emplace(topPages.begin() + j, *page_reverse[i], total);
                inserted = true;
                break;
            }
        }
        if (!inserted && topPages.size() < n)
        {
            topPages.emplace_back(*page_reverse[i], total);
        }

        if (topPages.size() > n)
        {
            topPages.pop_back();
        }

        if (!noAnsi)
        {
            cout << "\0338" << i + 1 << "/" << urls << "urls)";
        }
    }
    if (!noAnsi)
    {
        cout << "\033[2K\r";
    }
    cout << "Extracted top " << n << " urls (" << urls << " total)" << endl;

    return topPages;
}

//-------------------------------------------- Constructeurs - destructeur
Analyzer::Analyzer(const char *nomfichier, const char *optS, bool optE, char optT, bool optA)
    : nomfichier(nomfichier), urlbase(optS), exclureMedias(optE), heure(optT), noAnsi(optA)
// Algorithme :
//    - Parcours séquentiel des lignes du fichier .log et création des objets Log correspondants
//    grâce à la classe LogStream
//    - Filtrage des objets Log selon les options renseignées en paramètre
{
#ifdef MAP
    cout << "Appel au constructeur de <Analyzer>" << endl;
#endif

    cout << "Parsing \"" << nomfichier << "\"...";
    if (noAnsi)
    {
        cout << endl;
    }
    else
    {
        cout << " ("
             << "\0337";
    }

    unsigned int lines = 0;
    unsigned int ignored = 0;
    Log log;

    LogStream file(nomfichier, ios::in);
    while (file.GetLog(log))
    {
        if (starts_with(log.sourcePage, "http")) // Elimination de l'URL de base
        {
            int start = 4 + 3;
            if (starts_with(log.sourcePage, "s", start))
            {
                start++;
            }
            if (starts_with(log.sourcePage, urlbase, start))
            {
                log.sourcePage.erase(0, start + strlen(urlbase));
            }
        }

        // Filtrage selon les critères de temps et de type
        if ((optT != -1 && log.date.hour != optT) || (optE && (ends_with(log.targetPage, ".png") || ends_with(log.targetPage, ".jpg") || ends_with(log.targetPage, ".css") || ends_with(log.targetPage, ".js"))))
        {
            ignored++;
            if (!noAnsi)
            {
                cout << "\0338" << lines << " logs, " << ignored << " ignored)";
            }
            continue;
        }

        int cible = getOrAddPageNumber(log.targetPage);
        int referrer = getOrAddPageNumber(log.sourcePage);

        auto match = hits[cible].find(referrer);
        int old = match == hits[cible].end() ? 0 : match->second;
        (hits[cible])[referrer] = old + 1;

        lines++;
        if (!noAnsi)
        {
            cout << "\0338" << lines << " logs, " << ignored << " ignored)";
        }
    }

    if (!noAnsi)
    {
        cout << "\033[2K\r";
    }
    cout << "Parsed \"" << nomfichier << "\" (" << lines << " logs, " << ignored << " ignored)" << endl;
}

Analyzer::~Analyzer()
{
#ifdef MAP
    cout << "Appel au destructeur de <Analyzer>" << endl;
    // Ouvrir le flux
#endif
}

//------------------------------------------------------------------ PRIVE

//----------------------------------------------------- Méthodes protégées
int Analyzer::getOrAddPageNumber(string page)
{
    auto it = page_indices.find(page);
    if (it != page_indices.end())
    {
        return it->second;
    }
    // Si la page n'existe pas encore
    int number = page_indices.size();
    auto a = page_indices.emplace(page, number);
    page_reverse.emplace_back((string *)&(a.first->first));
    hits.emplace_back();
    return number;
}