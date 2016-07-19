#ifndef ARDUINO_YEELINKLIB_W5100CLLIENT_H
#define ARDUINO_YEELINKLIB_W5100CLLIENT_H

#include <WString.h>
#include <EthernetClient.h>
#include "yl_tcp_client.h"

class yl_w5100_client : public yl_tcp_client
{
public:
	yl_w5100_client();
	yl_w5100_client(uint8_t sock);

	virtual long connect(const char *host, uint16_t port);
	virtual size_t send(long);
	virtual size_t send_ln(long);
	virtual size_t send(const String &);
	virtual long available();
	virtual long recv();
	virtual long recv(uint8_t *buf, size_t size);
	virtual void flush();
	virtual void stop();

private:
	EthernetClient client_;
};

#endif
