
/* License:
 *
 * MIT License
 *
 * Copyright (c) 2020 magistermaks
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/* Options: (define to enable, only one can be selected)
 * TERMCO_NONE            - Define only base. (only __TERMCO_ANSI_* codes)
 * TERMCO_STD_ANSI        - Use standard ANSI color codes.
 * TERMCO_WIN_ANSI        - Use Windows 10 flavor of ANSI.
 * TERMCO_COLOR_ID        - Defines color ids and termco_get() function.
 * TERMCO_IMPLEMENTATION  - If defined this file will act as .c not .h
 * TERMCO_WINDOWS         - Don't check build environment, assume windows.
 * TERMCO_LINUX           - Don't check build environment, assume linux-like.
 */

/* Usage:
 *  S_RESET - Reset all text attributes.
 *  F_<COLOR>_RESET - Reset foreground color.
 *  B_<COLOR>_RESET - Reset background color.
 *  S_<BOLD/UNDERLINE/BLINK/DIM>_RESET - Reset text feature.
 *  F_<COLOR> - Set foreground color.
 *  B_<COLOR> - Set background color.
 *  S_<BOLD/UNDERLINE/BLINK/DIM> - Set text feature.
 *  termco_init() to enable, termco_exit() to disable
 *
 * Colors:
 *  WHITE, BLACK, GRAY, RED, GREEN, YELLOW, BLUE, MAGENTA, CYAN.
 *
 * Light/Strong variants:
 *  L_GRAY, L_RED, L_GREEN, L_YELLOW, L_BLUE, L_MAGENTA, L_CYAN
 */

/* Example:
 *  int main() {
 *    termco_init();
 *    std::cout << F_RED << " red " << F_BLUE << S_BOLD << " bold + blue " << F_RESET << " just bold  " << S_RESET << " default " << std::endl;
 *    termco_exit();
 *    return 0;
 *  }
 */

/* Versions:
 * 1.0 - Initial version.
 */

#ifdef TERMCO_WINDOWS
#define __TERMCO_WINDOWS
#elif defined TERMCO_LINUX
#define __TERMCO_LINUX
#else
#ifdef _WIN32
#define __TERMCO_WINDOWS
#elif defined __APPLE__
#define __TERMCO_LINUX
#elif defined __linux__
#define __TERMCO_LINUX
#elif defined __unix__
#define __TERMCO_LINUX
#else
#error "Build environment test failed. Define NBI_LIB_WINDOWS or NBI_LIB_LINUX."
#endif
#endif

#ifndef TERMCO_H
#define TERMCO_H

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __TERMCO_WINDOWS
#include <windows.h>
#endif

#ifdef TERMCO_IMPLEMENTATION

void termco_exit() {

#ifdef __TERMCO_WINDOWS
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD mode = 0;
    GetConsoleMode(handle, &mode);
    mode &= ~4;
    SetConsoleMode(handle, mode);
#endif

}

void termco_init() {

#ifdef __TERMCO_WINDOWS
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD mode = 0;
    GetConsoleMode(handle, &mode);
    mode |= 4;
    SetConsoleMode(handle, mode);
#endif
    atexit( termco_exit );
    return;
}

#else

void termco_init();
void termco_exit();

#endif

#ifndef __TERMCO_ANSI_BASE
#define __TERMCO_ANSI_BASE

