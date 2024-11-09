SAN_DEBUG ?= 0
CXXFLAGS ?= -std=c++20
FLAG ?= csawctf{placeholder}

dept_of_sanitization: dept_of_sanitization.o libsan.so
	$(CXX) -o $@ dept_of_sanitization.o -L. -lsan -Wl,-rpath,'$$ORIGIN'

dept_of_sanitization.o: dept_of_sanitization.c
	# sanitize-address-outline-instrumentation forces calls to us
	# -mllvm -asan-stack=false prevents always inlined checks of stack stuff
	$(CC) -c -o $@ -fsanitize=address -fsanitize-address-outline-instrumentation -mllvm -asan-stack=false $^

libsan.so: libsan.o asan_abi_shim.o xxhash.o
	$(CXX) -shared -o $@ $^ -ldl

asan_abi_shim.o: asan_abi_shim.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $^

libsan.o: san.c
	$(CC) -fPIC -DDEBUG=$(SAN_DEBUG) -DFLAG=$(FLAG) -c -o $@ $^

xxhash.o: xxhash.c
	$(CC) -c -o $@ $^

.PHONY: clean
clean:
	rm -f dept_of_sanitization *.o *.so
