/*****************************************************************************
** EraseThread.cpp
** Author: Kraku
*****************************************************************************/
#include "headers/EraseThread.h"
#include "headers/Logic.h"
#include "headers/const.h"
#include "headers/Settings.h"

void EraseThread::run()
{
    const time_t tp = time(nullptr); /* time for erase */
    unsigned char packet[PACKETSIZE] = {};
    packet[0] = DATA;
    packet[1] = ERASE;
    packet[2] = mem;
    packet[3] = mbc;
    packet[4] = par;
    packet[5] = Settings::dap;
    const unsigned short crc16 = Logic::generate_crc16(packet);
    packet[PACKETSIZE - 2] = crc16 / 256;
    packet[PACKETSIZE - 1] = crc16 % 256;
    end = false;

    /* send erase packet */
    if (port->send_packet(packet) < PACKETSIZE)
    {
        port->close_port();
        emit error(SEND_ERROR);
        return;
    }

    do
    {
        /* end of thread - operation canceled */
        if (end)
        {
            port->close_port();
            emit error(END);
            return;
        }
        /* erase done */
        if (port->receive_char() == ACK)
        {
            port->close_port();
            emit set_progress(1, 1);
            emit error(true);
            return;
        }
        if (time(nullptr) != tp)
            emit set_progress(time(nullptr) - tp, DELTIME);
    }
    while (time(nullptr) - tp < DELTIME);
    /* wait for erase DELTIME sec.
     * hardware independent!
     */

    emit set_progress(1, 1); //100%
    port->close_port();
    emit error(TIMEOUT);
}
