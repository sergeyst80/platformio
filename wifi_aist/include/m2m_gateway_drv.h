#ifndef M2M_GATEWAY_DRV_H
#define M2M_GATEWAY_DRV_H
#include "m2m_gateway_struct.h"

enum enumPacketType {INIT, TRANSMIT1, TRANSMIT2, HEARTBEAT};

class T_m2m_gateway {
private:

    struct_m2m_gateway_Header m2m_gateway_Header;
    struct_m2m_gateway_Connect m2m_gateway_Connect;
    struct_m2m_gateway_HeartBeat m2m_gateway_HeratBeat;

public:

    struct struct_TxRxBuffer TxRxBuffer;
    T_m2m_gateway(unsigned int GatewayAdress);
    void SetGatewayAdress(unsigned int GatewayAdress);
    void MakePacket(enumPacketType PacketType);
};

#endif // M2M_GATEWAY_DRV_H
