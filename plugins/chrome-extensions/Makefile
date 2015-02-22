all:	clean
	for i in *; do [ -d "$$i" ] && zip -r "$$i".zip "$$i" || true; done

clean:
	rm *.zip || true
