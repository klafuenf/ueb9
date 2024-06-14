// rob.h
// #include "ueb4_header.h"
#include "arm.h"
using namespace std;
#include <iostream>

#ifndef _rob_H
#define _rob_H

class rob {
 private:
  double pos[2];
  int knickung = 0;

 public:
  rob();   // Konstruktor
  ~rob();  // Destructor

  double origin[2], origin_a[2], origin_e[2];
  double grundposition[2];

  int anz_loes;
  double xa, ya, xe, ye;

  double linien[1000][2][2];  // [1000 linien] [je einen anfangs- und endpunkt] [je x und Y]
  int l = -1;                 // Zaehler fuer die aktuell letzte Linie
  int linie_ein = 0;          // spruehdose an oder aus

  void getgeo(const char*, const char*);
  void calc(double* sollpunkt_0);
  void show();
  double getposx();
  double getposy();
  void set_knick(int);
  void set_color(int f1, int f2);
  void init_linie();
  void set_anfang_zu_ende();

  // void set_line_color(int);
  /// int anz_p;
  // int stiftfarbe = 1;
  arm arm_1;
  arm arm_2;
};

#endif
