all: clause instance main clean

main: main.cpp clause instance
	@g++ -Wc++11-extensions -c main.cpp -o main.o
	@g++ -Wc++11-extensions main.o clause.o instance.o -o main

clause: clause/clause.cpp clause/clause.h clause/tests.cpp
	@g++ -c clause/clause.cpp -o clause.o
	@g++ -c clause/tests.cpp -o clause_tests.o
	@g++ clause.o clause_tests.o -o clause_tests
	@echo Running clause object unit tests
	@./clause_tests
	@echo All clause object unit tests passed
	@rm clause_tests

instance: clause instance/instance.cpp instance/instance.h instance/tests.cpp
	@g++ -c instance/instance.cpp -o instance.o
	@g++ -c instance/tests.cpp -o instance_tests.o
	@g++ instance.o instance_tests.o clause.o -o instance_tests
	@echo Running instance object unit tests
	@./instance_tests
	@echo All instance object unit tests passed
	@rm instance_tests

clean:
	@rm *.o