#define __TERMCO_ANSI_FOREGROUND_BLACK "\x1b[30m"
#define __TERMCO_ANSI_FOREGROUND_RED "\x1b[31m"
#define __TERMCO_ANSI_FOREGROUND_GREEN "\x1b[32m"
#define __TERMCO_ANSI_FOREGROUND_YELLOW "\x1b[33m"
#define __TERMCO_ANSI_FOREGROUND_BLUE "\x1b[34m"
#define __TERMCO_ANSI_FOREGROUND_MAGENTA "\x1b[35m"
#define __TERMCO_ANSI_FOREGROUND_CYAN "\x1b[36m"
#define __TERMCO_ANSI_FOREGROUND_RESET "\x1b[39m"
#define __TERMCO_ANSI_FOREGROUND_LIGHT_GRAY "\x1b[37m"
#define __TERMCO_ANSI_FOREGROUND_DARK_GRAY "\x1b[90m"
#define __TERMCO_ANSI_FOREGROUND_LIGHT_RED "\x1b[91m"
#define __TERMCO_ANSI_FOREGROUND_LIGHT_GREEN "\x1b[92m"
#define __TERMCO_ANSI_FOREGROUND_LIGHT_YELLOW "\x1b[93m"
#define __TERMCO_ANSI_FOREGROUND_LIGHT_BLUE "\x1b[94m"
#define __TERMCO_ANSI_FOREGROUND_LIGHT_MAGENTA "\x1b[95m"
#define __TERMCO_ANSI_FOREGROUND_LIGHT_CYAN "\x1b[96m"
#define __TERMCO_ANSI_FOREGROUND_WHITE "\x1b[97m"
#define __TERMCO_ANSI_BACKGROUND_RESET "\x1b[49m"
#define __TERMCO_ANSI_BACKGROUND_BLACK "\x1b[40m"
#define __TERMCO_ANSI_BACKGROUND_RED "\x1b[41m"
#define __TERMCO_ANSI_BACKGROUND_GREEN "\x1b[42m"
#define __TERMCO_ANSI_BACKGROUND_YELLOW "\x1b[43m"
#define __TERMCO_ANSI_BACKGROUND_BLUE "\x1b[44m"
#define __TERMCO_ANSI_BACKGROUND_MAGENTA "\x1b[45m"
#define __TERMCO_ANSI_BACKGROUND_CYAN "\x1b[46m"
#define __TERMCO_ANSI_BACKGROUND_LIGHT_GRAY "\x1b[47m"
#define __TERMCO_ANSI_BACKGROUND_DARK_GRAY "\x1b[100m"
#define __TERMCO_ANSI_BACKGROUND_LIGHT_RED "\x1b[101m"
#define __TERMCO_ANSI_BACKGROUND_LIGHT_GREEN "\x1b[102m"
#define __TERMCO_ANSI_BACKGROUND_LIGHT_YELLOW "\x1b[103m"
#define __TERMCO_ANSI_BACKGROUND_LIGHT_BLUE "\x1b[104m"
#define __TERMCO_ANSI_BACKGROUND_LIGHT_MAGENTA "\x1b[105m"
#define __TERMCO_ANSI_BACKGROUND_LIGHT_CYAN "\x1b[106m"
#define __TERMCO_ANSI_BACKGROUND_WHITE "\x1b[107m"
#define __TERMCO_ANSI_SPECIAL_RESET_ALL "\x1b[0m"
#define __TERMCO_ANSI_SPECIAL_BOLD "\x1b[1m"
#define __TERMCO_ANSI_SPECIAL_BOLD_RESET "\x1b[21m"
#define __TERMCO_ANSI_SPECIAL_DIM "\x1b[2m"
#define __TERMCO_ANSI_SPECIAL_DIM_RESET "\x1b[22m"
#define __TERMCO_ANSI_SPECIAL_ITALIC "\x1b[3m"
#define __TERMCO_ANSI_SPECIAL_ITALIC_RESET "\x1b[23m"
#define __TERMCO_ANSI_SPECIAL_UNDERLINED "\x1b[4m"
#define __TERMCO_ANSI_SPECIAL_UNDERLINED_RESET "\x1b[24m"
#define __TERMCO_ANSI_SPECIAL_BLINK "\x1b[5m"
#define __TERMCO_ANSI_SPECIAL_BLINK_RESET "\x1b[25m"
#define __TERMCO_ANSI_SPECIAL_HIDDEN "\x1b[8m"
#define __TERMCO_ANSI_SPECIAL_HIDDEN_RESET "\x1b[28m"
#define __TERMCO_WINDOWS_ANSI_FOREGROUND_WHITE "\x1b[37m"
#define __TERMCO_WINDOWS_ANSI_BACKGROUND_WHITE "\x1b[47m"
#define __TERMCO_NOT_SUPPORTED ""

