#include "fmIndex.cpp"
#include "suffixArray.cpp"
#include <bits/stdc++.h>
#include <chrono>
#include <math.h>

using namespace std;


// int parseLine(char* line){
//     int i = strlen(line);
//     const char* p = line;
//     while (*p <'0' || *p > '9') p++;
//     line[i-3] = '\0';
//     i = atoi(p);
//     return i;
// }

// int getValue(){ 
//     FILE* file = fopen("/proc/self/status", "r");
//     int result = -1;
//     char line[128];

//     while (fgets(line, 128, file) != NULL){
//         if (strncmp(line, "VmRSS:", 6) == 0){
//             result = parseLine(line);
//             break;
//         }
//     }
//     fclose(file);
//     return result;
// }

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



    FMIndex fmIndex(text);
    SuffixArray suffixArray(text);


    vector<float> fmIndexTimes;
    vector<float> suffixArrayTimes;
    vector<float> kpmTimes;

    for(int i=0; i<30;i++){
        
        // FMIndex
        auto start = chrono::high_resolution_clock::now();
        fmIndex.count(pattern);
        auto end = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
        fmIndexTimes.push_back(duration.count());

        // Suffix Array
        start = chrono::high_resolution_clock::now();
        suffixArray.count(pattern, text);
        end = chrono::high_resolution_clock::now();
        duration = chrono::duration_cast<chrono::microseconds>(end - start);
        suffixArrayTimes.push_back(duration.count());

        // KPM
        start = chrono::high_resolution_clock::now();
        KMPcount(text, pattern);
        end = chrono::high_resolution_clock::now();
        duration = chrono::duration_cast<chrono::microseconds>(end - start);
        kpmTimes.push_back(duration.count());

    }

    // Pasamos a ms
    for(int i=0;i<30;i++){
        suffixArrayTimes[i] /= 1000;
        fmIndexTimes[i] /= 1000;
        kpmTimes[i] /= 1000;
    }

    float fmIndexTime = 0;
    float suffixArrayTime = 0;
    float kpmTime = 0;

    for(int i=0; i<30;i++){
        fmIndexTime += fmIndexTimes[i];
        suffixArrayTime += suffixArrayTimes[i];
        kpmTime += kpmTimes[i];
    }

    //promedios
    fmIndexTime /= 30;
    suffixArrayTime /= 30;
    kpmTime /= 30;

    float suffixArrayVar = 0;
    float fmIndexVar = 0;
    float kpmVar = 0;

    for(int i=0;i<30;i++){
        suffixArrayVar += pow(suffixArrayTimes[i] - suffixArrayTime, 2);
        fmIndexVar += pow(fmIndexTimes[i] - fmIndexTime, 2);
        kpmVar += pow(kpmTimes[i] - kpmTime, 2);
    }


    int fmIndexSize = sizeof(fmIndex);
    int suffixArraySize = sizeof(suffixArray);


    cout << textSize << "," << fmIndexTime << "," << suffixArrayTime << "," << kpmTime << "," << fmIndexSize << "," << suffixArraySize << "," << fmIndexVar << "," << suffixArrayVar << "," << kpmVar << endl;


    return 0;
}