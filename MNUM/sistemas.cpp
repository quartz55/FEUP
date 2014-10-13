#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <iomanip>

using std::cout; using std::cin;

//q1

float q1(float x, float y){
    return (sqrt((x*y+5*x-1)/2));
}

float q2(float x, float y){
    return (sqrt(x+3*log(x)));
}

//####################

bool condition(float x1, float x2, float e){
    float result = fabs(x1-x2);
    return (result <= e);
}

int main(){
    float e = 0.00001f;
    unsigned int n = 0;
    bool cond = false;
    bool cond2 = false;
    float xn = 4;
    float yn = 4;
    float xn_1 = q1(xn, yn);
    float yn_1 = q2(xn, yn);
    cout << std::setw(3) << "n" << " | ";
    cout << std::setw(12) << "Xn" << " | ";
    cout << std::setw(12) << "Yn" << " | ";
    cout << std::setw(15) << "Xn+1=Q1(Xn,Yn)" << " | ";;
    cout << std::setw(15) << "Yn+1=Q2(Xn,Yn)" << " | ";;
    cout << std::setw(12) << "|x1-x2| <= E" << " | ";
    cout << std::setw(12) << "|y1-y2| <= E" << "\n\n";

    while(1){
        cout.precision(6);
        cout << std::setw(3) << n << " | ";
        cout << std::setw(12) << xn << " | ";
        cout << std::setw(12) << yn << " | ";
        cout << std::setw(15) << xn_1 << " | ";
        cout << std::setw(15) << yn_1 << " | ";
        if(cond==true) cout << std::setw(12)<< "STOP    ";
        else cout << std::setw(12) << " ";
        cout << " | ";
        if(cond2==true) cout << std::setw(12)<< "STOP    ";
        else cout << std::setw(12) << " ";
        cout << " | ";
        cout << std::endl;

        if(cond == true && cond2 == true){
            break;
        }
        cond = condition(xn_1, xn, e);
        cond2 = condition(yn_1, yn, e);
        xn = xn_1;
        yn = yn_1;
        xn_1 = q1(xn, yn);
        yn_1 = q2(xn, yn);
        n++;
    }

    return 0;
}
