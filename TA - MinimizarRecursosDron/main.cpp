/* 
 * File:   main.cpp
 * Author: bryan
 *
 * Created on 24 de junio de 2024, 04:45 PM
 */

#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <ctime>
#include "Ubicaciones.h"
#include "Dron.h"
#include "Nodo.h"
#include "energiaVecinos.h"
#define MAXUBICACIONES 50
#define alfa 0.5
#define ITERACIONES 1000
using namespace std;

// Función para calcular la distancia euclidiana entre dos clientes
double calcularDistancia(const  Ubicaciones& c1, const Ubicaciones& c2) {
    return sqrt(pow(c1.x - c2.x, 2) + pow(c1.y - c2.y, 2));
}

bool compara(energiaVecinos a, energiaVecinos b){
    return a.cantEnergia < b.cantEnergia;
    
}
int verifica(vector <energiaVecinos>ciudades,int maxrcl){
    int cont=0;
    for(int i=0;i<ciudades.size();i++) 
        if(maxrcl>=ciudades[i].cantEnergia)
            cont++;
    return cont;    
}

bool obtenerHabilitacion(struct Ubicaciones ubicacionX, struct Ubicaciones ubicacionY){
    
    if(ubicacionX.x==ubicacionY.x and ubicacionX.y==ubicacionY.y)return false;
    if(ubicacionY.tipo=='V')return true;
    if(ubicacionY.tipo=='C' or ubicacionY.tipo=='B')return false;
}

void generarMapa(struct nodo mapa[][MAXUBICACIONES],struct Ubicaciones arrUbicaciones[], int cantUbicaciones){
    
    struct nodo nodoAux;
    
    for(int i=0;i<cantUbicaciones;i++){
        for(int j=0;j<cantUbicaciones;j++){
            nodoAux.distancia=calcularDistancia(arrUbicaciones[i],arrUbicaciones[j]);
            nodoAux.habilitado=obtenerHabilitacion(arrUbicaciones[i], arrUbicaciones[j]);
            nodoAux.ubicacionDestino=arrUbicaciones[j];
            nodoAux.puntoLlegada=j;
            mapa[i][j]=nodoAux;
        }
    }
}

bool validarDron(struct nodo nodoMapa, struct Dron dron){
    
    struct Ubicaciones ubicacionLlegada=nodoMapa.ubicacionDestino;
    int energia=nodoMapa.distancia*(dron.peso+dron.cantidadPeso);

    if(nodoMapa.habilitado==false)return false;
    if(dron.tasaConsumo+energia>dron.maxCantidadEnergia)return false;
    if(ubicacionLlegada.tipo=='V'){
        if(dron.cantidadPaquetes+1<=dron.maxCantidadPaquetes 
            and dron.cantidadPeso+ubicacionLlegada.peso<=dron.maxCapacidadPeso)return true;
        else return false;
    }else return true;
}

void completarEnergiaVecinos(vector<energiaVecinos> &energiaVecinos, vector<nodo>vecinos,struct Dron dron){
    
    struct energiaVecinos energiaAux;
    for (int i = 0; i < vecinos.size(); i++) {
        energiaAux.cantEnergia=vecinos[i].distancia*(dron.peso+dron.cantidadPeso);
        energiaAux.puntoLlegada=vecinos[i].puntoLlegada;
        energiaVecinos.push_back(energiaAux);
    }

}

