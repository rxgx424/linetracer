#include <iostream>
#include "opencv2/opencv.hpp"
#include <unistd.h>
#include <sys/time.h>
#include <signal.h>
#include "dxl.hpp"

using namespace std;
using namespace cv;
bool mode = false;   //다이나믹셀 모드(false=동작X)
bool ctrl_c_pressed = false; //ctrl+c 여부
void ctrlc_handler(int) { ctrl_c_pressed = true; } //ctrl+c가 눌리면 true로 변경

int main(void)
{
	VideoCapture source("6_lt_ccw_100rpm_out.mp4"); //출력영상
	if (!source.isOpened()) { cerr << "failed!" << endl; return -1; } //예외처리

	Mat src, dst, src2; //영상
	Mat labels, stats, centroids; //레이블링 변수
	int cnt;
	Dxl mx;  //다이나믹셀
	int rvel = 0, lvel = 0, error;
	struct timeval start, end1; //시간 변수
	double time1;
	Point min(320, 45); //최소값 변수
	signal(SIGINT, ctrlc_handler); //시그널 핸들러 지정
	if (!mx.open()) { cout << "dynamixel open error" << endl; return -1; } //예외처리
	while (true) {
		source >> src; //영상 불러오기
		if (src.empty()) { cerr << "frame empty!" << endl; break; }  //예외처리
		writer1 << src; //영상출력
		gettimeofday(&start, NULL); //시작시간
		cvtColor(src, src, COLOR_BGR2GRAY); //컬러->그레이
		src = src(Rect(0, 270, 640, 90)); //ROI
		src2 = src.clone(); //ROI영상 src2에 복사
		src2 = src + (80 - mean(src)[0]); //밝기 조정
		GaussianBlur(src2, dst, Size(5,5), 5); //노이즈 제거
		threshold(src2, dst, 135, 255, THRESH_BINARY); //이진화
		cnt = connectedComponentsWithStats(dst, labels, stats, centroids); //바운딩박스
		cvtColor(dst, dst, COLOR_GRAY2BGR); //그레이->컬러
		int c[2];  //c[0]=현재값, c[1]=최소값
		int n = 0; //최소값의 cnt값
		Point pt = min; //라인의 중심값
if (mx.kbhit()) //키보드 입력 체크
		{
			char ch = mx.getch(); //키입력 받기
			if (ch == 'q') break; //q이면 종료
			else if (ch == 's') mode = true; //s면 다이나믹셀 모드 true
		}
		for (int i = 1;i < cnt;i++) {
			int* p = stats.ptr<int>(i);
			if (i == 1) c[1] = abs(sqrt(pow(pt.x - centroids.at<double>(i, 0), 2) + pow(pt.y - centroids.at<double>(i, 1), 2)));
			c[0] = abs(sqrt(pow(pt.x - centroids.at<double>(i, 0), 2) + pow(pt.y - centroids.at<double>(i, 1), 2)));
			if ((c[0] < dst.cols / 4 && c[0] <= c[1]) { //최소값구하기
				c[1] = c[0];   //최소값 저장 (int)
				min = Point(centroids.at<double>(i, 0), centroids.at<double>(i, 1));      //최소값의 무게중심 저장 (Point)
				n = i; //최소값의 cnt값 저장
			}
			rectangle(dst, Rect(p[0], p[1], p[2], p[3]), Scalar(0, 0, 255), 2);
		} //검출된 모든 객체 빨간색 박스
		if (n > 0) rectangle(dst, Rect(stats.ptr<int>(n)[0], stats.ptr<int>(n)[1], stats.ptr<int>(n)[2], stats.ptr<int>(n)[3]), Scalar(255), 3); //라인이 검출되면 라인에 파란색 박스
		circle(dst, Point(min), 1, Scalar(255, 0, 0), 2);
		if (ctrl_c_pressed) break; //Ctrl+c입력시 탈출
		error = (dst.cols / 2) - min.x; //에러값(min.x=라인의 x값)
		lvel = 100 - 0.15 * error;
		rvel = -(100 + 0.15 * error);
		if (mode) mx.setVelocity(lvel, rvel); //다이나믹 셀 모드가 true면 작동
		gettimeofday(&end1, NULL); //끝 시간
		usleep(20 * 1000);
		time1 = end1.tv_sec - start.tv_sec + (end1.tv_usec - start.tv_usec) / 1000000.0;
		cout << "err:" << error << ", lvel:" << lvel << ", rvel:" << rvel << ", time:" << time1 << endl;
		writer2 << src2; //ROI 영상출력
		writer3 << dst;  //라인검출 영상출력
		waitKey(33);
	}
	mx.close(); // 장치닫기
	return 0;
}

