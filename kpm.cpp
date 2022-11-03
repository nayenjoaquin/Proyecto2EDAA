#include <iostream>
#include <bits/stdc++.h>
#include <string.h>
#include <chrono>
#include <math.h>

using namespace std;


void tableSuffixPrefix(string pattern, int* table){

    table[0] = 0;
    int largoSuffijo = 0;
    int i = 1;
    int largoPatron = pattern.length();
    while (i<largoPatron)
    {
        if(pattern[i] == pattern[largoSuffijo]){
            largoSuffijo++;
            table[i] = largoSuffijo;
            i++;
        }else{
            if(largoSuffijo != 0){

                largoSuffijo = table[largoSuffijo-1];

            }else{
                table[i] = 0;
                i++;
            }
        }
    }



}
int KMPcount(string text, const string pattern){

    int rep = 0;
    int m = pattern.length();
    int n = text.length();
    int i = 0; //indice texto
    int j = 0; //indice patron

    int table[m];
    tableSuffixPrefix(pattern, table);
    while ((n-i) >= (m-j))
    {   
        //si encontramos un caracter igual sumamos a i y j
        if(pattern[j] == text[i]){
            i++;
            j++;
            
        }
        // si j es igual a m significa que encontramos el patron
        // asi que sumamos al contador de repeticiones y llevamos a j
        // al ultimo caracter del sufijo que igual es prefijo
        if(j==m){

            rep++;
            j = table[j-1];
        
        }else if(i<n && pattern[j] != text[i]){ // si encontramos una diferencia y todavia nos encontramos en el string

            // si j es distinto de cero, o sea tenemos coincidencia llevamos a j al ultimo caracter del sufijo que igual es prefijo
            if(j!= 0){
                
                j = table[j-1];
            
            }else{ //si j es igual a 0 nos movemos a la siguiente posicion a comparar en el texto
                i++;
            }

        }

    }
    return rep;
}


int main(int argc, char const *argv[]) {

    string textFileName = argv[1];
    string pattern = argv[2];
    int textSize = atoi(argv[3]);

    ifstream textFile("data/" + textFileName);
    string text;
    string str;

    int count = 0;
    while(getline(textFile, str) && text.size() < textSize){
        text += str;
        count++;
    };

    text = text.substr(0, textSize);

    vector<float> times;

    for(int i = 0; i < 10; i++){
        auto start = chrono::high_resolution_clock::now();
        int count = KMPcount(text, pattern);
        auto end = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
        times.push_back(duration.count());
    }

    float time = 0;
    for(int i = 0; i < times.size(); i++){
        time += times[i];
    }
    time /= 30;

    float var = 0;
    for(int i = 0; i < times.size(); i++){
        var += pow(times[i] - time, 2);
    }

    cout << textSize << "," << time << "," << var << endl;

    return 0;
}