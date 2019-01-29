#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>

#include "Convolution.hpp"
#include "NonLinearActivation.hpp"
#include "SubSampling.hpp"
#include "VectorProbabilities.hpp"

using namespace std;

void printHelp(int f) {
    ifstream in("./README.md");
    if (!in) {
        cout << "README.md missing.\n";
        return;
    }

    int a, b;
    switch(f) {
        case 1:
                a = 1;
                b = 19;
                break;
        case 2:
                a = 21;
                b = 31;
                break;
        case 3:
                a = 33;
                b = 47;
                break;
        case 4:
                a = 49;
                b = 59;
                break;
        default:
                a = 1;
                b = 59;
                break;
    }

    string l;
    for (int i = 0; i < a - 1; i++) {
        getline(in, l);
    }
    for (int i = a - 1; i < b; i++) {
        getline(in, l);
        cout << l << "\n";
    }

    exit(EXIT_FAILURE);
}

void checkFile(char* path, char* rows, char* cols) {
    ifstream file (path);
    int r = atoi(rows);
    int c = atoi(cols);

    if (!file) {
        cout << "Cannot open input file: " << path << "\n";;
        exit(EXIT_FAILURE);
    } else if (count(istreambuf_iterator<char> (file), istreambuf_iterator<char> (), '\n') != r*c) {
        cout << "Input file length wrong: " << path << "\n";
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char** argv) {
    if (argc <= 3) {
        printHelp(0);
    }

    string str = argv[1];
    if (str.find("convolution") != string::npos || str.find("matrixmult") != string::npos) {
        if (argc != 7) {
            cout << "Incorrect number Arguments, see format below.\n";
            printHelp(1);
        }
        if (strtol(argv[2], NULL, 10) == 0 && atoi(argv[2]) != 0) {
            cout << "Error in Argument 2, see format below.\n";
            printHelp(1);
        }
        if (strtol(argv[4], NULL, 10) == 0) {
            cout << "Error in Argument 4, see format below.\n";
            printHelp(1);
        }
        if (strtol(argv[6], NULL, 10) == 0) {
            cout << "Error in Argument 6, see format below.\n";
            printHelp(1);
        }

        if (strtol(argv[3], NULL, 10) != 0) {
            cout << "Error in Argument 3, see format below.\n";
            printHelp(1);
        } else {
            checkFile(argv[3], argv[4], argv[4]);
        }
        if (strtol(argv[5], NULL, 10) != 0) {
            cout << "Error in Argument 5, see format below.\n";
            printHelp(1);
        } else {
            checkFile(argv[5], argv[6], argv[6]);
        }

        return Convolution(argv);
    } else if (str.find("subsampling") != string::npos) {
        if (argc != 6) {
            cout << "Incorrect number Arguments, see format below.\n";
            printHelp(2);
        }
        if (str.find("maxpooling") == string::npos && str.find("avgpooling") == string::npos) {
            cout << "Error in Argument 1, see format below.\n";
            printHelp(2);
        }
        if (strtol(argv[3], NULL, 10) == 0) {
            cout << "Error in Argument 3, see format below.\n";
            printHelp(2);
        }
        if (strtol(argv[4], NULL, 10) == 0) {
            cout << "Error in Argument 4, see format below.\n";
            printHelp(2);
        }
        if (strtol(argv[5], NULL, 10) == 0) {
            cout << "Error in Argument 5, see format below.\n";
            printHelp(2);
        }

        if (strtol(argv[2], NULL, 10) != 0) {
            cout << "Error in Argument 2, see format below.\n";
            printHelp(2);
        } else {
            checkFile(argv[2], argv[3], argv[3]);
        }

        return SubSampling(argv);
    } else if (str.find("nonlinearactivation") != string::npos) {
        if (argc != 5) {
            cout << "Incorrect number Arguments, see format below.\n";
            printHelp(3);
        }
        if (str.find("relu") == string::npos && str.find("tanh") == string::npos) {
            cout << "Error in Argument 1, see format below.\n";
            printHelp(3);
        }
        if (strtol(argv[3], NULL, 10) == 0) {
            cout << "Error in Argument 3, see format below.\n";
            printHelp(3);
        }
        if (strtol(argv[4], NULL, 10) == 0) {
            cout << "Error in Argument 4, see format below.\n";
            printHelp(3);
        }

        if (strtol(argv[2], NULL, 10) != 0) {
            cout << "Error in Argument 2, see format below.\n";
            printHelp(3);
        } else {
            checkFile(argv[2], argv[3], argv[4]);
        }

        return NonLinearActivation(argv);
    } else if (str.find("vectorprobabilities") != string::npos) {
        if (argc != 4) {
            cout << "Incorrect number Arguments, see format below.\n";
            printHelp(4);
        }
        if (str.find("sigmoid") == string::npos && str.find("softmax") == string::npos) {
            cout << "Error in Argument 1, see format below.\n";
            printHelp(4);
        }
        if (strtol(argv[3], NULL, 10) == 0) {
            cout << "Error in Argument 3, see format below.\n";
            printHelp(4);
        }

        if (strtol(argv[2], NULL, 10) != 0) {
            cout << "Error in Argument 2, see format below.\n";
            printHelp(4);
        } else {
            checkFile(argv[2], argv[3], (char*) "1");
        }

        return VectorProbabilities(argv);
    } else {
        cout << "Unrecognized command, see format below.\n";
        printHelp(0);
    }

    return 0;
}
