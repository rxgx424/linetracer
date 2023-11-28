#include <iostream>
#include <opencv2/opencv.hpp>
#include <signal.h>
using namespace std;
using namespace cv;
// 1_lt_ccw_50rpm_out
// 2_lt_ccw_50rpm_in
// 3_lt_cw_50rpm_out
// 4_lt_cw_50rpm_in
// 5_lt_cw_100rpm_out
// 6_lt_ccw_100rpm_out
// 7_lt_ccw_100rpm_in
// 8_lt_cw_100rpm_in
bool ctrl_c_pressed = false;
void ctrlc_handler(int)
{
	ctrl_c_pressed = true;
}
//라인트레이서
/*int main(void)
{
	VideoCapture cap("2_lt_ccw_50rpm_in.mp4");
	if (!cap.isOpened()) { cerr << "failed!" << endl; return -1; }
	Mat src, dst, src2;
	Mat labels, stats, centroids;
	int th, cnt;
	int err=0, rvel=0, lvel=0;
	Point pt(320,45);
	Mat min{500,500,500,500,500}; //임의의 숫자, err로 나오지않을 큰 수
	double minVal;
	Point minLoc;
	int wy=0;
	signal(SIGINT, ctrlc_handler);
	while (true) {
		cap >> src;
		if (src.empty()) break;
		imshow("src", src);
		cvtColor(src, src, COLOR_BGR2GRAY);
		src = src(Rect(0,270,640,90));
		src2 = src.clone();
		src2 = src + (100 - mean(src)[0]);
		threshold(src2, dst, 135, 255, THRESH_BINARY);
		cnt = connectedComponentsWithStats(dst, labels, stats, centroids);
		cvtColor(dst, dst, COLOR_GRAY2BGR);
		if (ctrl_c_pressed) break;
		for (int i = 1;i < cnt;i++) {
			int* p = stats.ptr<int>(i);
			err = dst.cols / 2 - pt.x;
			cout << "err" << err << endl;
			min.at<int>(i,0) = abs(err);
			/*if (p[4] > 2000) {
				minMaxLoc(min, &minVal, 0, &minLoc);
				wy = minLoc.y;
				cout << "wy" << wy << endl;
				if (centroids.at<double>(wy, 0) <15 || centroids.at<double>(wy, 1) <15) {
					circle(dst, Point(pt), 1, Scalar(0, 0, 255), 2);
				}
				else {
					//rectangle(dst, Rect(p[0], p[1], p[2], p[3]), Scalar(0, 0, 255), 2);
					cout << "ddddd" << endl;
					pt = Point(centroids.at<double>(wy, 0), centroids.at<double>(wy, 1));
					cout << "pt: " << pt<< endl;
				}
			}
			if (pt.x - centroids.at<double>(i, 0) < dst.cols / 4 && pt.x - centroids.at<double>(i, 0) > -(dst.cols / 4)) {  //-160~160
				rectangle(dst, Rect(p[0], p[1], p[2], p[3]), Scalar(255, 0, 0), 2);
				//pt = Point(centroids.at<double>(i, 0), centroids.at<double>(i, 1));
				cout << "i: "<<i << endl;
			}
			else rectangle(dst, Rect(p[0], p[1], p[2], p[3]), Scalar(0, 0, 255), 2);
			//rectangle(dst, Rect(stats.ptr<int>(minLoc.y+1)[0], stats.ptr<int>(minLoc.y + 1)[1], stats.ptr<int>(minLoc.y + 1)[2], stats.ptr<int>(minLoc.y + 1)[3]), Scalar(255, 0, 0), 2);
		}
		rvel = 100 - 1 * err;
		lvel = -(100 + 1 * err);
		minMaxLoc(min, &minVal, 0, &minLoc);
		int wy = minLoc.y;
		cout << "wy"<<wy << endl;
		pt = Point(centroids.at<double>(wy, 0), centroids.at<double>(wy, 1));
		if (centroids.at<double>(wy, 0) < 15 || centroids.at<double>(wy, 1) < 15) {
			circle(dst, Point(pt), 1, Scalar(0, 0, 255), 2);
		}
		else {
			rectangle(dst, Rect(stats.ptr<int>(wy)[0], stats.ptr<int>(wy)[1], stats.ptr<int>(wy)[2], stats.ptr<int>(wy)[3]), Scalar(0, 0, 255), 2);
			cout << "ddddd" << endl;
			cout << "pt: " << pt << endl;
		}
		circle(dst, Point(pt), 1, Scalar(255, 0, 0), 2);
		imshow("dst", dst);
		imshow("src2", src2);
		waitKey(33);
	}
	return 0;
}
*/
// lanefollow_100rpm_ccw
// lanefollow_100rpm_cw
//레인팔로우(X)
/*int main(void)
{
	VideoCapture cap("lanefollow_100rpm_ccw.mp4");
	if (!cap.isOpened()) { cerr << "failed!" << endl; return -1; }
	Mat src, dst, src2;
	Mat labels, stats, centroids;
	int th, cnt;
	Point pt_r,pt_l,pt(320, 45);  //왼쪽 레인 중심좌표(pt_l), 오른쪽 레인 중심좌표(pt_r)
	while (true) {
		cap >> src;
		if (src.empty()) break;
		imshow("src", src);
		cvtColor(src, src, COLOR_BGR2GRAY);
		src = src(Rect(0, 270, 640, 90));
		src2 = src.clone();
		src2 = src + (80 - mean(src)[0]);
		threshold(src2, dst, 135, 255, THRESH_BINARY);
		cnt = connectedComponentsWithStats(dst, labels, stats, centroids);
		cvtColor(dst, dst, COLOR_GRAY2BGR);

		for (int i = 1;i < cnt;i++) {
			int* p = stats.ptr<int>(i);
			if (pt.x - centroids.at<double>(i, 0) < dst.cols / 4 && pt.x - centroids.at<double>(i, 0) > -(dst.cols / 4)) {  //-160~160
				rectangle(dst, Rect(p[0], p[1], p[2], p[3]), Scalar(255, 0, 0), 2);
				pt = Point(centroids.at<double>(i, 0), centroids.at<double>(i, 1));
			}
			
			else rectangle(dst, Rect(p[0], p[1], p[2], p[3]), Scalar(0, 0, 255), 2);
			circle(dst, Point(pt), 1, Scalar(255, 0, 0), 2);
		}
		imshow("dst", dst);
		imshow("src2", src2);
		waitKey(33);
	}
	return 0;
}*/

