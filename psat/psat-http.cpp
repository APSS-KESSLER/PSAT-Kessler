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
	"Hello"
	"</html>"
	"\r\n";
}

char const *newLine = "\r\n\r\n";

namespace psat {

HTTPSession::HTTPSession(WiFiClient &client): client(client) {
	LOG_INFO_P("HTTP"); Serial.print("Client connected with IP '"); Serial.print(client.remoteIP()); Serial.println('\'');
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

	if (strcmp("GET", verb) != 0) {
		if (strcmp("HEAD", verb) == 0) {
			return ResponseType::HEADER;
		} else {
			return ResponseType::ERROR;
		}
	}

	char url[16] = { 0 };

	length = client.readBytesUntil(' ', url, sizeof(url) - 1);
	if (strcmp("/data", url) == 0) {
		return ResponseType::DATA;
	} else if (strcmp("/", url) == 0) {
		return ResponseType::PAGE;
	} else {
		return ResponseType::ERROR;
	}

	// Not sure why the function does not take a char const *, but it seems not to edit the argument
	client.find(const_cast<char *>(newLine), 4);
}

void HTTPSession::sendError() {
	LOG_INFO("HTTP", "Sending error response");
	client.print(errorResponse);
}

void HTTPSession::sendPage() {
	LOG_INFO("HTTP", "Sending page response");
	client.print(page);
}

void HTTPSession::sendData(psat::Data const &data) {
	LOG_INFO("HTTP", "Sending data response");
}

void HTTPSession::sendHeader() {
	LOG_INFO("HTTP", "Sending header response");
	client.print(header);
}

}