#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sys/ioctl.h>
#include <unistd.h>

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

void clearScreen() {
    std::cout << "\033[2J\033[H";
}

void moveCursor(int row, int col) {
    std::cout << "\033[" << row + 1 << ";" << col + 1 << "H";
}

std::string getCurrentTime() {
    auto now = std::chrono::system_clock::now();
    std::time_t now_time = std::chrono::system_clock::to_time_t(now);
    std::tm local_time = *std::localtime(&now_time);
    std::ostringstream oss;
    oss << std::put_time(&local_time, "%H:%M:%S");
    return oss.str();
}

void drawRectangleWithText(int cols, int rows, const std::string &text) {
    int rectWidth = text.length() + 8;
    int rectHeight = 5;

    int startCol = (cols - rectWidth) / 2;
    int startRow = (rows - rectHeight) / 2;

    // Используем альтернативный буфер — переключаемся сразу
    std::cout << "\033[?1049h";  // Включить альтернативный буфер
    clearScreen();

    // Верхняя граница
    moveCursor(startRow, startCol);
    std::cout << '+';
    for (int i = 0; i < rectWidth - 2; ++i) std::cout << '-';
    std::cout << '+';

    moveCursor(startRow + 1, startCol);
    std::cout << '|';
    for (int i = 0; i < rectWidth - 2; ++i) std::cout << ' ';
    std::cout << '|';

    int textCol = startCol + (rectWidth - text.length()) / 2;
    moveCursor(startRow + 2, textCol);
    std::cout << text;

    moveCursor(startRow + 3, startCol);
    std::cout << '|';
    for (int i = 0; i < rectWidth - 2; ++i) std::cout << ' ';
    std::cout << '|';

    moveCursor(startRow + 4, startCol);
    std::cout << '+';
    for (int i = 0; i < rectWidth - 2; ++i) std::cout << '-';
    std::cout << '+';
}

int main() {
    std::cout << std::unitbuf;

    // Обработчик для восстановления основного буфера при завершении
    auto restoreBuffer = []() {
        std::cout << "\033[?1049l" << std::flush;  // Выключить альтернативный буфер
    };

    try {
        while (true) {
            int cols, rows;
            getTerminalSize(cols, rows);
            std::string timeStr = getCurrentTime();
            drawRectangleWithText(cols, rows, timeStr);
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    } catch (...) {
        restoreBuffer();
        throw;
    }

    restoreBuffer();
    return 0;
}
