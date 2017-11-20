
#ifndef _APP_DATA_H_
#define _APP_DATA_H_

#include "type_def.h"
#include "slavedata.h"

#define DEFAULT_SENSOR {0}

typedef struct Sensor
{
	uint16 usId;
	uint16 usStatusA;
	uint16 usStatusB;
	int16  usTempearture;
	uint16 usHumidity;
	uint16 usCo2;
	uint16 usVoc;
	uint16 usPM1;
	uint16 usPM25;
	uint16 usPM10;
	uint16 usUM03;
	uint16 usUM05;
	uint16 usUM10;
	uint16 usUM25;
	uint16 usUM50;
	uint16 usUM100;
	int16  usTmpEQ;
	uint16 usHmdEQ;
	uint16 usCo2EQ;
	uint16 usVocEQ;
	uint16 usPM10EQ;
	uint16 usPM25EQ;
	uint16 usPM100EQ;
	uint16 usFlashTimes;
	uint16 usNFlashTimes;
}SensorStruct;

typedef struct Data
{     	    
	SensorStruct stSensor[5];
	AerkateStruct stAerkate;
}DataStruct;

extern DataStruct Data;

#endif 

/* End of file */

