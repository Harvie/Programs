//CFLAGS=-lasound make beatdetect; echo -----; ./beatdetect hw:default
	#include <stdio.h>
	#include <stdlib.h>
	#include <alsa/asoundlib.h>
	      
	main (int argc, char *argv[])
	{
		int i;
		int err;
		short buf[128];
		snd_pcm_t *capture_handle;
		snd_pcm_hw_params_t *hw_params;
	
		if ((err = snd_pcm_open (&capture_handle, argv[1], SND_PCM_STREAM_CAPTURE, 0)) < 0) {
			fprintf (stderr, "cannot open audio device %s (%s)\n", 
				 argv[1],
				 snd_strerror (err));
			exit (1);
		}
		if ((err = snd_pcm_hw_params_malloc (&hw_params)) < 0) {
			fprintf (stderr, "cannot allocate hardware parameter structure (%s)\n",
				 snd_strerror (err));
			exit (1);
		}
				 
		if ((err = snd_pcm_hw_params_any (capture_handle, hw_params)) < 0) {
			fprintf (stderr, "cannot initialize hardware parameter structure (%s)\n",
				 snd_strerror (err));
			exit (1);
		}
	
		if ((err = snd_pcm_hw_params_set_access (capture_handle, hw_params, SND_PCM_ACCESS_RW_INTERLEAVED)) < 0) {
			fprintf (stderr, "cannot set access type (%s)\n",
				 snd_strerror (err));
			exit (1);
		}
	
		if ((err = snd_pcm_hw_params_set_format (capture_handle, hw_params, SND_PCM_FORMAT_S16_LE)) < 0) { //SND_PCM_FORMAT_S16_LE
			fprintf (stderr, "cannot set sample format (%s)\n",
				 snd_strerror (err));
			exit (1);
		}
	
/*			   puts("lolopre");
		if ((err = snd_pcm_hw_params_set_rate_near (capture_handle, hw_params, 44100, 0)) < 0) {
			   puts("loloerr");
			fprintf (stderr, "cannot set sample rate (%s)\n",
				 snd_strerror (err));
			exit (1);
		}
	
		if ((err = snd_pcm_hw_params_set_channels (capture_handle, hw_params, 2)) < 0) {
			fprintf (stderr, "cannot set channel count (%s)\n",
				 snd_strerror (err));
			exit (1);
		}
*/
	
		if ((err = snd_pcm_hw_params (capture_handle, hw_params)) < 0) {
			fprintf (stderr, "cannot set parameters (%s)\n",
				 snd_strerror (err));
			exit (1);
		}
	
		snd_pcm_hw_params_free (hw_params);
	
		if ((err = snd_pcm_prepare (capture_handle)) < 0) {
			fprintf (stderr, "cannot prepare audio interface for use (%s)\n",
				 snd_strerror (err));
			exit (1);
		}

		//for (i = 0; i < 10; ++i) {
		for (i = 0; 1; ++i) {
			if ((err = snd_pcm_readi (capture_handle, buf, 128)) != 128) {
				fprintf (stderr, "read from audio interface failed (%s)\n", snd_strerror (err));
				exit (1);
			} else {
				int j;
				for(j=0;j<128;j++) if((int)buf[j] > 1000) {
					printf("BOOM! %d\n", (int)buf[j]);
					break;
				}
			}
		}
	
		snd_pcm_close (capture_handle);
		exit (0);
	}
