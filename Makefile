all:
	mkdir -p build
	cd gtest; $(MAKE) all
	g++ -o build/tst test_func.cpp build/gtest-all.o build/gtest-main.o -pthread

test:
	./build/tst

clean: 
	cd gtest; $(MAKE) clean
	rm -rf build