void modificarMapa(struct nodo mapa[][MAXUBICACIONES], int vecinoVisitado,struct Ubicaciones infoVecino, int cantUbicaciones){
    
    //DESABILITAMOS QUE CUALQUIER NODO PUEDA IR UNA CIUDAD YA VISITADA
    struct Ubicaciones ubicacionAux;
    int idAux, clienteAux;
    char tipoAux;
    for (int i = 0; i < cantUbicaciones; i++) {
        mapa[i][vecinoVisitado].habilitado=false;
    }
    
    if(infoVecino.tipo=='V'){
        tipoAux=infoVecino.tipo;
        idAux=infoVecino.id;
        for (int i = 0; i < cantUbicaciones; i++){
            ubicacionAux=mapa[0][i].ubicacionDestino;
            //Encontramos cual es comprador el cual se habilita al recoger un paquete de un vendedor
            if(ubicacionAux.id==idAux and ubicacionAux.tipo=='C')clienteAux=i;
        }
        for (int i = 0; i < cantUbicaciones; i++) {
            mapa[i][clienteAux].habilitado=true;
        }
    }
}

void modificamosDron(struct Dron &dron, struct nodo nodoMapa){
    
    struct Ubicaciones ubicacionLlegada=nodoMapa.ubicacionDestino;
    dron.tasaConsumo+=nodoMapa.distancia*(dron.peso+dron.cantidadPeso);
    dron.cantidadPeso+=ubicacionLlegada.peso;
    
    if(ubicacionLlegada.tipo=='V') dron.cantidadPaquetes++;
    if(ubicacionLlegada.tipo=='C') dron.cantidadPaquetes--;
    
}

bool validarSolucion(vector<Ubicaciones>solucion, struct nodo mapa[][MAXUBICACIONES], 
        int ubicacionActual, struct Dron &dron, int cantUbicaciones){
    
    int energia;
    //Se verifica si en el vector solución se han visitado todos los nodos;
    if(solucion.size()==cantUbicaciones){
        //Verificamos si hay energía para regresar a la base
        energia=mapa[ubicacionActual][0].distancia*(dron.peso+dron.cantidadPeso);
        if(energia+dron.tasaConsumo<dron.maxCantidadEnergia){
            dron.tasaConsumo+=energia;
            return true;
        }
        else return false;
    }else return false;
}

int construccionGRASP(vector<Ubicaciones>&solucion,struct Dron &dron, struct Ubicaciones arrUbicaciones[], int cantUbicaciones){
    
    struct nodo mapa[MAXUBICACIONES][MAXUBICACIONES];
    int vecinoVisitado, ubicacionActual=0, total = 0;
    int beta,tau,maxrcl,indmax,inda;

    generarMapa(mapa, arrUbicaciones, cantUbicaciones);
    solucion.push_back(arrUbicaciones[0]);   

    while(true){
        vector<nodo>vecinos;
        vector<energiaVecinos> energiaVecinos;
        nodo paso;

        for(int i=0;i<cantUbicaciones;i++){//en la ciudad a la que le estés buscando ruta
            //SOLO PUSHEAMOS LOS NODOS QUE ES POSIBLE VISITAR
            if(validarDron(mapa[ubicacionActual][i], dron)){
                paso=mapa[ubicacionActual][i];
                vecinos.push_back(paso);
            }
        } 
        
        completarEnergiaVecinos(energiaVecinos, vecinos, dron);
        if(!vecinos.empty()){
            sort(energiaVecinos.begin(),energiaVecinos.end(),compara);
            beta = energiaVecinos[0].cantEnergia;
            tau = energiaVecinos[energiaVecinos.size()-1].cantEnergia;
            maxrcl = round(beta + alfa*(tau-beta));
            indmax = verifica(energiaVecinos,maxrcl);
            srand(time(NULL));
            inda = rand()%indmax;
            vecinoVisitado = energiaVecinos[inda].puntoLlegada;
            total+=energiaVecinos[inda].cantEnergia;
            solucion.push_back(arrUbicaciones[energiaVecinos[inda].puntoLlegada]);       
            modificarMapa(mapa, vecinoVisitado,arrUbicaciones[vecinoVisitado], cantUbicaciones);
            modificamosDron(dron, mapa[ubicacionActual][vecinoVisitado]);
        }else{
            //SE PUEDE DAR EL CASO DE QUE SE HAN VISITADO TODOS LOS NODOS O NO
            if(validarSolucion(solucion, mapa, ubicacionActual, dron, cantUbicaciones)){
                total+=mapa[ubicacionActual][0].distancia*(dron.peso+dron.cantidadPeso);
                solucion.push_back(arrUbicaciones[0]);
            }
            else total =0;
            break;
        }

        ubicacionActual=vecinoVisitado;
    }
    return total;
}

