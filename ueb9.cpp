// ueb9.cpp++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Programm fuer Angewandte Informatik "
//  erstellt von Kruell
// Erweiterung
//  um Hintergrundfarben und Sleep -Funktion, Messagebox,....
//  // siehe auch Pilzschaf winapi https://www.youtube.com/watch?v=tg0dG_fBKow

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include <stdio.h>
#include <windows.h>

#include "arm.h"
#include "rob.h"

using namespace std;

void unterprog1(HWND hwnd, HDC hdc, RECT rect) {
  int i, k;
  char wtext[200];
  double drehpunkt[2];
  double shiftpunkt[2];
  double verschiebung[2];
  double winkel;

  anz_rob = 3;
  // Wenn mit mehreren Roboter zu arbeiten ist, ist diese Zahl höher als 1 einzustellen // max. 5 Roboter sind angelegt

  v_band = 0.02;  // Pixel je ms

  t_max = 500;
  t_step = 20;

  Robbi[0].getgeo("arm1.txt", "arm2b.txt");
  Robbi[1].getgeo("arm1.txt", "arm2.txt");
  Robbi[2].getgeo("arm1.txt", "arm2d.txt");
  Robbi[3].getgeo("arm1.txt", "arm2d.txt");
  Robbi[4].getgeo("arm1.txt", "arm2d.txt");

  Robbi[0].set_color(2, 3);  /// Farbe 1 für Arm_1, Farbe 2 für Arm_2
  Robbi[1].set_knick(1);
  anz_wst = 5;
  for (i = 0; i < anz_wst; i++) {
    wst[i].x = 60 + 20 * i;
    wst[i].y = 20;
    wst[i].greifer = 0;  // nicht gegriffen
    wst[i].typ = 1;      // klein rund
    wst[i].winkel = 0;
  }

  wst[2].typ = 2;  // groß, rund
  wst[3].typ = 3;  // Polygon-Form

  wst[anz_wst - 1].x = wst[anz_wst - 1].x + 400;

  anz_bandpunkte = punkte_r(band, "band.txt");
  anz_form_ws_punkte = punkte_r(form_ws, "form_wst.txt");
  anz_koerper_punkte = punkte_r(koerper, "koerper8.txt");

  drehpunkt[0] = 0;  // x
  drehpunkt[1] = 0;  // y

  verschiebung[0] = 150;
  verschiebung[1] = 200;

  for (i = 0; i < anz_rob; i++) {
    Robbi[i].origin[0] = verschiebung[0] + 50 * i;  // kann ggf entfallen
    Robbi[i].origin[1] = verschiebung[1];           //  kann ggf entfallen
    Robbi[i].origin_a[0] = Robbi[i].origin[0];
    Robbi[i].origin_a[1] = Robbi[i].origin[1];
    Robbi[i].origin_e[0] = Robbi[i].origin[0];
    Robbi[i].origin_e[1] = Robbi[i].origin[1];

    Robbi[i].xa = 200;
    Robbi[i].ya = 200;
    Robbi[i].xe = 240;
    Robbi[i].ye = 80 + 10 * i;
    Robbi[i].linie_ein = 0;
    Robbi[i].set_color(i, i);
    // Robbi[0].origin_a[0] = 50 ;
  }
  // Robbi[0].origin_a[0] = 50;
  bewegen(hwnd);
  for (i = 0; i < anz_wst; i++) {
    Robbi[0].xe = wst[i].x;
    Robbi[0].ye = wst[i].y;

    if (i == anz_wst - 1) {
      Robbi[0].origin_e[0] = Robbi[0].origin_e[0] + 300;
    }
    bewegen(hwnd);
    wst[i].greifer = 1;  // gegriffen
    Robbi[0].linie_ein = 1;
    Robbi[0].xe = band[anz_bandpunkte - 1][0];
    Robbi[0].ye = band[anz_bandpunkte - 1][1];
    if (i == anz_wst - 1) {
      Robbi[0].origin_e[0] = Robbi[0].origin_e[0] - 300;
    }

    bewegen(hwnd);
    wst[i].greifer = 2;  // gegriffen vom Band
  }                      // for i...

  Robbi[0].xe = wst[3].x + t_max * v_band;
  Robbi[0].ye = wst[3].y;

  Robbi[1].xe = wst[3].x + t_max * v_band;
  Robbi[1].ye = wst[3].y;

  bewegen(hwnd);

  Robbi[1].xe = 250;
  Robbi[1].ye = 50;

  wst[3].greifer = 1;

  Robbi[0].xe = 200;
  Robbi[0].ye = 80;
  bewegen(hwnd);
  wst[3].greifer = 0;  // nicht gegriffen
  Robbi[0].xe = 300;
  Robbi[0].ye = 80;
  bewegen(hwnd);
  Robbi[0].linie_ein = 0;
  for (i = 0; i < anz_koerper_punkte; i++) {
    Robbi[0].xe = koerper[i][0];
    Robbi[0].ye = koerper[i][1];
    bewegen(hwnd);
    Robbi[0].linie_ein = 1;
  }
  Robbi[0].linie_ein = 0;
}  // unterprog1

