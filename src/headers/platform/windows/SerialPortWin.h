/*****************************************************************************
** SerialPortWin.h - Serial driver for windows
** Author: Kraku
*****************************************************************************/
#ifndef SERIALPORTWIN_H
#define SERIALPORTWIN_H

#include <windows.h>
#include <QObject>
#include "../../AbstractPort.h"

 
class SerialPortWin : public AbstractPort
{
Q_OBJECT
	HANDLE hCommDev; /* port descriptor */
    DCB dcb; /* serial port control structure */
    COMSTAT Stat; /* additional info about port */
    DWORD Errors; /* represents error type */



	bool opened;
public:
	SerialPortWin();
	bool open_port (QString port_name);
	bool close_port ();
	int receive_char ();
	bool send_char (unsigned char character);
	int receive_packet (unsigned char *packet);
	int send_packet (unsigned char *packet);
	bool isOpen()
	{
		return opened;
	}
signals:
	void error(int err);
};

#endif
