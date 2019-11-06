#include "m2m_gateway_drv.h"
#include <iostream>
#include <algorithm>
#include <string.h>
using namespace std;

template<typename T>
void ConvLittleToBig(T* input) {
    char* t = (char*)(input);
    int i = 0;
    int size = sizeof(T) - 1;
    while (i < size) {
        std::swap(*(t+i), *(t + size - i));
       ++i;
    }
}

unsigned short CalcCRC16(char *Data, size_t DataLen) {
    char i;
    int result = 0;

    while (DataLen--) {
        result = result ^ ((static_cast<unsigned short>(*Data++)) << 8);
        for (i = 8; i; i--) {
            if (result & 0x8000)
                result = (result << 1) ^ 0x1021;
            else
                result <<= 1;
        }
    }
    return static_cast<unsigned short>(result);
}

T_m2m_gateway::T_m2m_gateway(unsigned int GatewayAdress) {
    m2m_gateway_Header.Flag = 0x24;
    SetGatewayAdress(GatewayAdress);
    m2m_gateway_Connect.IMSI1[14] = '1';
    m2m_gateway_Connect.IMSI2[14] = '2';
    m2m_gateway_Connect.RSSI1 = (char)255;
    m2m_gateway_Connect.RSSI2 = (char)255;
    m2m_gateway_Connect.SIM1check = 1;
    m2m_gateway_Connect.SIM2check = 1;
    m2m_gateway_Connect.SIMactive = 1;
    m2m_gateway_HeratBeat.RSSI = static_cast<char>(255);
    m2m_gateway_HeratBeat.CPUtemp = 0;
    ConvLittleToBig(&m2m_gateway_HeratBeat.CPUtemp);
    m2m_gateway_HeratBeat.HearBeatPeriod = 1;
}

void T_m2m_gateway::SetGatewayAdress(unsigned int GatewayAdress) {
    m2m_gateway_Header.GatewayAdress = GatewayAdress;
    ConvLittleToBig(&m2m_gateway_Header.GatewayAdress);
}

template<typename T>
void PrepareData(T& input, struct struct_m2m_gateway_Header& header, struct struct_TxRxBuffer& TxRxBuffer) {
    TxRxBuffer.BufSize = 0;
    size_t offset = 0;
    header.DataSize = sizeof(input);
    ConvLittleToBig(&header.DataSize);
    TxRxBuffer.BufSize += sizeof(header);
    memcpy(TxRxBuffer.Buf, &header, TxRxBuffer.BufSize);
    offset += TxRxBuffer.BufSize;
    TxRxBuffer.BufSize += sizeof(input);
    memcpy(TxRxBuffer.Buf + offset, &input, TxRxBuffer.BufSize);
    unsigned short t = CalcCRC16(TxRxBuffer.Buf, TxRxBuffer.BufSize);
    memcpy(TxRxBuffer.Buf+TxRxBuffer.BufSize, &t, 2);
    TxRxBuffer.BufSize += 2;
}

void T_m2m_gateway::MakePacket(enumPacketType PacketType) {
    switch (PacketType) {
        case INIT:
            m2m_gateway_Header.PacketType = 0;
            PrepareData(m2m_gateway_Connect, m2m_gateway_Header, TxRxBuffer);
        break;
        case TRANSMIT1:
            break;
        case TRANSMIT2:
        break;
        case HEARTBEAT:
            m2m_gateway_Header.PacketType = 3;
            PrepareData(m2m_gateway_HeratBeat, m2m_gateway_Header, TxRxBuffer);
        break;
    }
}

