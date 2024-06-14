// ueb8b.fenster.cpp

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Programm fuer Angewandte Informatik "
// erstellt von Kruell
// Erweiterung
// um Hintergrundfarben und Sleep -Funktion, Messagebox,....
// // siehe auch Pilzschaf winapi https://www.youtube.com/watch?v=tg0dG_fBKow

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include <stdio.h>
#include <windows.h>

#include "arm.h"
#include "rob.h"

using namespace std;
double t_max;
double t_step;
double t;  // Zeit in ms

double sollpunkt_0[2];  // der Sollpunkt im Bildschirmkoordinatensystem

// arm Arm[2];
rob Robbi[5];
rob& rl = Robbi[0];  // Referenz auf Roboter links
rob& rr = Robbi[1];  // Referenz auf Roboter rechts

struct element wst[100];
int anz_wst;

Montauf montauf[2];

double kabel[100];
int anz_kabel;

double band[100][2];
int anz_bandpunkte;
double v_band;

double form_ws[100][2];
int anz_form_ws_punkte;

double koerper[1000][2];
int anz_koerper_punkte;

int anz_rob;

HPEN stift1, stift2, stift3, stift[5];
HBRUSH hintergrund1, hintergrund2;
HBRUSH hintergrund[3];
HDC hdc;
PAINTSTRUCT ps;
RECT rect;

HWND hwnd;
int punkte_r(double punkte[][2], const char* dateiname);

// rob Robbi[5];

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);  // prototyp
void unterprog1(HWND hwnd, HDC hdc, RECT rect);
// void unterprog2(HWND hwnd, HDC hdc, RECT rect);
void movexy(HWND hwnd);
void PaintArm(HWND hwnd, double arm[][2], int anz_p);
void PaintPolyline(HWND hwnd, double polyg[][2], int anz_p);
void PaintPolygon(HWND hwnd, double polyg[][2], int anz_p);

