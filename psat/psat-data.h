#ifndef PSAT_DATA_H
#define PSAT_DATA_H

namespace psat {

struct Data {
	uint32_t time;
	int32_t rssi;

	size_t writeToString(char *string) const {
		return static_cast<size_t>(sprintf(string,
		"{"
			"\"time\":%u,"
			"\"rssi\":%d"
		"}", time, rssi));
	};
};

}

#endif