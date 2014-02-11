proj=troll23

unexport LD_PRELOAD
$(proj).so: $(proj).c
	$(CC) -shared -fPIC -ldl $? -o $@
	# To test:
	# export LD_PRELOAD=$(PWD)/$@
	# touch /tmp/deleteme; rm /tmp/deleteme
	# Or:
	# make go
	# make ssh

go: test $(proj).so
	LD_PRELOAD=$(PWD)/$(proj).so ./test

ssh: $(proj).so
	LD_PRELOAD=$(PWD)/$(proj).so ./sshkeys.sh