void minruta(struct Ubicaciones arrUbicaciones[], struct Dron dronBase,struct Dron &mejorDron, 
        vector <Ubicaciones> &mejorsolucion, int cantUbicaciones){
    
    int energiaTotalViaje, min=999999;
    
    struct Dron dronIterativo;
    
    for(int k=0;k<ITERACIONES;k++){

        dronIterativo=dronBase;
        vector <Ubicaciones> solucion;
        
        energiaTotalViaje=construccionGRASP(solucion, dronIterativo, arrUbicaciones, cantUbicaciones);
            
        if(energiaTotalViaje<min and energiaTotalViaje>0){
            mejorsolucion.erase(mejorsolucion.begin(),mejorsolucion.begin()+mejorsolucion.size());
            mejorsolucion.insert(mejorsolucion.begin(),solucion.begin(),solucion.begin()+solucion.size());    
            mejorDron=dronIterativo;
            min=energiaTotalViaje;
        } 
    }
    
}

void imprimirSolucion(vector<Ubicaciones>solucion, struct Dron dron){
    
    if(solucion.size()){
        cout<<"Información del dron: "<<endl;
        cout<<"Capacidad máxima energética: "<<dron.maxCantidadEnergia<<endl;
        cout<<"Peso máximo de carga útil: "<<dron.maxCapacidadPeso<<endl;
        cout<<"Consumo energético en las entregas: "<<dron.tasaConsumo<<endl;
        //cout<<"Peso de carga al final del viaje: "<<dron.cantidadPeso<<endl;

        cout<<endl<<"Información de la ruta recorrida: "<<endl;
        cout<<"Orden de los nodo visitados: ";
        for (int i = 0; i < solucion.size(); i++) {
            cout<<solucion[i].id<<"-"<<solucion[i].tipo;
            if(solucion.size()-2>=i)cout<<" -> ";
        }
        cout<<endl<<endl;
    }else{
        cout<<"No es posible encontrar un recorrido viable en este conjunto de datos."<<endl;
        cout<<"Intente ejecutar nuevamente el problema o cambiando los parámetros del dron."<<endl;
    }
}

int main(int argc, char** argv) {
    
    struct Ubicaciones arrUbicaciones[]={
        {0,'B',0,0, 0},
        {1,'V',100,80, 10},
        {2,'V',70,40, 15},
        {3,'V',34,10, 5},
        {4,'V',-35,56, 13},
        {5,'V',2,6, 10},
        {1,'C',111,-310, -10},
        {2,'C',-32,-40, -15},
        {3,'C',-89,10, -5},
        {4,'C',150,99, -13},
        {5,'C',10,4, -10},
    };

//    struct Ubicaciones arrUbicaciones[]={
//        {0,'B',0,0, 0},//representa a la base de donde salen y vienen los drones
//        {1,'V',2,6, 10},//venta 1 posición vendedor
//        {2,'V',6,8, 15},
//        {3,'V',-5,4, 8},
//        {1,'C',10,4, -10},//venta 1 posición cliente
//        {2,'C',8,-2, -15},
//        {3,'C',-7,-4, -8},
//    };
    
    int cantidadUbicaciones=sizeof(arrUbicaciones)/sizeof(arrUbicaciones[0]);
    
    struct Dron dronBase={20, 30, 50000, 3};
    struct Dron mejorDron;
    vector <Ubicaciones> mejorsolucion;
    
    minruta(arrUbicaciones, dronBase,mejorDron, mejorsolucion, cantidadUbicaciones);
    imprimirSolucion(mejorsolucion, mejorDron);
    
    return 0;
}


