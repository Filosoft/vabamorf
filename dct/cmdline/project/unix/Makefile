MFLIST=$(wildcard Makefile.*.ar wildcard Makefile.*.exe)

all:
	for d in $(MFLIST); do echo == $$d ; make -s -f $$d all; done
	-rm etana.ar etyhh.ar fsc.ar dct-nomvrb.ar

clean:
	for d in $(MFLIST); do echo == $$d ; make -s -f $$d clean; done

help:
	# Sihid: all.....Kompileeri sõnastiku tegemise programmid
	#        clean...Kustuta kokkukompileeritud sõnastiku tegemise programmid 

