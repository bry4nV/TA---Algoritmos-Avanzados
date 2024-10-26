/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cppFiles/file.h to edit this template
 */

/* 
 * File:   Ubicaciones.h
 * Author: bryan
 *
 * Created on 25 de junio de 2024, 10:48 AM
 */

#ifndef UBICACIONES_H
#define UBICACIONES_H

// Estructura para representar la ubicación de un cliente (vendedor o comprador) y base.
struct Ubicaciones {
    int id;
    char tipo;//puede ser Vendedor, Cliente o Base 'V', 'C' o 'B'
    //bool esVendedor;  // true si es vendedor, false si es comprador
    double x, y;      // Coordenadas del cliente
    double peso; //El peso del paquete vendedor o el peso del paquete que recibirá el comprador
};

#endif /* UBICACIONES_H */

