proj=unlink-wrapper

unexport LD_PRELOAD
$(proj).so: $(proj).c
	$(CC) -shared -fPIC -ldl $? -o $@
	# To test:
	# export LD_PRELOAD=$(PWD)/$@
	# touch /tmp/deleteme; rm /tmp/deleteme
