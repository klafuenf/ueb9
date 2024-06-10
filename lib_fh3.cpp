//#include "b9b.h"

#include <windows.h>
#include <windowsx.h>
#include <commctrl.h>
#include <string.h>
#include <math.h>
#include <stdio.h>
#include <conio.h>
#include <time.h>


int punkte_r(double punkte[][2], const char* dateiname) //siehe auch Kapitel 10
//int punkte_r(double punkte[][2], const std::string& dateiname)//siehe auch Kapitel 10
{
    FILE* eingabe; /*eingabedatei */
    int i;

    if ((eingabe = fopen(dateiname, "r")) == NULL)
    //if((eingabe = fopen("i.txt", "r")) == NULL)
    {
        printf("\n");
        printf("\nDatei %s konnte zum Lesen nicht geoeffnet werden!", dateiname);
        exit(1);
        return (0);
    }

    //fscanf(eingabe, "%i ", &anz_ecken);
    i = 0;
    while (!feof(eingabe))
    {
        fscanf(eingabe, "%lf %lf", &punkte[i][0], &punkte[i][1]);
        i = i + 1;
    }
    fclose(eingabe);
    return (i);
}


void BildschirmMittig01mm(HDC hdc, HWND hwnd, RECT rect)
{
    SetMapMode(hdc, MM_LOMETRIC); //Bildschirm 0,1 mm, x rechts, y aufwärts
    GetClientRect(hwnd, &rect); // holt sich die akt. Rechteckpos. (rect.left, rect.top, rect.right, rect.bottom
    SetViewportOrgEx(hdc, (rect.right - rect.left) / 2, -(rect.top - rect.bottom) / 2, NULL);
    //setzt Koordinatenursp. mittig
}
