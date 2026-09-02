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
class PageReports;
class PageSettings;
class PageTables;
class PageArchive;

class PageTests;
class PanelCategory;
class PanelTests;
class PanelViewTest;
class PageChannelB;

class NotebookDebug;

class PanelRight;

extern PageReports   *ThePageReports;
extern PageSettings  *ThePageSettings;
extern PageTables    *ThePageTables;
extern PageArchive   *ThePageArchive;
extern PageTests     *ThePageTests;
extern PageDebug     *ThePageDebug;
extern PageMeasures  *ThePageMeasures;

extern PanelCategory *ThePanelCategory;
extern PanelTests    *ThePanelTests;
extern PanelViewTest *ThePanelViewTest;
extern PageChannelB  *ThePageChannelB;

extern NotebookDebug *TheNotebookDebug;

extern PanelRight    *ThePanelRight;


extern bool need_write_data_to_file;            // Записать считанные с АЦП значения в файл
