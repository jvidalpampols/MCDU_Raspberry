//hid_read.cpp
#include <iostream>
#include <hidapi/hidapi.h>
#include <unistd.h>
#include <cstring>
#include "hidTools.h"
//#include "hidTools.h"

using namespace std;

// const unsigned short vendor_id = 0x0000;
// const unsigned short product_id = 0x0013;

int main() {
   
    if (init_hid()!=true) {
        std::cerr << "Error inicializando HIDAPI\n";
        return 1;
    }
    else{
        cout << "\nHID Device init OK\n"
             << endl;
    }

    //getchar();

    // Encender MCDU LED's RDY y FM2

    // Leer datos
    unsigned char buffer[64];
    unsigned char previous[64] = {0}; // Estado anterior

    cout << "Comprobando codigo teclas MCDU" << endl <<
    "Pulsar OVFY para salir" << endl;
    
    // while (true) {
    while (buffer[0] != 0x02)
    { // MCDU OVFY Key
        int res = hid_read(handle, buffer, sizeof(buffer));
        if (res > 0)
        {
            if (memcmp(buffer, previous, res) != 0)
            {
                std::cout << "Cambio detectado: ";
                for (int i = 0; i < res; ++i)
                {
                    printf("%02X ", buffer[i]);
                    previous[i] = buffer[i];
                }
                std::cout << std::endl;
            }
            // Si son iguales, se ignora
        }
        usleep(5000); // evitar 100% CPU
    }//while(!OVFY)
    
    //Turn MCDU LED's OFF
    cmd[1] = 0;
    cmd[2] = 0;
    //hid_write(handle, cmd, sizeof(cmd));
    int result = hid_write(handle, cmd, sizeof(cmd));
    
    if (result < 0) {
        std::cerr << "Error enviando paquete de activación\n";
    } else {
        std::cout << "Paquete de activación enviado.\n";
    }

    std::cout << "\n MCDU LED's OFF. \nWait 2 seconds to exit.\n";
    sleep(2);
    
    hid_close(handle);
    hid_exit();
    return 0;
}