/*#include <iostream>
#include "opencv2/opencv.hpp"
#include <unistd.h>
#include <sys/time.h>
#include <signal.h>
#include "dxl.hpp"

using namespace std;
using namespace cv;
bool ctrl_c_pressed = false;
void ctrlc_handler(int){ ctrl_c_pressed = true; }
// 1_lt_ccw_50rpm_out
// 2_lt_ccw_50rpm_in
// 3_lt_cw_50rpm_out
// 4_lt_cw_50rpm_in
// 5_lt_cw_100rpm_out
// 6_lt_ccw_100rpm_out
// 7_lt_ccw_100rpm_in
// 8_lt_cw_100rpm_in
int main(void)
{
	VideoCapture cap("2_lt_ccw_50rpm_in.mp4");
	if (!cap.isOpened()) { cerr << "failed!" << endl; return -1; }

	string wr1 = "appsrc ! videoconvert ! video/x-raw, format=BGRx ! nvvidconv ! nvv4l2h264enc insert-sps-pps=true ! h264parse ! rtph264pay pt=96 ! udpsink host=192.168.0.72 port=8451 sync=false";
    VideoWriter writer1(wr1, 0, (double)30, Size(640, 360), true);
    if (!writer1.isOpened()) { cerr << "Writer open failed!" << endl; return -1;}

	string wr2 = "appsrc ! videoconvert ! video/x-raw, format=BGRx ! nvvidconv ! nvv4l2h264enc insert-sps-pps=true ! h264parse ! rtph264pay pt=96 ! udpsink host=192.168.0.72 port=8452 sync=false";
    VideoWriter writer2(wr2, 0, (double)30, Size(640, 90), false);
    if (!writer1.isOpened()) { cerr << "Writer open failed!" << endl; return -1;}

	string wr3 = "appsrc ! videoconvert ! video/x-raw, format=BGRx ! nvvidconv ! nvv4l2h264enc insert-sps-pps=true ! h264parse ! rtph264pay pt=96 ! udpsink host=192.168.0.72 port=8453 sync=false";
    VideoWriter writer3(wr3, 0, (double)30, Size(640, 90), true);
    if (!writer1.isOpened()) { cerr << "Writer open failed!" << endl; return -1;}

	Mat src, dst, src2;  //영상
	Mat labels, stats, centroids; 
	int cnt; 
	Dxl mx;  //다이나믹셀
    struct timeval start,end1;
    double time1;
	int rvel = 0, lvel = 0, error;
	Point pt(320,45);
	int c[]={0}; double minVal; Point minLoc;
	signal(SIGINT, ctrlc_handler);
    if(!mx.open()) { cout << "dynamixel open error"<<endl; return -1; }
	while (true) {
		cap >> src;
		if (src.empty()) break;
		writer1<<src;
		cvtColor(src, src, COLOR_BGR2GRAY);
		src = src(Rect(0,270,640,90));
		src2 = src.clone();
		src2 = src + (100 - mean(src)[0]);
		threshold(src2, dst, 135, 255, THRESH_BINARY);
		cnt = connectedComponentsWithStats(dst, labels, stats, centroids);
		cvtColor(dst, dst, COLOR_GRAY2BGR);
		for (int i = 1;i < cnt;i++) {
			int* p = stats.ptr<int>(i);
			error = dst.cols / 2 - pt.x;
			c[i-1] = error;
			minmaxLoc(c,&minVal,0,&minLoc);
			if (p[4] > 2000) {
				if (pt.x - centroids.at<double>(i, 0) < dst.cols / 4 && pt.x - centroids.at<double>(i, 0) > -(dst.cols / 4)) {  //-160~160
					rectangle(dst, Rect(p[0], p[1], p[2], p[3]), Scalar(255, 0, 0), 2);
					pt = Point(centroids.at<double>(i, 0), centroids.at<double>(i, 1));
				}
			}
			cout<<minLoc<<", "<<minVal<<endl;
			lvel = 50 - 0.5 * error;
			rvel = -(50 + 0.5 * error);
			circle(dst, Point(pt), 1, Scalar(255, 0, 0), 2);

		}
		mx.setVelocity(lvel,rvel);
        if (ctrl_c_pressed) break; //Ctrl+c입력시 탈출
        usleep(20*1000);
        gettimeofday(&end1,NULL);
        time1 =end1.tv_sec-start.tv_sec+(end1.tv_usec-start.tv_usec)/1000000.0;
        cout <<"err:"<<error<<", rvel:"<<rvel<<", lvel:"<<lvel<<", time:"<<time1<< endl;
		writer2<<src2;
		writer3<<dst;
		waitKey(33);
	}
	mx.close(); // 장치닫기
	return 0;
}*/

