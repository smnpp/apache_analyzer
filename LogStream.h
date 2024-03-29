/*************************************************************************
            LogStream  -  Manipule un fichier .log pour transformer ses lignes en objets spécialisés
            'Log' permettant l'analyse statistique des logs de ce fichier.
                             -------------------
    début                : 16/01/2024
    copyright            : (C) 2024 par Habba, Ifkirne, Jeanne, Perret, Pierre
    e-mail               : ___
*************************************************************************/

//---------- Interface de la classe LogStream (fichier LogStream.h) ----------------
#if !defined(LOGSTREAM_H)
#define LOGSTREAM_H

//--------------------------------------------------- Interfaces utilisées
using namespace std;
#include <fstream>
#include <string>

//------------------------------------------------------------------ Types
struct Date {
    short year;
    char month[4];
    unsigned char day;
    unsigned char hour;
    unsigned char minutes;
    unsigned char seconds;
    short UTC;
};

struct Log  // Un objet 'Log' rassemble les caractéristiques d'une ligne d'un fichier .log
{           // en un objet facilement manipulable à des fins d'analyse statistique.
    string IP;
    string userLogname;
    string authenticatedUser;
    struct Date date;
    string actionType;
    string targetPage;
    short responseCode;
    int dataAmount;
    string sourcePage;
    string clientId;
};

//------------------------------------------------------------------------
// Manipule un fichier .log pour transformer ses lignes en objets spécialisés
// 'Log' permettant l'analyse statistique des logs de ce fichier.
//------------------------------------------------------------------------
class LogStream : public ifstream {
    //----------------------------------------------------------------- PUBLIC
public:
    //----------------------------------------------------- Méthodes publiques
    bool GetLog(Log &log);
    // Mode d'emploi :
    //      Construit un objet Log à partir des informations contenues dans une
    //      seule ligne du fichier .log associé à l'objet LogStream. Renvoie
    //      'false' si la ligne considérée est vide, 'true' sinon.
    // Contrat :
    //      Le fichier .log associé à l'objet LogStream doit strictement respecter
    //      la syntaxe vue dans l'exemple du log Apache vu en TP.

    //-------------------------------------------- Constructeurs - destructeur
    LogStream(const char *nom, ios::openmode mode);
    // Mode d'emploi :
    //      Constructeur par défaut de la classe LogStream. Se comporte exactement
    //      de la même manière que le constructeur de la classe Ifstream.
    // Contrat :
    //      Aucun

    virtual ~LogStream();
};

#endif  // LOGSTREAM_H
