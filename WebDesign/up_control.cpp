#include <iostream>
#include <string>
#include <vector>
using namespace std;

int main()
{
    cout << "Content-type:text/html\n\n";
    string line;
    cin >> line;
    cout << line;
    if(line=="up"){
        cout<<"up";
    }else{
        cout<<"not up";
    }
}