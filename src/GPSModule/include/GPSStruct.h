
typedef uint8_t byte;

struct GPSMsgCatcher {
    byte syncBytes[3];
    byte header[25];
    byte garbage[16];
    byte northing[8];
    byte easting[8];
    byte height[8];
    byte footer[40];
    byte CRC[4];
};

struct CRCFields {
    byte GPSMsg[108];
    byte CRC[4];
};

struct AllDataArray {
    byte fullMsg[112];
};

union GPSFields {
    GPSMsgCatcher fields;
    CRCFields msgAndCRC;
    AllDataArray All;
};
