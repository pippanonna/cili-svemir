#include <iostream>
#include <fstream>
#include <cstring>
#include <string>

using namespace std;

bool provjeri(bool *susjedi, int *boje, int n, int currVrh, int currBoja)
{
    for (int i = 0; i < n; i++)       // prodi sve susjede vrha currVrh i pogledaj ako su vec obojeni tom bojom
        if (susjedi[currVrh * n + i]) // postoji brid
            if (currBoja == boje[i])
                return false;
    return true;
}

bool traziKromBroj(bool *susjedi, int *boje, int n, int m, int currVrh)
{

    if (currVrh == n) // ako smo prosli sve vrhove(0 do n - 1) nasli smo dobro obojen graf
    {
        return true;
    }
    for (int b = 1; b <= m; b++)
    {

        if (provjeri(susjedi, boje, n, currVrh, b)) // moze li se graf obojati tom bojom?
        {
            boje[currVrh] = b;

            if (traziKromBroj(susjedi, boje, n, m, currVrh + 1))
            {                // idi na sljedecu vrh i probaj ga obojati
                return true; // ako je uspjelo vraca true u n rekurzivnih poziva
            }

            boje[currVrh] = 0; // ako nije uspjelo resetiraj vrh i probaj s drugom bojom
        }
    }

    return false; // ako neki vrh nismo uspjeli obojati s nijednom od m boja, onda se graf ne moze obojati s m boja
}

int kromatskiBroj(bool *susjedi, int *boje, int n)
{

    int broj = 1; // kromatski broj
    while (traziKromBroj(susjedi, boje, n, broj, 0) == false)
    {
        broj++;
    }

    return broj;
}

int main(void)
{

    ifstream myfile;
    string s;

    cout << "Upisi ime datoteke: "; // otvaranje datoteke
    cin >> s;
    myfile.open(s);

    int n, elem;

    getline(myfile, s); // broj vrhova grafa
    n = stoi(s);
    getline(myfile, s); // broj elemenata skupa S
    elem = stoi(s);

    int *skup = new int[elem];

    getline(myfile, s); // upisivanje elemenata skupa S
    string delim = " ";
    for (int i = 0; i < elem; i++)
    {
        size_t pos = s.find(delim);
        if (pos != string::npos)
        {
            skup[i] = stoi(s.substr(0, pos));
        }
        else
        {
            skup[i] = stoi(s);
        }
        s.erase(0, pos + 1);
    }

    myfile.close();

    bool *susjedi = new bool[n * n];

    for (int k = 0; k < n; k++) // matrica susjedstva
    {
        for (int l = k; l < n; l++)
        {
            susjedi[n * k + l] = susjedi[n * l + k] = false;
            if (l != k)
            {
                susjedi[n * k + l] = susjedi[n * l + k] = false;
                for (int i = 0; i < elem; i++)
                {
                    if (l - k == skup[i])
                    {
                        susjedi[n * k + l] = susjedi[n * l + k] = true;
                    }
                }
            }
        }
    }

    int *boje = new int[n];
    for (int i = 0; i < n; i++)
    {
        boje[i] = 0;
    }

    int brBoja = kromatskiBroj(susjedi, boje, n);

    cout << "Broj boja kojima se moze obojat graf: " << brBoja << endl;

    delete skup;
    delete susjedi;
    delete boje;
    return 0;
}