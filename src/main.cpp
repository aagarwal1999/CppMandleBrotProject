#include <iostream>
#include <memory>
#include <thread>
#include <chrono>
#include "opencv2/highgui/highgui.hpp"
#include "Mandlebrot.h"
#include "Window.h"


std::string WINDOW_NAME = "Mandlebrot";

void CallBackFunc(int event, int xpos, int ypos, int flags, void* userdata)
{
  	 // if unrelated mouse event --> return
  	 if(event != cv::EVENT_LBUTTONDOWN && event != cv::EVENT_RBUTTONDOWN){
       return;
     }
  	 
     // MandleBrot object will always be passed as reference
     Mandlebrot* pThis = (Mandlebrot*) userdata;
  	 
     if(pThis->mtxData.try_lock_for(std::chrono::milliseconds(1))) {
       std::lock_guard<std::timed_mutex>(pThis->mtxData, std::adopt_lock); 
       // ensures that concurrent mouse events do not interfere with each other. Timeout of 1 ms
       if  ( event == cv::EVENT_LBUTTONDOWN )
       {	
            pThis->ZoomIn(xpos, ypos);

       }
       else if  ( event == cv::EVENT_RBUTTONDOWN )
       {
            pThis->ZoomOut();
       }
       std::shared_ptr<void> data =  pThis->getImgData();
	   cv::Mat img(pThis->ImgHeight(), pThis->ImgWidth(), CV_8UC3, data.get());
       cv::imshow(WINDOW_NAME, img);
     }
}


int main()
{
     Window<int> imgBounds(0, 800, 0, 800);
     Window<double> startingBounds(-2.2, 1.2, -1.7, 1.7);
     // initialize starting frame
     Mandlebrot mandleBrot(imgBounds, startingBounds);
   	std::shared_ptr<void> data =  mandleBrot.getImgData();
	cv::Mat img(imgBounds.Height(), imgBounds.Width(), CV_8UC3, data.get());
     cv::namedWindow(WINDOW_NAME, 1);
     //set the callback function for any mouse event
     cv::setMouseCallback(WINDOW_NAME, CallBackFunc, &mandleBrot);
     //show the image
     cv::imshow(WINDOW_NAME, img);
     // Wait until user press some key
     cv::waitKey(0);

     return 0;

}