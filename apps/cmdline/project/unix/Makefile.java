CC=g++
FLAGS=-O2 -DNDEBUG -Wall -std=gnu++0x -fdata-sections -ffunction-sections -Wl,--gc-sections
SOFLAGS=-fpic -shared -fno-strict-aliasing
INCLUDE=-I /usr/lib/jvm/default-java/include
COMMONCPP=../../java/utils.cpp ../../../../lib/proof/*.cpp ../../../../lib/etana/*.cpp ../../../lib/*.cpp ../../../../lib/fsc/*.cpp ../../../../lib/fsc/fsjni/*.cpp
LIBS=-pthread -ldl

all: cpp java

cpp: cpp_linguistic cpp_disambiguator

cpp_linguistic:
	$(CC) $(FLAGS) $(SOFLAGS) $(INCLUDE) -o liblinguistic_jni.so \
	../../java/linguistic_jni.cpp $(COMMONCPP) \
	-Wl,--version-script,../../java/linguistic_jni.map $(LIBS)

cpp_disambiguator:
	$(CC) $(FLAGS) $(SOFLAGS) $(INCLUDE) -o libdisambiguator_jni.so \
	../../java/disambiguator_jni.cpp ../../../../lib/etyhh/*.cpp $(COMMONCPP) \
	-Wl,--version-script,../../java/disambiguator_jni.map $(LIBS)

java:
	javac -d . ../../java/*.java
