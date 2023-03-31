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
    }else if(line=="down"){
        cout<<"down";
    }else if(line=="left"){
        cout<<"left";
    }else if(line=="right"){
        cout<<"right";
    }else if(line=="ok"){
        cout<<"ok";
    }
}