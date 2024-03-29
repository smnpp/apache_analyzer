/*************************************************************************
    Programme principal - Interprète les options entrées par l'utilisateur
    afin d'analyser un fichier .log pour générer un top 10 des pages les plus visitées,
    et accessoirement, un graphe de tous les hits au format GraphViz (.dot)
                             -------------------
    début                : 16/01/2024
    copyright            : (C) 2024 par Habba, Ifkirne, Jeanne, Perret, Pierre
    e-mail               : ___
*************************************************************************/

//---------------------------------------------------------------- INCLUDE

//-------------------------------------------------------- Include système
#include <cstring>
#include <fstream>
#include <iostream>

//------------------------------------------------------ Include personnel
#include "Analyzer.h"
#include "LogStream.h"

//------------------------------------------------------ FONCTIONS LOCALES
bool is_options(const char *arg) {
    return strlen(arg) == 2 && arg[0] == '-';
}

void assert_valid_arg(int argc, char *argv[], int i) {
    if (i == argc || is_options(argv[i])) {
        string s = string("Missing argument for \"") + argv[i - 1] + "\"";
        throw invalid_argument(s);
    }
}

void display_help() {
    cout <<
        R"(Analog [options] <log>
Utility to analyse .log files
Options:
    -h: Display this help message (run "Analog -h")
    -e: Exclude media files (jpeg, png, ...)
    -t <hour>: Only include logs from hour <hour> to <hour>+1 (not included)
    -g <graph>: Generate a dot graph file named <graph>
    -s <url>: Root url of the website (defaults to "intranet-if.insa-lyon.fr")
    -a: Disable ANSI escape codes outputs
Argument:
    <log>: Log file to analyse)";
}

//------------------------------------------------------ POINT D'ENTREE
int main(int argc, char *argv[])
// Mode d'emploi :
//      Interprète les options entrées par l'utilisateur
//      afin d'analyser un fichier .log pour générer un top 10 des pages les plus visitées,
//      et accessoirement, un graphe de tous les hits au format GraphViz (.dot)
// Contrat :
//      Aucun
{
    try {
        bool exclude = false;
        const char *graph = nullptr;
        char time = -1;
        const char *source = nullptr;
        bool noAnsi = false;
        const char *fichier = nullptr;

        if (argc == 1)  // Cas où l'utilisateur a seulement renseigné "analog-h"
        {
            display_help();
            return 0;
        }
        for (int i = 1; i < argc; i++) {
            if (is_options(argv[i])) {
                switch (argv[i][1]) {
                    case 'h':
                        if (argc != 2) {
                            throw invalid_argument("\"-h\" has to be the only argument");
                        }
                        display_help();
                        return 0;
                    case 'a':
                        noAnsi = true;
                        break;
                    case 'e':
                        exclude = true;
                        break;
                    case 'g':
                        assert_valid_arg(argc, argv, ++i);
                        graph = argv[i];
                        break;
                    case 't': {
                        assert_valid_arg(argc, argv, ++i);
                        time = stoi(argv[i]);

                        if (time < 0 || 24 <= time) {
                            time %= 24;
                            cerr << "Filter time is greater than 24 and was wrapped to " << int(time) << endl;
                        }
                        break;
                    }
                    case 's':
                        assert_valid_arg(argc, argv, ++i);
                        source = argv[i];
                        break;
                    default:
                        string s = string("Unknown option \"") + argv[i] + "\"";
                        throw invalid_argument(s);
                }
            } else if (i + 1 != argc) {
                throw invalid_argument("The log file has to be the last argument");
            } else {
                fichier = argv[i];
            }
        }
        if (fichier == nullptr) {
            throw invalid_argument("The log file argument was not provided");
        }

        LogStream stream(fichier, ios::in);
        if (!stream.good()) {
            throw invalid_argument("The log file does not exist or could not be read");
        }
        stream.close();
        Analyzer ana(fichier, source == nullptr ? "intranet-if.insa-lyon.fr" : source, exclude, time, noAnsi);

        if (graph != nullptr) {
            ana.Graphique(graph);
        }
        ana.AfficheTop10();
    } catch (const exception &e) {
        cerr << e.what() << '\n';
        cerr << "Run \"Analog -h\" for help" << std::endl;
        return -1;
        // throw;
    }
    return 0;
}