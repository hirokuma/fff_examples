all: mk_fff
	mkdir -p build
	g++ -Ifff/build/_deps/googletest-src/googletest/include -o build/tst test_func.cpp -Lfff/build/lib -lgtest -lgtest_main -pthread

mk_fff:
	mkdir -p fff/build
	cd fff && cmake -B build -DFFF_GENERATE=ON -DFFF_UNIT_TESTING=ON && cmake --build build && cd ..

test:
	./build/tst

clean: 
	rm -rf fff/build build
