#include <opencv4/opencv2/opencv.hpp>
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <unistd.h>
#include <fcgi_stdio.h>
#include <fstream>
#include <string>

using namespace std;
#include "Garden_Defender/Objects/Camera.h"
#include "Garden_Defender/Objects/Detector.h"


int main( int argc, char *argv[] )
{
  while( FCGI_Accept() >= 0 )
  {
         
        char *contentLengthStr = getenv("CONTENT_LENGTH");
        int contentLength = contentLengthStr ? std::stoi(contentLengthStr) : 0;

        
        char *buf = new char[contentLength + 1];
        memset(buf, 0, contentLength + 1);
        if (contentLength > 0) {
            int len = 0;
            while (len < contentLength && !feof(stdin)) {
                int n = fread(buf + len, 1, contentLength - len, stdin);
                if (n <= 0) {
                    break;
                }
                len += n;
            }
        }

        if(strcmp(buf, "up")==0)
        {
            FCGI_printf( "Status: 200 OK\r\n" );
            FCGI_printf( "Content-Type: text/html\r\n\r\n" );
            FCGI_printf( "up" );
        }
        else if(strcmp(buf, "down")==0)
        {
            FCGI_printf( "Status: 200 OK\r\n" );
            FCGI_printf( "Content-Type: text/html\r\n\r\n" );
            FCGI_printf( "down" );
        }
        else if(strcmp(buf, "right")==0)
        {
            FCGI_printf( "Status: 200 OK\r\n" );
            FCGI_printf( "Content-Type: text/html\r\n\r\n" );
            FCGI_printf( "right" );
        }
        else if(strcmp(buf, "left")==0)
        {
            FCGI_printf( "Status: 200 OK\r\n" );
            FCGI_printf( "Content-Type: text/html\r\n\r\n" );
            FCGI_printf( "left" );
        }
        else if(strcmp(buf, "ok")==0)
        {
            FCGI_printf( "Status: 200 OK\r\n" );
            FCGI_printf( "Content-Type: text/html\r\n\r\n" );
            FCGI_printf( "ok" );
        }
        else if(strcmp(buf, "Automatic")==0)
        {
            ofstream outfile("record.txt");
            outfile << "1";
            outfile.close();
            //automatic
            //MoveServo->autometic();
            cv::Mat video_stream;
            Camera cam1(640, 480);
            cv::VideoCapture real_time = cam1.getVideoCapture();

            Turret turret(23);
            turret.changePosition(0, 0);
            turret.moveT();

            Detector detector_model("Garden_Defender/yoloFastestV2.onnx", 0.3, 0.4, 0.4, real_time, &turret);
            detector_model.detectT();

            do
            {
                std::cout << '\n'
                          << "Press a Enter to close...";

            } while (std::cin.get() != '\n');

            real_time.release();
            turret.release();
            FCGI_printf( "Status: 200 OK\r\n" );
            FCGI_printf( "Content-Type: text/html\r\n\r\n" );
            FCGI_printf( "Automatic" );
        }
        else if(strcmp(buf, "Manual")==0)
        {
            ofstream outfile("record.txt");
            outfile << "0";
            outfile.close();
            FCGI_printf( "Status: 200 OK\r\n" );
            FCGI_printf( "Content-Type: text/html\r\n\r\n" );
            FCGI_printf( "Manual" );
        }
  }
  return 0;
}
