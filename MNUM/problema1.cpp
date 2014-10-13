#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <iomanip>

using std::cout; using std::cin;

float xn(float a,float b){
  return ((a+b)/2);
}

float f1(float x){
  return (x-2*log(x)-5);
}



/* float f1(float x){
 *   return (pow(2,sqrt(x)) - 10*x +1);
 * }
 */
float fa_X_fxn(float fa, float fxn){
  return (fa*fxn);
}

bool condition(float x1, float x2, float e){
  float result = fabs(x1-x2);
  //cout << e << "   " << result;
  return (result <= e);
}

int main(){
  float e = 0.00001f;
  unsigned int n = 0;
  float a = 0.01;
  float b = 1;
  float XN, fa, fxn, fa_fxn;
  bool cond = false;
  cout << std::setw(3) << "n" << " | " << std::setw(12) << "a" << " | " << std::setw(12) << "b" << " | "<< std::setw(12) << "Xn" << " | ";
  cout << std::setw(12) << "f(a)" << " | " << std::setw(12) << "f(Xn)" << " | " << std::setw(12) << "f(a)*f(Xn)" << " | ";
  cout << std::setw(8) << "|x1-x2| <= E" << "\n\n";

  while(1){
    cout.precision(5);
    XN = xn(a,b);
    fa = f1(a);
    fxn = f1(XN);
    fa_fxn = fa_X_fxn(fa,fxn);
    cout << std::setw(3) << n << " | " << std::setw(12) << a << " | " << std::setw(12) << b << " | "<< std::setw(12) << XN << " | ";
    cout << std::setw(12) << fa << " | " << std::setw(12) << fxn << " | " << std::setw(12) << fa_fxn << " | ";
    if(cond==true) cout << "  OMG YES";
    else cout << std::setw(8) << "NOPE";
    cout << std::endl;
    if(fa_fxn <= 0){
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
