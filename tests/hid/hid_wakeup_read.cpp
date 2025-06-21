//hid_wakeup_read.cpp
#include <iostream>
#include <hidapi/hidapi.h>
#include <unistd.h>
#include <cstring>
using namespace std;

const unsigned short vendor_id = 0x0000;
const unsigned short product_id = 0x0013;
unsigned char buf[65] = {0}; // Make sure this is big enough: 1 for Report ID + 64 for data
unsigned char cmd[65] = {0}; // HID report ID is often 0
unsigned char cmdByte_1 = 0b00000000; //Byte _1 Global to keep the cmd's to LED's
unsigned char cmdByte_2 = 0b00000000; //Byte_2 Global to keep the cmd's to LED's

int main() {
    if (hid_init()) {
        std::cerr << "Error inicializando HIDAPI\n";
        return 1;
    }

    // const unsigned short vendor_id = 0x0000;
    // const unsigned short product_id = 0x0013;

    hid_device* device = hid_open(vendor_id, product_id, nullptr);
    if (!device) {
        std::cerr << "No se pudo abrir el dispositivo HID 0000:0013\n";
        return 1;
    }

    // No bloqueante
    hid_set_nonblocking(device, 1);

    // Enviar paquete para "despertar"
    //Apagar todos los MCDU LED's
    //unsigned char wakeup_1[2] = {0x00, 0x00};
    
    // Enviar paquete para "despertar"
    //Apagar todos los MCDU LED's
    //int result = hid_write(device, wakeup_1, sizeof(wakeup_1));
    cmd[1] = 0;
    cmd[2] = 0;
    //hid_write(handle, cmd, sizeof(cmd));
    int result = hid_write(device, cmd, sizeof(cmd));
    
    if (result < 0) {
        std::cerr << "Error enviando paquete de activación\n";
    } else {
        std::cout << "Paquete de activación enviado.\n";
    }
    std::cout << "Wait 5 second to light on RDY and FM2.\n";
    sleep(5);
     
    //Encender MCDU LED's RDY y FM2
    
    
   //unsigned char wakeup_2 [2] = {0x40, 0x01};
    //result = hid_write(device, wakeup_2, sizeof(wakeup_2));
    cmd[1] = 0x40; //RDY led
    cmd[2] = 0x01; // FM2 led
    result = hid_write(device, cmd, sizeof(cmd));
    if (result < 0) {
        std::cerr << "Error enviando paquete de activación\n";
    } else {
        std::cout << "Paquete de activación enviado.\n";
    }

    // Leer datos
    unsigned char buffer[64];
    unsigned char previous[64] = {0}; // Estado anterior

    cout << "Comprobando codigo teclas MCDU" << endl <<
    "Pulsar OVFY para salir" << endl;
    
    // while (true) {
    while (buffer[0] != 0x02)
    { // MCDU OVFY Key
        int res = hid_read(device, buffer, sizeof(buffer));
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
    result = hid_write(device, cmd, sizeof(cmd));
    
    if (result < 0) {
        std::cerr << "Error enviando paquete de activación\n";
    } else {
        std::cout << "Paquete de activación enviado.\n";
    }

    std::cout << "\n MCDU LED's OFF. \nWait 2 seconds to exit.\n";
    sleep(2);
    
    hid_close(device);
    hid_exit();
    return 0;
}
