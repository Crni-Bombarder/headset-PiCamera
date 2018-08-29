#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <thread>
#include <mutex>
#include <string>

extern "C" {
    #include "Odroid.h"
    #include "ClientTCP.h"
    #include "Display.h"
}

using namespace std;
using namespace cv;

int running;
int newFrame;
mutex Mtx;

void sendCommand(string str)
{
    uint n;
    Packet size;
    Packet data;

    size.dataSize = sizeof(uint);
    size.data = (char*)&n;
    n = str.size();

    data.dataSize = n;
    data.data = (char*)str.c_str();

    sendDataTCP(&size);
    sendDataTCP(&data);
}

void recepSDPFile(string fileName)
{
    int n;
    Packet packet;
    Packet* tcpPacket;

    FILE* fd = fopen(fileName.c_str(), "w");
    packet.data = (char*)&n;
    packet.dataSize = sizeof(int);
    readFullDataTCP(&packet);
    tcpPacket = createPacket(n);
    readFullDataTCP(tcpPacket);
    fwrite(tcpPacket->data, tcpPacket->dataSize, 1, fd);
    fclose(fd);
    freePacket(tcpPacket);
}

void signalProcessingThread(Mat* mat)
{
    Mat3b hsv;
    Mat1b mask1,mask2,mask3,mask4;
    Scalar mean1,dev1;
    double M1, D1;
    int r, g, b;

    while(running)
    {
        if (!newFrame)
        {
            usleep(10);
            continue;
        }
        newFrame = 0;
        Mtx.lock();
        cvtColor(*mat, hsv, COLOR_RGB2HSV);
        Mtx.unlock();

        // Red
        inRange(hsv, Scalar(0, 70, 50), Scalar(10, 255, 255), mask1);
        meanStdDev(mask1,mean1,dev1);
        M1 = mean1.val[0];
        D1 = dev1.val[0];
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
            // imshow("Mask", mask1);
            cout << "Rojo" << endl;
        }
        else if (g==1)
        {
            // imshow("Mask", mask2);
            cout << "Verde" << endl;
        }
        else if (b==1)
        {
            // imshow("Mask", mask3);
            cout << "Azul" << endl;
        }

        else{
            inRange(hsv, Scalar(0,0, 40), Scalar(5, 5,5), mask4);
            cout << "Color no seleccionado" << endl;
        }
        // End Color detection
        // ********************
        r=0;
        g=0;
        b=0;
    }
}

int main(int argc, char* argv[]){

    const string sdpFile("pres.sdp");

    if (argc < 3)
    {
        cout << "Missing argument" << endl;
        exit(1);
    }

    connectClientTCP(argv[1], atoi(argv[2]));

    sendCommand("format h264\n");
    sendCommand("start\n");
    recepSDPFile(sdpFile);
    Frame* cframe = initVideoReception((char*)sdpFile.c_str());
    createWindow(cframe->width, cframe->height, cframe->width, cframe->height);
    Mat frame(Size(cframe->width, cframe->height), CV_8UC3, cframe->data);
    running = 1;
    thread thrd(signalProcessingThread, &frame);

    while(1){
        getNewFrame(NULL);
        Mtx.lock();
        convertFrame(NULL);
        Mtx.unlock();
        displayFrame(cframe);
        newFrame = 1;
    }

    destroyWindow();
    endVideoReception();
    disconnectClientTCP();

    return 0;
}
