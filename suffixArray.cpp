#include <bits/stdc++.h>

using namespace std;

class SuffixArray{
private:

    int *suffixArray;

    struct suffix{
        int index;
        char *suff;
    };

    int cmp(struct suffix a, struct suffix b){
        return strcmp(a.suff, b.suff) < 0? 1 : 0;
    }

    int *buildSuffixArray(char *txt, int n){
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

    int count(char *pat, char *txt, int *suffArr, int n){
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

public:

    SuffixArray(string text){
        int n = text.length();
        char txt[n];
        strcpy(txt,text.c_str());

        suffixArray = buildSuffixArray(txt, n);
    }

    int count(string pattern, string text){
        int n = text.length();
        char txt[n];
        strcpy(txt,text.c_str());

        int m = pattern.length();
        char pat[m];
        strcpy(pat,pattern.c_str());


        return count(pat, txt, suffixArray, n);
    }

    
};