#include <iostream>
#include <bits/stdc++.h>
#include <string.h>
#include <chrono>
#include <math.h>

using namespace std;

//funcion para obtener uso de memoria sacada de stack overflow
int parseLine(char* line){
    int i = strlen(line);
    const char* p = line;
    while (*p <'0' || *p > '9') p++;
    line[i-3] = '\0';
    i = atoi(p);
    return i;
}

int getValue(){ 
    FILE* file = fopen("/proc/self/status", "r");
    int result = -1;
    char line[128];

    while (fgets(line, 128, file) != NULL){
        if (strncmp(line, "VmRSS:", 6) == 0){
            result = parseLine(line);
            break;
        }
    }
    fclose(file);
    return result;
}


//implementacion geeks for geeks suffix array
// la unica diferencia en comparacion a la implemetacion hecha en geeks for geeks es que la
//funcion search cuenta la cantidad de veces que encuentra la palabra en vez de devolver si se encuentra 
//en el texto

//estructura de sufijo la cual almacena el indice de inicio y 
//el sufijo contenido 
// struct suffix
// {
//     int index;
//     char *suff;
// };
  
// // Acomparador que se ultilizara para comparar los sufijos,
// // este comparador se usa cuando debemos buscar si el sufijo esta contenido, este nos ayuda para hacer una busqueda binaria
// int cmp(struct suffix a, struct suffix b)
// {
//     return strcmp(a.suff, b.suff) < 0? 1 : 0;
// }
  
// //con esta funcion construimos el suffix array
// int *buildSuffixArray(char *txt, const int n)
// {
//     struct suffix suffixes[n];
  
//     //con este for llenamos el arreglo sufijos, con lo cual guardaremos todos los sufijos, 
//     //y guardamos las cadenas para porder ordenarlas en orden alfabetico
//     for (int i = 0; i < n; i++)
//     {
//         suffixes[i].index = i;
//         suffixes[i].suff = (txt+i);
//     }
  
//     // ordenamos el arreglo
//     sort(suffixes, suffixes+n, cmp);
  
//     // guardamos los indices del arreglo
//     int *suffixArr = new int[n];
//     for (int i = 0; i < n; i++)
//         suffixArr[i] = suffixes[i].index;
  
//     return  suffixArr;
// }


// int countSA(char *pat, char *txt, int *suffArr, int n)
// {
//     int m = strlen(pat);
//     int rep =0 , middle, comp;
//     int l = 0, r = n-1;
//     bool found = false;
//     //con este bucle buscamos si el patron se encuentra en texto
//     //esto solo nos encuentra una de las repeticiones
//     while (l <= r)
//     {
        
//         middle = l + (r - l)/2;
//         comp = strncmp(pat, txt+suffArr[middle], m);

//         if (comp == 0)
//         {
//             rep++;
//             found = true;
//             break;
//         }
//         if (comp < 0){
//             r = middle - 1;
//         }
//         else{
//         l = middle + 1;
//         }
//     }
//     //con este bucle buscamos todas las repeticiones del patron que se encuentren
//     //a la izquierda de la posicion inicial en la que lo encontramos en el arreglo de sufijos
//     int left =  middle - 1;
//     while (found)
//     {
//         if(strncmp(pat, txt+suffArr[left], m) == 0){
//             left--;
//             rep++;
//         }else{
//             break;
//         }
//     }
//     //con este bucle buscamos todas las repeticiones del patron que se encuentren
//     //a la derecha de la posicion inicial en la que lo encontramos en el arreglo de sufijos
//     int right = middle +1;
//     while (found)
//     {
//         if(strncmp(pat, txt+suffArr[right], m) == 0){
//             right++;
//             rep++;
//         }else{
//             break;
//         }
//     }
//     return rep;
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



// int main(){
//     string text;
//     getline(cin,text);
//     string Pat = "FL";
    
//     int rep = KMPcount(text,Pat);
//     cout<< rep<< endl;


//     /*
//     auto start = chrono::high_resolution_clock::now();
//     auto end = chrono::high_resolution_clock::now();
//     chrono::duration<double> diff = end - start;


//     int *suffArr = buildSuffixArray(txt,text.length());
//     int kb_usage = getValue();
//     int rep;
//     vector<double> resultSufArr;
//     for (int i = 0; i < 30; i++)
//     {
//         start =chrono::high_resolution_clock::now();
//         rep = search(pat,txt,suffArr,text.length());
//         end = chrono::high_resolution_clock::now();
//         diff = end-start;
//         resultSufArr.push_back(diff.count());

        
//     }
//     cout<< rep<< endl;
//     double  promedioSufArr = 0,varianzaSufArr = 0;

//     //resultados Suffix array
//     for(vector<double>::iterator it = resultSufArr.begin();it != resultSufArr.end();it++){
//         promedioSufArr += *it;
//     }
//     promedioSufArr /= resultSufArr.size();
//     for(vector<double>::iterator it = resultSufArr.begin();it != resultSufArr.end();it++){
//         varianzaSufArr += pow((*it-promedioSufArr), 2.0); 
//     }
//     cout<<Pat<< ","<< promedioSufArr<< ","<< varianzaSufArr<<","<<kb_usage<< endl;
//     */
//     return 0;

// }