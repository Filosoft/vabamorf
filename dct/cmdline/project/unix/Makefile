# Kompileerimiseks: make -s -j all

include ../../../../apps/cmdline/project/unix/Makefile.base

# muutujad (lokaalsed)

LIBS=dct-nomvrb.ar etyhh.ar etana.ar fsc.ar
LIBS_DB=$(LIBS:.ar=.db.ar)
EXE=dct-conv11x dct-conv4 dct-init3 dct-noom dct-palakuju dct-vrb dct-t3mesta dct-t3pakitud
EXE_DB=$(addsuffix .db,$(EXE))

# sihid
all : lib lib_db exe exe_db

exe : $(EXE)

exe_db: $(EXE_DB)

lib: $(LIBS)

lib_db: $(LIBS_DB)

clean :
	-rm -rf dct-conv4*
	-rm -rf dct-conv4.db*
	-rm -rf dct-init3
	-rm -rf dct-init3.db*
	-rm -rf dct-noom*
	-rm -rf dct-noom.db*
	-rm -rf dct-palakuju*
	-rm -rf dct-palakuju.db*
	-rm -rf dct-vrb*
	-rm -rf dct-vrb.db*
	-rm -rf dct-t3mesta*
	-rm -rf dct-t3mesta.db*
	-rm -rf dct-t3pakitud*
	-rm -rf dct-t3pakitud.db*
	-rm -rf dct-conv11x*
	-rm -rf dct-conv11x.db*
	-rm -rf dct-nomvrb.ar*
	-rm -rf dct-nomvrb.db.ar*
	-rm -rf etyhh.ar*
	-rm -rf etyhh.db.ar*
	-rm -rf etana.ar*
	-rm -rf etana.db.ar*
	-rm -rf fsc.ar*
	-rm -rf fsc.db.ar*

# teegid

