#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sys/ioctl.h>
#include <unistd.h>

// Получить текущий размер терминала (столбцы x строки)
void getTerminalSize(int &cols, int &rows) {
    struct winsize w;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == 0) {
        cols = w.ws_col;
        rows = w.ws_row;
    } else {
        cols = 80;
        rows = 24;
    }
}

// Очистить экран и переместить курсор в начало
void clearScreen() {
    std::cout << "\033[2J\033[H";
}

// Переместить курсор в позицию (row, col) - 0-based
void moveCursor(int row, int col) {
    std::cout << "\033[" << row + 1 << ";" << col + 1 << "H";
}

// Получить текущее время в виде строки "HH:MM:SS"
std::string getCurrentTime() {
    auto now = std::chrono::system_clock::now();
    std::time_t now_time = std::chrono::system_clock::to_time_t(now);
    std::tm local_time = *std::localtime(&now_time);
    std::ostringstream oss;
    oss << std::put_time(&local_time, "%H:%M:%S");
    return oss.str();
}

// Нарисовать прямоугольник с текстом по центру
void drawRectangleWithText(int cols, int rows, const std::string &text) {
    // Размеры прямоугольника
    int rectWidth = text.length() + 8;   // + отступы
    int rectHeight = 5;                  // Высота прямоугольника

    // Координаты верхнего левого угла (центрирование)
    int startCol = (cols - rectWidth) / 2;
    int startRow = (rows - rectHeight) / 2;

    // Очищаем экран
    clearScreen();

    // Верхняя граница
    moveCursor(startRow, startCol);
    std::cout << '+';
    for (int i = 0; i < rectWidth - 2; ++i) std::cout << '-';
    std::cout << '+';

    // Пустая строка
    moveCursor(startRow + 1, startCol);
    std::cout << '|';
    for (int i = 0; i < rectWidth - 2; ++i) std::cout << ' ';
    std::cout << '|';

    // Строка с текстом (по центру)
    int textCol = startCol + (rectWidth - text.length()) / 2;
    moveCursor(startRow + 2, textCol);
    std::cout << text;

    // Пустая строка
    moveCursor(startRow + 3, startCol);
    std::cout << '|';
    for (int i = 0; i < rectWidth - 2; ++i) std::cout << ' ';
    std::cout << '|';

    // Нижняя граница
    moveCursor(startRow + 4, startCol);
    std::cout << '+';
    for (int i = 0; i < rectWidth - 2; ++i) std::cout << '-';
    std::cout << '+';
}

int main() {
    // Отключаем буферизацию вывода
    std::cout << std::unitbuf;

    while (true) {
        int cols, rows;
        getTerminalSize(cols, rows);

        std::string timeStr = getCurrentTime();
        drawRectangleWithText(cols, rows, timeStr);

        // Ждём 1 секунду
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    return 0;
}
