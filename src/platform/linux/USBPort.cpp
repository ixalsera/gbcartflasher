/*****************************************************************************
** USBPort.cpp
** Author: Kraku
*****************************************************************************/
extern "C" {
#include <time.h>
#include <stdio.h>
}

#include <ftdi.h>
#include <iostream>

#include "../../headers/const.h"
#include "../../headers/platform/linux/USBPort.h"
#include "../../headers/Settings.h"


USBPort::USBPort() : AbstractPort()
{
    ftdi_init(&ftdic);
}

bool USBPort::open_port(QString /*port_name*/)
{
    if (int ret; (ret = ftdi_usb_open(&ftdic, 0x0403, 0x6001)) < 0)
    {
        fprintf(stderr, "unable to open ftdi device: %d (%s)\n", ret, ftdi_get_error_string(&ftdic));
        ftdi_free(&ftdic);
        return false;
    }
    /* choose speed */
    if (Settings::speed == STANDARD)
    {
        if (ftdi_set_baudrate(&ftdic, 185000) < 0)
            return false;
    }
    else if (Settings::speed == LOW)
    {
        if (ftdi_set_baudrate(&ftdic, 125000) < 0)
            return false;
    }
    else if (Settings::speed == HIGH)
    {
        if (ftdi_set_baudrate(&ftdic, 375000) < 0)
            return false;
    }

    if (ftdi_set_latency_timer(&ftdic, 2) < 0)
        return false;
    if (ftdi_set_line_property(&ftdic, BITS_8, STOP_BIT_1, NONE) < 0)
        return false;
    //if(FT_SetTimeouts(ftHandle,5000,0) != FT_OK)
    //      return false;
    //if(ftdi_enable_bitbang(&ftdic,0xFF) < 0)
    //      return false;

    return true;
}

bool USBPort::close_port()
{
    ftdi_usb_close(&ftdic);
    return true;
}

int USBPort::send_packet(unsigned char packet[PACKETSIZE])
{
    return ftdi_write_data(&ftdic, packet, PACKETSIZE);
}


bool USBPort::send_char(unsigned char character)
{
    return ftdi_write_data(&ftdic, &character, 1) == 1;
}

int USBPort::receive_char()
{
    time_t tp = time(nullptr);
    unsigned char character;
    int bytesReceived = 0;
    do
    {
        bytesReceived = ftdi_read_data(&ftdic, &character, 1);
        if (bytesReceived != 0)
            break;
    }
    while (time(nullptr) - tp < SLEEPTIME);

    if (bytesReceived == 0)
        return TIMEOUT;
    if (character == ACK || character == END)
        return character;

    return NAK;
}

int USBPort::receive_packet(unsigned char* packet)
{
    time_t tp = time(nullptr);
    int bytesReceived = 0;
    do
    {
        bytesReceived = ftdi_read_data(&ftdic, packet, 1);
    }
    while (time(nullptr) - tp < SLEEPTIME && bytesReceived == 0);
    if (bytesReceived == 0)
        return TIMEOUT;

    if (packet[0] != DATA)
    {
        if (packet[0] == ACK)
            return ACK;
        if (packet[0] == END)
            return END;


        return NAK;
    }
    unsigned int remaining = PACKETSIZE - 1;
    do
    {
        const int bytesLeft = remaining;

        bytesReceived = ftdi_read_data(&ftdic, &packet[PACKETSIZE - remaining], bytesLeft);
        remaining -= bytesReceived;
        tp = time(nullptr);
    }
    while (time(nullptr) - tp < SLEEPTIME && remaining != 0);
    if (remaining > 0)
        return TIMEOUT;

    return DATA;
}
