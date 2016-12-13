.PHONY: all clean clean-subdirs

all: lib/libalglib.a

lib/libalglib.a:
	cd alglib/cpp/src && g++ -c *.cpp && ar rcs libalglib.a *.o
	mkdir -p include/alglib/ lib/
	cp alglib/cpp/src/*.h include/alglib/
	cp alglib/cpp/src/libalglib.a lib/


clean: clean-subdirs
	rm -rf include/* lib/*

clean-subdirs:
	rm -f alglib/cpp/src/*.{o,a}