#endif

#ifndef TERMCO_WIN_ANSI
#ifndef TERMCO_STD_ANSI
#ifndef TERMCO_NONE
#error "No profile selected! Define TERMCO_WIN_ANSI, TERMCO_STD_ANSI or TERMCO_NONE!"
#endif
#endif
#endif

#ifdef TERMCO_NONE
#ifdef TERMCO_COLOR_ID
#error "TERMCO_COLOR_ID requires TERMCO_WIN_ANSI or TERMCO_STD_ANSI!"
#endif
#endif

#ifdef TERMCO_STD_ANSI

#ifndef __TERMCO_STD_ANSI
#define __TERMCO_STD_ANSI

#define F_GRAY __TERMCO_ANSI_FOREGROUND_DARK_GRAY
#define F_BLACK __TERMCO_ANSI_FOREGROUND_BLACK
#define F_RED __TERMCO_ANSI_FOREGROUND_RED
#define F_GREEN __TERMCO_ANSI_FOREGROUND_GREEN
#define F_YELLOW __TERMCO_ANSI_FOREGROUND_YELLOW
#define F_BLUE __TERMCO_ANSI_FOREGROUND_BLUE
#define F_MAGENTA __TERMCO_ANSI_FOREGROUND_MAGENTA
#define F_CYAN __TERMCO_ANSI_FOREGROUND_CYAN
#define F_RESET __TERMCO_ANSI_FOREGROUND_RESET

#define B_GRAY __TERMCO_ANSI_BACKGROUND_DARK_GRAY
#define B_BLACK __TERMCO_ANSI_BACKGROUND_BLACK
#define B_RED __TERMCO_ANSI_BACKGROUND_RED
#define B_GREEN __TERMCO_ANSI_BACKGROUND_GREEN
#define B_YELLOW __TERMCO_ANSI_BACKGROUND_YELLOW
#define B_BLUE __TERMCO_ANSI_BACKGROUND_BLUE
#define B_MAGENTA __TERMCO_ANSI_BACKGROUND_MAGENTA
#define B_CYAN __TERMCO_ANSI_BACKGROUND_CYAN
#define B_RESET __TERMCO_ANSI_BACKGROUND_RESET

#define S_BOLD __TERMCO_ANSI_SPECIAL_BOLD
#define S_BOLD_RESET __TERMCO_ANSI_SPECIAL_BOLD_RESET
#define S_UNDERLINED __TERMCO_ANSI_SPECIAL_UNDERLINED
#define S_UNDERLINED_RESET __TERMCO_ANSI_SPECIAL_UNDERLINED_RESET
#define S_BLINK __TERMCO_ANSI_SPECIAL_BLINK
#define S_BLINK_RESET __TERMCO_ANSI_SPECIAL_BLINK_RESET
#define S_DIM __TERMCO_ANSI_SPECIAL_DIM
#define S_DIM_RESET __TERMCO_ANSI_SPECIAL_DIM_RESET

#define F_L_GRAY __TERMCO_ANSI_FOREGROUND_LIGHT_GRAY
#define F_L_RED __TERMCO_ANSI_FOREGROUND_LIGHT_RED
#define F_L_GREEN __TERMCO_ANSI_FOREGROUND_LIGHT_GREEN
#define F_L_YELLOW __TERMCO_ANSI_FOREGROUND_LIGHT_YELLOW
#define F_L_BLUE __TERMCO_ANSI_FOREGROUND_LIGHT_BLUE
#define F_L_MAGENTA __TERMCO_ANSI_FOREGROUND_LIGHT_MAGENTA
#define F_L_CYAN __TERMCO_ANSI_FOREGROUND_LIGHT_CYAN

