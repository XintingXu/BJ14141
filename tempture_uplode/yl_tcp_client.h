#ifndef ARDUINO_YEELINKLIB_TCPCLLIENT_H
#define ARDUINO_YEELINKLIB_TCPCLLIENT_H

#include <WString.h>

class yl_tcp_client
{
public:
	virtual long connect(const char *host, uint16_t port) = 0;
	virtual size_t send(long) = 0;
	virtual size_t send_ln(long) = 0;
	virtual size_t send(const String &) = 0;
	virtual long available() = 0;
	virtual long recv() = 0;
	virtual long recv(uint8_t *buf, size_t size) = 0;
	virtual void flush() = 0;
	virtual void stop() = 0;
};

#endif
