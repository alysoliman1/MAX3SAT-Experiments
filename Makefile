all: clause instance bfs clean

bfs: bfs.cpp clause instance
	@g++ -std=c++11 -c bfs.cpp -o bfs.o
	@g++ -std=c++11 bfs.o clause.o instance.o search.o -o build/bfs

clause: clause/clause.cpp clause/clause.hpp clause/tests.cpp
	@g++ -std=c++11 -c clause/clause.cpp -o clause.o
	@g++ -std=c++11 -c clause/tests.cpp -o clause_tests.o
	@g++ -std=c++11 clause.o clause_tests.o -o clause_tests
	@echo Running clause object unit tests
	@./clause_tests
	@echo All clause object unit tests passed
	@rm clause_tests

instance: clause instance/instance.cpp instance/instance.hpp instance/search.cpp instance/tests.cpp
	@g++ -c instance/instance.cpp -std=c++11 -o instance.o
	@g++ -c instance/search.cpp -std=c++11 -o search.o
	@g++ -c instance/tests.cpp -std=c++11 -o instance_tests.o
	@g++ instance.o instance_tests.o clause.o search.o -std=c++11 -o instance_tests
	@echo Running instance object unit tests
	@./instance_tests
	@echo All instance object unit tests passed
	@rm instance_tests

clean:
	@rm *.o
