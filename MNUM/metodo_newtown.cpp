
#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <iomanip>

using std::cout; using std::cin;

//F1

/* float f1(float x){
 *   return (x-2*log(x)-5);
 * }
 * 
 * float f1_diff(float x){
 *   return (1-2/x);
 * }
 */

//F2

    float f1(float x){
    return (pow(2,sqrt(x)) - 10*x +1);
    }

float f1_diff(float x){
  return ((log(2)+pow(2,sqrt(x)-1))/sqrt(x))-10;
}


//F3

/* float f1(float x){
 *   return ((1/tan(x))*sin(3*x)-x+1);
 * }
 */


float xn_plus1(float xn){
  return (xn-(f1(xn)/f1_diff(xn)));
}

bool condition(float x1, float x2, float e){
  float result = fabs(x1-x2);
  return (result <= e);
}

int main(){
  float e = 0.00001f;
  unsigned int n = 0;
  float guess = 2;
  bool cond = false;
  float xn_1;
  cout << std::setw(3) << "n" << " | " << std::setw(12) << "Xn" << " | " << std::setw(12) << "f(Xn)" << " | ";
  cout << std::setw(12) << "f'(Xn)" << " | " << std::setw(12) << "Xn+1" << " | ";;
  cout << std::setw(8) << "|x1-x2| <= E" << "\n\n";

  while(1){
    xn_1 = xn_plus1(guess);
    cout.precision(5);
    cout << std::setw(3) << n << " | " << std::setw(12) << guess << " | " << std::setw(12) << f1(guess) << " | ";
    cout << std::setw(12) << f1_diff(guess) << " | " << std::setw(12) << xn_1 << " | ";
    if(cond==true) cout << std::setw(8)<< "STOP";
    cout << std::endl;

    if(cond == true){
      break;
    }
    cond = condition(xn_1, guess, e);
    guess = xn_1;
    n++;
  }

  return 0;
}
