/* Modified code from http://tldp.org/HOWTO/MIDI-HOWTO-9.html
 * This will print ALSA-Seq (MIDI) events to stdout for further processing eg.: in Arduino
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <alsa/asoundlib.h>

#define MIDI_NOTE_OFF	128
#define MIDI_NOTE_ON	144
#define MIDI_CONTROL	176
#define MIDI_PITCH_BEND	224
#define MIDI_FORMAT	"%c%c%c"
#define MIDI_FORMAT_ERR	"%d:%d:%d\n"

snd_seq_t *open_seq();
void midi_action(snd_seq_t *seq_handle);

snd_seq_t *open_seq() {

  snd_seq_t *seq_handle;
  int portid;

  if (snd_seq_open(&seq_handle, "hw", SND_SEQ_OPEN_DUPLEX, 0) < 0) {
    fprintf(stderr, "Error opening ALSA sequencer.\n");
    exit(1);
  }
  snd_seq_set_client_name(seq_handle, "ALSA Sequencer Pipe");
  if ((portid = snd_seq_create_simple_port(seq_handle, "ALSA Sequencer Pipe",
            SND_SEQ_PORT_CAP_WRITE|SND_SEQ_PORT_CAP_SUBS_WRITE,
            SND_SEQ_PORT_TYPE_APPLICATION)) < 0) {
    fprintf(stderr, "Error creating sequencer port.\n");
    exit(1);
  }
  return(seq_handle);
}

void midi_action(snd_seq_t *seq_handle) {

  snd_seq_event_t *ev;

  do {
    snd_seq_event_input(seq_handle, &ev);
    switch (ev->type) {
      case SND_SEQ_EVENT_CONTROLLER:
        printf(MIDI_FORMAT, MIDI_CONTROL, ev->data.control.channel, ev->data.control.value);
        break;
      case SND_SEQ_EVENT_PITCHBEND:
        printf(MIDI_FORMAT, MIDI_PITCH_BEND, ev->data.control.channel, ev->data.control.value);
        break;
      case SND_SEQ_EVENT_NOTEON:
        printf(MIDI_FORMAT, MIDI_NOTE_ON, ev->data.control.channel, ev->data.note.note);
        break;
      case SND_SEQ_EVENT_NOTEOFF:
        printf(MIDI_FORMAT, MIDI_NOTE_OFF, ev->data.control.channel, ev->data.note.note);
        break;
    }
		fflush(stdout); fflush(stderr);
    snd_seq_free_event(ev);
  } while (snd_seq_event_input_pending(seq_handle, 0) > 0);
}

int main(int argc, char *argv[]) {

  snd_seq_t *seq_handle;
  int npfd;
  struct pollfd *pfd;

  seq_handle = open_seq();
  npfd = snd_seq_poll_descriptors_count(seq_handle, POLLIN);
  pfd = (struct pollfd *)alloca(npfd * sizeof(struct pollfd));
  snd_seq_poll_descriptors(seq_handle, pfd, npfd, POLLIN);

	fprintf(stderr, "COMMAND:CHANNEL:NOTE\nCommands are described on http://www.ec.vanderbilt.edu/computermusic/musc216site/MIDI.Commands.html\n\n");
  while (1) {
    if (poll(pfd, npfd, 100000) > 0) {
      midi_action(seq_handle);
    }
  }
}
