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