// 2026/03/17 10:46:22 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


class Application;
class Display;
class IGrid;
class MainWindow;
class PanelErrors;
class PanelConfig;
class PanelCalc;
class WindowCursors;
struct AutoCursors;
class PanelDebug;


extern AutoCursors   *TheAutoCursors;
extern Application   *TheApp;
extern Display       *TheDisplay;
extern IGrid         *TheGrid;
extern MainWindow    *TheMainWindow;

extern PanelConfig   *ThePanelConfig;
extern PanelCalc     *ThePanelCalc;
extern WindowCursors *TheWindowCursors;     // Настройки курсоров

extern PanelErrors   *ThePanelErrors;

extern PanelDebug    *ThePanelDebug;