#define B_L_GRAY __TERMCO_ANSI_BACKGROUND_LIGHT_GRAY
#define B_L_RED __TERMCO_ANSI_BACKGROUND_LIGHT_RED
#define B_L_GREEN __TERMCO_ANSI_BACKGROUND_LIGHT_GREEN
#define B_L_YELLOW __TERMCO_ANSI_BACKGROUND_LIGHT_YELLOW
#define B_L_BLUE __TERMCO_ANSI_BACKGROUND_BLUE
#define B_L_MAGENTA __TERMCO_ANSI_BACKGROUND_LIGHT_MAGENTA
#define B_L_CYAN __TERMCO_ANSI_BACKGROUND_LIGHT_CYAN

#define S_RESET __TERMCO_ANSI_SPECIAL_RESET_ALL

#endif

#endif

#ifdef TERMCO_WIN_ANSI

#ifndef __TERMCO_WIN_ANSI
#define __TERMCO_WIN_ANSI

#define F_GRAY __TERMCO_ANSI_FOREGROUND_DARK_GRAY
#define F_BLACK __TERMCO_ANSI_FOREGROUND_BLACK
#define F_RED __TERMCO_ANSI_FOREGROUND_RED
#define F_GREEN __TERMCO_ANSI_FOREGROUND_GREEN
#define F_YELLOW __TERMCO_ANSI_FOREGROUND_YELLOW
#define F_BLUE __TERMCO_ANSI_FOREGROUND_BLUE
#define F_MAGENTA __TERMCO_ANSI_FOREGROUND_MAGENTA
#define F_CYAN __TERMCO_ANSI_FOREGROUND_CYAN
#define F_RESET __TERMCO_ANSI_FOREGROUND_RESET

#define B_GRAY __TERMCO_ANSI_BACKGROUND_DARK_GRAY
#define B_BLACK __TERMCO_ANSI_BACKGROUND_BLACK
#define B_RED __TERMCO_ANSI_BACKGROUND_RED
#define B_GREEN __TERMCO_ANSI_BACKGROUND_GREEN
#define B_YELLOW __TERMCO_ANSI_BACKGROUND_YELLOW
#define B_BLUE __TERMCO_ANSI_BACKGROUND_BLUE
#define B_MAGENTA __TERMCO_ANSI_BACKGROUND_MAGENTA
#define B_CYAN __TERMCO_ANSI_BACKGROUND_CYAN
#define B_RESET __TERMCO_ANSI_BACKGROUND_RESET

#define S_BOLD __TERMCO_ANSI_SPECIAL_BOLD
#define S_BOLD_RESET __TERMCO_ANSI_SPECIAL_BOLD_RESET
#define S_UNDERLINED __TERMCO_ANSI_SPECIAL_UNDERLINED
#define S_UNDERLINED_RESET __TERMCO_ANSI_SPECIAL_UNDERLINED_RESET
#define S_BLINK __TERMCO_ANSI_SPECIAL_BLINK
#define S_BLINK_RESET __TERMCO_ANSI_SPECIAL_BLINK_RESET
#define S_DIM __TERMCO_ANSI_SPECIAL_DIM
#define S_DIM_RESET __TERMCO_ANSI_SPECIAL_DIM_RESET

#define F_L_GRAY __TERMCO_WINDOWS_ANSI_FOREGROUND_WHITE
#define F_L_RED __TERMCO_ANSI_FOREGROUND_LIGHT_RED
#define F_L_GREEN __TERMCO_ANSI_FOREGROUND_LIGHT_GREEN
#define F_L_YELLOW __TERMCO_ANSI_FOREGROUND_LIGHT_YELLOW
#define F_L_BLUE __TERMCO_ANSI_FOREGROUND_LIGHT_BLUE
#define F_L_MAGENTA __TERMCO_ANSI_FOREGROUND_LIGHT_MAGENTA
#define F_L_CYAN __TERMCO_ANSI_FOREGROUND_LIGHT_CYAN

