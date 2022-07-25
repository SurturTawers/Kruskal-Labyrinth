#include <iostream>
#include <string>
#include <cmath>
#include <vector>
#include <algorithm>
using namespace std;
typedef struct Grafo{
    int V, A;
    vector<pair<int,pair<int,int>>> aristas;
};
typedef struct Subset{
    int padre;
    int rank;
};
int getParams(string input, int* params){
    int inputSize=input.length(), i=0, k=0;
    string aux;
    while(i<inputSize){ //Guardo uno a uno los parametros en el string aux
        while(input[i]!=' ' && i<inputSize){ // mientras no se encuentre un espacio y no sobrepase el tamaño del input
            //cout<<"input "<<input[i]<<endl;
            aux.push_back(input[i]);
            i++;
        }
        if(aux.size()!=0){ //si hay algún parametro
            params[k]= stoi(aux); //lo transformo de string a int y lo guardo en params
            aux.clear();
            k++;
        }
        i++;
    }
    if(k!=2){   //verifico que hayan 6 parámetros
        cout<<"Numero incorrecto de parametros: "<<k<<" de 6"<<endl;
        return 0;
    }
    for(int z=0;z<2;z++){ //verifico que sean valores positivos
        if(params[z]<=0){
            cout<<"Ingrese valores positivos mayores que cero -> "<<params[z]<<endl;
            return 0;
        }
    }
    return 1;
}
void printGrid(int** grid, int n, int m){
    cout<<"------------GRID------------\n"<<endl;
    for(int i=0;i<n+1;i++){
        for(int j=0;j<m+1;j++){
            if(j==m){
                cout<<"*"<<endl;
            }else{
                cout<<"*---------------";
            }
        }
        if(i!=n){
            for(int k=0;k<m+1;k++){
                if(i==0 && k==0){
                    cout<<" \t"<<grid[i][k]<<"\t";
                }else if(i==n-1 && k==m){
                    cout<<" "<<endl;
                }else if(k==m){
                    cout<<"|"<<endl;
                }else{
                    cout<<"|\t"<<grid[i][k]<<"\t";
                }
            }
        }
    }
}
void anadirArista(Grafo* graph, int u, int v, int w){
    graph->aristas.push_back({w,{u,v}});
}
Grafo* crearGrafo(int V, int A){
    Grafo* grafo;
    grafo=new Grafo;
    grafo->V=V;
    grafo->A=A;
    return grafo;
}
int Random(int a, int b){
    int p=(int)log2f((float)(b-a))+1;
    int r=0;
    for(int i=0;i<p;i++){
        r=2*r+rand()%2;
        if(r>b-a){
            r=0;
            i=0;
        }
    }
    return r+a;
}
void setAristas(Grafo* graph,int**grid, int n, int m){
    for(int i=0;i<n;i++){
        for(int j=0;j<m;j++){
            if(j==m-1 && i<n-1){
                anadirArista(graph,grid[i][j],grid[i+1][j],Random(1,m*n));
            }else if(i==n-1&& j<m-1){
                anadirArista(graph,grid[i][j],grid[i][j+1],Random(1,n*m));
            }else if(j==m-1&&i==n-1){
                break;
            }else{
                anadirArista(graph,grid[i][j],grid[i][j+1],Random(1,n*m));
                anadirArista(graph,grid[i][j],grid[i+1][j],Random(1,m*n));
            }
        }
    }
    vector<pair<int,pair<int,int>>>::iterator it;
    string input;
    int option;
    cout<<"\nMostrar aristas?\n[1]: Si\t[2]: No"<<endl;
    getline(cin,input);
    option=stoi(input);
    while(option!=1 && option!=2){
        cout<<"\nIngrese una opcion valida\nMostrar aristas?\n[1]: Si\t[2]: No"<<endl;
        getline(cin,input);
        option=stoi(input);
    }
    if(option==1){
        cout<<"\n------------ARISTAS------------"<<endl;
        for(it=graph->aristas.begin(); it!=graph->aristas.end();it++){
            cout<<it->second.first<<"\t-\t"<<it->second.second<<"\tPESO: "<<it->first<<endl;
        }
    }
}
void makeSet(Subset* subsets, int V){
    for(int i=0;i<V;i++){
        subsets[i].padre=i;
        subsets[i].rank=0;
    }
}
int Find(Subset* subsets, int i){
    if(subsets[i].padre!=i){
        subsets[i].padre= Find(subsets, subsets[i].padre);
    }
    return subsets[i].padre;
}
void Union(Subset* subsets, int x, int y){
    if(subsets[x].rank<subsets[y].rank){
        subsets[x].padre=y;
    }else if(subsets[x].rank>subsets[y].rank){
        subsets[y].padre=x;
    }else{
        subsets[y].padre = x;
        subsets[x].rank++;
    }
}
bool sortbysec(const pair<int,int> &a,const pair<int,int> &b){
    return (a.first < b.first);
}
void createLabyrinth(int** grid, Grafo* graph, Subset* subsets, int n, int m){
    int set_u, set_v;
    vector<pair<int,int>> used;
    vector<pair<int,int>>::iterator it1;
    vector<pair<int,pair<int,int>>>::iterator it;
    sort(graph->aristas.begin(),graph->aristas.end());
    for(it=graph->aristas.begin(); it!=graph->aristas.end();it++){
        int set_u=Find(subsets,it->second.first);
        int set_v=Find(subsets,it->second.second);
        if(set_u!=set_v){
            Union(subsets,set_u,set_v);
            used.push_back(it->second);
        }
    }
    int flag=1;
    sort(used.begin(),used.end(),sortbysec);
    cout<<"------------LABERINTO------------\n"<<endl;
    for(int i=0;i<n+1;i++){
        for(int j=0;j<m+1;j++){
            if(j==m){
                cout<<"*"<<endl;
            }else{
                if(j<m && i>0 && i<n){
                    for(it1=used.begin(); it1!=used.end();it1++){
                        if(grid[i-1][j]==it1->first && grid[i][j]==it1->second){
                            cout<<"*\t\t";
                            flag=0;
                        }
                    }
                    if(flag){
                        cout<<"*---------------";
                    }else{
                        flag=1;
                    }
                }else{
                    cout<<"*---------------";
                }
            }
        }
        if(i!=n){
            flag=1;
            for(int k=0;k<m+1;k++){
                if(i==0 && k==0){
                    cout<<" \t"<<grid[i][k]<<"\t";
                }else if(i==n-1 && k==m){
                    cout<<" "<<endl;
                }else if(k==m){
                    cout<<"|"<<endl;
                }else{
                    if(k>0 && k<m){
                        for(it1=used.begin(); it1!=used.end();it1++){
                            if(grid[i][k-1]==it1->first && grid[i][k]==it1->second){
                                cout<<" \t"<<grid[i][k]<<"\t";
                                flag=0;
                            }
                        }
                        if(flag){
                            cout<<"|\t"<<grid[i][k]<<"\t";
                        }else{
                            flag=1;
                        }
                    }else{
                        cout<<"|\t"<<grid[i][k]<<"\t";
                    }
                }
            }
        }
    }
    string input;
    int option;
    cout<<"\nMostrar aristas?\n[1]: Si\t[2]: No"<<endl;
    getline(cin,input);
    option=stoi(input);
    while(option!=1 && option!=2){
        cout<<"\nIngrese una opcion valida\nMostrar aristas?\n[1]: Si\t[2]: No"<<endl;
        getline(cin,input);
        option=stoi(input);
    }
    if(option==1) {
        cout << "\n------------ARISTAS USADAS------------" << endl;
        for (it1=used.begin();it1!=used.end();it1++) {
            cout<<it1->first<<"\t-\t"<<it1->second<<endl;
        }
    }
}
void createGridGraph(int**& grid, Grafo*& graph, Subset*& subsets, int n, int m){
    grid=new int*[n];
    int k=0;
    for(int i=0;i<n;i++){
        grid[i]=new int[m];
        for(int j=0;j<m;j++){
            grid[i][j]=k;
            k++;
        }
    }
    printGrid(grid,n,m);
    graph=crearGrafo(n*m,2*n*m);
    setAristas(graph,grid,n, m);
    subsets=new Subset[n*m];
    makeSet(subsets,n*m);
}
int Kruskal(Grafo* graph, Subset* subsets){
    int mst_wt=0;

    sort(graph->aristas.begin(),graph->aristas.end());
    vector<pair<int,pair<int,int>>>::iterator it;
    cout<<"\n------------MST------------"<<endl;
    for(it=graph->aristas.begin(); it!=graph->aristas.end();it++){
        int u=it->second.first;
        int v=it->second.second;
        int set_u=Find(subsets,u);
        int set_v=Find(subsets,v);

        if(set_u!=set_v){
            cout<<u<<"\t-\t"<<v<<"\tPESO: "<<it->first<<endl;
            mst_wt+=it->first;
            Union(subsets, set_u, set_v);
        }
    }
    return mst_wt;
}
int main(){
    int** grid;
    int option=0;
    int params[2];
    Grafo* graph;
    Subset* subsets;
    string input;
    cout<<"Ingrese los valores n y m positivos, mayores que cero y separados por un espacio:\n[n]: número de filas de la grilla\n[m]: número de columnas de la grilla"<<endl;
    getline(cin,input);
    if(getParams(input, params)){
        srand(time(NULL));
        createGridGraph(*&grid, *&graph, *&subsets, params[0],params[1]);
        cout<<"\nSeleccione que hacer ingresando el número que corresponda\n[1]: Calcular el MST (Minimum Spanning Tree) de la grilla creada\n[2]: Crear un Laberinto con la grilla creada"<<endl;
        getline(cin,input);
        option=stoi(input);
        while(option!=1 && option!=2){
            cout<<"\nIngrese número que corresponda\n[1]: Calcular el MST (Minimum Spanning Tree) de la grilla creada\n[2]: Crear un Laberinto con la grilla creada"<<endl;
            getline(cin,input);
            option=stoi(input);
        }
        if(option==1){
            int a=Kruskal(graph,subsets);
            cout<<"\nPeso de MST: "<<a<<"\nAdios :)"<<endl;
        }else if(option==2){
            createLabyrinth(grid,graph,subsets,params[0],params[1]);
            cout<<"\nAdios :)"<<endl;
        }
    }else{
        cout<<"Adios :("<<endl;
        return 0;
    }
    return 0;
}
