#include <stdio.h>
#include <math.h>
#include <getopt.h>

float goertzel_mag(int numSamples,float TARGET_FREQUENCY,int SAMPLING_RATE, float* data)
{
    int     k,i;
    float   floatnumSamples;
    float   omega,sine,cosine,coeff,q0,q1,q2,magnitude,real,imag;

    float   scalingFactor = numSamples / 2.0;

    floatnumSamples = (float) numSamples;
    k = (int) (0.5 + ((floatnumSamples * TARGET_FREQUENCY) / (float)SAMPLING_RATE));
    omega = (2.0 * M_PI * k) / floatnumSamples;
    sine = sin(omega);
    cosine = cos(omega);
    coeff = 2.0 * cosine;
    q0=0;
    q1=0;
    q2=0;

    for(i=0; i<numSamples; i++)
    {
        q0 = coeff * q1 - q2 + data[i];
        q2 = q1;
        q1 = q0;
    }

    // calculate the real and imaginary results
    // scaling appropriately
    real = (q2 - q1 * cosine) / scalingFactor;
    imag = (q1 * sine) / scalingFactor;

    magnitude = sqrtf(real*real + imag*imag);
    return magnitude;
}

void print_help(char ** argv) {
	printf(
		"%s takes raw (wav) audio stream and computes power (or magnitude)\n"
		"of desired frequencies using Goertzel algorithm for time frames\n"
		"of fixed length (specified in samples or relative to sample rate).\n"
		"This can be used in various frequency detection applications\n"
		"like guitar tuning, DTMF decoding and many others...\n"
		"\n"
		"http://en.wikipedia.org/wiki/Goertzel_algorithm\n"
		"\n"
		"Curently only raw unsigned 8bit (u8) mono audio is supported, but\n"
		"samplerate may vary. You can convert other formats before processing.\n"
		"\n"
		"On lower samplerates and frame sizes this may perform sub-optimally. Eg.:\n"
		"When set to detect 440Hz (at 8000Hz samplerate and ~4000 samples)\n"
		"it actually detects something around 438,3Hz rather than 400Hz...\n"
		"If you can't increase samplerate way around this is just to increase sensitivity.\n"
		"\n"
		,argv[0]
	);

	printf(
		"Arguments:\n"
		"\t-i <file>\tInput from file (default STDIN)\n"
		"\t-o <file>\tOutput to file (default STDOUT)\n"
		"\t-a <file>\tOutput to file (append) (default STDOUT)\n"
		"\n"
		"\t-r <samplerate>\tInput samplerate (deault 8000 Hz)\n"
		"\t-c <count>\tFrame size in samples (default 4000 Samples)\n"
		"\t-d <divisor>\tFrame size ( count = samplerate/divisor ) (default 2)\n"
		"\n"
		"\t-f <freq>\tAdd frequency in Hz to detect (use multiple times, default 440 Hz)\n"
		"\n"
		"\t-n <format>\tSet number output format\n"
		"\t\tf: float\t23.4223 (default)\n"
		"\t\ti: integer\t23\n"
		"\t\tb: binary\t(0|1)\n"
		"\t\tB: Boolean\t(false|true)\n"
		"\n"
		"\t-t <treshold>\tSet treshold (used in filter, see -l) (defaults -1)\n"
		"\t-l <filter>\tSet line filter\n"
		"\t\tf: Falldown:\tprint only when over treshold or just crossed (default)\n"
		"\t\tt: Treshold:\tprint only when over treshold\n"
		"\t\tc: Crossed:\tprint only when treshold crossed\n"
		"\t-u\t\tInvert\ttreshold (values under treshold will be displayed)\n"
		"\n"
		"\t-q\t\tQuiet mode: print only values\n"
		"\n"
		"\t-?\t\tPrint help\n"
		"\n"
	);

	printf(
		"Usage examples:\n"
		"\tarecord | %s\n"
		"\tsox input.mp3 -b 8 -c 1 -r 8000 -t wav - | %s\n"
		"\t%s -n -q -l -r 8000 -d 20 -t $tresh -f 697 [-f 770 ...]\n"
		"\n"
		,argv[0],argv[0],argv[0]
	);

	printf(
		"Frequencies for DTMF decoding:\n"
		"\t-f 697 -f 770 -f 852 -f 941 -f 1209 -f 1336 -f 1477 -f 1633 -t 10\n"
	);
}

