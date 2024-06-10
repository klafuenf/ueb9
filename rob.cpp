//rob.cpp
#include "rob.h"

rob::rob()
{
};

rob::~rob()
{
};

void rob::getgeo(const char* dat1, const char* dat2)
{
    //arm::anz_p = punkte_r(arm::Arm, dat1);
    arm_1.getgeo(dat1);
    arm_2.getgeo(dat2);
}


void rob::calc(double* sollpunkt_0)
{
    double sollpunkt[2], alpha1[2], alpha2[2], drehpunkt[2], shiftpunkt[2];
    //int kn = 0;  // 0: linksknickung 1: rechtsknickung
    char wtext[200];

    sollpunkt[0] = sollpunkt_0[0] - rob::origin[0];
    sollpunkt[1] = sollpunkt_0[1] - rob::origin[1];

    rob::anz_loes = SuchenWinkel(sollpunkt, arm_1.l, arm_2.l, alpha1, alpha2);

    if (rob::anz_loes >= 1)
    {
        drehpunkt[0] = 0;
        drehpunkt[1] = 0;

        arm_1.calc(alpha1[rob::knickung], drehpunkt, rob::origin);

        shiftpunkt[0] = arm_1.Armn[arm_1.anz_p - 1][0];
        shiftpunkt[1] = arm_1.Armn[arm_1.anz_p - 1][1];

        arm_2.calc(alpha2[rob::knickung], drehpunkt, shiftpunkt);

        rob::pos[0] = arm_2.Armn[arm_2.anz_p - 1][0];
        rob::pos[1] = arm_2.Armn[arm_2.anz_p - 1][1];
    } // anz_loes..

    else
    {
        //sprintf(wtext, "Die Pos x = %3.2lf, y = %3.2lf ist nicht erreichbar",sollpunkt_0[0], sollpunkt_0[0]);
        //SetWindowText(hwnd, wtext); //Titelleiste akt.
    }


    if (rob::linie_ein == 1)
    {
        rob::linien[rob::l][1][0] = rob::pos[0];
        rob::linien[rob::l][1][1] = rob::pos[1];
    }
}


void rob::show()
{
    int i;
    SelectObject(hdc, stift3);
    for (i = 0; i <= rob::l; i++)
    {
        MoveToEx(hdc, (int)rob::linien[i][0][0], (int)rob::linien[i][0][1], NULL);
        LineTo(hdc, (int)rob::linien[i][1][0], (int)rob::linien[i][1][1]);
    }
    SelectObject(hdc, stift1);


    arm_1.show();
    arm_2.show();
}

double rob::getposx()
{
    return (rob::pos[0]);
}

double rob::getposy()
{
    return (rob::pos[1]);
}

void rob::set_knick(int k)
{
    rob::knickung = k;
}

void rob::set_color(int f1, int f2)
{
    arm_1.set_line_color(f1);
    arm_2.set_line_color(f2);
}

void rob::init_linie()
{
    rob::l++;
    rob::linien[rob::l][0][0] = rob::getposx();
    rob::linien[rob::l][0][1] = rob::getposy();
    rob::linien[rob::l][1][0] = rob::getposx();
    rob::linien[rob::l][1][1] = rob::getposy();
}


void rob::set_anfang_zu_ende()
{
    rob::xa = rob::xe;
    rob::ya = rob::ye;
    rob::origin_a[0] = rob::origin_e[0];
    rob::origin_a[1] = rob::origin_e[1];
}
