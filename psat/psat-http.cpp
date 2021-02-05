#include "psat-http.h"

#include "psat-log.h"
#include "psat-data.h"

namespace {

char const errorResponse[] = 
	"HTTP/1.1 400 Bad Request\r\n"
	"\r\n";

char const putResponse[] = 
	"HTTP/1.1 204 No Content\r\n"
	"\r\n";

char const page[] = 
	"<!doctypehtml><script>let d=e=>document.getElementById(e),p=(e,t)=>fetch(e,{method:\"PUT\",body:t}),g=e=>fetch(e,{method:\"GET\"}),led=e=>{d(\"led\").style.top=e+\"00%\",p(\"/led\",e)},sound=e=>{d(\"sound\").style.top=e+\"00%\",p(\"/sound\",e)};setInterval(()=>{g(\"/data\").then(e=>e.json()).then(e=>{d(\"rssi\").innerText=e.rssi})},5e3);</script><style>*{box-sizing:border-box}.sidepanel{background-color:#d1e8e2;flex:0 1 25%;padding:2%}.control{display:grid;grid:auto-flow/1fr 25%;align-content:start}.control>span+div{grid-column-start:2;background-color:#c5ceca;border-radius:50%;padding-bottom:20%;margin:40%;cursor:pointer}.control>span{place-self:center auto}.control>h2{grid-column-end:span 2}#led+div{grid-area:2/-2/span 3/span 1;border-left:5px solid#c5ceca;justify-self:center}#led{grid-area:2/2}#sound+div{grid-area:6/-2/span 4/span 1;border-left:5px solid#c5ceca;justify-self:center}#sound{grid-area:6/2}#led,#sound{top:0;background-color:#116466;border-radius:50%;padding-bottom:30%;margin:35%;z-index:1;position:relative;transition:top .5s}#data{background-color:#c5ceca;display:grid;grid:auto-flow/1fr 1fr}#chart{background-color:#c5ceca;place-self:stretch}.suck{display:flow}.suck>div{background-color:red;border-radius:50%;margin:10%}</style><body style=display:flex;align-items:stretch;height:100vh;margin:0;background-color:#2c3531><div class=\"sidepanel control\"><div id=led></div><div></div><h2>LED Modes</h2><span style=grid-row-start:2>Smooth Jazz</span><div onclick=led(0) style=grid-row-start:2></div><span style=grid-row-start:3>Panic</span><div onclick=led(1) style=grid-row-start:3></div><span style=grid-row-start:4>Smooth Jazz</span><div onclick=led(2) style=grid-row-start:4></div><div id=sound></div><div></div><h2>Sound Modes</h2><span style=grid-row-start:6>Smooth Jazz</span><div onclick=sound(0) style=grid-row-start:6></div><span style=grid-row-start:7>Panic</span><div onclick=sound(1) style=grid-row-start:7></div><span style=grid-row-start:8>Smooth Jazz</span><div onclick=sound(2) style=grid-row-start:8></div></div><div style=flex:auto;display:grid;row-gap:50px;margin:50px><div id=data><span>WiFi Strength (RSSI): <span id=rssi></span></span></div><canvas id=chart></div><div class=\"sidepanel suck\"><div onclick='p(\"/camera\",0)'>Take Picture</div></div>";
}

namespace psat {

HTTPSession::HTTPSession(WiFiClient &client): client(client) {
	LOG_INFO_P("HTTP"); Serial.print("Client connected with IP '"); Serial.print(client.remoteIP()); Serial.println('\'');
}

void HTTPSession::process(psat::Data const &data, PutCallback putCallback) {
	char verb[16] = { 0 };
	char url[16] = { 0 };
	
	client.readBytesUntil(' ', verb, sizeof(verb) - 1);
	client.readBytesUntil(' ', url, sizeof(url) - 1);
	client.find(const_cast<char *>("\r\n\r\n"), 4);

	switch (verb[0]) {
	case 'P':		
		if (putCallback(url, client.parseInt())) {
			sendPutResponse();
			return;
		}

	case 'G':
		if (strcmp("/data", url) == 0) {
			sendData(data);
			return;
		} else if (strcmp("/", url) == 0) {
			sendPage();
			return;
		}
	}
	
	sendErrorResponse();
}

HTTPSession::~HTTPSession() {
	LOG_INFO("HTTP", "Client disconnected");
	client.stop();
}

void HTTPSession::sendOKResponse(char const *data, size_t length, char const *contentType) {
	client.printf(
		"HTTP/1.1 200 OK\r\n"
		"Content-Length: %lu\r\n"
		"Content-Type: %s\r\n"
		"\r\n", length, contentType);

	write(data, length);
}

void HTTPSession::sendErrorResponse() {
	LOG_INFO("HTTP", "Sending error response");
	client.print(errorResponse);
}

void HTTPSession::sendPutResponse() {
	LOG_INFO("HTTP", "Sending put response");
	client.print(putResponse);
}

void HTTPSession::sendPage() {
	LOG_INFO("HTTP", "Sending page response");
	sendOKResponse(page, sizeof(page), "text/html");
}

void HTTPSession::sendData(psat::Data const &data) {
	char jsonData [1024];
	size_t length = data.writeToString(jsonData);
	
	LOG_INFO("HTTP", "Sending data response");
	sendOKResponse(jsonData, length, "application/json");
}

void HTTPSession::write(char const *data, size_t length) {
	client.clearWriteError();

	while (length > 0) {
		size_t written = client.write(data, length > SOCKET_BUFFER_MAX_LENGTH ? SOCKET_BUFFER_MAX_LENGTH : length);
		if (written > 0) LOG_INFO_F("HTTP", "Wrote %u bytes", written);
		data += written;
		length -= written;

		if (client.getWriteError()) {
			LOG_ERROR("HTTP", "Write error");
			return;
		}
	}
}

}