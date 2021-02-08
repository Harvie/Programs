#include <nekobee.h>
#include <nekobee_voice.h>
//#include <nekobee_voice_render.h>
#include <nekobee_synth.h>
#include <nekobee_types.h>
#include <esp_task_wdt.h>
#include "driver/i2s.h"
#include "freertos/queue.h"


#include <BLEMidi.h>
//void connected();
//void disconnected();


nekobee_synth_t fSynth;

#define SAMPLE_RATE 30000
#define BUF_LEN XSYNTH_NUGGET_SIZE //512

void nekobee_handle_raw_event(nekobee_synth_t* const synth, const uint8_t size, const uint8_t* const data)
{
    if (size != 3)
        return;

    switch (data[0] & 0xf0)
    {
    case 0x80:
        nekobee_synth_note_off(synth, data[1], data[2]);
        break;
    case 0x90:
        if (data[2] > 0)
            nekobee_synth_note_on(synth, data[1], data[2]);
        else
            nekobee_synth_note_off(synth, data[1], 64); /* shouldn't happen, but... */
        break;
    case 0xB0:
        nekobee_synth_control_change(synth, data[1], data[2]);
        break;
    default:
        break;
    }
}


static const i2s_port_t i2s_num = I2S_NUM_0; // i2s port number


  static const i2s_config_t i2s_config = {    
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX /*| I2S_MODE_DAC_BUILT_IN*/ ),                                                                                                                                                            
    .sample_rate = SAMPLE_RATE,                                                                                             
    .bits_per_sample = I2S_BITS_PER_SAMPLE_32BIT,  // only the top 8 bits will actually be used by the internal DAC, but using 8 bits stra> 
    .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,  // always use stereo output. mono seems to be buggy, and the overhead is insignifcant o> 
    .communication_format = (i2s_comm_format_t)(I2S_COMM_FORMAT_I2S | I2S_COMM_FORMAT_I2S_LSB),  // this appears to be the correct setting> 
    .intr_alloc_flags = 0, // default interrupt priority                                                                                    
    .dma_buf_count = 4,    // 8*128 bytes of buffer corresponds to 256 samples (2 channels, see above, 2 bytes per sample per channel)      
    .dma_buf_len = BUF_LEN,    
    .use_apll = false         
  };   

static const i2s_pin_config_t pin_config = {
    .bck_io_num = 26,
    .ws_io_num = 25,
    .data_out_num = 17,
    .data_in_num = I2S_PIN_NO_CHANGE
};

uint8_t midii=0, midichar, mididata[8];


void synth_task( void * pvParameters ) {



    i2s_driver_install(i2s_num, &i2s_config, 0, NULL);   //install and start i2s driver

    //i2s_set_pin(i2s_num, NULL); //for internal DAC, this will enable both of the internal channels
    i2s_set_pin(i2s_num, &pin_config);
    
    //You can call i2s_set_dac_mode to set built-in DAC output mode.
    //i2s_set_dac_mode(I2S_DAC_CHANNEL_BOTH_EN);

    //i2s_set_sample_rates(i2s_num, SAMPLE_RATE); //set sample rates

    //i2s_driver_uninstall(i2s_num); //stop & destroy i2s driver
    
    //i2s_adc_disable(i2s_num);




    

        float *out;    
    out = (float*)malloc(sizeof(float)*BUF_LEN);    
        int32_t *outdma;    
    outdma = (int32_t*)malloc(sizeof(int32_t)*BUF_LEN*8);

  int i = 0, n=60, v=60;    
  //nekobee_synth_note_on(&fSynth, n , v);    
  while(1) {  
    if(i==0) {
        esp_task_wdt_reset();     
        //nekobee_synth_note_off(&fSynth, n , v);    
        n = 16+rand()%63;    
        v = 16+rand()%63;  
         n = rand()%127;    
        v = rand()%127;                                                                                                                   
        //nekobee_synth_note_on(&fSynth, n , v);                                                                                              
    }




  while(Serial.available()) {
   
    midichar = Serial.read();
    if(midichar & 0b10000000) {
       midii = 0;
    } else if(midii == 0) continue;
    mididata[midii] = midichar;
    midii++;

    if(midii >= 3) {
      nekobee_handle_raw_event(&fSynth, midii, mididata);
      midii = 0;
    }
  }
                                                                                                                                            
    nekobee_synth_render_voices(&fSynth, out, BUF_LEN, 1);
    for(int i = 0; i < BUF_LEN; i++) {
      outdma[i*2] = outdma[(i*2)+1] = lrintf(out[i] * 2147483647); //((127)+round((out[i]*120.0)))*255;  
    }

    size_t bytes_written;
    i2s_write(i2s_num, outdma, BUF_LEN*4*2, &bytes_written, portMAX_DELAY);
                                                                                                                                            
    //printf("%d\t%f\n", outdma[0], out[0]);                                                                                                               
    //fwrite (out, sizeof(float)*BUF_LEN, 1, stdout);
    //Serial.println(out[0]);                                                                                              
    //Serial.println(uxTaskGetStackHighWaterMark(NULL));                                                                                    
    //Serial.printf("%d\n", outdma[0]);                                                                                                           
    //mgos_msleep(30);                                                                                                                      
    //delay(1);                                                                                                                            
    i++; if(i>400) i=0;                                                                                                                     
  }    
  
  }


uint8_t bn=0, bv=0;

void onNoteOn(uint8_t channel, uint8_t note, uint8_t velocity)
{
  nekobee_synth_note_off(&fSynth, bn , bv);  
  nekobee_synth_note_on(&fSynth, note , velocity);  
  bn = note, bv = velocity;
  Serial.print("on");
  Serial.println(note);
}

