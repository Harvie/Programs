#include <stdio.h>
#include <math.h>
#include <getopt.h>

float goertzel_mag(int numSamples,int TARGET_FREQUENCY,int SAMPLING_RATE, float* data)
{
    int     k,i;
    float   floatnumSamples;
    float   omega,sine,cosine,coeff,q0,q1,q2,magnitude,real,imag;

    float   scalingFactor = numSamples / 2.0;

    floatnumSamples = (float) numSamples;
    k = (int) (0.5 + ((floatnumSamples * TARGET_FREQUENCY) / SAMPLING_RATE));
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
    real = (q1 - q2 * cosine) / scalingFactor;
    imag = (q2 * sine) / scalingFactor;

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
		"\t-d <ratio>\tFrame size (default 2) (samplerate will be divided by this number to get frame size same as -c)\n"
		"\n"
		"\t-f <freq>\tAdd frequency in Hz to detect (use multiple times, if no added 440 Hz will be...)\n"
		"\n"
		"\t-t <treshold>\tSet treshold (used to hide magnitudes lower than treshold) (defaults -1)\n"
		"\t-n\t\tPrint integers rather than floats\n"
		"\t-l\t\tDo not repeat values while still over treshold\n"
		"\t-b\t\tDo not print first value that will fall under treshold\n"
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

void addfreq(int *freqs, int freq) {
	int i = 0;
	while(freqs[i]!=-1) i++;
	freqs[i]=freq;
	freqs[i+1]=-1;
}

int main(int argc, char ** argv) {
	int samplerate = 8000;
	int samplecount = 4000;
	int treshold = -1;
	char noreturn = 0;
	char repeat = 1;
	char integers=0;
	char verbose=1;
	int freqs[argc+1]; freqs[0]=-1;

	int opt;
	while ((opt = getopt(argc, argv, "?i:o:a:r:c:d:f:t:nlbq")) != -1) {
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
				addfreq(freqs, atoi(optarg));
				break;
			case 't':
				treshold = atoi(optarg);
				break;
			case 'n':
				integers = 1;
				break;
			case 'l':
				repeat = 0;
				break;
			case 'b':
				noreturn = 1;
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
			"#Detected tone: %d Hz\n"
			"#Samplerate: %d Hz\n"
			"#Frame lenght: %d samples\n"
			"#Treshold: %d\n"
			"#\n"
			,freqs[0],samplerate,samplecount,treshold);
		fflush(stderr);

		printf("#Position");
		int i; for(i=0;freqs[i]!=-1;i++) {
			printf("\t%2dHz",freqs[i]);
		}
		puts("");
	}

	char print=0, printnow=0, printlast = 0;
	while(!feof(stdin)) {
		int i;

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

			//Set print true if over treshold or if changed to false (print for the last time after going under treshold)
			printnow = power[i] > treshold;
			print = !(!repeat && printlast && !(!printnow)) && (print || printnow || (printlast && !noreturn));
		}
		printlast = printnow;
		fflush(stdout);

		//Print data
		if(print) {
			printf("%8.2f", position);
			for(i=0;freqs[i]!=-1;i++) {
				printf("\t");
				if(integers)
					printf("%d",(int)power[i]);
				else
					printf("%.4f",power[i]);
			}
			puts("");
			fflush(stdout);
		}

		//Increase time
		position += ((float)samplecount/(float)samplerate);
	}
}
