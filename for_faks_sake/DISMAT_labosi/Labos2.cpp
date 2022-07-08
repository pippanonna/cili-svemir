#include <iostream>
#include <math.h>
#include <algorithm>

using namespace std;

int pohlepniAlgoritam(int *polje, int n, int poc) {
    bool *beenThere = new bool[n];
    // cout << poc;
    for (int i = 0; i < n; i++) {
        beenThere[i] = false;
    }
    int poz = poc, brojac = n - 1;
    int put = 0;
    beenThere[poc] = true;
    while (brojac != 0) {
        int min = 0, current;
        for (int i = 0; i < n; i++) {
            if (min == 0 && !beenThere[i]) {
                min = polje[poz * n + i];
                current = i;
            } else if (polje[poz * n + i] < min && polje[poz * n + i] != 0
            && !beenThere[i]) {
                current = i;
                min = polje[poz * n + i];
            }
        }
        // cout << "( " << min << " )"; 
        put += min;
        beenThere[current] = true;
        poz = current;
        // cout << " - " << poz;

        brojac--;
    }
    put += polje[poz * n + poc];
    // cout << "( " << polje[poz * n + poc] << " )"; 
    // cout << " - " << poc << endl;

    delete beenThere;
    return put;
}

int iscrpnaPretraga(int *polje, int n, int poc) {
    int *permutations = new int[n];
    permutations[0] = poc;
    int j = 1;
    for (int i = 0; i < n; i++) {
        if (i != poc) {
            permutations[j] = i;
            j++;
        }
    }
    std::sort(permutations + 1, permutations + n);
    int min = 0;
    do {
        int i;
        int current = 0;
        
        for (i = 0; i < n - 1; i++) {
            current += polje[permutations[i] * n + permutations[i + 1]];
            
        }
        current += polje[permutations[i] * n + permutations[0]];

        if (min == 0 || current < min) {
            min = current;
        } 
    } while (std::next_permutation(permutations + 1, permutations + n));

    delete permutations;
    return min;
}


int main(void) {
    size_t n;
    int a, b;
    cout << "Unesite redom, odvojene razmakom, parametre n, a i b: ";
    cin >> n >> a >> b;
    // cout << n << " " << a << " " << b << endl;
    

    int *weight = new int[n * n]; 
    int redak, stupac;
    int min = 0;

    for (int k = 1; k <= n; k++) {
        for (int l = k; l <= n; l++) {
            if (k == l) { weight[(k - 1) * n + (l - 1)] = 0; }
            else {
                weight[(k - 1) * n + (l - 1)] =  weight[(l - 1) * n + (k - 1)] =
                std::pow( std::abs(k - l), 3) + 1;
                if (min == 0 || min >= weight[(k - 1) * n + (l - 1)]) {
                    min = weight[(k - 1) * n + (l - 1)];
                    redak = k - 1;
                    stupac = l - 1;
                    /* cout << "Redak: " << redak << " Stupac: " << stupac
                    << " Min: " << min << endl; */
                }
            }
        }
    }


    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cout << weight[i * n + j] << " ";
        }
        cout << endl;
    }
    int rez_pohl = 0;
    /* for (int k = 0; k < n; k++) {
        for (int l = k; l < n; l++) {
            if (weight[k * n + l] == min) {
                int temp = pohlepniAlgoritam(weight, n, k);
                if (rez_pohl == 0 || temp < rez_pohl) {
                    rez_pohl = temp;
                }
                temp = pohlepniAlgoritam(weight, n, l);
                if (rez_pohl == 0 || temp < rez_pohl) {
                    rez_pohl = temp;
                }
            }
        }
    } */

    rez_pohl = pohlepniAlgoritam(weight, n, redak);
    int temp = pohlepniAlgoritam(weight, n, stupac);
    if (temp < rez_pohl) { rez_pohl = temp; }

    cout << "Pohlepni algoritam nalazi ciklus duljine " <<
     rez_pohl << endl;
    
    int rez_iscrp = iscrpnaPretraga(weight, n, redak);
    cout << "Iscrpni algoritam nalazi ciklus duljine " << rez_iscrp << endl;
    
    cout << "Pohlepni algoritam na ovom grafu";
    if (rez_pohl != rez_iscrp ) { cout << " ne"; }
    cout << " daje optimalno rjesenje!" << endl;

    /* int polje[25] ={0, 2, 8, 4, 6,
             2, 0, 7, 3, 6,
             8, 7, 0, 9, 8,
             4, 3, 9, 0, 5,
             6, 6, 8, 5, 0};

    rez_redak = pohlepniAlgoritam(polje, 5, 1);
    cout << "Pohlepni algoritam nalazi ciklus duljine " << rez_redak << endl;
    
    rez_iscrp = iscrpnaPretraga(polje, 5, 1);
    cout << "Iscrpni algoritam nalazi ciklus duljine " << rez_iscrp << endl;
    
    cout << "Pohlepni algoritam na ovom grafu";
    if (rez_redak != rez_iscrp) { cout << " ne"; }
    cout << " daje optimalno rjesenje!"; */


    delete weight;
    return 0;
}