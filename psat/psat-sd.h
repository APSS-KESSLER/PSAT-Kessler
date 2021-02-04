#ifndef PSAT_SD_H
#define PSAT_SD_H

#include <SD.h>

#include "psat-log.h"

namespace {

constexpr int chipSelect = 10;

}

namespace psat {

struct Data;

class CardModule {
	SDFile fileOutput;

public:
	void setup();
	void writeToCard(psat::Data const &data);

private:
	void writeHeader();
};

}

#endif