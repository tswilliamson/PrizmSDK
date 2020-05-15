
// sound code relevant to both platforms. Specifically code meant to 
// convert sound sampling to 1-bit sound (enabled by default in winsim, can be
// disabled via bSimulate1Bit = false)

#include "snd.h"

// volume has to be modulated in a number of ways do to reality of 1-bit audio:
const int numSoundSettings = 4;
const int perVolumeSettings[5][numSoundSettings] = {
	{ 0,9, 3,0xCC },
	{ 1,9, 2,0xCC },
	{ 2,9, 2,0xFF },
	{ 3,9, 1,0xFF },
	{ 4,10,1,0xFF },
};

static int sndSettings[numSoundSettings] = {
	perVolumeSettings[2][0],	// vol #
	perVolumeSettings[2][1],	// bit shift used to determine step size of voltage. Smaller = more volts, higher = less accuracy
	perVolumeSettings[2][2],	// bit shift used directly on sample data, which pushes the overall voltage lower
	perVolumeSettings[2][3],	// final additional masking done to output bits. Can be used to force down bits while maintaining voltage simulation
};

static int curVoltage = 8192;

inline unsigned char BTC(int target) {
	unsigned char btc = 0;
	const int shift = sndSettings[1];

	for (int bit = 0; bit < 8; bit++) {
		btc <<= 1;

		int dist = (32768 - curVoltage) >> shift;
		int upVoltage = curVoltage + dist;

		dist = curVoltage >> shift;
		int downVoltage = curVoltage - dist;

		// an early out when voltage is reached that reduces distortion:
		if ((bit & 1) == 0 && upVoltage > target && downVoltage < target) {
			// just maintain position with 1/0 flipping
			int useBit = (upVoltage - target < target - downVoltage) ? 1 : 0;
			btc |= useBit;
			bit++;
			while (bit < 8) {
				btc <<= 1;
				useBit = useBit ^ 1;
				btc |= useBit;
				bit++;
			}
			break;
		}

		int disthigh = upVoltage - target;
		if (disthigh < 0) disthigh = -disthigh;
		int distlow = downVoltage - target;
		if (distlow < 0) distlow = -distlow;

		if (disthigh < distlow) {
			btc |= 1;
			curVoltage = upVoltage;
		} else {
			curVoltage = downVoltage;
		}
	}

	return btc & sndSettings[3];
}

// Convert samples into 16-bit chunks of 1-bit BTC audio
void ConvertSamples(int* samples, unsigned char* convertedBuffer, int numSamples) {
	const int bitShift = sndSettings[2];
	const int voltageOffset = 8192; //  16384 - (8192 >> bitShift);
	for (int i = 0; i < numSamples; i++) {
		int sampleTarget = (*(samples++) >> bitShift) + voltageOffset;

		*(convertedBuffer++) = BTC((curVoltage + sampleTarget) / 2);
		*(convertedBuffer++) = BTC(sampleTarget);
	}
}

void updateSettings() {
	const int* settings = perVolumeSettings[sndSettings[0]];
	for (int i = 0; i < numSoundSettings; i++) {
		sndSettings[i] = settings[i];
	}
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