void addfreq(float *freqs, float freq) {
	int i = 0;
	while(freqs[i]!=-1) i++;
	freqs[i]=freq;
	freqs[i+1]=-1;
}

int main(int argc, char ** argv) {
	int samplerate = 8000;
	int samplecount = 4000;

	int treshold = -1;
	char filter = 0;
	char under = 0;

	char format=0;
	char verbose=1;

	float freqs[argc+1]; freqs[0]=-1;


	float floatarg;
	int opt;
	while ((opt = getopt(argc, argv, "?i:o:a:r:c:d:f:t:n:l:uq")) != -1) {
		switch (opt) {
			case 'i':
				freopen(optarg, "r", stdin);
				break;
			case 'o':
				freopen(optarg, "w", stdout);
				break;
			case 'a':
				freopen(optarg, "a", stdout);
				break;
			case 'r':
				samplerate = atoi(optarg);
				break;
			case 'c':
				samplecount = atoi(optarg);
				break;
			case 'd':
				samplecount = samplerate/atoi(optarg);
				break;
			case 'f':
				sscanf(optarg,"%f",&floatarg);
				addfreq(freqs, floatarg);
				break;
			case 't':
				treshold = atoi(optarg);
				break;
			case 'n':
				format = optarg[0];
				break;
			case 'l':
				filter = optarg[0];
				break;
			case 'u':
				under = 1;
				break;
			case 'q':
				verbose = 0;
				break;
			case '?':
				print_help(argv);
				return 0;
				break;
		}
	}

	if(freqs[0]==-1) addfreq(freqs, 440);
	float samples[samplecount];
	float position = 0;

	if(verbose) {
		fprintf(stderr,
			"#Detected tone: %.2f Hz\n"
			"#Samplerate: %d Hz\n"
			"#Frame lenght: %d samples\n"
			"#Treshold: %d\n"
			"#\n"
			,freqs[0],samplerate,samplecount,treshold);
		fflush(stderr);

		printf("#Position");
		int i; for(i=0;freqs[i]!=-1;i++) {
			printf("\t%2.0fHz",freqs[i]); //TODO: print decimal places
		}
		puts("");
	}

	int i;
	char print=0, printnow=0;
	char laststate[argc]; for(i=0;freqs[i]!=-1;i++) laststate[i]=-1;
	while(!feof(stdin)) {

		//Sample data
		for(i=0;i<samplecount && !feof(stdin);i++) {
			unsigned char sample;
			fread(&sample,1,1,stdin);
			samples[i]=sample;
			//printf("%d\n", sample);
		}

		//Apply goertzel
		float power[argc];
		print=0;
		for(i=0;freqs[i]!=-1;i++) {
			power[i] = goertzel_mag(samplecount, freqs[i], samplerate, samples);

			//Decide if we will print
			printnow = under ? power[i] < treshold : power[i] > treshold; //Is over/under treshold?
			switch(filter) {
				case 'c': //Print if treshold crossed
					print = print || (laststate[i] != printnow);
					break;
				default:
				case 'f': //Print if over treshold or falled down
					print = print || (laststate[i] != printnow);
				case 't': //Print if over treshold
					print = print || printnow;
			}
			laststate[i] = printnow; //Store last state
		}
		fflush(stdout);

		//Print data
		if(print) {
			printf("%8.2f", position);
			for(i=0;freqs[i]!=-1;i++) {
				printf("\t");
				switch(format) {
					case 'i':
						printf("%d",(int)round(power[i]));
						break;
					case 'b':
						printf("%d",power[i]>treshold);
						break;
					case 'B':
						if(power[i]>treshold) printf("true");
							else printf("false");
						break;
					case 'f':
					default:
						printf("%7.5f",power[i]);
				}
			}
			puts("");
			fflush(stdout);
		}

		//Increase time
		position += ((float)samplecount/(float)samplerate);
	}
}
