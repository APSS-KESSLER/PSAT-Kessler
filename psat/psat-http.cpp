#include "psat-http.h"

#include "psat-log.h"
#include "psat-data.h"

namespace {

char const *errorResponse = 
	"HTTP/1.1 400 Bad Request\r\n"
	"\r\n";

char const *header = 
	"HTTP/1.1 200 OK\r\n"
	"Content-Type: text/html; charset=UTF-8\r\n"
	"Connection: close\r\n"
	"Refresh: 5\r\n"
	"\r\n";

char const *page = 
	"<!DOCTYPE html>"
	"<html>"
	"</html>"
	"\r\n";
}

namespace psat {

HTTPSession::HTTPSession(WiFiClient &client): client(client) {
	LOG_INFO_P("HTTP"); Serial.print("Client connected with IP '"); Serial.print(client.remoteIP()); Serial.println("'\n");
}

void HTTPSession::process(psat::Data const &data) {
	switch (parseHeader()) {
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

HTTPSession::ResponseType HTTPSession::parseHeader() {
	char verb[16] = { 0 };
	auto result = ResponseType::ERROR;
	auto length = client.readBytesUntil(' ', verb, sizeof(verb) - 1);

	if (!strcmp("GET", verb)) {
		if (strcmp("HEAD", verb)) {
			return ResponseType::HEADER;
		} else {
			return ResponseType::ERROR;
		}
	}

	char url[16] = { 0 };

	length = client.readBytesUntil(' ', url, sizeof(url) - 1);
	if (strcmp("/data", url)) {
		return ResponseType::DATA;
	} else {
		return ResponseType::PAGE;
	}

	client.find("\r\n\r\n", 4);
}

void HTTPSession::sendError() {
	client.print(errorResponse);
}

void HTTPSession::sendPage() {

}

void HTTPSession::sendData(psat::Data const &data) {

}

void HTTPSession::sendHeader() {

}

}