void onNoteOff(uint8_t channel, uint8_t note, uint8_t velocity)
{
  nekobee_synth_note_off(&fSynth, note , velocity);  
  Serial.print("off");
  Serial.println(note);
}




void setup() {
    Serial.begin(115200);
  
  nekobee_init_tables();

    // init synth
    fSynth.sample_rate = SAMPLE_RATE;
    fSynth.deltat = 1.0f / (float)SAMPLE_RATE;
    fSynth.nugget_remains = 0;

    fSynth.note_id = 0;
    fSynth.polyphony = XSYNTH_DEFAULT_POLYPHONY;
    fSynth.voices = XSYNTH_DEFAULT_POLYPHONY;
    fSynth.monophonic = XSYNTH_MONO_MODE_ONCE;
    fSynth.glide = 0;
    fSynth.last_noteon_pitch = 0.0f;
    fSynth.vcf_accent = 0.0f;
    fSynth.vca_accent = 0.0f;

    for (int i=0; i<8; ++i)
        fSynth.held_keys[i] = -1;

    fSynth.voice = nekobee_voice_new();
    fSynth.voicelist_mutex_grab_failed = 0;
    //pthread_mutex_init(&fSynth.voicelist_mutex, nullptr);

    fSynth.channel_pressure = 0;
    fSynth.pitch_wheel_sensitivity = 0;
    fSynth.pitch_wheel = 0;

    for (int i=0; i<128; ++i)
    {
        fSynth.key_pressure[i] = 0;
        fSynth.cc[i] = 0;
    }
    fSynth.cc[7] = 127; // full volume

    fSynth.mod_wheel  = 1.0f;
    fSynth.pitch_bend = 1.0f;
    fSynth.cc_volume  = 1.0f;

    // Default values
    /*
    fParams.waveform = 0.0f;
    fParams.tuning = 0.0f;
    fParams.cutoff = 25.0f;
    fParams.resonance = 25.0f;
    fParams.envMod = 50.0f;
    fParams.decay  = 75.0f;
    fParams.accent = 25.0f;
    fParams.volume = 75.0f;
    fParams.bypass = false;
    */

    // Internal stuff
    fSynth.waveform  = 0.0f;
    fSynth.tuning    = 1.0f;
    fSynth.cutoff    = 5.0f;
    fSynth.resonance = 0.8f;
    fSynth.envmod    = 0.3f;
    fSynth.decay     = 0.0002f;
    fSynth.accent    = 0.3f;
    fSynth.volume    = 0.75f;

    //nekobee_synth_render_voices(&fSynth, NULL, 0, 1); //Update controls
    nekobee_synth_init_controls(&fSynth);
    //nekobee_synth_note_on(&fSynth, 80, 128);

    Serial.println("READY");

    xTaskCreate(&synth_task, "blinking_led", 5*configMINIMAL_STACK_SIZE, NULL, 0, NULL);



  BLEMidiServer.begin("NekoMIDI");
  BLEMidiServer.setOnConnectCallback([](){     // To show how to make a callback with a lambda function
    Serial.println("Connected");
  });
  BLEMidiServer.setOnDisconnectCallback([](){     // To show how to make a callback with a lambda function
    Serial.println("Disconnected");
  });
  BLEMidiServer.setNoteOnCallback(onNoteOn);
  BLEMidiServer.setNoteOffCallback(onNoteOff);
  //BLEMidiServer.setControlChangeCallback(onControlChange);
  //BLEMidiServer.enableDebugging();
        
}


 
int i=0;
void loop() {
  /*
 void
 nekobee_voice_render(nekobee_synth_t *synth, nekobee_voice_t *voice,
                     float *out, unsigned long sample_count,
                     int do_control_update)

 void nekobee_synth_render_voices(nekobee_synth_t *synth, float *out,                                                                    
                                 unsigned long sample_count,                                                                            
                                int do_control_update);
 */
  
  //nekobee_synth_render_voices(&fSynth, outbuffer, len, update_controls);

  //float out;

  //nekobee_synth_render_voices(&fSynth, &out, 1, 0);

  //Serial.println(out);
  //Serial.println(i++);
  delay(30);

      

    //fSynth.waveform  = 0.0f;
    //fSynth.tuning    = 1.0f;

    int adc2 = 0;
    if(adc2_get_raw(ADC2_CHANNEL_2, ADC_WIDTH_12Bit, &adc2) == ESP_OK)
      fSynth.cutoff    = ((float)adc2)/(4095.0f/40.0f);
    if(adc2_get_raw(ADC2_CHANNEL_0, ADC_WIDTH_12Bit, &adc2) == ESP_OK)
      fSynth.resonance    = ((float)adc2)/(4095.0f);

    fSynth.envmod    = ((float)analogRead(35))/4095.0f;

    //fSynth.decay     = 0.0002f;
    fSynth.decay     = ((float)analogRead(34))/(4095.0f/0.0004f);
    fSynth.accent    = ((float)analogRead(36))/4095.0f;
    fSynth.volume    = ((float)analogRead(39))/4095.0f;

    /*
    Serial.print("SYNTH:");
    Serial.print("\tCUT:"); Serial.print(fSynth.cutoff);
    Serial.print("\tRES:"); Serial.print(fSynth.resonance);
    Serial.print("\tENV:"); Serial.print(fSynth.envmod);
    Serial.print("\tDEC:"); Serial.printf("%.7f", fSynth.decay);
    Serial.print("\tACC:"); Serial.print(fSynth.accent);
    Serial.print("\tVOL:"); Serial.print(fSynth.volume);
    Serial.println();
    */
  
}
