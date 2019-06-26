#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>

using namespace std;

double f(double x, double y, double a, double b){
    return (a*x - b*y);
}

double runge(double t, double y0, double step, double a, double b){
    double k1, k2, k3, k4;
    k1 = f(t, y0, a, b);
    k2 = f(t+step/2, y0+step*k1/2, a, b);
    k3 = f(t+step/2, y0+step*k2/2, a, b);
    k4 = f(t+step, y0+step*k3, a, b);
    double y = y0 + step*(k1+2*k2+2*k3+k4)/6;
    return y;
}

int main()
{
    double a = 0;
    double b = 0;
    double d = 0;
    double step = 0.0001;

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
        double initValue = d;
        double c = d + a/(b*b);
        double max_diff = 0;
        std::vector<double> time, y;
        time.push_back(0);
        y.push_back(initValue);
        for(double t = step; t < M_PI; t += step) {
            double dy = runge(time.back(), y.back(), step, a, b);
            time.push_back(t);
            y.push_back(dy);
        }
        for (unsigned int i = 0; i < time.size(); i++)
        {
            double func = (a/b)*(time[i] - 1/b) + c*exp(-b*time[i]);
            //cout << "u(t) = " << func << "\ty(t) = " << y[i] << endl;
            double diff = abs(y[i] - func);
            if(diff > max_diff) {
                max_diff = diff;
            }
            //cout << "cos(t) = " << cos(time[i]) << "\ty(t) = " << y[i] << endl;
        }
        cout << "< Max difference: " << max_diff << " >" << endl;
    }

    return 0;
}