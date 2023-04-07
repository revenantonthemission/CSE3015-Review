OBJECTS = dog.o blackcow.o turtle.o main.o
TARGET = animal_exe

$(TARGET): $(OBJECTS)
	gcc -o animal_exe $(OBJECTS)
dog.o: dog.c
	gcc -c -o dog.o dog.c
blackcow.o: blackcow.c
	gcc -c -o blackcow.o blackcow.c
turtle.o: turtle.c
	gcc -c -o turtle.o turtle.c
main.o: main.c
	gcc -c -o main.o main.c

_PHONY: clean
clean:
	rm -rf $(TARGET) $(OBJECTS)
