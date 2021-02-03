#ifndef PSAT_HTTP_H
#define PSAT_HTTP_H

#include "WiFi101.h"
#include "psat-wifi.h"

namespace psat {

struct Data;

class HTTPSession {
private:
	enum class ResponseType {
		ERROR,
		PAGE,
		DATA,
		HEADER
	};
	
	WiFiClient &client;
public:
	HTTPSession(WiFiClient &c);
	
	void process(psat::Data const &data, PutCallback callback);
	
	~HTTPSession() noexcept;

private:
	ResponseType parseHeader(PutCallback callback);
	void sendError();
	void sendPage();
	void sendData(psat::Data const &data);
	void sendHeader();

	void write(char const *data, size_t length);
};

}

#endif