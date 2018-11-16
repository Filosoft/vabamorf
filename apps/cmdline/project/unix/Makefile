MFLIST=$(wildcard Makefile.*.ar wildcard Makefile.*.exe)

all:
	for d in $(MFLIST); do echo == $$d ; make -s -f $$d all; done

clean:
	for d in $(MFLIST); do echo == $$d ; make -s -f $$d clean; done

help:
	# Sihid: all.....Kompileeri sõnastiku tegemise programmid
	#        clean...Kustuta kokkukompileeritud sõnastiku tegemise programmid 

