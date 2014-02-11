proj=cano

unexport LD_PRELOAD
$(proj).so: $(proj).c
	$(CC) -shared -fPIC -ldl $? -o $@
