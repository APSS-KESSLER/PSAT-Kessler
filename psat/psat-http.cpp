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
	"<!doctypehtml><script>let x=[],v=e=>e.pressureAltitude,d=e=>document.getElementById(e),p=(e,t)=>fetch(e,{method:'PUT',body:t}),g=e=>fetch(e,{method:'GET'}),led=e=>{d('led').style.top=e+'00%',p('/led',e)},sound=e=>{d('sound').style.top=e+'00%',p('/sound',e)},c=()=>{let n=d('chart').getContext('2d');if(n.save(),n.clearRect(0,0,1e3,1e3),n.lineWidth=2,n.lineCap='round',n.strokeStyle='#222',n.beginPath(),n.moveTo(800,800),n.lineTo(200,800),n.lineTo(200,200),n.stroke(),!(x.length<2)){var r=x[x.length-1];let t={min:1/0,max:-1/0},e=x.filter(e=>r.millis-e.millis<=6e4);if(Array.isArray(v(r)))for(let i=0;i<v(r).length;i++)t=e.reduce((e,t)=>({min:Math.min(e.min,v(t)[i]),max:Math.max(e.max,v(t)[i])}),t);else t=e.reduce((e,t)=>({min:Math.min(e.min,v(t)),max:Math.max(e.max,v(t))}),t);t.min==t.max&&(--t.min,t.max+=1);var l={x:.01,y:600/(t.min-t.max)},o={x:800-r.millis*l.x,y:800-t.min*l.y};n.lineJoin='round';var s=['#3e3',n.strokeStyle='#e33','#33e'];if(Array.isArray(v(r)))for(let i=0;i<v(r).length;i++)n.strokeStyle=s[i],n.beginPath(),e.forEach((e,t)=>n[t?'lineTo':'moveTo'](o.x+l.x*e.millis,o.y+l.y*v(e)[i])),n.stroke();else n.beginPath(),e.forEach((e,t)=>n[t?'lineTo':'moveTo'](o.x+l.x*e.millis,o.y+l.y*v(e))),n.stroke();[0,.2,.4,.6,.8,1].forEach(e=>{n.fillText('-'+(60*(1-e)).toFixed()+'s',200+600*e,900),n.fillText((e*t.max+(1-e)*t.min).toFixed(2),100,800-600*e)}),n.restore()}};setInterval(()=>{g('/data').then(e=>e.json()).then(e=>{x.push(e),d('rssi').innerText=e.rssi,d('acceleration').innerText=e.acc.join(', '),d('gyro').innerText=e.gyro.join(', '),d('mag').innerText=e.mag.join(', '),d('temp').innerText=e.temp,d('pressure').innerText=e.pressure,d('altitude-pressure').innerText=e.pressureAltitude,d('gps').innerText=e.gps.fix?e.gps.satellites:'No lock',d('lat').innerText=e.gps.latitude,d('lon').innerText=e.gps.longitude,d('altitude-gps').innerText=e.gps.altitude,d('photos').innerText=e.photoNumber}),c()},1e3);</script><style>*{box-sizing:border-box}.sidepanel{background-color:#d1e8e2;flex:0 1 25%;padding:2%}.control{display:grid;grid:auto-flow/1fr 25%;align-content:start}.control>span+div{grid-column-start:2;background-color:#c5ceca;border-radius:50%;padding-bottom:20%;margin:40%;cursor:pointer}.control>span{place-self:center auto}.control>h2{grid-column-end:span 2}#led+div{grid-area:2/-2/span 3/span 1;border-left:5px solid#c5ceca;justify-self:center}#led{grid-area:2/2}#sound+div{grid-area:6/-2/span 4/span 1;border-left:5px solid#c5ceca;justify-self:center}#sound{grid-area:6/2}#led,#sound{top:0;background-color:#116466;border-radius:50%;padding-bottom:30%;margin:35%;z-index:1;position:relative;transition:top .5s}#data{background-color:#c5ceca;border-radius:20px;display:grid;grid:auto-flow/1fr 1fr 1fr;align-content:start;gap:20px;padding:40px}#data>div{border-radius:20px;background-color:#d1e8e2;padding:20px}#chart{background-color:#c5ceca;border-radius:20px;place-self:stretch}.suck{display:flow;flex-direction:column;padding:2%}.suck>div{background-color:#116466;border-radius:20px;padding:20px;text-align:center;color:#fff;font-weight:700;cursor:pointer;margin-bottom:20px}</style><body style=display:flex;align-items:stretch;height:100vh;margin:0;background-color:#2c3531;font-family:sans-serif><div class=\"sidepanel control\"><div id=led></div><div></div><h2>LED Modes</h2><span style=grid-row-start:2>Smooth Jazz</span><div onclick=led(0) style=grid-row-start:2></div><span style=grid-row-start:3>Panic</span><div onclick=led(1) style=grid-row-start:3></div><span style=grid-row-start:4>Smooth Jazz</span><div onclick=led(2) style=grid-row-start:4></div><div id=sound></div><div></div><h2>Sound Modes</h2><span style=grid-row-start:6>Smooth Jazz</span><div onclick=sound(0) style=grid-row-start:6></div><span style=grid-row-start:7>Panic</span><div onclick=sound(1) style=grid-row-start:7></div><span style=grid-row-start:8>Smooth Jazz</span><div onclick=sound(2) style=grid-row-start:8></div></div><div style=overflow:auto;flex:auto;display:grid;row-gap:50px;margin:50px><div id=data><div onclick=\"v=x=>x.rssi\">WiFi Strength (RSSI): <span id=rssi></span></div><div onclick=\"v=x=>x.acc\">Acceleration (m s-2): <span id=acceleration></span></div><div onclick=\"v=x=>x.gyro\">Gyro (rad s-1): <span id=gyro></span></div><div onclick=\"v=x=>x.mag\">Magnetic Field (uT): <span id=mag></span></div><div>GPS Lock: <span id=gps></span></div><div onclick=\"v=x=>x.temp\">Temperature (C):<span id=temp></span></div><div onclick=\"v=x=>x.pressure\">Pressure (Pa): <span id=pressure></span></div><div onclick=\"v=x=>x.gps.latitude\">Latitude (deg): <span id=lat></span></div><div onclick=\"v=x=>x.gps.longitude\">Longitude (deg): <span id=lon></span></div><div onclick=\"v=x=>x.pressureAltitude\">Altitude - Pressure (m): <span id=altitude-pressure></span></div><div onclick=\"v=x=>x.gps.altitude\">Altitude - GPS (m): <span id=altitude-gps></span></div><div onclick=\"v=x=>x.photoNumber\">Photos Taken: <span id=photos></span> / 999</div></div><canvas height=1000 id=chart width=1000></div><div class=\"sidepanel suck\"><h2>Command Module</h2><div onclick='p(\"/camera\",0)'>Take single picture</div><div onclick='p(\"/camera\",1)'>Enable continuous pictures</div><div onclick='p(\"/camera\",2)'>Take video</div><div onclick='p(\"/altitude\",+prompt(\"Altitude (mm above sealevel)\",\"0\"))'>Set altitude</div><div onclick='p(\"/time\",+prompt(\"Time (unix)\",\"0\"))'>Set rtc time</div><div onclick='p(\"/gps\",+prompt(\"Ms to wait\",\"30000\"))'>Turn off chip until Gps lock is found</div><div onclick='p(\"/arm\",0)'>Arm the psat to detect launch</div><div onclick='p(\"/arm\",1)'>Disarm the psat to detect launch</div></div>";
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