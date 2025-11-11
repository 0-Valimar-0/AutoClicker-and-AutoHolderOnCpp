#include <Windows.h>
#include <iostream>
#include <string>

int main() {
    std::cout << "===Program launched===\n";
    std::cout << "Hotkeys:\n";
    std::cout << "  F9 - Hold/Release key '1'\n";
    std::cout << "  F10 - Exit program\n\n";

    /* Виртуальный код левой кнопки мыши (при желании, можно поменять на любую другую клавишу.)
    полный список виртуальных кодов клавиш тут -> https://learn.microsoft.com/ru-ru/windows/win32/inputdev/virtual-key-codes*/
    BYTE targetKey = 0x01;
    
    bool isHolding = false; // Флаг, отслеживающий состояние удержания
    bool running = true;    // Флаг для основного цикла программы

    std::cout << "Press F9 to toggle hold/release" << std::endl;

    // Главный цикл программы
    while (running) {
        // Проверяем состояние "горячих клавиш"

        // Если нажата F10, выходим из программы
        if (GetAsyncKeyState(VK_F10) & 0x8000) {
            running = false;
            std::cout << "Exiting..." << std::endl;
            // Перед выходом отпускаем клавишу, если она была нажата
            if (isHolding) {
                INPUT input = { 0 };
                input.type = INPUT_KEYBOARD;
                input.ki.wVk = targetKey;
                input.ki.dwFlags = KEYEVENTF_KEYUP;
                SendInput(1, &input, sizeof(INPUT));
            }
            Sleep(300);
        }

        // Если нажата F9, переключаем режим удержания
        if (GetAsyncKeyState(VK_MBUTTON) & 0x8000) {
            isHolding = !isHolding; // Меняем состояние на противоположное
            
            INPUT input = { 0 };
            input.type = INPUT_KEYBOARD;
            input.ki.wVk = targetKey;
            
            if (isHolding) {
                // Нажимаем и УДЕРЖИВАЕМ клавишу
                input.ki.dwFlags = 0; // KEYDOWN
                SendInput(1, &input, sizeof(INPUT));
                std::cout << "Key HOLDING..." << std::endl;
            } else {
                // ОТПУСКАЕМ клавишу
                input.ki.dwFlags = KEYEVENTF_KEYUP;
                SendInput(1, &input, sizeof(INPUT));
                std::cout << "Key RELEASED." << std::endl;
            }
            Sleep(300); // Задержка для антидребезга
        }

        // Небольшая пауза, чтобы не нагружать процессор
        Sleep(10);
    }

    return 0;
}
