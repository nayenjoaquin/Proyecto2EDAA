#include "fmIndex.cpp"
#include <bits/stdc++.h>
#include <chrono>
#include <math.h>

using namespace std;

//implementacion geeks for geeks suffix array
// la unica diferencia en comparacion a la implemetacion hecha en geeks for geeks es que la
//funcion search cuenta la cantidad de veces que encuentra la palabra en vez de devolver si se encuentra 
//en el texto

//estructura de sufijo la cual almacena el indice de inicio y 
//el sufijo contenido 
struct suffix
{
    int index;
    char *suff;
};
  
// Acomparador que se ultilizara para comparar los sufijos,
// este comparador se usa cuando debemos buscar si el sufijo esta contenido, este nos ayuda para hacer una busqueda binaria
int cmp(struct suffix a, struct suffix b)
{
    return strcmp(a.suff, b.suff) < 0? 1 : 0;
}
  
//con esta funcion construimos el suffix array
int *buildSuffixArray(string texto, int n)
{
    char txt[texto.length()];
    strcpy(txt, texto.c_str());


    struct suffix suffixes[n];
  
    //con este for llenamos el arreglo sufijos, con lo cual guardaremos todos los sufijos, 
    //y guardamos las cadenas para porder ordenarlas en orden alfabetico
    for (int i = 0; i < n; i++)
    {
        suffixes[i].index = i;
        suffixes[i].suff = (txt+i);
    }
  
    // ordenamos el arreglo
    sort(suffixes, suffixes+n, cmp);
  
    // guardamos los indices del arreglo
    int *suffixArr = new int[n];
    for (int i = 0; i < n; i++)
        suffixArr[i] = suffixes[i].index;
  
    return  suffixArr;
}


int countSA(string patron, string texto, int *suffArr, int n)
{
    char pat[patron.length()];
    strcpy(pat, patron.c_str());

    char txt[texto.length()];
    strcpy(txt, texto.c_str());

    int m = strlen(pat);
    int rep =0 , middle, comp;
    int l = 0, r = n-1;
    bool found = false;
    //con este bucle buscamos si el patron se encuentra en texto
    //esto solo nos encuentra una de las repeticiones
    while (l <= r)
    {
        
        middle = l + (r - l)/2;
        comp = strncmp(pat, txt+suffArr[middle], m);

        if (comp == 0)
        {
            rep++;
            found = true;
            break;
        }
        if (comp < 0){
            r = middle - 1;
        }
        else{
        l = middle + 1;
        }
    }
    //con este bucle buscamos todas las repeticiones del patron que se encuentren
    //a la izquierda de la posicion inicial en la que lo encontramos en el arreglo de sufijos
    int left =  middle - 1;
    while (found)
    {
        if(strncmp(pat, txt+suffArr[left], m) == 0){
            left--;
            rep++;
        }else{
            break;
        }
    }
    //con este bucle buscamos todas las repeticiones del patron que se encuentren
    //a la derecha de la posicion inicial en la que lo encontramos en el arreglo de sufijos
    int right = middle +1;
    while (found)
    {
        if(strncmp(pat, txt+suffArr[right], m) == 0){
            right++;
            rep++;
        }else{
            break;
        }
    }
    return rep;
}

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
    int *suffArr = buildSuffixArray((char*)text.c_str(), text.size());


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
        countSA(pattern, text, suffArr, text.size());
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
    int suffixArraySize = sizeof(suffArr);


    cout << textSize << "," << fmIndexTime << "," << suffixArrayTime << "," << kpmTime << "," << fmIndexSize << "," << suffixArraySize << "," << fmIndexVar << "," << suffixArrayVar << "," << kpmVar << endl;


    return 0;
}