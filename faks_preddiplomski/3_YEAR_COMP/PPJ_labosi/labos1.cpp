#include <iostream>
#include <string>
#include <vector>
#include <fstream>

using namespace std;

// komentar pocinje sa dva / i zavrsava sa /n
// idn pocinje sa alpha i nastavlja s alpha ili num
// broj pocinje sa num ili -num i ima sam num
// ostali znakovi imaju definiran izgled
// treba ispisati i red u kojem se nalaze

string pronadiOp(char c);
void printRijec(string *rijec, string *znak, int red);
void procesirajRijec(string *rijec, string *znak, int red);

int main(void)
{

    vector<string> unifZnakovi{"IDN", "BROJ"};

    string rijec = "";
    string znak = "";
    int red = 1;
    bool komentar = 0;

    char c;
    while (cin.get(c))
    {
        // cout << c << " novi znak\n";

        if (komentar)
        {
            if (c == '\n')
            {
                komentar = 0;
                red++;
            }
        }
        else
        {
            if (isspace(c))
            {
                // cout << "Neki space\n";
                if (!rijec.empty())
                    procesirajRijec(&rijec, &znak, red);
                if (c == '\n')
                    red++;
            }
            else if (ispunct(c))
            {
                if (rijec.compare("") != 0)
                {
                    procesirajRijec(&rijec, &znak, red);
                }

                // cout << "Rijec prazna\n";
                if (c == '/' && cin.peek() == '/')
                {
                    // cout << "Komentar!!!\n";
                    komentar = 1;
                    cin.get(c);
                }
                else
                {
                    rijec += c;
                    znak = pronadiOp(c);
                    printRijec(&rijec, &znak, red);
                }
            }
            else if (isalnum(c))
            {
                if (rijec.empty())
                {
                    if (isalpha(c))
                    {
                        znak = unifZnakovi[0];
                    }
                    else if (isdigit(c))
                    {
                        znak = unifZnakovi[1];
                    }
                }
                else if (isalpha(c) && znak.compare(unifZnakovi[1]) == 0)
                {
                    procesirajRijec(&rijec, &znak, red);
                    znak = unifZnakovi[0];
                }
                rijec += c;
            }
        }
    }
}

string pronadiOp(char c)
{
    vector<string> unifZnakovi{"OP_PRIDRUZI", "OP_PLUS", "OP_MINUS", "OP_PUTA", "OP_DIJELI", "L_ZAGRADA", "D_ZAGRADA"};
    vector<char> punctZnakovi{'=', '+', '-', '*', '/', '(', ')'};

    for (long unsigned int i = 0; i < punctZnakovi.size(); i++)
    {
        if (c == punctZnakovi[i])
        {
            return unifZnakovi[i];
        }
    }
    return "Greska";
};

void printRijec(string *rijec, string *znak, int red)
{
    cout << *znak << " " << red << " " << *rijec << endl;
    *rijec = "";
    *znak = "";
};

void procesirajRijec(string *rijec, string *znak, int red)
{
    // cout << "Procesiram " << *rijec << endl;
    vector<string> unifZnakovi{"KR_ZA", "KR_OD", "KR_DO", "KR_AZ", "IDN", "BROJ"};
    vector<string> kljRijeci{"za", "od", "do", "az"};
    if ((*rijec).compare(unifZnakovi[5]) == 0)
    {
        printRijec(rijec, znak, red);
    }
    else
    {
        for (long unsigned int i = 0; i < kljRijeci.size(); i++)
        {
            if ((*rijec).compare(kljRijeci[i]) == 0)
            {
                *znak = unifZnakovi[i];
                break;
            }
        }
        printRijec(rijec, znak, red);
    }
}
