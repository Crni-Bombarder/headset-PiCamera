#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;
 
int main(){
 
  // Create a VideoCapture object and open the input file
  // If the input is the web camera, pass 0 instead of the video file name
  VideoCapture cap(0); 
    
  // Check if camera opened successfully
  if(!cap.isOpened()){
    cout << "Error opening video stream or file" << endl;
    return -1;
  }
 
  int r=0;
  int g=0;
  int b=0;
  int contador=0;
  while(1){
 
    Mat frame;
    // Capture frame-by-frame
    cap >> frame;
  
    // If the frame is empty, break immediately
    if (frame.empty())
      break;
    
    // Display the resulting frame
    imshow( "Frame", frame );
 
    // ********************
    // Start color detection
    
    Mat3b hsv;
    cvtColor(frame, hsv, COLOR_BGR2HSV);
    Mat1b mask1,mask2,mask3,mask4;
    
    Scalar mean1,dev1;

    
    // Red
    inRange(hsv, Scalar(0, 70, 50), Scalar(10, 255, 255), mask1);
    meanStdDev(mask1,mean1,dev1);
    double M1 = mean1.val[0];
    double D1 = dev1.val[0];
    if (M1>15)
        r=1;
        
    // Green
     inRange(hsv, Scalar(40,70, 50), Scalar(80, 255, 255), mask2);
    meanStdDev(mask2,mean1,dev1);
    M1 = mean1.val[0];
    D1 = dev1.val[0];
    if (M1>15)
        g=1;
     
    
    // Blue
    inRange(hsv, Scalar(85, 50, 40), Scalar(135, 255, 255), mask3);
    meanStdDev(mask3,mean1,dev1);
    M1 = mean1.val[0];
    D1 = dev1.val[0];
    if (M1>15)
       b=1;
     
    //Mat1b mask = mask1; 
    if (r==1)
    {
       imshow("Mask", mask1);
       cout << "Rojo" << endl;
    }
       else if (g==1)
    {
       imshow("Mask", mask2);
       cout << "Verde" << endl;
    }
       else if (b==1)
    {
       imshow("Mask", mask3);
       cout << "Azul" << endl;
    }
       
       else{
           inRange(hsv, Scalar(0,0, 40), Scalar(5, 5,5), mask4);
             imshow("Mask", mask4);
           cout << "Color no seleccionado" << endl;
       }
   // cout << contador << "  " << M1 <<  "   " << D1 << " " << r << "  " << g << " " << b << endl;
    // End Color detection
    // ********************
    r=0;
    g=0;
    b=0;
    
    
    
    // Press  ESC on keyboard to exit
    char c=(char)waitKey(25);
    if(c==27)
      break;
    
    contador++;
    frame.release();
  }
  
  // When everything done, release the video capture object
  cap.release();
 
  // Closes all the frames
  destroyAllWindows();
     
  return 0;
}


