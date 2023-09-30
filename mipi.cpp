#include "mipi.h"

void create_mipi_packet(uint8_t *mipi_packet, uint8_t data_type, uint8_t vc, uint8_t data_id, uint32_t paylod)
{
    // Create the MIPI DSI data packet
    // uint8_t mipi_packet[8];
    mipi_packet[0] = 0x1C;                    // Start of Packet (SoT)
    mipi_packet[1] = ((vc << 4) | data_type); // Data Type and VC
    mipi_packet[2] = data_id;                 // Data Identifier
    mipi_packet[3] = (payload >> 24) & 0xFF;  // Data Payload (MSB)
    mipi_packet[4] = (payload >> 16) & 0xFF;
    mipi_packet[5] = (payload >> 8) & 0xFF;
    mipi_packet[6] = payload & 0xFF; // Data Payload (LSB)
    mipi_packet[7] = 0xFC;           // End of Packet (EoT)
}

void write_mipi(uint8_t *packet) {
    write_differential_pair()
}
