#include "psat-http.h"

#include "psat-log.h"
#include "psat-data.h"

namespace {

char const errorResponse[] = 
	"HTTP/1.1 400 Bad Request\r\n"
	"\r\n";

char const header[] = 
	"HTTP/1.1 200 OK\r\n"
	"Content-Type: text/html; charset=UTF-8\r\n"
	"\r\n";

char const page[] = "<!DOCTYPE html><html><head><script>let post = (url, number) => fetch(url, {method: \"POST\", body: number});let led = (number) => {document.getElementById('led').style.top = number + '00%';post(\"/led\", number);};let sound = (number) => {document.getElementById('sound').style.top = number + '00%';post(\"/sound\", number);};</script><style>* {box-sizing: border-box;}.sidepanel {background-color:#d1e8e2;flex: 0 1 25%;padding: 2%;}.grid {display: grid;grid: auto-flow / 1fr 25%;align-content: start;}.grid > span + div {grid-column-start: 2;background-color: #c5ceca;border-radius: 50%;padding-bottom: 20%;margin: 40%;cursor: pointer;}.grid > span {place-self: center auto;}.grid > h2 {grid-column-end: span 2;}#led + div {grid-area: 2 / -2 / span 3 / span 1;border-left: 5px solid#c5ceca;justify-self: center;}#led {grid-area: 2 / 2;}#sound + div {grid-area: 6 / -2 / span 4 / span 1;border-left: 5px solid#c5ceca;justify-self: center;}#sound {grid-area: 6 / 2;}#led, #sound {top: 0;background-color: #116466;border-radius: 50%;padding-bottom: 30%;margin: 35%;z-index: 1;position: relative;transition: top 0.5s;}</style></head><body style=\"display: flex; align-items: stretch; height: 100vh; margin: 0; background-color: #2c3531;\"><div class=\"sidepanel grid\"><div id=\"led\"></div><div></div><h2>LED Modes</h2><span style=\"grid-row-start: 2\">Smooth Jazz</span><div style=\"grid-row-start: 2\" onclick=\"led(0)\"></div><span style=\"grid-row-start: 3\">Panic</span><div style=\"grid-row-start: 3\" onclick=\"led(1)\"></div><span style=\"grid-row-start: 4\">Smooth Jazz</span><div style=\"grid-row-start: 4\" onclick=\"led(2)\"></div><div id=\"sound\"></div><div></div><h2>Sound Modes</h2><span style=\"grid-row-start: 6\">Smooth Jazz</span><div style=\"grid-row-start: 6\" onclick=\"sound(0)\"></div><span style=\"grid-row-start: 7\">Panic</span><div style=\"grid-row-start: 7\" onclick=\"sound(1)\"></div><span style=\"grid-row-start: 8\">Smooth Jazz</span><div style=\"grid-row-start: 8\" onclick=\"sound(2)\"></div></div><div style=\"flex: auto\"></div><div class=\"sidepanel\"></div></body></html>\r\n";

}

char const *newLine = "\r\n\r\n";

namespace psat {

HTTPSession::HTTPSession(WiFiClient &client): client(client) {
	LOG_INFO_P("HTTP"); Serial.print("Client connected with IP '"); Serial.print(client.remoteIP()); Serial.println('\'');
}

void HTTPSession::process(psat::Data const &data, PutCallback putCallback) {
	switch (parseHeader(putCallback)) {
	case ResponseType::ERROR:
		sendError();
		break;
	case ResponseType::DATA:
		sendHeader();
		sendData(data);
		break;
	case ResponseType::PAGE:
		sendHeader();
		sendPage();
		break;
	case ResponseType::HEADER:
		sendHeader();
		break;
	}
}

HTTPSession::~HTTPSession() {
	LOG_INFO("HTTP", "Client disconnected");
	client.stop();
}

HTTPSession::ResponseType HTTPSession::parseHeader(PutCallback putCallback) {
	char verb[16] = { 0 };
	char url[16] = { 0 };
	
	client.readBytesUntil(' ', verb, sizeof(verb) - 1);

	switch (verb[0]) {
	case 'H': 
		return ResponseType::HEADER;
	
	case 'P':
		client.readBytesUntil(' ', url, sizeof(url) - 1);
		client.find(const_cast<char *>("\r\n\r\n"), 4);
		
		{
			long data = client.parseInt();

			if (putCallback(url, data)) {
				return ResponseType::HEADER;
			} else {
				return ResponseType::ERROR;
			}
		}

	case 'G':
		client.readBytesUntil(' ', url, sizeof(url) - 1);

		if (strcmp("/data", url) == 0) {
			return ResponseType::DATA;
		} else if (strcmp("/", url) == 0) {
			return ResponseType::PAGE;
		} else {
			return ResponseType::ERROR;
		}
	}
	
	return ResponseType::ERROR;
}

void HTTPSession::sendError() {
	LOG_INFO("HTTP", "Sending error response");
	write(errorResponse, sizeof(errorResponse));
}

void HTTPSession::sendPage() {
	LOG_INFO("HTTP", "Sending page response");
	
	write(page, sizeof(page));
}

void HTTPSession::sendData(psat::Data const &data) {
	LOG_INFO("HTTP", "Sending data response");
}

void HTTPSession::sendHeader() {
	LOG_INFO("HTTP", "Sending header response");
	write(header, sizeof(header));
}

void HTTPSession::write(char const *data, size_t length) {
	while (length > 0) {
		size_t written = client.write(data, length > SOCKET_BUFFER_MAX_LENGTH ? SOCKET_BUFFER_MAX_LENGTH : length);
		if (written > 0) LOG_INFO_F("HTTP", "Wrote %u bytes", written);
		data += written;
		length -= written;
	}
}

}