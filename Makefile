sample: sampleDriver.cpp Audiotron.cpp Audiotron.h AudiotronBase.h AudiotronBase.cpp
	g++ -o audiotron --std=c++11 sampleDriver.cpp Audiotron.cpp AudiotronBase.cpp

.PHONY: clean
clean:
	rm audiotron
