1.	Install apache server
https://www.apachehaus.com/cgi-bin/download.plx
2.	update file: conf/httpd.conf

（1）rootpath: 
Define SRVROOT "D:\soft\Apache24"
ServerRoot "${SRVROOT}"
 (2)static file path(eg photos):
  ScriptAlias /cgi-bin/ "${SRVROOT}/cgi-bin/"
  Alias /files/ "${SRVROOT}/cgi-bin/Images/"
(in <IfModule>)
 (3)then in html, the picture path is '/files/logo.png'

3.	test config
cmd in admin mode:
 httpd -t
if it shows syntax ok, then config successfully!

4.	start and stop serve
httpd -k start(will show noting)
httpd -k stop(will show is stoping and stoped)

5.	after starting 
go to http://localhost
show initial html of apache then it means apache server has been installed successfully! 
 

6.	Place the compiled c++ file(.exe) in the folder shown below：
 D:/soft/Apache24/cgi-bin

7.	Visit
At this point, visit your browser at http://localhost/cgi-bin/control.exe and you will see the html, C++ file like blow：
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

int main ()
{

   cout << "Content-type:text/html\r\n\r\n";
   fstream f("video.html");
   string line;
   while(getline(f,line)){
    cout << line << endl;
   }
   f.close();

   return 0;
}

