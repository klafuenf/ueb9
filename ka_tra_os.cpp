/********************************************************************/
/* Programmname:     prog4_1                                        */
/* Dateiname:        prog4_1.c                                      */
/* Autor:            Georg Kruell                                  */
/* Letzte Aenderung: 17.02.2020                                     */
/********************************************************************/
/* Programmbeschreibung:                                            */
/* Lernprogramm zur Schleifenbildung (for)                          */
/* Summenbildung, Mittelwert bilden                                 */
/********************************************************************/
#include <math.h>
#include <stdio.h>

void tramatxy(double dx, double dy, double mat[3][3]) {
  mat[0][0] = 1;
  mat[0][1] = 0;
  mat[0][2] = dx;

  mat[1][0] = 0;
  mat[1][1] = 1;
  mat[1][2] = dy;

  mat[2][0] = 0;
  mat[2][1] = 0;
  mat[2][2] = 1;
}

void matvek(double mat[3][3], double pv[3], double pn[3]) {
  pn[0] = pv[0] * mat[0][0] + pv[1] * mat[0][1] + pv[2] * mat[0][2];
  pn[1] = pv[0] * mat[1][0] + pv[1] * mat[1][1] + pv[2] * mat[1][2];
  pn[2] = 1;
}

void rotmatxy(double winkel, double mat[3][3]) {
  double bogenmass;
  double const PI = 3.14159265;
  bogenmass = PI * winkel / 180;

  mat[0][0] = cos(bogenmass);
  mat[0][1] = sin(bogenmass);
  mat[0][2] = 0;

  mat[1][0] = -sin(bogenmass);
  mat[1][1] = cos(bogenmass);
  mat[1][2] = 0;

  mat[2][0] = 0;
  mat[2][1] = 0;
  mat[2][2] = 1;
}  // rotmatxy

// TrafoPoint(arm1[i], arm1n[i], alpha1[1], drehpunkt, shiftpunkt);

void TrafoPoint(double pa[], double pe[], double winkel, double drehpunkt[], double shiftpunkt[]) {
  double p1[3], p2[3], p3[3], p4[3], p5[3], matrix[3][3];
  p1[0] = pa[0];
  p1[1] = pa[1];
  p1[2] = 1;

  tramatxy(-drehpunkt[0], -drehpunkt[1], matrix);
  matvek(matrix, p1, p2);
  rotmatxy(winkel, matrix);
  matvek(matrix, p2, p3);
  tramatxy(drehpunkt[0], drehpunkt[1], matrix);
  matvek(matrix, p3, p4);
  tramatxy(shiftpunkt[0], shiftpunkt[1], matrix);
  matvek(matrix, p4, p5);

  pe[0] = p5[0];
  pe[1] = p5[1];

  // return(0);
}
