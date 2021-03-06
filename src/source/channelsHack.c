#include "channelsHack.h"
#define MAX_SNR 40
#define MIN_SNR 9

/*
 int mapSNR(){
 	int snr = getSensorValue(IBUS_MEAS_TYPE_SNR);
 	if(snr == 0x8000 || snr <= MIN_SNR) return -10000;
 	if(snr >= MAX_SNR) return 10000;
 	snr -=9;
 	snr = (snr * 645) - 10000;
 	return snr;
 }
 int mapError(){
    int error = getSensorValue(IBUS_MEAS_TYPE_ERR);
    if(error == 0x8000) return -10000;
    return ((100 - error) * 200) - 10000;
 }


void applaySwitchesHook(int32_t targetArray[]){
    applaySwitches(targetArray);
    //uint32_t maxChannels = *((int32_t *)MODEL_MAX_CHANNELS);
    //if(maxChannels >= 11)
    //targetArray[10] = 1000;
    //targetArray[11] = 2000;
}
*/

uint8_t getSensor2(int32_t id, int32_t* result){

    int index = 0;
    do {
        sensorInfo_s *ptr = (sensorInfo_s *)(SENSORS_ARRAY + 12 *index);
        if(ptr->id == id){
            *result = ptr->value;
            return 1;
        }
        index++;
    } while(index < 32);
    return 0;
}

int32_t mapSNR(){
    int32_t value;
 	if(!getSensor2(IBUS_MEAS_TYPE_SNR, &value))  return -10000;
    value = value & 0xFFFF;
 	if(value >= MAX_SNR) return 10000;
 	value -=9;
 	value = (value * 645) - 10000;
 	return value;
}
int32_t mapError() {
    int32_t value;
 	if(!getSensor2(IBUS_MEAS_TYPE_ERR, &value))  return -10000;
    value = value & 0xFFFF;
    return ((100 - value) * 200) - 10000;
}

void applaySwitchesHook(int32_t targetArray[]){
    applaySwitches(targetArray);
    uint32_t maxChannels = *((int32_t *)MODEL_MAX_CHANNELS);
    for(uint32_t i = maxChannels +1; i <= TOTAL_CHANNELS; i++){
        targetArray[i-1] = 0;
    }
    targetArray[10] = mapSNR();
    targetArray[11] = mapError();
}