void PaintArm(HWND hwnd, double arm[][2], int anz_p) {
  double r = 4;

  // PaintPolygon(hwnd, arm, anz_p-2);
  PaintPolyline(hwnd, arm, anz_p - 2);
  Ellipse(hdc, (int)arm[anz_p - 2][0] - r, (int)arm[anz_p - 2][1] - r, (int)arm[anz_p - 2][0] + r, (int)arm[anz_p - 2][1] + r);
  Ellipse(hdc, (int)arm[anz_p - 1][0] - r, (int)arm[anz_p - 1][1] - r, (int)arm[anz_p - 1][0] + r, (int)arm[anz_p - 1][1] + r);
}

void PaintPolygon(HWND hwnd, double polyg[][2], int anz_p) {
  int i;
  POINT punkte[1000];
  for (i = 0; i < anz_p; i++) {
    punkte[i].x = (int)polyg[i][0];
    punkte[i].y = (int)polyg[i][1];
  }
  Polygon(hdc, punkte, anz_p);
}

void PaintPolyline(HWND hwnd, double polyg[][2], int anz_p) {
  int i;
  POINT punkte[1000];
  for (i = 0; i < anz_p; i++) {
    punkte[i].x = (int)polyg[i][0];
    punkte[i].y = (int)polyg[i][1];
  }
  Polyline(hdc, punkte, anz_p);
}

void PaintWst(HWND hwnd, struct element* ws, int anz_wst) {
  int r = 5;   // Radius unseres kleinen runden Werkstueckes
  int rg = 8;  // Radius unseres großen runden Werkstueckes
  int i;       // lokal hier für die Nummerierung der Werkstücke
  int k;       // lokal hier für die Nummerierung der Punkte jede Werkstückes

  double dpunkt[2];
  double spunkt[2];
  double form_wsn[100][2];
  for (i = 0; i < anz_wst; i++)

    if (ws[i].typ == 1) {
      Ellipse(hdc, (int)ws[i].x - r, (int)ws[i].y - r, (int)ws[i].x + r, (int)ws[i].y + r);
    }

    else if (ws[i].typ == 2) {
      Ellipse(hdc, (int)ws[i].x - rg, (int)ws[i].y - rg, (int)ws[i].x + rg, (int)ws[i].y + rg);
    }

    else if (ws[i].typ == 3) {
      dpunkt[0] = 0;
      dpunkt[1] = 0;
      spunkt[0] = ws[i].x;
      spunkt[1] = ws[i].y;
      for (k = 0; k < anz_form_ws_punkte; k++) {
        TrafoPoint(form_ws[k], form_wsn[k], ws[i].winkel, dpunkt, spunkt);
      }
      PaintPolygon(hwnd, form_wsn, anz_form_ws_punkte);
    }
}
