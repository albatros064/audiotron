sample: sampleDriver.cpp Audiotron.cpp Audiotron.h
	g++ -o audiotron --std=c++11 sampleDriver.cpp Audiotron.cpp

.PHONY: clean
clean:
	rm audiotron
