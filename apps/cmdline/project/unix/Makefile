# Kompileerimiseks: make -s -j all

include Makefile.base

# muutujad (lokaalsed)

LIBS=json.ar proof.ar etyhh.ar etana.ar fsc.ar
LIBS_DB=$(LIBS:.ar=.db.ar)
EXE=vmeta vmyhh etdisamb etana etsyn stlspeller
EXE_DB=$(addsuffix .db,$(EXE))

# sihid
all : lib lib_db exe exe_db

exe : $(EXE)

exe_db: $(EXE_DB)

lib: $(LIBS)

lib_db: $(LIBS_DB)


clean :
	-rm -rf vmeta*
	-rm -rf vmeta.db*
	-rm -rf vmyhh*
	-rm -rf vmyhh.db*
	-rm -rf etdisamb*
	-rm -rf etdisamb.db*
	-rm -rf etana*
	-rm -rf etana.db*
	-rm -rf etsyn*
	-rm -rf etsyn.db*
	-rm -rf stlspeller*
	-rm -rf stlspeller.db*
	-rm -rf etyhh.ar*
	-rm -rf etyhh.db.ar*
	-rm -rf etana.ar*
	-rm -rf etana.db.ar*
	-rm -rf fsc.ar*
	-rm -rf fsc.db.ar*
	-rm -rf proof.ar*
	-rm -rf proof.db.ar*
	-rm -rf json.ar*
	-rm -rf json.db.ar*


# teegid	
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

proof.ar : $(wildcard $(ROOTDIR)/lib/proof/*.cpp) $(wildcard $(ROOTDIR)/lib/proof/*.h)
	$(call compile2ar,$@,../$(ROOTDIR)/lib/$(basename $@))

proof.db.ar : $(wildcard $(ROOTDIR)/lib/proof/*.cpp) $(wildcard $(ROOTDIR)/lib/proof/*.h)
	$(call compile2dbar,$@,../$(ROOTDIR)/lib/$(basename $(basename $@)))

json.ar : $(wildcard $(ROOTDIR)/apps/lib/*.cpp) $(wildcard $(ROOTDIR)/apps/lib/*.h)
	$(call compile2ar,$@,../$(ROOTDIR)/apps/lib)

json.db.ar : $(wildcard $(ROOTDIR)/apps/lib/*.cpp) $(wildcard $(ROOTDIR)/apps/lib/*.h)
	$(call compile2dbar,$@,../$(ROOTDIR)/apps/lib)


# programmid
vmeta : $(LIBS) $(wildcard $(ROOTDIR)/apps/cmdline/vmeta/*.cpp) $(wildcard $(ROOTDIR)/apps/cmdline/vmeta/*.h)
	$(call compile2bin,$@,../$(ROOTDIR)/apps/cmdline/$(basename $@))

vmeta.db : $(LIBS_DB) $(wildcard $(ROOTDIR)/apps/cmdline/vmeta/*.cpp) $(wildcard $(ROOTDIR)/apps/cmdline/vmeta/*.h)
	$(call compile2dbbin,$@,../$(ROOTDIR)/apps/cmdline/$(basename $@))

vmyhh : $(LIBS) $(wildcard $(ROOTDIR)/apps/cmdline/vmyhh/*.cpp) $(wildcard $(ROOTDIR)/apps/cmdline/vmyhh/*.h)
	$(call compile2bin,$@,../$(ROOTDIR)/apps/cmdline/$(basename $@))

vmyhh.db : $(LIBS_DB) $(wildcard $(ROOTDIR)/apps/cmdline/vmyhh/*.cpp) $(wildcard $(ROOTDIR)/apps/cmdline/vmyhh/*.h)
	$(call compile2dbbin,$@,../$(ROOTDIR)/apps/cmdline/$(basename $@))

etdisamb : $(LIBS) $(wildcard $(ROOTDIR)/apps/cmdline/etdisamb/*.cpp) $(wildcard $(ROOTDIR)/apps/cmdline/etdisamb/*.h)
	$(call compile2bin,$@,../$(ROOTDIR)/apps/cmdline/$(basename $@))

etdisamb.db : $(LIBS_DB) $(wildcard $(ROOTDIR)/apps/cmdline/etdisamb/*.cpp) $(wildcard $(ROOTDIR)/apps/cmdline/etdisamb/*.h)
	$(call compile2dbbin,$@,../$(ROOTDIR)/apps/cmdline/$(basename $@))

etana : $(LIBS) $(wildcard $(ROOTDIR)/apps/cmdline/etana/*.cpp) $(wildcard $(ROOTDIR)/apps/cmdline/etana/*.h)
	$(call compile2bin,$@,../$(ROOTDIR)/apps/cmdline/$(basename $@))

etana.db : $(LIBS_DB) $(wildcard $(ROOTDIR)/apps/cmdline/etana/*.cpp) $(wildcard $(ROOTDIR)/apps/cmdline/etana/*.h)
	$(call compile2dbbin,$@,../$(ROOTDIR)/apps/cmdline/$(basename $@))

etsyn : $(LIBS) $(wildcard $(ROOTDIR)/apps/cmdline/etsyn/*.cpp) $(wildcard $(ROOTDIR)/apps/cmdline/etsyn/*.h)
	$(call compile2bin,$@,../$(ROOTDIR)/apps/cmdline/$(basename $@))

etsyn.db : $(LIBS_DB) $(wildcard $(ROOTDIR)/apps/cmdline/etsyn/*.cpp) $(wildcard $(ROOTDIR)/apps/cmdline/etsyn/*.h)
	$(call compile2dbbin,$@,../$(ROOTDIR)/apps/cmdline/$(basename $@))

stlspeller : $(LIBS) $(wildcard $(ROOTDIR)/apps/cmdline/stlspeller/*.cpp) $(wildcard $(ROOTDIR)/apps/cmdline/stlspeller/*.h)
	$(call compile2bin,$@,../$(ROOTDIR)/apps/cmdline/$(basename $@))

stlspeller.db : $(LIBS_DB) $(wildcard $(ROOTDIR)/apps/cmdline/stlspeller/*.cpp) $(wildcard $(ROOTDIR)/apps/cmdline/stlspeller/*.h)
	$(call compile2dbbin,$@,../$(ROOTDIR)/apps/cmdline/$(basename $@))





















