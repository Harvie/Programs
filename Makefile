.PHONY: help clean

help:
	#use: make [help|clean]

clean:
	find | grep '~$$' | while read i; do rm -v "$$i"; done;
