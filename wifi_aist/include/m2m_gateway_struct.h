#ifndef M2M_GATEWAY_STRUCT_H
#define M2M_GATEWAY_STRUCT_H
#include <string>

#pragma pack(push, 1)

struct struct_m2m_gateway_Header {
public:
    char Flag;
    unsigned int GatewayAdress;
    char PacketType;
    unsigned short DataSize;
};

struct struct_m2m_gateway_Connect {
    char IMSI1[15] = "11111111111111";
    char RSSI1;
    char SIM1check;
    char IMSI2[15] = "22222222222222";
    char RSSI2;
    char SIM2check;
    char SIMactive;
};

struct struct_m2m_gateway_HeartBeat {
public:
    char HearBeatPeriod;
    short CPUtemp;
    char RSSI;
};

#pragma pack(pop)

struct struct_TxRxBuffer {
    char Buf[100];
    size_t BufSize = 0;
};

#endif // M2M_GATEWAY_STRUCT_H
