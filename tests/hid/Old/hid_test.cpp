//hid_test.cpp
#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>

int main() {
    const char* device = "/dev/hidraw0";

    // Abrir el dispositivo en modo lectura-escritura binario
    int fd = open(device, O_RDWR);
    if (fd < 0) {
        std::cerr << "Error: No se pudo abrir " << device << std::endl;
        return 1;
    }

    std::cout << "Dispositivo abierto. Enviando comando de inicio..." << std::endl;

    // Enviar un posible byte de activación
    unsigned char init_byte[1] = {0x00}; // Prueba también con 0xF0 o 0xAA si esto no funciona
    ssize_t written = write(fd, init_byte, 1);
    if (written <= 0) {
        std::cerr << "No se pudo enviar el byte de activación." << std::endl;
    }

    std::cout << "Esperando datos del dispositivo..." << std::endl;

    const size_t buffer_size = 16;
    std::vector<unsigned char> buffer(buffer_size);

    while (true) {
        ssize_t bytes_read = read(fd, buffer.data(), buffer_size);
        if (bytes_read > 0) {
            std::cout << "Datos recibidos: ";
            for (int i = 0; i < bytes_read; ++i) {
                std::cout << std::hex << std::setw(2) << std::setfill('0')
                          << static_cast<int>(buffer[i]) << " ";
            }
            std::cout << std::dec << std::endl;
        } else {
            usleep(10000); // 10ms
        }
    }

    close(fd);
    return 0;
}
