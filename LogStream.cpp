/*************************************************************************
            LogStream  -  Manipule un fichier .log pour transformer ses lignes en objets spécialisés
            'Log' permettant l'analyse statistique des logs de ce fichier.
                             -------------------
    début                : 16/01/2024
    copyright            : (C) 2024 par Habba, Ifkirne, Jeanne, Perret, Pierre
    e-mail               : ___
*************************************************************************/

//---------- Réalisation de la classe <LogStream> (fichier LogStream.cpp) ------------

//---------------------------------------------------------------- INCLUDE

//-------------------------------------------------------- Include système
#include <iostream>
#include <regex>

//------------------------------------------------------ Include personnel
#include "LogStream.h"

//----------------------------------------------------------------- PUBLIC

//----------------------------------------------------- Méthodes publiques
bool LogStream::GetLog(Log &log)
// Algorithme :
//     On utilise Regex Search pour détecter puis extraire les infos de la ligne du fichier .log
{
    string s;
    if (!std::getline(*this, s)) // Cas d'une ligne vide
    {
        return false;
    }

    smatch m;
    regex e("(\\d+\\.\\d+\\.\\d+\\.\\d+) (\\\".*?\\\"|-) (\".*?\"|-) \\[(\\d+)\\/(\\S*?)\\/(\\d+)\\:(\\d+)\\:(\\d+)\\:(\\d+) (.)(\\d+)\\] \"(.*?) (.*?) HTTP.*?\" (\\d+) (\\d+|-) \"(.*?)\" \"(.*?)\"");
    regex_search(s, m, e); // Les résultats de la recherche se trouvent dans 'm'

    struct Log newLog;

    newLog.IP = string(m[1]);
    newLog.userLogname = string(m[2]);
    newLog.authenticatedUser = string(m[3]);
    newLog.date.day = stoi(string(m[4]));
    string month = string(m[5]);

    for (unsigned int i = 0; i < 3; i++) // Stockage des 3 lettres du mois dans un tableau de char à 4 cases
    {
        newLog.date.month[i] = month[i];
    }
    newLog.date.month[3] = '\0';

    newLog.date.year = short(stoi(string(m[6])));
    newLog.date.hour = stoi(string(m[7]));
    newLog.date.minutes = stoi(string(m[8]));
    newLog.date.seconds = stoi(string(m[9]));

    if (string(m[10]) == "+") // Stockage du décalage horaire en tant qu'entier
    {
        newLog.date.UTC = stoi(string(m[11]));
    }
    else
    {
        newLog.date.UTC = -1 * (stoi(string(m[11])));
    }

    newLog.actionType = string(m[12]);
    newLog.targetPage = string(m[13]);
    newLog.responseCode = stoi(string(m[14]));
    newLog.dataAmount = m[15] == "-" ? 0 : stoi(string(m[15])); // Le symbole '-' implique qu'aucune donnée n'est échangée
    newLog.sourcePage = string(m[16]);
    newLog.clientId = string(m[17]);

    log = newLog;

    return true;
}

//-------------------------------------------- Constructeurs - destructeur
LogStream::LogStream(const char *nom, ios::openmode mode) : ifstream(nom, mode)
// Algorithme :
//      Aucun
{
#ifdef MAP
    cout << "Appel au constructeur de <LogStream>" << endl;
#endif
}

LogStream::~LogStream()
{
#ifdef MAP
    cout << "Appel au destructeur de <LogStream>" << endl;
#endif
}