//mcdu_template.cpp
#include <iostream>
#include <ncursesw/ncurses.h>
#include <hidapi/hidapi.h>

int main()
{
    // Inicializar ncurses
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);
    start_color();
    init_pair(1, COLOR_GREEN, COLOR_BLACK);
    attron(COLOR_PAIR(1));
    printw("Iniciando MCDU...\n");
    refresh();

    // Inicializar HIDAPI
    if (hid_init())
    {
        printw("Error al inicializar HIDAPI.\n");
        refresh();
        getch();
        endwin();
        return 1;
    }
    // Buscar el dispositivo HID de OpenCockpits
    const unsigned short vendor_id = 0x0000;  // Actualiza si es necesario
    const unsigned short product_id = 0x0013; // ID del MCDU OpenCockpits

    hid_device *device = hid_open(vendor_id, product_id, nullptr);
    if (!device)
    {
        printw("No se encontró el dispositivo HID MCDU.\n");
    }
    else
    {
        printw("Dispositivo HID conectado correctamente.\n");
        hid_close(device);
    }
    // Cerrar HIDAPI y ncurses
    hid_exit();
    refresh();
    getch();
    endwin();
    return 0;
}
