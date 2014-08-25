CC=g++
FLAGS=-O2 -DNDEBUG -Wall -std=gnu++0x -fdata-sections -ffunction-sections -Wl,--gc-sections
SOFLAGS=-fpic -shared -fno-strict-aliasing
INCLUDE=-I /usr/lib/jvm/java-6-openjdk/include
FILES=../../java/*.cpp ../../../../lib/etana/*.cpp ../../../lib/*.cpp ../../../../lib/fsc/*.cpp ../../../../lib/fsc/fsjni/*.cpp
MAP=-Wl,--version-script,../../java/morph.map
LIBS=-pthread -ldl

all: cpp java

cpp:
	$(CC) $(FLAGS) $(SOFLAGS) $(INCLUDE) -o libmorph_jni.so $(FILES) $(MAP) $(LIBS)

java:
	javac -d . ../../java/*.java
