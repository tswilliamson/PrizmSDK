
// sound code relevant to both platforms. Specifically code meant to 
// convert sound sampling to 1-bit sound (enabled by default in winsim, can be
// disabled via bSimulate1Bit = false)

#include "snd.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifndef max
#define max(a,b)            (((a) > (b)) ? (a) : (b))
#endif

#ifndef min
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif

static int sndSettings[5] = {
	1,			// vol #
	12,			// amplitude divisor
	15,			// latency (out of 16)
	4,			// voltageDrop (out of 64)
	1,			// blendMode (out of 4)
};

int minVoltage = 0;
int maxVoltage = 0;

const int numSoundSettings = 5;

unsigned int lastSoundCounter;

struct BTCEntry {
	unsigned char bits;
	short voltageOffset;
};

static BTCEntry* btcTable = NULL;

inline const int ApplyLatency(const int val) {
	return val * sndSettings[2] / 16;
}

inline const int ApplyDrop(const int val) {
	return val * sndSettings[3] / 64;
}

void ComputeBTCTable() {
	if (btcTable == 0) {
		btcTable = (BTCEntry*)malloc(sizeof(BTCEntry) * 4096);
	}

	const int volDivisor = sndSettings[1];
	const int baseRise = 6400;
	const int baseDrop = ApplyDrop(baseRise);
	minVoltage = min(baseDrop * 8 / volDivisor, 4096);
	maxVoltage = max(16384 - baseRise * 8 / volDivisor, 12288);

	for (int targetVoltage = 8; targetVoltage < 32768 + 8; targetVoltage += 16) {
		for (int rollingBitState = 0; rollingBitState < 2; rollingBitState++) {
			unsigned bitState = rollingBitState;
			int voltage = 16384;
			int lastVoltage = 16384;
			unsigned word = 0;

			for (int b = 0; b < 8; b++) {
				int bit1Voltage, bit0Voltage;
				switch (bitState) {
					case 0:
						bit1Voltage = voltage + ApplyLatency(baseRise) / volDivisor;
						bit0Voltage = voltage - baseDrop / volDivisor;
						break;
					case 1:
						bit1Voltage = voltage + baseRise / volDivisor;
						bit0Voltage = voltage - ApplyLatency(baseDrop) / volDivisor;
						break;
				}
						
				if (abs(bit0Voltage - targetVoltage) > abs(bit1Voltage - targetVoltage)) {
					// bit1 is closer
					word |= (1 << b);
					bitState = 1;
					voltage = bit1Voltage;
				} else {
					bitState = 0;
					voltage = bit0Voltage;
				}

				// apply lowpass filter
				voltage = (voltage * 4 + lastVoltage) / 5;
				lastVoltage = voltage;
			}

			int entry = (targetVoltage / 16) * 2 + rollingBitState;
			btcTable[entry].bits = word;
			btcTable[entry].voltageOffset = voltage - 16384;
		}

	}
}

inline unsigned char LookupBTC(int& curVoltage, int destVoltage, unsigned short lastBits) {
	int voltageOffset = (destVoltage + 16384 - curVoltage);
	const BTCEntry& entry = btcTable[(voltageOffset / 16) * 2 + (lastBits & 1)];
	curVoltage += entry.voltageOffset;
	if (curVoltage < minVoltage) curVoltage = minVoltage;
	else if (curVoltage > maxVoltage) curVoltage = maxVoltage;
	return entry.bits;
}

// Convert samples into 16-bit chunks of 1-bit BTC audio
void ConvertSamples(int* samples, unsigned char* convertedBuffer, int numSamples) {
	static int curVoltage = 0;
	static int curSample = 0;

	int target1 = 0, target2 = 0;
	for (int iter = 0; iter < numSamples; iter++) {
		int lastSample = curSample;
		const int sampleScale = 6;
		int baseVoltage = 8192 - sampleScale * 512;
		curSample = samples[iter] * sampleScale / 16 + baseVoltage;
				
		switch (sndSettings[4]) {
			case 0:
			{
				// no blending
				target1 = curSample;
				target2 = curSample;
				break;
			}
			case 1:
			{
				// single average
				target1 = (curSample + lastSample) >> 1;
				target2 = curSample;
				break;
			}
			case 2:
			{
				// walking average
				target1 = (curSample + lastSample * 3) >> 2;
				target2 = (curSample * 3 + lastSample) >> 2;
				break;
			}
			case 3:
			{
				// embedded average
				target1 = (curSample + lastSample * 2 + curVoltage) >> 2;
				target2 = (curSample * 2 + lastSample + curVoltage) >> 2;
				break;
			}
		}

		static unsigned char bits = 0;
		static unsigned char bits2 = 0;
		static int numZero = 0;
		bits = LookupBTC(curVoltage, target1, bits2);
		*(convertedBuffer++) = bits;

		bits2 = LookupBTC(curVoltage, target2, bits);
		if (bits == 0 && bits2 == 0) numZero++;
		else numZero = 0;

		// need a voltage 'come down' on zero bits to avoid pop:
		const unsigned char bitDownTable[12] = {
			0, 64, 64, 0, 64, 0, 64, 0, 0, 0, 64, 0
		};
		if (numZero < 24) {
			bits2 |= bitDownTable[numZero / 2];
		}

		*(convertedBuffer++) = bits2;
	}
}

// cleans up the platform sound system, called when emulation ends
void FreeBTCTable() {
	if (btcTable) {
		free((void*)btcTable);
		btcTable = NULL;
	}
}

const int perVolumeSettings[5][5] = {
	{ 0,6, 15,4,1 },
	{ 1,12,15,4,1 },
	{ 2,18,15,4,1 },
	{ 3,24,15,4,1 },
	{ 4,32,15,4,1 },
};

void updateSettings() {
	const int* settings = perVolumeSettings[sndSettings[0]];
	for (int i = 0; i < numSoundSettings; i++) {
		sndSettings[i] = settings[i];
	}

	ComputeBTCTable();
}

void sndVolumeUp() {
	if (sndSettings[0] < 4) {
		sndSettings[0]++;

		updateSettings();
	}
}


void sndVolumeDown() {
	if (sndSettings[0] > 0) {
		sndSettings[0]--;

		updateSettings();
	}
}