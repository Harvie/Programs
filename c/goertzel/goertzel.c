#include <stdio.h>
#include <math.h>
#include <getopt.h>


/*
	Usage examples
	arecord | ./goertzel
	sox input.mp3 -b 8 -c 1 -r 8000 -t wav - | ./goertzel

	Arguments for DTMF decoding:
	-f 697 -f 770 -f 852 -f 941 -f 1209 -f 1336 -f 1477 -f 1633 -t 10
*/

float goertzel_mag(int numSamples,int TARGET_FREQUENCY,int SAMPLING_RATE, float* data)
{
	/*
		On lower samplerates and frame sizes this may perform sub-optimally. Eg.:
		When set to detect 440Hz (at 8000Hz samplerate and ~4000 samples)
		it actually detects something around 438,3Hz rather than 400Hz...
		If you can't increase samplerate way around this is just to increase sensitivity.
	*/

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
	printf("help me %s\n", argv[0]);
}

void addfreq(int *freqs, int freq) {
	int i = 0;
	while(freqs[i]!=-1) i++;
	freqs[i]=freq;
	freqs[i+1]=-1;
}

int main(int argc, char ** argv) {
	/*
	int samples[] = {0,1,2,3,4,5,6,7,8,9,8,7,6,5,4,3,2,1};
	int samplecount = 18;
	float power = goertzel(samplecount, samples, 1.2, 18);
	printf("G: %f\n", power);
	*/

	int samplerate = 8000;
	int samplecount = 4000;
	int treshold = -1;
	char noreturn = 0;
	char repeat = 1;
	char integers=0;
	char verbose=1;
	int freqs[argc+1]; freqs[0]=-1;

	int opt;
	while ((opt = getopt(argc, argv, "?d:r:s:f:t:iqna")) != -1) {
		switch (opt) {
			case 'r':
				samplerate = atoi(optarg);
				break;
			case 's':
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
			case 'i':
				integers = 1;
				break;
			case 'a':
				repeat = 0;
				break;
			case 'n':
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
