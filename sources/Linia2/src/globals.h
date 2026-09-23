// 2026/03/17 10:46:22 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#pragma once


class Application;
class AutoCursors;
class IGrid;
class MainWindow;

extern Application   *TheApp;
extern AutoCursors   *TheAutoCursors;
extern IGrid         *TheGrid;
extern MainWindow    *TheMainWindow;

class PageArchive;
class PageDebug;
class PageMeasures;
class PageTables;
class PageArchive;
class PageSTM32;

class PageTests;
class PageChannelB;

extern PageTables    *ThePageTables;
extern PageArchive   *ThePageArchive;
extern PageTests     *ThePageTests;
extern PageDebug     *ThePageDebug;

extern bool need_write_data_to_file;            // Записать считанные с АЦП значения в файл
