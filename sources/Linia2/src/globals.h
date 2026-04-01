// 2026/03/17 10:46:22 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


class Application;
class Display;
class IGrid;
class MainWindow;
class PanelCalc;
class PanelConfig;
class PanelDebug;
class PanelErrors;
class PanelRight;
class WindowCursors;
struct AutoCursors;

extern Application   *TheApp;
extern AutoCursors   *TheAutoCursors;
extern Display       *TheDisplay;
extern IGrid         *TheGrid;
extern MainWindow    *TheMainWindow;
extern PanelConfig   *ThePanelConfig;
extern PanelCalc     *ThePanelCalc;
extern WindowCursors *TheWindowCursors;     // Настройки курсоров
extern PanelDebug    *ThePanelDebug;
extern PanelErrors   *ThePanelErrors;
extern PanelRight    *ThePanelRight;