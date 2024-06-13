// arm.cpp
#include "arm.h"

arm::arm(){};

arm::~arm(){};

void arm::getgeo(const char* dat1) {
  arm::anz_p = punkte_r(arm::Arm, dat1);
  arm::l = arm::Arm[arm::anz_p - 1][1] - arm::Arm[arm::anz_p - 2][1];
  // Voraussetzung ist, dass die Arme senkrecht modelliert sind
}

void arm::calc(double winkel, double* drehpunkt, double* shiftpunkt) {
  int i;
  for (i = 0; i < arm::anz_p; i++) {
    TrafoPoint(arm::Arm[i], arm::Armn[i], winkel, drehpunkt, shiftpunkt);
  }
}

void arm::show() {
  int i;
  SelectObject(hdc, stift[stiftfarbe]);
  PaintArm(hwnd, arm::Armn, arm::anz_p);
  // PaintPolyline(hwnd, arm::Armn, arm::anz_p-2);
  // PaintPolygon(hwnd, arm::Armn, arm::anz_p - 2);
  SelectObject(hdc, stift1);
}

void arm::set_line_color(int f) {
  // this->stiftfarbe = f;
  arm::stiftfarbe = f;
}
