#include <iostream>
#include <string>
#include <vector>
#include <fstream>

using namespace std;

void program(vector<string> *stog, int brUnifZnak);
void listaNaredbi(vector<string> *stog, int brUnifZnak);
void naredba(vector<string> *stog, int brUnifZnak);
void naredbaPridruzivanja(vector<string> *stog, int brUnifZnak);
void zaPetlja(vector<string> *stog, int brUnifZnak);
void E(vector<string> *stog, int brUnifZnak);
void ELista(vector<string> *stog, int brUnifZnak);
void T(vector<string> *stog, int brUnifZnak);
void TLista(vector<string> *stog, int brUnifZnak);
void P(vector<string> *stog, int brUnifZnak);

vector<string> readFileToVector()
{
    vector<string> lines;
    string line;
    while (getline(cin, line))
    {
        lines.push_back(line);
    }
    return lines;
}

int findPlace(vector<string> niz, string znak) {
    for (int i = 0; i < niz.size(); i++) {
        if (!znak.compare(niz[i])) {
            return i;
        }
    }
    return -1;
}

vector<string> zavZnakovi{"IDN", "BROJ", "OP_PRIDRUZI", "OP_PLUS", "OP_MINUS", "OP_PUTA", "OP_DIJELI", "L_ZAGRADA", "D_ZAGRADA", "KR_ZA", "KR_DO", "KR_OD", "KR_AZ", "" };

vector<string> nezavZnakovi{"<program>", "<lista_naredbi>", "<naredba>", "<naredba_pridruzivanja>", "<za_petlja>", "<E>", "<E_lista>", "<T>", "<T_lista>", "<P>", "uvlaka"};

int uvlaka = 0;

int main(int argc,  char **argv) {
    vector<string> stog;
    stog.push_back(nezavZnakovi[0]);
    string line;

    vector<string> linije = readFileToVector();

    for (int i = 0; i < linije.size(); i++) {
        cout << "Linija " << i << ": " << linije[i] << endl; 
    }
    for(int i = 0; i < linije.size() + 1; i++) {
        //cout << linije[i] << "------";

        int brZnaka = 13;
        
        string znak, rijec;
        int red;
        if (i != linije.size()) {
        char delim = ' ';
        int start = 0, end = linije[i].find(delim);

        znak = linije[i].substr(start, end - start);

        start = end + 1;
        end = linije[i].find(delim, start);
        red = stoi(linije[i].substr(start, end - start));

        start = end + 1;
        end = linije[i].find(delim, start);
        rijec = linije[i].substr(start, end - start);

        //cout << znak << " " << red << " " << rijec << endl;

        brZnaka = findPlace(zavZnakovi, znak);
        }

        string currVrh = stog.back();
        stog.pop_back();

        int brFunkcije = findPlace(nezavZnakovi, currVrh);
        if (brFunkcije == 10) {
            uvlaka--;
            i--;
        } else if (brFunkcije >= 0) {
            for (int j = 0; j < uvlaka; j++) {
                cout << " ";
            }
            cout << currVrh << endl;
            i--;
            switch (brFunkcije)
            {
            case 0:
                program(&stog, brZnaka);
                break;
            case 1:
                listaNaredbi(&stog, brZnaka);
                break;
            case 2:
                naredba(&stog, brZnaka);
                break;
            case 3:
                naredbaPridruzivanja(&stog, brZnaka);
                break;
            case 4:
                zaPetlja(&stog, brZnaka);
                break;
            case 5:
                E(&stog, brZnaka);
                break;
            case 6:
                ELista(&stog, brZnaka);
                break;
            case 7:
                T(&stog, brZnaka);
                break;
            case 8:
                TLista(&stog, brZnaka);
                break;
            case 9:
                P(&stog, brZnaka);
                break;
            default:
                break;
            }
        } else {
           if (brZnaka != 13 && findPlace(zavZnakovi, currVrh) == brZnaka) {
            for (int j = 0; j < uvlaka; j++) {
                cout << " ";
            }
            cout << znak << " " << red << " " << rijec << endl;
           } else if (brZnaka != 13) {
            cout << "err";
           }
        }



    }
    cout << endl;

}

void program(vector<string> *stog, int brUnifZnak) {
    (*stog).push_back("uvlaka");
    uvlaka++;
    if(brUnifZnak == 0 || brUnifZnak == 9 || brUnifZnak == 13) {
        (*stog).push_back("<lista_naredbi>");
    } else {
        cout << "err" << endl; 
    }
};

