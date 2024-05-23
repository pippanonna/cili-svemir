#include <iostream>
#include <string>
#include <vector>
#include <fstream>

using namespace std;
class Definicija {
    public:
        int brRetkaDef;
        string rijec;
        string nacinDef;
        Definicija(int brRetkaDef, string rijec);
        
};

Definicija::Definicija(int brRetkaDef, string rijec) {
    this->brRetkaDef = brRetkaDef;
    this->rijec = rijec;
    this->nacinDef = "jednako";
}

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

int trimLine(string &line) {
    int brUvlake = 0;
    int i = 0;
    for (char c : line) {
        if (isspace(c)) {
            brUvlake++;
        } else {
            line = line.substr(i, line.size() - i);
            break;
        } 
        i++;
    }
    return brUvlake;
}

int findPlace(vector<string> niz, string znak) {
    for (int i = 0; i < niz.size(); i++) {
        if (!znak.compare(niz[i])) {
            return i;
        }
    }
    return -1;
}

vector<string> izlaz;

vector<string> zavZnakovi{"IDN", "BROJ", "OP_PRIDRUZI", "OP_PLUS", "OP_MINUS", "OP_PUTA", "OP_DIJELI", "L_ZAGRADA", "D_ZAGRADA", "KR_ZA", "KR_DO", "KR_OD", "KR_AZ", "" };

vector<string> nezavZnakovi{"<program>", "<lista_naredbi>", "<naredba>", "<naredba_pridruzivanja>", "<za_petlja>", "<E>", "<E_lista>", "<T>", "<T_lista>", "<P>", "uvlaka", "greska"};

vector<Definicija> definicije;
int findDefinicija(Definicija def) {
    for (int i = definicije.size() - 1; i >= 0; i--) {
        if(!definicije[i].rijec.compare(def.rijec)) {
            return i;
        }
    }
    return -1;
}

int main(int argc,  char **argv) {
    vector<string> linije = readFileToVector(); // ulaz

    int brReda = 0;
    bool definirano = false, definiranoUZa = false;

    for (int i = 0; i < linije.size(); i++) {

        trimLine(linije[i]);
        
        string rijec;
        int red, brZnaka;

        if(linije[i].at( 0) == '<' || linije[i].at(0) == '$') {
            continue;
        }

        char delim = ' ';
        int start = 0, end = linije[i].find(delim);

        brZnaka = findPlace(zavZnakovi, linije[i].substr(start, end - start));

        start = end + 1;
        end = linije[i].find(delim, start);
        red = stoi(linije[i].substr(start, end - start));

        start = end + 1;
        end = linije[i].find(delim, start);
        rijec = linije[i].substr(start, end - start);

        if (red != brReda) {
            brReda = red;
            definirano = false;
        }
        Definicija def = Definicija(brReda, rijec);

        if(brZnaka == 9) { //za
            definiranoUZa = true;
            definirano = false;
        } else if (brZnaka == 12) { //az
            int br = definicije.size();
            for(int j = definicije.size() - 1; j >= 0; j--) {
                if(!definicije[j].nacinDef.compare("za")) {
                    br = j;
                    break;
                }
            }
            while(definicije.size() > br) {
                definicije.erase(definicije.begin() + br);

            }
        } else if(brZnaka == 0 && !definirano) {
            if(definiranoUZa) {
                
                definiranoUZa = false;
                def.nacinDef = "za";
                definicije.push_back(def);
            } else if (findDefinicija(def) == -1) {
                definicije.push_back(def);
            }

            definirano = true;
        } else if (brZnaka == 0) {
            int brDef = findDefinicija(def);
            if(brDef == -1) {
                cout << "err " << red << " " << rijec << endl;
                i = linije.size();
            } else if (definicije[brDef].brRetkaDef == red) {
                cout << "err " << red << " " << rijec << endl;
                i = linije.size();
            } else {
                cout << red << " " << definicije[brDef].brRetkaDef << " " << rijec << endl;
            }
        }

    }
}


