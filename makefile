SHELL := /bin/bash
compile_opts = -std=c++14 -mavx -march=native -Wall -pthread

.PHONY: clean testcover testcpp spell leaks profilecpp

CPPLINKS = -I/usr/include/eigen3/ -I/usr/local/include/eigen3/ -lm -ladept -lsundials_cvodes -lsundials_cvode -lsundials_nvecserial -lstdc++ -lcppunit

ckine.so: model.cpp model.hpp reaction.hpp
	g++ $(compile_opts) -O3 $(CPPLINKS) model.cpp --shared -fPIC $(CPPLINKS) -o $@

libckine.debug.so: model.cpp model.hpp reaction.hpp
	g++ -g $(compile_opts) -O3 $(CPPLINKS) model.cpp --shared -fPIC $(CPPLINKS) -o $@

cppcheck: libckine.debug.so model.hpp cppcheck.cpp reaction.hpp
	g++ -g $(compile_opts) -L./ cppcheck.cpp $(CPPLINKS) -lckine.debug -Wl,-rpath=./ -o $@

testcpp: cppcheck
	./cppcheck

cprofile.svg: cppcheck
	valgrind --tool=callgrind ./cppcheck
	gprof2dot -f callgrind -n 5.0 callgrind.out.* | dot -Tsvg -o cprofile.svg

leaks: cppcheck
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --trace-children=yes ./cppcheck
