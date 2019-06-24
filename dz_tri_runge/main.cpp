#include <iostream>
#include <fstream>
#define USE_MATH_DEFINES
#include <cmath>
#include <vector>

using namespace std;

float f(float x, float y, float a, float b){
    return (a*x - b*y);
}

float runge(float t, float y0, float step, float a, float b){
    float k1, k2, k3, k4;
    k1 = f(t, y0, a, b);
    k2 = f(t+step/2, y0+step*k1/2, a, b);
    k3 = f(t+step/2, y0+step*k2/2, a, b);
    k4 = f(t+step, y0+step*k3, a, b);
    float y = y0 + step*(k1+2*k2+2*k3+k4)/6;
    return y;
}

int main()
{
    float a = 0;
    float b = 0;
    float d = 0;
    float step = 0.01;

    cout << "< Program started >" << endl;
    ifstream inFile;
    inFile.open("data.txt");
    if (inFile.is_open()){
        cout << "< File opened > " << endl;
    } else{
        cerr << "< Warning! File didn't opened! >" << endl;
        return 1;
    }
    cout << "< Step time: " << step << " >" << endl << endl;
    cout << "< Reading init values >" << endl;

    for (int iteration = 0; iteration < 13; iteration++){
        inFile >> a;
        inFile >> b;
        inFile >> d;
        cout << "< a = " << a << "\tb = " << b << "\td = " << d << " >" << endl;

        // RUNGE HERE //
        float initValue = d;
        float c = d + a/(b*b);
        float max_diff = 0;
        std::vector<float> time, y;
        time.push_back(0);
        y.push_back(initValue);
        for(float t = step; t < 1; t+=step) {
            float dy = runge(time.back(), y.back(), step, a, b);
            time.push_back(t);
            y.push_back(dy);
        }
        for (int i=0; i<time.size(); i++)
        {
            float func = (a/b)*(time[i] - 1/b) + c*exp(-b*time[i]);
            //cout << "u(t) = " << func << "\ty(t) = " << y[i] << endl;
            float diff = abs(y[i] - func);
            if(diff > max_diff) {
                max_diff = diff;
            }
            //cout << "cos(t) = " << cos(time[i]) << "\ty(t) = " << y[i] << endl;
        }
        cout << "< Max difference: " << max_diff << " >" << endl;
    }

    return 0;
}