
// recomputes the tabling for bit tables for 1-bit sound
void ComputeBTCTable();

// Convert samples into 16-bit chunks of 1-bit BTC audio
void ConvertSamples(int* samples, unsigned char* convertedBuffer, int numSamples);

// frees memory used by BTC table
void FreeBTCTable();