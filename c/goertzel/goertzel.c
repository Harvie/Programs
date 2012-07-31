#include <stdio.h>
#include <math.h>

/*
	Usage examples
	arecord | ./goertzel
	sox input.mp3 -b 8 -c 1 -r 8000 -t wav - | ./goertzel
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

int main() {
	/*
	int samples[] = {0,1,2,3,4,5,6,7,8,9,8,7,6,5,4,3,2,1};
	int samplecount = 18;
	float power = goertzel(samplecount, samples, 1.2, 18);
	printf("G: %f\n", power);
	*/

	int samplerate = 8000;
	int samplecount = 4000;
	float samples[samplecount];
	float position = 0;
	fprintf(stderr,"Position (Secs)\tMagnitude\n");
	while(!feof(stdin)) {
		int i;
		for(i=0;i<samplecount && !feof(stdin);i++) {
			unsigned char sample;
			fread(&sample,1,1,stdin);
			samples[i]=sample;
			//printf("%d\n", sample);
		}
		position += ((float)samplecount/(float)samplerate);
		float power = goertzel_mag(samplecount, 440, samplerate, samples);
		printf("%f\t%f\n", position, power);
	}
}
