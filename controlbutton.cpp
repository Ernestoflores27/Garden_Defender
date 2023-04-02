#include <iostream>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <unistd.h>
#include <fcgi_stdio.h>
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
            FCGI_printf( "Status: 200 OK\r\n" );
            FCGI_printf( "Content-Type: text/html\r\n\r\n" );
            FCGI_printf( "dowAutomaticn" );
        }
        else if(strcmp(buf, "Manual")==0)
        {
            FCGI_printf( "Status: 200 OK\r\n" );
            FCGI_printf( "Content-Type: text/html\r\n\r\n" );
            FCGI_printf( "Manual" );
        }
  }
  return 0;
}
