
TARGET=libvl53l0x.a

all: lib/${TARGET}

lib/${TARGET}: ./bin/${TARGET}
	mkdir $(dir $@); cp -f $< $@

./bin/${TARGET}:
	make -f Makefile.top

clean:
	make -f Makefile.top $@

.PHONY: clean

