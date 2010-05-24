/*
 * BrainDuino
 * Arduino BrainFuck Interpreter
 * <~~Harvie 2oo8
 *
 * Wanna real hardware BF processor?!
 */

#define memsize 100
#define stacksize 20
#define baudrate 9600
#define pwmoutpin 11
#define analoginpin 0

void brainfuck(int memory, char cycle, char *bfcode, char pwmpin, char ainpin) {
  //Serial.println("BF-STARTED");
  unsigned char tape[memory]; memset(tape, 0, sizeof(tape));   memory = 0;
  int stack[cycle];  memset(stack, 0, sizeof(stack)); cycle = -1;

  for(int i = 0;bfcode[i]!=0;i++) {
    switch(bfcode[i]) {
      //Basic BF instructions
      case '>': memory++; if(memory>=sizeof(tape)) { Serial.println("BF-EXIT-MEM-OVERFLOW!"); return; } break;
      case '<': memory--; if(memory<0) { Serial.println("BF-EXIT-MEM-UNDERFLOW!"); return; } break;
      case '+': tape[memory]++; break;
      case '-': tape[memory]--; break;
      case '.': Serial.print(tape[memory], BYTE); break;
      case ',': while(Serial.available()<=0) delay(30); tape[memory] = Serial.read(); break;
      case '[': cycle++; if(cycle>=sizeof(stack)) { Serial.println("BF-EXIT-STACK-OVERFLOW!"); return; } stack[cycle] = i; break;
      case ']': if(tape[memory] != 0) { i = stack[cycle]-1; } cycle--; if(cycle<-1) { Serial.println("BF-EXIT-STACK-UNDERFLOW!"); return; } break;
      //Optional BF instructions
      case '#': Serial.print("BF-DEBUG: POS: "); Serial.print(memory, DEC);
                Serial.print(" TAPE: "); Serial.println(tape[memory], BYTE);
                Serial.print(" TAPE: "); Serial.println(tape[memory], BYTE);
      break;
      case '@': Serial.println("BF-EXIT-BY-CODE!"); return; break;
      //Arduino extensions for BF
      case '!': if(pwmpin>=0 && tape[memory]>0 && tape[memory]<255) analogWrite(pwmpin, tape[memory]); break; //Set pwmpin output value
      case '?': if(ainpin>=0) tape[memory]=analogRead(ainpin); break; //Read ainpin analog value (sux-a-bit)
      case '_': delay(10); break; //Wait for a while
    }
  }
  //Serial.println("BF-ENDED");
  return;
}

void setup()                    // run once, when the sketch starts
{
  Serial.begin(baudrate);
  char bfcode[]=
  //"++++++++++[>+++++++>++++++++++>+++>+<<<<-]>++.>+.+++++++..+++.>++.<<+++++++++++++++.>.+++.------.--------.>+.>."; //Hello World
  //"[,.]"; //cat
  //"[?!+]"; //Read ananlog input pin and write it's value to pwm output pin
  //"[+[-!__]+++++++++++++++++++++++++++++++++++++++]"; //Fade LED on pwmpin
  "[[-]++++++++++[>+++++++>++++++++++>+++>+<<<<-]>++.>+.+++++++..+++.>++.<<+++++++++++++++.>.+++.------.--------.>+.>.[-]<[-]<[-]<[-!_]<+]"; //Hello World with pwm fading

  brainfuck(memsize, stacksize, bfcode, pwmoutpin, analoginpin);
}


void loop() {                     // run over and over again

}


