//ueb8b.h
// // siehe auch Pilzschaf winapi https://www.youtube.com/watch?v=tg0dG_fBKow

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#ifndef _UEB8b_HEADER_H
#define _UEB8b_HEADER_H


#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "arm.h"
#include "rob.h"

struct element
{
    double x;
    double y;
    int typ; // 1: klein, rund; 2: groß, rund;...
    int greifer; //0: nicht gegriffen; 1: vom Robbi gegriffen; .....
    double winkel;
};

extern struct element wst[100];
extern int anz_wst;

extern double band[100][2];
extern int anz_bandpunkte;
extern double v_band;

extern double form_ws[100][2];
extern double anz_form_ws_punkte;

extern double koerper[1000][2];
extern int anz_koerper_punkte;

///extern double linien[1000][2][2];// [1000 linien] [je einen anfangs- und endpunkt] [je x und Y]
///extern int l; // Zähler für die aktuell letzte Linie
///extern int linie_ein; // sprühdose an oder aus

extern HDC hdc;
extern HWND hwnd;
extern HPEN stift1, stift2, stift3, stift[5];
extern double sollpunkt_0[2]; // der Sollpunkt im Bildschirmkoordinatensystem

extern int anz_rob;
extern double t_max;
extern double t_step;
extern double t; // Zeit in ms
extern rob Robbi[5];
//extern arm Arm[2];
//extern double xa[5], ya[5], xe[5], ye[5];
//extern double org_a[5], org_e[5];
extern PAINTSTRUCT ps;


int punkte_r(double punkte[][2], const char* dateiname); //siehe auch Kapitel 10
void TrafoPoint(double pa[], double pe[], double winkel, double drehpunkt[], double shiftpunkt[]);
void PaintPolyline(HWND hwnd, double polyg[][2], int anz_p);
void PaintPolygon(HWND hwnd, double polyg[][2], int anz_p);
void PaintArm(HWND hwnd, double arm[][2], int anz_p);
int SuchenWinkel(double sollpunkt[2], double L1, double L2, double alpha1[2], double alpha2[2]);
void bewegen(HWND hwnd);
//void PaintWst(HWND hwnd, struct element ws, int anz_wst);
void PaintWst(HWND hwnd, struct element* ws, int anz_wst);
void PaintLinien(HWND hwnd);

#endif
