#MCU=at90s2313
MCU=attiny2313
PORT=/dev/ttyUSB0
CC=avr-gcc
OBJCOPY=avr-objcopy
PROJECT=main
RESET=false
# optimize for size:
CFLAGS=-g -mmcu=$(MCU) -Wall -Wstrict-prototypes -Os -mcall-prologues
#-------------------
all: $(PROJECT).hex
#-------------------
$(PROJECT).hex : $(PROJECT).out 
	$(OBJCOPY) -R .eeprom -O ihex $(PROJECT).out 	$(PROJECT).hex 
$(PROJECT).out : $(PROJECT).o 
	$(CC) $(CFLAGS) -o $(PROJECT).out -Wl,-Map,$(PROJECT).map $(PROJECT).o 
$(PROJECT).o : $(PROJECT).c 
	$(CC) $(CFLAGS) -Os -c $(PROJECT).c
asm : $(PROJECT).c 
	$(CC) $(CFLAGS) -O -S $(PROJECT).c
# you need to erase first before loading the program.
# load (program) the software into the eeprom:
load: $(PROJECT).hex
	$(RESET) && stty -F $(PORT) 19200 hupcl && sleep 2 && stty -F $(PORT) 19200 -hupcl && sleep 2 || true
	avrdude -cstk500v1 -P$(PORT) -b19200 -p $(MCU) -Uflash:w:$(PROJECT).hex:i || $(MAKE) RESET=true MAKE=true load
	# uisp -dlpt=/dev/parport0 --erase --upload --verify if=$(PROJECT).hex -dprog=dapa  -v=3 --hash=12
#-------------------
clean:
	rm -f *.o *.map *.out *.hex
#-------------------
