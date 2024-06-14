// ueb9_bewegen.cpp+<<<-+++++++++++++++++++++++++++++++++++++++++++++++++++
// Programm fuer Angewandte Informatik "
//  erstellt von Kruell

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include <stdio.h>
#include <windows.h>

#include "arm.h"
#include "rob.h"
using namespace std;

void bewegen(HWND hwnd) {
  char wtext[200];
  int i;
  static int richtung2 = 1;
  static double gesamtzeit = 0;
  for (i = 0; i < anz_rob; i++) {
    if (Robbi[i].linie_ein == 1) {
      Robbi[i].init_linie();
    }
  }
  for (t = 0; t <= t_max; t = t + t_step) {
    for (i = 0; i < anz_rob; i++) {
      sollpunkt_0[0] = Robbi[i].xa + (t / t_max) * (Robbi[i].xe - Robbi[i].xa);
      sollpunkt_0[1] = Robbi[i].ya + (t / t_max) * (Robbi[i].ye - Robbi[i].ya);
      Robbi[i].origin[0] = Robbi[i].origin_a[0] + (t / t_max) * (Robbi[i].origin_e[0] - Robbi[i].origin_a[0]);
      // Interpolation der Verschiebung der Roboterpos in x-richtung
      Robbi[i].origin[1] = Robbi[i].origin_a[1] + (t / t_max) * (Robbi[i].origin_e[1] - Robbi[i].origin_a[1]);
      // fuer den Fall, dass man den Robbi in y verschieben moechte
      Robbi[i].calc(sollpunkt_0);
    }  // for i < anz_rob...
    for (i = 0; i < anz_wst; i++) {
      if (wst[i].greifer == 1)  // vom Robbi 1 gegriffen
      {
        wst[i].x = rl.getposx();
        wst[i].y = rl.getposy();
      } else if (wst[i].greifer == 2)  // vom Robbi 2 gegriffen
      {
        wst[i].x = rr.getposx();
        wst[i].y = rr.getposy();
      }
      if (wst[i].greifer == 3)  // vom band gegriffen
      {
        wst[i].x = wst[i].x + v_band * t_step;
        wst[i].winkel = wst[i].winkel + 0.1 * t_step;
      }
    }  // for i y anz_wst
    for (i = 0; i < 2; i++) {
      if (montauf[i].greifer == 1) {
        montauf[i].x = rl.getposx();
        montauf[i].y = rl.getposy();
      }
      if (montauf[i].greifer == 2) {
        montauf[i].x = rr.getposx();
        montauf[i].y = rr.getposy();
      }
    }

    InvalidateRect(hwnd, NULL, TRUE);  // leert den Bildschirm
    hdc = BeginPaint(hwnd, &ps);       // beginnt, neu zu zeichnen

    PaintPolygon(hwnd, band, anz_bandpunkte - 1);
    PaintWst(hwnd, wst, anz_wst);
    PaintMontauf(hwnd, montauf, 2);
    for (i = 0; i < anz_rob; i++) {
      Robbi[i].show();
    }
    if (Robbi[0].anz_loes >= 1) {
      sprintf(wtext, "Robbi[0]:  Pos.: x = %3.2lf, y = %3.2lf; Gesamtzeit:  %.2lf s", Robbi[0].arm_2.Armn[Robbi[0].arm_2.anz_p - 1][0],
              Robbi[0].arm_2.Armn[Robbi[0].arm_2.anz_p - 1][1], gesamtzeit / 1000);
    } else {
      sprintf(wtext, "Die Pos x = %3.2lf, y = %3.2lf ist nicht erreichbar", sollpunkt_0[0], sollpunkt_0[0]);
    }
    SetWindowText(hwnd, wtext);  // Titelleiste akt.

    Sleep(t_step);
    gesamtzeit = gesamtzeit + t_step;
    // Das Programm wird t-step lang gestoppt. Wenn man davon ausgeht, dass die Rechenzeit gegen Null geht, kann man an dieser Stelle auch die Gesamtzeit mit
    // t_step hochzaehlen
  }  // for t..

  for (i = 0; i < anz_rob; i++) {
    Robbi[i].set_anfang_zu_ende();
  }

}  // bewegen
