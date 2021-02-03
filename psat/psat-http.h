#ifndef PSAT_HTTP_H
#define PSAT_HTTP_H

#include "WiFi101.h"

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
	
	void process(psat::Data const &data);
	
	~HTTPSession() noexcept;

private:
	ResponseType parseHeader();
	void sendError();
	void sendPage();
	void sendData(psat::Data const &data);
	void sendHeader();
};

}

#endif