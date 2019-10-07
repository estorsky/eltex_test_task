all:
	-ctags -R .
	cd mysystemd_shared_lib; make all
	cd mysystemd; make all
	cd mysystemd/deamons; make all

clean:
	rm -f tags
	cd mysystemd_shared_lib; make clean
	cd mysystemd; make clean
	cd mysystemd/deamons; make clean