#define B_L_GRAY __TERMCO_WINDOWS_ANSI_BACKGROUND_WHITE
#define B_L_RED __TERMCO_ANSI_BACKGROUND_LIGHT_RED
#define B_L_GREEN __TERMCO_ANSI_BACKGROUND_LIGHT_GREEN
#define B_L_YELLOW __TERMCO_ANSI_BACKGROUND_LIGHT_YELLOW
#define B_L_BLUE __TERMCO_ANSI_BACKGROUND_BLUE
#define B_L_MAGENTA __TERMCO_ANSI_BACKGROUND_LIGHT_MAGENTA
#define B_L_CYAN __TERMCO_ANSI_BACKGROUND_LIGHT_CYAN

#define S_RESET __TERMCO_ANSI_SPECIAL_RESET_ALL

#endif

#endif

#ifdef TERMCO_COLOR_ID

#ifndef __TERMCO_COLOR_ID
#define __TERMCO_COLOR_ID

#define ID_F_GRAY 0
#define ID_F_BLACK 1
#define ID_F_RED 2
#define ID_F_GREEN 3
#define ID_F_YELLOW 4
#define ID_F_BLUE 5
#define ID_F_MAGENTA 6
#define ID_F_CYAN 7
#define ID_F_RESET 8
#define ID_B_GRAY 9
#define ID_B_BLACK 10
#define ID_B_RED 11
#define ID_B_GREEN 12
#define ID_B_YELLOW 13
#define ID_B_BLUE 14
#define ID_B_MAGENTA 15
#define ID_B_CYAN 16
#define ID_B_RESET 17
#define ID_S_BOLD 18
#define ID_S_BOLD_RESET 19
#define ID_S_UNDERLINED 20
#define ID_S_UNDERLINED_RESET 21
#define ID_S_BLINK 22
#define ID_S_BLINK_RESET 23
#define ID_S_DIM 24
#define ID_S_DIM_RESET 25
#define ID_F_L_GRAY 26
#define ID_F_L_RED 27
#define ID_F_L_GREEN 28
#define ID_F_L_YELLOW 29
#define ID_F_L_BLUE 30
#define ID_F_L_MAGENTA 31
#define ID_F_L_CYAN 32
#define ID_B_L_GRAY 33
#define ID_B_L_RED 34
#define ID_B_L_GREEN 35
#define ID_B_L_YELLOW 36
#define ID_B_L_BLUE 37
#define ID_B_L_MAGENTA 38
#define ID_B_L_CYAN 39
#define ID_S_RESET 40

#endif

#ifndef __TERMCO_COLOR_TYPE
#define __TERMCO_COLOR_TYPE
typedef unsigned char color_t;
#endif

#ifdef TERMCO_IMPLEMENTATION

const char * termco_get( color_t color_id ) {
    static char colors[41][7] = {
        F_GRAY,
        F_BLACK,
        F_RED,
        F_GREEN,
        F_YELLOW,
        F_BLUE,
        F_MAGENTA,
        F_CYAN,
        F_RESET,
        B_GRAY,
        B_BLACK,
        B_RED,
        B_GREEN,
        B_YELLOW,
        B_BLUE,
        B_MAGENTA,
        B_CYAN,
        B_RESET,
        S_BOLD,
        S_BOLD_RESET,
        S_UNDERLINED,
        S_UNDERLINED_RESET,
        S_BLINK,
        S_BLINK_RESET,
        S_DIM,
        S_DIM_RESET,
        F_L_GRAY,
        F_L_RED,
        F_L_GREEN,
        F_L_YELLOW,
        F_L_BLUE,
        F_L_MAGENTA,
        F_L_CYAN,
        B_L_GRAY,
        B_L_RED,
        B_L_GREEN,
        B_L_YELLOW,
        B_L_BLUE,
        B_L_MAGENTA,
        B_L_CYAN,
        S_RESET
    };

    return colors[color_id];
}

#else

const char * termco_get( color_t color_id );

#endif

#endif

#ifdef __cplusplus
}
#endif

#endif /* TERMCO_H */
