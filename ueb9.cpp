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

int kabel_lesen(double kabel[], const char* dateiname) {
  FILE* eingabe; /*eingabedatei */
  int i;

  if ((eingabe = fopen(dateiname, "r")) == NULL) {
    printf("\n");
    printf("\nDatei %s konnte zum Lesen nicht geoeffnet werden!", dateiname);
    exit(1);
    return (0);
  }

  i = 0;
  while (!feof(eingabe)) {
    fscanf(eingabe, "%lf", &kabel[i]);
    i = i + 1;
  }
  fclose(eingabe);
  return (i);
}

void unterprog1(HWND hwnd, HDC hdc, RECT rect) {
  int i, k;
  char wtext[200];
  double winkel;
  double verschiebung[] = {400, 400};
  double reservoir[] = {verschiebung[0] - 220, verschiebung[1] - 160};

  Montauf& ma_l = montauf[0];
  Montauf& ma_r = montauf[1];

  // Linke Montagevorrichtung
  ma_l.x = verschiebung[0] - 30;
  ma_l.y = verschiebung[1] - 200;

  // Rechte Montagevorrichtung
  ma_r.x = verschiebung[0] + 30;
  ma_r.y = verschiebung[1] - 200;

  anz_rob = 2;
  // Wenn mit mehreren Roboter zu arbeiten ist, ist diese Zahl hoeher als 1 einzustellen // max. 5 Roboter sind angelegt

  v_band = 0.02;  // Pixel je ms

  t_max = 500;
  t_step = 20;

  Robbi[0].getgeo("arm1.txt", "arm2.txt");
  Robbi[1].getgeo("arm1.txt", "arm2.txt");
  //Robbi[2].getgeo("arm1.txt", "arm2.txt");
  //Robbi[3].getgeo("arm1.txt", "arm2.txt");
  //Robbi[4].getgeo("arm1.txt", "arm2.txt");

  anz_kabel = kabel_lesen(kabel, "kabel.txt");

  Robbi[0].set_color(2, 3);  /// Farbe 1 fuer Arm_1, Farbe 2 fuer Arm_2
  rr.set_knick(1);
  anz_wst = (anz_kabel+1) * 2;  // immer gerade Anzahl
  for (i = 0; i < anz_wst; i++) {
    wst[i].x = reservoir[0];
    wst[i].y = reservoir[1];

    wst[i].greifer = 0;  // nicht gegriffen
    wst[i].typ = 3;      // form
    wst[i].winkel = 0;
    wst[i].breite = 50;
  }

  anz_bandpunkte = punkte_r(band, "band.txt");
  anz_form_ws_punkte = punkte_r(form_ws, "form_wst.txt");
  double ablage[] = {band[anz_bandpunkte - 1][0], band[anz_bandpunkte - 1][1]};


  for (i = 0; i < anz_rob; i++) {
    Robbi[i].origin[0] = verschiebung[0];  // kann ggf entfallen
    Robbi[i].origin[1] = verschiebung[1];  //  kann ggf entfallen
    Robbi[i].origin_a[0] = Robbi[i].origin[0];
    Robbi[i].origin_a[1] = Robbi[i].origin[1];
    Robbi[i].origin_e[0] = Robbi[i].origin[0];
    Robbi[i].origin_e[1] = Robbi[i].origin[1];
    Robbi[i].grundposition[0] = Robbi[i].origin[0];
    Robbi[i].grundposition[1] = Robbi[i].origin[1] - 40;

    Robbi[i].linie_ein = 0;
    Robbi[i].set_color(i, i);
    // Robbi[0].origin_a[0] = 50 ;
  }
  rl.grundposition[0] = rl.origin[0] - 60;
  rr.grundposition[0] = rr.origin[0] + 60;

  rl.xa = rl.grundposition[0];
  rl.ya = rl.grundposition[1];
  rr.xa = rr.grundposition[0];
  rr.ya = rr.grundposition[1];

  int wst_zaehler = -1;
  for (i = 0; i < anz_kabel; i++) {
    double kabel_laenge = kabel[i];
    // sichere Grundposition
    rl.xe = rl.grundposition[0];
    rl.ye = rl.grundposition[1];
    rr.xe = rr.grundposition[0];
    rr.ye = rr.grundposition[1];

    bewegen(hwnd);

    // rechte Montagevorrichtung ausrichten
    // aktuelle Position der Montagevorrichtung
    rr.xe = ma_r.x;
    rr.ye = ma_r.y;
    bewegen(hwnd);
    ma_r.greifer = 2;  // Montagevorrichtung aufgreifen
    // Neue Position der Montagevorrichtung
    rr.xe = ma_l.x + wst[wst_zaehler].breite + kabel_laenge;
    bewegen(hwnd);
    ma_r.greifer = 0;  // Montagevorrichtung losgelassen

    // Erstes Werkstueck i hohlen
    // Reservoir anfahren
    wst_zaehler++;
    rr.xe = reservoir[0];
    rr.ye = reservoir[1];
    bewegen(hwnd);
    wst[wst_zaehler].greifer = 2;  // Werkstueck aufgreifen
    rr.xe = ma_r.x;                // Rechte Montagevorrichtung anfahren
    rr.ye = ma_r.y;
    bewegen(hwnd);
    wst[wst_zaehler].greifer = 0;  // Werkstueck loslassen

    // Zweites Werkstueck
    // Reservoir anfahren
    wst_zaehler++;
    rr.xe = reservoir[0];
    rr.ye = reservoir[1];
    bewegen(hwnd);
    wst[wst_zaehler].greifer = 2;  // Werkstueck aufgreifen
    rr.xe = ma_l.x;                // Linkes Montagevorrichtung anfahren
    rr.ye = ma_l.y;
    bewegen(hwnd);
    wst[wst_zaehler].greifer = 0;  // Werkstueck loslassen

    rr.xe = rr.grundposition[0];
    rr.ye = rr.grundposition[1];
    bewegen(hwnd);

    // Draht anbringen
    // Linke Montagevorrichtung anfahren
    rl.xe = wst[wst_zaehler].right();
    rl.ye = wst[wst_zaehler].y;
    bewegen(hwnd);
    rl.linie_ein = 1;
    // Auf recht Montagevorrichtung verfahren
    rl.xe = wst[wst_zaehler - 1].left();
    rl.ye = wst[wst_zaehler - 1].y;
    bewegen(hwnd);
    rl.linie_ein = 0;

    // Verbundene Werkstuecke auf Band transportieren
    rl.xe = ma_l.x;
    rl.ye = ma_l.y;
    rr.xe = ma_r.x;
    rr.ye = ma_r.y;
    bewegen(hwnd);

    rl.l = -1;  // gerade Linie loeschen

    wst[wst_zaehler - 1].greifer = 1;  // Werkstueck aufgreifen
    wst[wst_zaehler].greifer = 2;
    rl.xe = ablage[0] - 30;
    rl.ye = ablage[1];
    rr.xe = ablage[0] + 30;
    rr.ye = ablage[1];
    bewegen(hwnd);
    wst[wst_zaehler - 1].greifer = 0;  // Werkstueck loslassen
    wst[wst_zaehler].greifer = 0;

    // bewegen(hwnd);
    // wst[i].greifer = 1;  // gegriffen
    // Robbi[0].linie_ein = 1;
    // Robbi[0].xe = band[anz_bandpunkte - 1][0];
    // Robbi[0].ye = band[anz_bandpunkte - 1][1];
    // if (i == anz_wst - 1) {
    // Robbi[0].origin_e[0] = Robbi[0].origin_e[0] - 300;
    // }

    // bewegen(hwnd);
    // wst[i].greifer = 2;  // gegriffen vom Band
  }  // for i...

  // sichere Grundposition
  rl.xe = rl.grundposition[0];
  rl.ye = rl.grundposition[1];
  rr.xe = rr.grundposition[0];
  rr.ye = rr.grundposition[1];

  bewegen(hwnd);
  return;
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
  int rg = 8;  // Radius unseres grossen runden Werkstueckes
  int i;       // lokal hier fuer die Nummerierung der Werkstuecke
  int k;       // lokal hier fuer die Nummerierung der Punkte jede Werkstueckes

  double dpunkt[2];
  double spunkt[2];
  double form_wsn[100][2];
  for (i = 0; i < anz_wst; i++)

    if (ws[i].typ == 1) {
      Ellipse(hdc, (int)ws[i].x - r, (int)ws[i].y - r, (int)ws[i].x + r, (int)ws[i].y + r);
    } else if (ws[i].typ == 2) {
      Ellipse(hdc, (int)ws[i].x - rg, (int)ws[i].y - rg, (int)ws[i].x + rg, (int)ws[i].y + rg);
    } else if (ws[i].typ == 3) {
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

void PaintMontauf(HWND hwnd, Montauf* ma, int anz_ma) {
  int r = 20;
  for (int i = 0; i < anz_ma; i++) {
    int x = (int)ma[i].x;
    int y = (int)ma[i].y;
    Rectangle(hdc, x - r, y - r, x + r, y + r);
  }
}