//라인트레이서
int main(void)
{
	VideoCapture cap("2_lt_ccw_50rpm_in.mp4");
	if (!cap.isOpened()) { cerr << "failed!" << endl; return -1; }
	Mat src, dst, src2;
	Mat labels, stats, centroids;
	int th, cnt;
	int err = 0, rvel = 0, lvel = 0;
	Point pt(320, 45);
	Mat min{ 500,500,500,500,500 }; //임의의 숫자, err로 나오지않을 큰 수
	double minVal;
	Point minLoc;
	int wy = 0;
	signal(SIGINT, ctrlc_handler);
	while (true) {
		cap >> src;
		if (src.empty()) break;
		imshow("src", src);
		cvtColor(src, src, COLOR_BGR2GRAY);
		src = src(Rect(0, 270, 640, 90));
		src2 = src.clone();
		src2 = src + (100 - mean(src)[0]);
		threshold(src2, dst, 135, 255, THRESH_BINARY);
		cnt = connectedComponentsWithStats(dst, labels, stats, centroids);
		cvtColor(dst, dst, COLOR_GRAY2BGR);
		if (ctrl_c_pressed) break;
		for (int i = 1;i < cnt;i++) {
			int* p = stats.ptr<int>(i);
			err = dst.cols / 2 - pt.x;
			cout << "err" << err << endl;
			min.at<int>(i, 0) = abs(err);
			if (pt.x - centroids.at<double>(i, 0) < dst.cols / 4 && pt.x - centroids.at<double>(i, 0) > -(dst.cols / 4)) {  //-160~160
				rectangle(dst, Rect(p[0], p[1], p[2], p[3]), Scalar(255, 0, 0), 2);
				cout << "i: " << i << endl;
			}
			else rectangle(dst, Rect(p[0], p[1], p[2], p[3]), Scalar(0, 0, 255), 2);
			//rectangle(dst, Rect(stats.ptr<int>(minLoc.y+1)[0], stats.ptr<int>(minLoc.y + 1)[1], stats.ptr<int>(minLoc.y + 1)[2], stats.ptr<int>(minLoc.y + 1)[3]), Scalar(255, 0, 0), 2);
		}
		rvel = 100 - 1 * err;
		lvel = -(100 + 1 * err);
		minMaxLoc(min, &minVal, 0, &minLoc);
		int wy = minLoc.y;
		cout << "wy" << wy << endl;
		pt = Point(centroids.at<double>(wy, 0), centroids.at<double>(wy, 1));
		if (centroids.at<double>(wy, 0) < 15 || centroids.at<double>(wy, 1) < 15) {
			circle(dst, Point(pt), 1, Scalar(0, 0, 255), 2);
		}
		else {
			rectangle(dst, Rect(stats.ptr<int>(wy)[0], stats.ptr<int>(wy)[1], stats.ptr<int>(wy)[2], stats.ptr<int>(wy)[3]), Scalar(0, 0, 255), 2);
			cout << "ddddd" << endl;
			cout << "pt: " << pt << endl;
		}
		circle(dst, Point(pt), 1, Scalar(255, 0, 0), 2);
		imshow("dst", dst);
		imshow("src2", src2);
		waitKey(33);
	}
	return 0;
}
