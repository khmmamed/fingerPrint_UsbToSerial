#include "fingerPrint.h"
#include <stdio.h>
#include "serial.h"

serial Serial;

#define SERIAL_WRITE(...) Serial.Write(__VA_ARGS__);
#define SERIAL_WRITE_U16(v) SERIAL_WRITE((uint8_t)(v>>8)); SERIAL_WRITE((uint8_t)(v & 0xFF));

#define GET_CMD_PACKET(...)\
        uint8_t data[] = {__VA_ARGS__};\
        fingerPrint_packet packet(FINGERPRINT_COMMANDPACKET, sizeof(data), data);\
        getStructurePacket(packet);\
        return 0;
#define SEND_CMD_PACKET(...) GET_CMD_PACKET(__VA_ARGS__); return packet.data[0];


fingerPrint::fingerPrint(uint32_t password ){
  thePassword = password;
  theAddress  = 0xFFFFFFFF;
}

bool fingerPrint::verifyPassword(void){
  return checkPassword() == FINGERPRINT_OK ;
}

uint8_t fingerPrint::checkPassword(void) {
  GET_CMD_PACKET(FINGERPRINT_VERIFYPASSWORD,
                  (uint8_t)(thePassword >> 24), (uint8_t)(thePassword >> 16),
                  (uint8_t)(thePassword >> 8), (uint8_t)(thePassword & 0xFF));
  if (packet.data[0] == FINGERPRINT_OK)
    return FINGERPRINT_OK;
  else
    return FINGERPRINT_PACKETRECIEVEERR;
}

uint8_t fingerPrint::getImage(void) {
  SEND_CMD_PACKET(FINGERPRINT_GETIMAGE);
}

uint8_t fingerPrint::image2Tz(uint8_t slot){
  SEND_CMD_PACKET(FINGERPRINT_IMAGE2TZ,slot);
};

uint8_t fingerPrint::createModel(void){
  SEND_CMD_PACKET(FINGERPRINT_REGMODEL);
}

uint8_t fingerPrint::emptyDatabase(void) {
  SEND_CMD_PACKET(FINGERPRINT_EMPTY);
}

uint8_t fingerPrint::getModel(void) {
  SEND_CMD_PACKET(FINGERPRINT_UPLOAD, 0x01);
}


void fingerPrint::getStructurePacket(const fingerPrint_packet &packet)
{
    SERIAL_WRITE_U16( packet.start_code);
    SERIAL_WRITE( packet.address[0]);
    SERIAL_WRITE( packet.address[1]);
    SERIAL_WRITE( packet.address[2]);
    SERIAL_WRITE( packet.address[3]);
    SERIAL_WRITE( packet.type);
    uint16_t wire_length = packet.length + 2;
    SERIAL_WRITE_U16(wire_length);

    uint16_t sum = ((wire_length)>>8) + ((wire_length)&0xFF) + packet.type;
    for (uint8_t i=0; i< packet.length; i++) {
        SERIAL_WRITE( packet.data[i] );
        sum += packet.data[i];
    }

    SERIAL_WRITE_U16(sum);

    return;
}