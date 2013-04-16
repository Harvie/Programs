#include <stdio.h>
#include <unistd.h>
#include <alsa/asoundlib.h>
#include <ftdi.h>


#define BAUD 31250

static snd_rawmidi_t *midi_in, *midi_out;
struct ftdi_context ftdi;

int main(void) {


	snd_rawmidi_open(&midi_in, &midi_out, "virtual", SND_RAWMIDI_NONBLOCK);
	//snd_rawmidi_open(&midi_in, &midi_out, "virtual", 0);


	if (ftdi_init( &ftdi )) {
		fprintf(stderr,  "usb - init error !\n");
		return 1;
	}

	if (ftdi_usb_open(&ftdi, 0x0403, 0x6001)) {
		fprintf(stderr,  "usb - open error (cannot find?) !\n");
		fprintf(stderr, "ftdi_usb_open failed, error (%s)\n", ftdi_get_error_string(&ftdi));
		ftdi_deinit( &ftdi );
		return 2;
	}

	if (ftdi_usb_reset( &ftdi )) {
		fprintf(stderr,  "usb - reset error !\n");
		ftdi_usb_close( &ftdi );
		ftdi_deinit( &ftdi );
		return 3;
	}

	ftdi_disable_bitbang( &ftdi );
	ftdi_set_baudrate(&ftdi, BAUD);

	unsigned char buf;
	int ret;
	while(1) {
		//FTDI2MIDI
		ret = ftdi_read_data(&ftdi,&buf,1);
		if(ret < 0) break;
		if(ret > 0) snd_rawmidi_write(midi_out, &buf, 1);

		//MIDI2FTDI
		ret = snd_rawmidi_read(midi_in,&buf,1);
		if(ret < 0 && ret != -EAGAIN) break;
		if(ret > 0) ftdi_write_data(&ftdi, &buf, 1);

		usleep(1000);
	}
	exit(0);
}
