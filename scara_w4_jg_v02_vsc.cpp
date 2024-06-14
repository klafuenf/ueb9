//-------------------------------------------------------------------------------------------------------------------
// Programm zur Ermittlung von notwendigen Winkeln eines SCARA-Roboters Erweiterung von scara_w.c, so dass die Arme nicht springen (auf der Basis des
// Cosinus-Satzes) Autor: Julian Gaiser /MBB3C Datum 27.12.10 basiert auf den Konventionen von SCARA-Modell.doc Programmname SuchenWinkel Eingaben double
// sollpunkt[2], 				double l1, double l2, double alpha1[2],double alpha2[2] double sollpunkt[2]:
// Adresse der x,y Koordinaten des Roboterendrehpunktes bezogen auf den Ursprungshiftpunkt (0,0) !!! double l1, double l2
// Laenge der Arme 1 und 2 (Abstand zwischen Ursprung(Px.6) und Endrehpunkt (Px.7) double alpha1[2],double alpha2[2])	Adresse der Winkelstellungen von Arm1
// und Arm2 gegenueber der Senkrechten
//										es koennen 2, eine oder keine Loesungen geben
// Ausgabe	int							0: keine Loesung; 1: eine Loesung; 2: 2 Loesungen:
// moeglicher ext. Aufruf des Programmes:anz_loesungen=SuchenWinkel(sollpunkt_0, l1, l2, alpha1, alpha2);
// L1,L2 = Laenge Roboarm1,Roboarm2

//---------------------------------------------------------------------------------------------------------------------
#include <math.h>

int SuchenWinkel(double sollpunkt[2], double L1, double L2, double alpha1[2], double alpha2[2])

{
  int anz_loesungen = 2;
  double const PI = 3.14159265;
  double r, xp, yp, a0, a11, a12, a2, a3, a4, b0, b1, w11, w12, w2;

  xp = sollpunkt[0];
  yp = sollpunkt[1];

  r = sqrt(xp * xp + yp * yp);

  b0 = xp * xp + yp * yp;
  b1 = (b0 - (L1 * L1) - (L2 * L2)) / (-2 * L1 * L2);

  a0 = ((L2 * L2) - (L1 * L1) - (r * r)) / (-2 * L1 * r);
  a3 = xp / r;
  a4 = yp / r;

  if (yp >= 0) a11 = -asin(a3);
  if (yp <= 0) a11 = -acos(a4);  // Berechnung des Ersten Winkels fuer linkes geknickt
  if (yp <= 0 && xp <= 0) a11 = acos(a4);

  if (yp >= 0) a12 = asin(a3);
  if (yp <= 0) a12 = acos(a4);  // Berechnung des Ersten Winkels fuer rechts geknickt
  if (yp <= 0 && xp <= 0) a12 = -acos(a4);
  a2 = acos(a0);  // Winkel des geknickten Armes

  w11 = a11 - a2;  // Winkel Arm 1 linksgeknickt

  w12 = a12 - a2;  // Winkel Arm 1 rechtsgeknickt

  w2 = -acos(b1) + PI;  // Winkel Arm 2

  alpha1[0] = -w11 * 180 / PI;  // Winkel linksgeknickt
  alpha2[0] = -w2 * 180 / PI;

  alpha1[1] = w12 * 180 / PI;  // Winkel rechtsgeknickt
  alpha2[1] = w2 * 180 / PI;

  /*
  if(alpha1[0]<=-360||alpha1[0]>=360||alpha2[0]<=-360||alpha2[0]>=360)
  {
  anz_loesungen=anz_loesungen-1;//Begrenzung des Drehwinkels alpha[0]Roboarm1
  }
  if(alpha1[1]<=-360||alpha1[1]>=360||alpha2[1]<=-360||alpha2[1]>=360)
  {
  anz_loesungen=anz_loesungen-1;//alpha[1]Roboarm2
   }
  */

  if (!(alpha1[0] <= 360 && alpha1[0] >= -360 && alpha2[0] <= 360 && alpha2[0] >= -360)) {
    anz_loesungen = anz_loesungen - 1;  // Begrenzung des Drehwinkels alpha[0]Roboarm1
  }
  if (!(alpha1[1] <= 360 && alpha1[1] >= -360 && alpha2[1] <= 360 && alpha2[1] >= -360)) {
    anz_loesungen = anz_loesungen - 1;  // alpha[1]Roboarm2
  }
  alpha2[0] = alpha1[0] + alpha2[0];
  alpha2[1] = alpha1[1] + alpha2[1];

  return (anz_loesungen);
}