void TrafoPoint(double pa[], double pe[], double winkel, double drehpunkt[], double shiftpunkt[]);
int punkte_r(double punkte[][2], const char* dateiname);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow) {
  static TCHAR szAppName[] = TEXT("HelloWin");

  MSG msg;
  WNDCLASS wndclass;

  wndclass.style = CS_HREDRAW | CS_VREDRAW;
  wndclass.lpfnWndProc = WndProc;
  wndclass.cbClsExtra = 0;
  wndclass.cbWndExtra = 0;
  wndclass.hInstance = hInstance;
  wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
  wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
  wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
  wndclass.lpszMenuName = NULL;
  wndclass.lpszClassName = szAppName;

  if (!RegisterClass(&wndclass)) {
    MessageBox(NULL, TEXT("Programm setzt WIN NT voraus"), szAppName, MB_ICONERROR);
    return 0;
  }

  hwnd = CreateWindow(

      szAppName, TEXT("Unser erstes Fenster fuer mb3b"), WS_OVERLAPPEDWINDOW, 100, 50,
      /*CW_USEDEFAULT,
      CW_USEDEFAULT,*/
      1000, 600, NULL, NULL, hInstance, NULL

  );

  stift[0] = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));      // durchgezogen, dicke 1,
  stift[1] = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));    // durchgezogen, dicke 1, rpt
  stift[2] = CreatePen(PS_SOLID, 1, RGB(0, 255, 0));    // durchgezogen, dicke 1, gruen
  stift[3] = CreatePen(PS_SOLID, 1, RGB(0, 0, 255));    // durchgezogen, dicke 1, Blau?
  stift[4] = CreatePen(PS_SOLID, 1, RGB(255, 255, 0));  // durchgezogen, dicke 1, gelb

  stift1 = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));   // durchgezogen, dicke 1, schwarz
  stift2 = CreatePen(PS_DASH, 1, RGB(0, 255, 0));  // gestrichelt, dicke 1, gruen
  stift3 = CreatePen(PS_DOT, 1, RGB(0, 0, 255));

  hintergrund1 = CreateSolidBrush(RGB(0, 255, 0));              // Flaeche gruen
  hintergrund2 = CreateHatchBrush(HS_CROSS, RGB(255, 255, 0));  // ueber kreuz gelb
  hintergrund[0] = CreateSolidBrush(RGB(255, 255, 255));        // Flaeche schwarz
  hintergrund[1] = CreateSolidBrush(RGB(0, 255, 0));            // Flaeche gruen
  hintergrund[2] = CreateSolidBrush(RGB(255, 255, 0));          // Flaeche gelb

  ShowWindow(hwnd, iCmdShow);

  while (GetMessage(&msg, NULL, 0, 0)) {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
  return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
  switch (message) {
    case WM_PAINT:
      // InvalidateRect(hwnd, NULL, TRUE);  //leert den Bildschirm
      hdc = BeginPaint(hwnd, &ps);  // beginnt, neu zu zeichnen
      GetClientRect(hwnd, &rect);
      unterprog1(hwnd, hdc, rect);

      return 0;

    case WM_DESTROY:
      PostQuitMessage(0);
      return 0;
    case WM_LBUTTONDOWN:                     // linke Maustaste
      Robbi[0].xe = (double)LOWORD(lParam);  // Neue Pos aus Mauskoordinaten
      Robbi[0].ye = (double)HIWORD(lParam);
      movexy(hwnd);
      return 0;
    case WM_KEYDOWN:  // Tastatursteuerung
      switch (wParam) {
        //
        case 'A':
          Robbi[0].xe -= 10;
          movexy(hwnd);
          break;  // Pfeiltaste links
        case 'S':
          Robbi[0].ye += 10;
          movexy(hwnd);
          break;  // Pfeiltaste unten
        case 'D':
          Robbi[0].xe += 10;
          movexy(hwnd);
          break;  // Pfeiltaste rechts
        case 'W':
          Robbi[0].ye -= 10;
          movexy(hwnd);
          break;  // Pfeiltaste oben
        case VK_LEFT:
          Robbi[1].xe -= 10;
          movexy(hwnd);
          break;  // Pfeiltaste links
        case VK_DOWN:
          Robbi[1].ye += 10;
          movexy(hwnd);
          break;  // Pfeiltaste unten
        case VK_RIGHT:
          Robbi[1].xe += 10;
          movexy(hwnd);
          break;  // Pfeiltaste rechts
        case VK_UP:
          Robbi[1].ye -= 10;
          movexy(hwnd);
          break;  // Pfeiltaste oben
        case 'X':
          Robbi[0].ye = Robbi[0].ye - 10;
          Robbi[0].xe = Robbi[0].xe + 10;
          movexy(hwnd);
          break;  // Eingabe von "d" ->diagonal
        case 'G':
          wst[3].greifer = 1;
          break;  // Werkstueck 4 'Gegriffen
        case 'L':
          wst[3].greifer = 0;
          break;  // Werkstueck 4 losgelassen
        case 'R':
          wst[3].winkel = wst[3].winkel + 5;
          movexy(hwnd);
          break;  // Werkstueck 4 drehen
        case '1':
          Robbi[0].linie_ein = 1;
          break;  // Spruehdose ein
        case '0':
          Robbi[0].linie_ein = 0;
          break;  // Spruehdose aus
      }
  }

  return DefWindowProc(hwnd, message, wParam, lParam);
}

void movexy(HWND hwnd) {
  char wtext[80];
  UpdateWindow(hwnd);
  sprintf(wtext, "Aktuelle Pos.: x = %3.2lf, y = %3.2lf", Robbi[0].xe, Robbi[0].ye);
  SetWindowText(hwnd, wtext);  // Titelleiste akt.
  // MoveToEx(hdc, (int)xa, (int)ya, NULL);
  // LineTo(hdc, (int)xe, (int)ye);
  bewegen(hwnd);
}
