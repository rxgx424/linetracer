CX = g++   //C++컴파일러 이름을 변수 CX에 저장

CXFLAGS = -g -Wall  //컴파일러 옵션(공백포함)을 CXFLAGS에 저장
CVFLAGS = `pkg-config opencv4 --cflags --libs`  //헤더파일경로, 라이브러리 파일을 자동으로 찾아주는 명령어
DXLFLAGS = -I/usr/local/include/dynamixel_sdk -ldxl_x64_cpp
TARGET = lanefollow
OBJS = main.o dxl.o //목적파일 목록을 변수에 저장

$(TARGET) : $(OBJS)  //실행파일 lanefollow를 만드는 규칙
	$(CX) -o $(TARGET) $(OBJS) $(CXFLAGS) $(DXLFLAGS) $(CVFLAGS)

main.o : main.cpp  //목적파일 main.o를 만드는 규칙
	$(CX) -c main.cpp $(CXFLAGS) $(DXLFLAGS) $(CVFLAGS)

dxl.o : dxl.hpp dxl.cpp  //목적파일 dxl.0를 만드는 규칙
	$(CX) -c dxl.cpp $(CXFLAGS) $(DXLFLAGS)

.PHONY : clean  //clean이 phony target임을 명시
clean :
	rm -rf $(TARGET) $(OBJS)
