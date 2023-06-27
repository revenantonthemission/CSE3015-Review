#컴파일의 최종 결과물을 TARGET 매크로로 지정한다.
TARGET = test

#컴파일에 필요한 목적 파일들을 OBJECTS 매크로로 지정한다.
OBJECTS = Str.o testmain.o

#사용되는 컴파일러를 g++로 지정한다.
CC = g++

#매크로를 이용해 실행 파일을 컴파일하는 규칙을 제시한다. test를 컴파일 하기 위해서는 OBJECTS에 명시된 목적 파일들이 필요하다.
$(TARGET): $(OBJECTS)
#목적 파일들을 이용해 g++가 test를 만들도록 하는 컴파일 명령이다.
	$(CC) $(OBJECTS) -o $(TARGET)

#목적 파일들은 컴파일 시 헤더 파일인 Str.h가 필요하다.
$(OBJECTS): Str.h

#확장자 규칙을 통해 .c 파일을 .o 파일로 자동으로 바꿔준다.
.SUFFIXES: .c .o

#clean이라는 이름의 프로그램이 있을 경우를 대비해 make clean이 clean 레이블의 코드를 실행하도록 설정한다.
.PHONY: clean

#다시 컴파일하기 위해 기존의 목적 파일과 실행 파일을 삭제한다.
clean:
	rm -f $(TARGET) $(OBJECTS)
