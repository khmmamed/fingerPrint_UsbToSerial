#ifndef FINGERPRINT_H
#define FINGERPRINT_H

#include <cstdint>
#include <cstring>

//Sensor Commande Codes
#define FINGERPRINT_OK 0x00
#define FINGERPRINT_PACKETRECIEVEERR 0x01
#define FINGERPRINT_NOFINGER 0x02
#define FINGERPRINT_IMAGEFAIL 0x03
#define FINGERPRINT_IMAGEMESS 0x06
#define FINGERPRINT_FEATUREFAIL 0x07
#define FINGERPRINT_NOMATCH 0x08
#define FINGERPRINT_NOTFOUND 0x09
#define FINGERPRINT_ENROLLMISMATCH 0x0A
#define FINGERPRINT_BADLOCATION 0x0B
#define FINGERPRINT_DBRANGEFAIL 0x0C
#define FINGERPRINT_UPLOADFEATUREFAIL 0x0D
#define FINGERPRINT_PACKETRESPONSEFAIL 0x0E
#define FINGERPRINT_UPLOADFAIL 0x0F
#define FINGERPRINT_DELETEFAIL 0x10
#define FINGERPRINT_DBCLEARFAIL 0x11
#define FINGERPRINT_PASSFAIL 0x13
#define FINGERPRINT_INVALIDIMAGE 0x15
#define FINGERPRINT_FLASHERR 0x18
#define FINGERPRINT_INVALIDREG 0x1A
#define FINGERPRINT_ADDRCODE 0x20
#define FINGERPRINT_PASSVERIFY 0x21

#define FINGERPRINT_STARTCODE 0xEF01

#define FINGERPRINT_COMMANDPACKET 0x1
#define FINGERPRINT_DATAPACKET 0x2
#define FINGERPRINT_ACKPACKET 0x7
#define FINGERPRINT_ENDDATAPACKET 0x8

#define FINGERPRINT_TIMEOUT 0xFF
#define FINGERPRINT_BADPACKET 0xFE

#define FINGERPRINT_GETIMAGE 0x01
#define FINGERPRINT_IMAGE2TZ 0x02
#define FINGERPRINT_REGMODEL 0x05
#define FINGERPRINT_STORE 0x06
#define FINGERPRINT_LOAD 0x07
#define FINGERPRINT_UPLOAD 0x08
#define FINGERPRINT_DELETE 0x0C
#define FINGERPRINT_EMPTY 0x0D
#define FINGERPRINT_SETPASSWORD 0x12
#define FINGERPRINT_VERIFYPASSWORD 0x13
#define FINGERPRINT_HISPEEDSEARCH 0x1B
#define FINGERPRINT_TEMPLATECOUNT 0x1D

/*
// Packet Structure EF01 FFFFFFFF 01 00 07 13 00 00 00 00 00 1B 
*/

struct fingerPrint_packet
{
    /* Constructor */
    fingerPrint_packet(uint8_t type, uint16_t length, uint8_t * data){

        this->start_code = FINGERPRINT_STARTCODE;
        this->type = type;
        this->length = length;
        address[0] = 0xFF; address[1] = 0xFF;
        address[2] = 0xFF; address[3] = 0xFF;

        if(length<64)
            memcpy(this->data, data, length);
        else
            memcpy(this->data, data, 64);
    }

    uint16_t start_code; //wakeUp code
    uint8_t address[4];  //password
    uint8_t type;       //type packet
    uint16_t length;    //length packet
    uint8_t data[64];   //raw buffer payload
};

class fingerPrint
{
    private:
    
        uint8_t checkPassword(void);
        uint32_t thePassword;
        uint32_t theAddress;

    public:

        fingerPrint(uint32_t password = 0x0);

        bool verifyPassword(void);
        uint8_t getImage(void);
        uint8_t image2Tz(uint8_t slot = 1);
        uint8_t createModel(void);

        uint8_t emptyDatabase(void);
        //uint8_t storeModel(uint16_t id);
        uint8_t getModel(void);

        void getStructurePacket(const fingerPrint_packet &p);

};



#endif