void listaNaredbi(vector<string> *stog, int brUnifZnak) {
    (*stog).push_back("uvlaka");
    uvlaka++;
    if (brUnifZnak == 0 || brUnifZnak == 9) {
        (*stog).push_back("<lista_naredbi>");
        (*stog).push_back("<naredba>");

    } else if (brUnifZnak == 12 || brUnifZnak == 13) {
        for (int j = 0; j < uvlaka; j++) {
            cout << " ";
        }
        cout << "$" << endl;
    }
};

void naredba(vector<string> *stog, int brUnifZnak) {
    (*stog).push_back("uvlaka");
    uvlaka++;
    if (brUnifZnak == 0) {
        (*stog).push_back("<naredba_pridruzivanja>");
    } else if (brUnifZnak == 9) {
        (*stog).push_back("<za_petlja>");
    }
};

void naredbaPridruzivanja(vector<string> *stog, int brUnifZnak) {
    (*stog).push_back("uvlaka");
    uvlaka++;
    if (brUnifZnak == 0) {
        (*stog).push_back("<E>");
        (*stog).push_back("OP_PRIDRUZI");
        (*stog).push_back("IDN");
    }
};

void zaPetlja(vector<string> *stog, int brUnifZnak) {
    (*stog).push_back("uvlaka");
    uvlaka++;
    if (brUnifZnak == 9) {
        (*stog).push_back("KR_AZ");
        (*stog).push_back("<lista_naredbi>");
        (*stog).push_back("<E>");
        (*stog).push_back("KR_DO");
        (*stog).push_back("<E>");
        (*stog).push_back("KR_OD");
        (*stog).push_back("IDN");
        (*stog).push_back("KR_ZA");
    }
};

void E(vector<string> *stog, int brUnifZnak) {
    (*stog).push_back("uvlaka");
    uvlaka++;
    if(brUnifZnak == 0 || brUnifZnak == 1 || brUnifZnak == 3 ||brUnifZnak == 4 || brUnifZnak == 7) {
        (*stog).push_back("<E_lista>");
        (*stog).push_back("<T>");
    }
};

void ELista(vector<string> *stog, int brUnifZnak) {
    (*stog).push_back("uvlaka");
    uvlaka++;
    if (brUnifZnak == 3) {
        (*stog).push_back("<E>");
        (*stog).push_back("OP_PLUS");
    } else if(brUnifZnak == 4) {
        (*stog).push_back("<E>");
        (*stog).push_back("OP_MINUS");
    } else if (brUnifZnak == 0 || brUnifZnak == 9 || brUnifZnak == 10 || brUnifZnak == 12 || brUnifZnak == 8 || brUnifZnak == 13) {
        for (int j = 0; j < uvlaka; j++) {
            cout << " ";
        }
        cout << "$" << endl;
    }
};

void T(vector<string> *stog, int brUnifZnak) {
    (*stog).push_back("uvlaka");
    uvlaka++;
    if (brUnifZnak == 0 || brUnifZnak == 1 || brUnifZnak == 3 || brUnifZnak == 4 || brUnifZnak == 7) {
        (*stog).push_back("<T_lista>");
        (*stog).push_back("<P>");
    }
};

void TLista(vector<string> *stog, int brUnifZnak) {
    (*stog).push_back("uvlaka");
    uvlaka++;
    if (brUnifZnak == 5) {
        (*stog).push_back("<T>");
        (*stog).push_back("OP_PUTA");
    } else if (brUnifZnak == 6) {
        (*stog).push_back("<T>");
        (*stog).push_back("OP_DIJELI");
    } else if (brUnifZnak == 0 || brUnifZnak == 9 || brUnifZnak == 10 || brUnifZnak == 12 || brUnifZnak == 3 || brUnifZnak == 4 || brUnifZnak == 8 || brUnifZnak == 13) {
        for (int j = 0; j < uvlaka; j++) {
            cout << " ";
        }
        cout << "$" << endl;
    }
    
};

void P(vector<string> *stog, int brUnifZnak) {
    (*stog).push_back("uvlaka");
    uvlaka++;
    if (brUnifZnak == 3) {
        (*stog).push_back("<P>");
        (*stog).push_back("OP_PLUS");
    } else if (brUnifZnak == 4) {
        (*stog).push_back("<P>");
        (*stog).push_back("OP_MINUS");
    } else if (brUnifZnak == 7) {
        (*stog).push_back("D_ZAGRADA");
        (*stog).push_back("<E>");
        (*stog).push_back("L_ZAGRADA");
    } else if (brUnifZnak == 0) {
        (*stog).push_back("IDN");
    } else if (brUnifZnak == 1) {
        (*stog).push_back("BROJ");

    }
};