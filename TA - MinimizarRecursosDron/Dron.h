/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cppFiles/file.h to edit this template
 */

/* 
 * File:   Dron.h
 * Author: bryan
 *
 * Created on 25 de junio de 2024, 10:49 AM
 */

#ifndef DRON_H
#define DRON_H

//Estructura para representar los datos del dron
struct Dron {
    double peso;
    double maxCapacidadPeso;     // Máxima capacidad de peso que puede transportar
    double maxCantidadEnergia;  // Máxima cantidad de energía del dron
    int maxCantidadPaquetes;    // Máximo número de paquetes que puede transportar
    double cantidadPeso;
    double tasaConsumo;       // Tasa de consumo de energía por unidad de peso
    int cantidadPaquetes;
};

#endif /* DRON_H */

