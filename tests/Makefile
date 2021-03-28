all: mk_fff
	mkdir -p build
	g++ -o build/tst test_func.cpp fff/build/*.o -pthread

mk_fff:
	mkdir -p fff/build
	cd fff/gtest; $(MAKE) all

test:
	./build/tst

clean: 
	cd fff/gtest; $(MAKE) clean
	rm -rf fff/build build

#	g++ -o build/tst test_func.cpp fff/build/gtest-all.o fff/build/gtest-main.o -pthread
