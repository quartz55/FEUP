#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <iomanip>

using std::cout; using std::cin;

//g1

 float g1(float x){
   return (2*log(x)+5);
 }

 float g2(float x){
   return (exp((x-5)/2));
 }


bool condition(float x1, float x2, float e){
    float result = fabs(x1-x2);
    return (result <= e);
}

int main(){
    float e = 0.00001f;
    unsigned int n = 0;
    bool cond = false;
    float xn = 0.1;
    float xn_1 = g2(xn);
    cout << std::setw(3) << "n" << " | " << std::setw(12) << "Xn" << " | ";
    cout << std::setw(12) << "Xn+1 = g1(Xn)" << " | ";;
    cout << std::setw(8) << "|x1-x2| <= E" << "\n\n";

    while(1){
        xn_1 = g1(xn);
        cout.precision(5);
        cout << std::setw(3) << n << " | ";
        cout << std::setw(12) << xn << " | " << std::setw(12) << xn_1 << " | ";
        if(cond==true) cout << std::setw(8)<< "STOP";
        cout << std::endl;

        if(cond == true){
            break;
        }
        cond = condition(xn_1, xn, e);
        xn = xn_1;
        n++;
    }

    return 0;
}
