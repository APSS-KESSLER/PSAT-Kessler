#ifndef PSAT_HTTP_H
#define PSAT_HTTP_H

#include "WiFi101.h"
#include "psat-wifi.h"

namespace psat {

struct Data;

class HTTPSession {
private:
	WiFiClient &client;
public:
	HTTPSession(WiFiClient &c);
	
	void process(psat::Data const &data, PutCallback callback);
	
	~HTTPSession() noexcept;

private:
	void sendOKResponse(char const *data, size_t length, char const *contentType);
	void sendPutResponse();
	void sendErrorResponse();

	void sendHeader();
	void sendPage();
	void sendData(psat::Data const &data);

	void write(char const *data, size_t length);
};

}

#endif