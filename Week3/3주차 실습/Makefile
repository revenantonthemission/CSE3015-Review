CC=g++
TARGET=test
OBJECTS=main.o array.o rangearray.o

$(TARGET): $(OBJECTS)
	$(CC) -o $(TARGET) $(OBJECTS)
%.o: %.c
	$(CC) -c $< -o $@
$(OBJECTS): Array.h RangeArray.h

.PHONY: clean
clean:
	rm -f $(TARGET) $(OBJECTS)
