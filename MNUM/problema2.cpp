#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <iomanip>

using std::cout; using std::cin;

//###################

//F1

   //float f1(float x){
   //return (x-2*log(x)-5);
   //}

//F2

    //float f1(float x){
     //return (pow(2,sqrt(x)) - 10*x +1);
     //}

//F3

float f1(float x){
  return ((1/tan(x))*sin(3*x)-x+1);
}

//###################

float xn(float a,float b){
  return ((a*f1(b)-b*f1(a))/(f1(b)-f1(a)));
}

bool condition(float x1, float x2, float e){
  float result = fabs(x1-x2);
  return (result <= e);
}

int main(){
  float e = 0.00001f;
  unsigned int n = 0;
  float a = 0.8;
  float b = 1.2;
  float XN, fa, fb, fxn, fa_fxn;
  bool cond = false;
  cout << std::setw(3) << "n" << " | " << std::setw(12) << "a" << " | " << std::setw(12) << "b" << " | ";
  cout << std::setw(12) << "f(a)" << " | " << std::setw(12) << "f(b)" << " | " << std::setw(12) << "Xn" << " | " << std::setw(12) << "f(a)*f(Xn)" << " | ";
  cout << std::setw(8) << "|x1-x2| <= E" << "\n\n";

  while(1){
    cout.precision(5);
    XN = xn(a,b);
    fa = f1(a);
    fb = f1(b);
    fxn = f1(XN);
    fa_fxn = fa*fxn;
    cout << std::setw(3) << n << " | " << std::setw(12) << a << " | " << std::setw(12) << b << " | ";
    cout << std::setw(12) << fa << " | " << std::setw(12) << fb << " | " << std::setw(12) << XN << " | " << std::setw(12) << fa_fxn << " | ";
    if(cond==true) cout << std::setw(8)<< "STOP";
    cout << std::endl;
    if(fa_fxn < 0){
      a=a;
      b=XN;
    }
    else {
      a=XN;
      b=b;
    }
    if(cond == true){
      break;
    }
    cond = condition(XN, xn(a,b), e);
    n++;
  }

  return 0;
}
