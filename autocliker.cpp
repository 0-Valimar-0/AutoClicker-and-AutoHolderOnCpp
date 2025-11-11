#include <iostream>
#include <Windows.h>
#include <thread>
#include <chrono>

// Функция для эмуляции нажатия клавиши W с использованием скан-кода. При желании, можно забиндить другую клавишу
void PressKey(BYTE scanCode) {
    INPUT input[2] = {};

    // Нажатие клавиши
    input[0].type = INPUT_KEYBOARD;
    input[0].ki.dwFlags = KEYEVENTF_SCANCODE;
    input[0].ki.wScan = scanCode;

    // Отпускание клавиши
    input[1].type = INPUT_KEYBOARD;
    input[1].ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;
    input[1].ki.wScan = scanCode;

    // Отправка событий
    SendInput(2, input, sizeof(INPUT));
}

int main() {
    std::cout << "Автокликер запущен. Нажмите HOME для начала/остановки.\n";
    std::cout << "Эмулируется нажатие клавиши W.\n";

    bool clicking = false;
    BYTE targetScanCode = 0x01; // Cкан-код клавиши мыши
    /*таблица со скан-кодами клавиш -> https://learn.microsoft.com/ru-ru/windows/win32/inputdev/virtual-key-codes*/

    while (true) {
        // Проверка нажатия HOME для активации
        if (GetAsyncKeyState(VK_HOME) & 0x8000) {
            clicking = !clicking;
            std::cout << (clicking ? "Запуск..." : "Остановка...") << "\n";
            Sleep(500); // Задержка для избежания двойного срабатывания
        }

        if (clicking) {
            PressKey(targetScanCode);
            std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Интервал между нажатиями
        }

        Sleep(10); // Уменьшение нагрузки на CPU
    }

    return 0;
}
