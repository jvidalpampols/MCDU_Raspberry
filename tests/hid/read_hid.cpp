#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <unistd.h>

int main() {
    const char* device = "/dev/hidraw0"; // Asegúrate de que es el correcto
    std::ifstream hid(device, std::ios::binary);

    if (!hid.is_open()) {
        std::cerr << "Error: No se pudo abrir " << device << std::endl;
        return 1;
    }

    std::cout << "Leyendo de " << device << "..." << std::endl;

    const size_t buffer_size = 16;
    std::vector<unsigned char> buffer(buffer_size);

    while (true) {
        hid.read(reinterpret_cast<char*>(buffer.data()), buffer_size);
        std::streamsize bytes_read = hid.gcount();

        if (bytes_read > 0) {
            std::cout << "Datos recibidos: ";
            for (int i = 0; i < bytes_read; ++i) {
                std::cout << std::hex << std::setw(2) << std::setfill('0')
                          << static_cast<int>(buffer[i]) << " ";
            }
            std::cout << std::dec << std::endl;
        } else {
            // Espera breve para evitar uso excesivo de CPU
            usleep(10000); // 10ms
        }
    }

    hid.close();
    return 0;
}
