CC=gcc

compile:
	mkdir -p build
	@read -p "Day #: " day; \
		$(CC) -o build/out day-$$day/src/main.c

run: compile
	./build/out
