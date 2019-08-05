
#define MSG_SIZE 112
#define DATA_SIZE 108
#define CRC_SIZE 4
#define SYNC_SIZE 3
#define HEADER_SIZE 25
#define DOUBLE_SIZE 8

typedef uint8_t byte;

struct GPSMsgCatcher {
    byte syncBytes[SYNC_SIZE];
    byte header[HEADER_SIZE];
    byte garbage[16];
    byte northing[DOUBLE_SIZE];
    byte easting[DOUBLE_SIZE];
    byte height[DOUBLE_SIZE];
    byte footer[40];
    byte CRC[CRC_SIZE];
};

struct CRCFields {
    byte GPSData[DATA_SIZE];
    byte CRC[CRC_SIZE];
};

struct AllDataArray {
    byte fullMsg[MSG_SIZE];
};

union GPSFields {
    GPSMsgCatcher fields;
    CRCFields dataAndCRC;
    AllDataArray all;
};

struct GPSMsg {
    GPSFields bytes;
};
