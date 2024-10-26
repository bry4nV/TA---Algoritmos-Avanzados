/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cppFiles/file.h to edit this template
 */

/* 
 * File:   Nodo.h
 * Author: bryan
 *
 * Created on 25 de junio de 2024, 11:08 PM
 */

#ifndef NODO_H
#define NODO_H

struct nodo{
    int puntoLlegada;
    int distancia;
    bool habilitado;//REFIERE A SI SE PUEDE VISITAR EL NODO
    struct Ubicaciones ubicacionDestino;
};

#endif /* NODO_H */

