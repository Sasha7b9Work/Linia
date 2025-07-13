// 2024/11/02 18:44:27 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


#define dS      5
#define X_0     10

#ifdef WIN32
#define Y(y)  (y)
#define Y_0   25
#define Y_1   65
#define Y_2   105
#define Y_3   145
#define Y_4   185
#else
#define Y(y) ((y) - 15)
#define Y_0  10
#define Y_1  50
#define Y_2  90
#define Y_3  130
#define Y_4 170
#endif

#define Y_1_S (Y_1 - dS)
#define Y_2_S (Y_2 - dS * 2)
#define Y_3_S (Y_3 - dS * 3)
#define Y_4_S (Y_4 - dS * 4)


namespace SD
{
    //  Static Box
    int Y_SB(int);

    // Размеры - dimensions
    namespace D
    {
        // ChooseSoundDialog
        namespace ChooseSound
        {
            wxSize SizeSpin();
            wxPoint PosSpin(int x, int y);
            wxSize SizeCombo();
            wxPoint PosCombo(int x, int y);
            int d();
            int y();
            wxSize Size();
        }

        namespace CommonPanel
        {
            int Delta();
        }

        namespace ColorDialog
        {
            wxSize Size();
            wxSize SizeSlider();
            int Spacer();
        }
    }
}