dct-nomvrb.ar : $(wildcard $(ROOTDIR)/dct/lib/dct-nomvrb/*.cpp) $(wildcard $(ROOTDIR)/dct/lib/dct-nomvrb/*.h)
	$(call compile2ar,$@,../$(ROOTDIR)/dct/lib/$(basename $@))

dct-nomvrb.db.ar : $(wildcard $(ROOTDIR)/dct/lib/dct-nomvrb/*.cpp) $(wildcard $(ROOTDIR)/dct/lib/dct-nomvrb/*.h)
	$(call compile2dbar,$@,../$(ROOTDIR)/dct/lib/$(basename $(basename $@)))
	
etyhh.ar : $(wildcard $(ROOTDIR)/lib/etyhh/*.cpp) $(wildcard $(ROOTDIR)/lib/etyhh/*.h)
	$(call compile2ar,$@,../$(ROOTDIR)/lib/$(basename $@))

etyhh.db.ar : $(wildcard $(ROOTDIR)/lib/etyhh/*.cpp) $(wildcard $(ROOTDIR)/lib/etyhh/*.h)
	$(call compile2dbar,$@,../$(ROOTDIR)/lib/$(basename $(basename $@)))

etana.ar : $(wildcard $(ROOTDIR)/lib/etana/*.cpp) $(wildcard $(ROOTDIR)/lib/etana/*.h)
	$(call compile2ar,$@,../$(ROOTDIR)/lib/$(basename $@))

etana.db.ar : $(wildcard $(ROOTDIR)/lib/etana/*.cpp) $(wildcard $(ROOTDIR)/lib/etana/*.h)
	$(call compile2dbar,$@,../$(ROOTDIR)/lib/$(basename $(basename $@)))

fsc.ar : $(wildcard $(ROOTDIR)/lib/fsc/*.cpp) $(wildcard $(ROOTDIR)/lib/fsc/*.h)
	$(call compile2ar,$@,../$(ROOTDIR)/lib/$(basename $@))

fsc.db.ar : $(wildcard $(ROOTDIR)/lib/fsc/*.cpp) $(wildcard $(ROOTDIR)/lib/fsc/*.h)
	$(call compile2dbar,$@,../$(ROOTDIR)/lib/$(basename $(basename $@)))

# programmid

dct-conv11x : $(LIBS) $(wildcard $(ROOTDIR)/dct/cmdline/dct-conv11x/*.cpp) $(wildcard $(ROOTDIR)/dct/cmdline/dct-conv11x/*.h)
	$(call compile2bin,$@,../$(ROOTDIR)/dct/cmdline/$(basename $@))

dct-conv11x.db : $(LIBS_DB) $(wildcard $(ROOTDIR)/dct/cmdline/dct-conv11x/*.cpp) $(wildcard $(ROOTDIR)/dct/cmdline/dct-conv11x/*.h)
	$(call compile2dbbin,$@,../$(ROOTDIR)/dct/cmdline/$(basename $@))

dct-conv4 : $(LIBS) $(wildcard $(ROOTDIR)/dct/cmdline/dct-conv4/*.cpp) $(wildcard $(ROOTDIR)/dct/cmdline/dct-conv4/*.h)
	$(call compile2bin,$@,../$(ROOTDIR)/dct/cmdline/$(basename $@))

dct-conv4.db : $(LIBS_DB) $(wildcard $(ROOTDIR)/dct/cmdline/dct-conv4/*.cpp) $(wildcard $(ROOTDIR)/dct/cmdline/dct-conv4/*.h)
	$(call compile2dbbin,$@,../$(ROOTDIR)/dct/cmdline/$(basename $@))

dct-init3 : $(LIBS) $(wildcard $(ROOTDIR)/dct/cmdline/dct-init3/*.cpp) $(wildcard $(ROOTDIR)/dct/cmdline/dct-init3/*.h)
	$(call compile2bin,$@,../$(ROOTDIR)/dct/cmdline/$(basename $@))

dct-init3.db : $(LIBS_DB) $(wildcard $(ROOTDIR)/dct/cmdline/dct-init3/*.cpp) $(wildcard $(ROOTDIR)/dct/cmdline/dct-init3/*.h)
	$(call compile2dbbin,$@,../$(ROOTDIR)/dct/cmdline/$(basename $@))

dct-noom : $(LIBS) $(wildcard $(ROOTDIR)/dct/cmdline/dct-noom/*.cpp) $(wildcard $(ROOTDIR)/dct/cmdline/dct-noom/*.h)
	$(call compile2bin,$@,../$(ROOTDIR)/dct/cmdline/$(basename $@))

dct-noom.db : $(LIBS_DB) $(wildcard $(ROOTDIR)/dct/cmdline/dct-noom/*.cpp) $(wildcard $(ROOTDIR)/dct/cmdline/dct-noom/*.h)
	$(call compile2dbbin,$@,../$(ROOTDIR)/dct/cmdline/$(basename $@))

dct-palakuju : $(LIBS) $(wildcard $(ROOTDIR)/dct/cmdline/dct-palakuju/*.cpp) $(wildcard $(ROOTDIR)/dct/cmdline/dct-palakuju/*.h)
	$(call compile2bin,$@,../$(ROOTDIR)/dct/cmdline/$(basename $@))

dct-palakuju.db : $(LIBS_DB) $(wildcard $(ROOTDIR)/dct/cmdline/dct-palakuju/*.cpp) $(wildcard $(ROOTDIR)/dct/cmdline/dct-palakuju/*.h)
	$(call compile2dbbin,$@,../$(ROOTDIR)/dct/cmdline/$(basename $@))

dct-vrb : $(LIBS) $(wildcard $(ROOTDIR)/dct/cmdline/dct-vrb/*.cpp) $(wildcard $(ROOTDIR)/dct/cmdline/dct-vrb/*.h)
	$(call compile2bin,$@,../$(ROOTDIR)/dct/cmdline/$(basename $@))

dct-vrb.db : $(LIBS_DB) $(wildcard $(ROOTDIR)/dct/cmdline/dct-vrb/*.cpp) $(wildcard $(ROOTDIR)/dct/cmdline/dct-vrb/*.h)
	$(call compile2dbbin,$@,../$(ROOTDIR)/dct/cmdline/$(basename $@))

dct-t3pakitud : $(LIBS) $(wildcard $(ROOTDIR)/dct/cmdline/dct-t3pakitud/*.cpp) $(wildcard $(ROOTDIR)/dct/cmdline/dct-t3pakitud/*.h)
	$(call compile2bin,$@,../$(ROOTDIR)/dct/cmdline/$(basename $@))

dct-t3pakitud.db : $(LIBS_DB) $(wildcard $(ROOTDIR)/dct/cmdline/dct-t3pakitud/*.cpp) $(wildcard $(ROOTDIR)/dct/cmdline/dct-t3pakitud/*.h)
	$(call compile2dbbin,$@,../$(ROOTDIR)/dct/cmdline/$(basename $@))

dct-t3mesta : $(LIBS) $(wildcard $(ROOTDIR)/dct/cmdline/dct-t3mesta/*.cpp) $(wildcard $(ROOTDIR)/dct/cmdline/dct-t3mesta/*.h)
	$(call compile2bin,$@,../$(ROOTDIR)/dct/cmdline/$(basename $@))

dct-t3mesta.db : $(LIBS_DB) $(wildcard $(ROOTDIR)/dct/cmdline/dct-t3mesta/*.cpp) $(wildcard $(ROOTDIR)/dct/cmdline/dct-t3mesta/*.h)
	$(call compile2dbbin,$@,../$(ROOTDIR)/dct/cmdline/$(basename $@))


