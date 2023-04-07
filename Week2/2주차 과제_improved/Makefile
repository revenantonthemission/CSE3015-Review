TARGET = exec #실행 파일의 이름을 TARGET 매크로로 지정
OBJECTS = starwars.o printer.o counter.o init.o #컴파일 과정에서 사용되는 목적 파일들을 OBJECTS 매크로로 지정
CC = gcc #컴파일러에 대한 매크로로 미리 정해져 있는 CC 매크로를 명시적으로 gcc로 재정의함.

$(TARGET): $(OBJECTS) #OBJECTS가 TARGET의 컴파일을 위해 반드시 필요하다는 것을 목표와 의존관계 설정을 통해 나타내고 있음.
	$(CC) -o $(TARGET) $(OBJECTS) -lm
#실제 컴파일을 위한 컴파일 명령. 매크로를 통해 명령을 구성했고, -o 옵션은 결과물이 TARGET이 되도록 하는 옵션이다.
#-lm 옵션은 math.h를 사용하기 위해 링킹 과정에서 math library라고 불리는 libm.a 파일을 가져오기 위해 사용되었다.
#-l 옵션은 뒤에 붙는 문자열을 라이브러리 이름으로 간주하고 lib문자열.a라는 이름을 가진 파일을 찾는다.
	
%.o: %.c #%는 target pattern을 위해 사용된 것으로, target pattern은 %앞에 붙은 문자열로 시작해 %뒤에 붙은 문자열로 끝나는 모든 파일 이름을 나타내는 표현이다. 따라서 %.o와 %.c는 각각 .o와 .c로 끝나는 모든 파일에 해당하며, 이 줄은 .o로 끝나는 파일을 컴파일하기 위해서는 .c로 끝나는 파일이 필요하다는 것을 명시하고 있다.
	$(CC) -c -o $@ -g $<
#CC는 컴파일러에 대한 매크로고 -c는 목적 파일까지만 만들겠다는 의미를 가진 옵션이며, -g는 gdb를 통한 디버깅을 가능하게 하는 옵션이다.
#$@과 $<는 고정적으로 사용되는 내부 매크로로 각각 현재의 목표 파일과 현재의 목표 파일 보다 더 최근에 갱신된 파일의 이름을 나타낸다.
#즉, 여기서 $@은 %.o. $<은 %.c에 해당한다.
#따라서 이 줄은 starwars.c, printer.c, counter.c, init.c를 각각 starwars.o, printer.o, counter.o, init.o로 컴파일하는 명령이다.
	
starwars.o printer.o counter.o init.o: header.h #각 목적 파일이 컴파일되기 위해서 header.h가 필요하다는 것을 명시함.

.PHONY: clean #.PHONY는 파일 이름과 레이블을 헷갈리지 않게 하기 위한 방법이다. 이 레이블 안에 있는 이름은 우선 레이블로 사용된다.
clean: #clean 레이블은 일반적으로 다시 컴파일을 하기 위해 지워야 하는 파일들을 지우는 명령을 담고 있다.
	rm $(TARGET) $(OBJECTS)
#소스 파일부터 다시 컴파일을 하기 위해 기존 컴파일의 결과물들을 지우는 명령이다.
