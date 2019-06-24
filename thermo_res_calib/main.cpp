#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>

#include "../Eigen/Dense"

using namespace std;

int main(){

    cout << "< Program started >" << endl;
    int values;
    ifstream inFile;
    inFile.open("data.txt");
    bool flag_file = inFile.is_open();
    if (flag_file){
        cout << "< File opened > " << endl;
    } else{
        cerr << "Warning! File didn't opened!" << endl;
        return 1;
    }
    inFile >> values;
    cout << "< Number of values: " << values << " >" << endl;

    Eigen::MatrixXf T(values,2);
    Eigen::VectorXf Z(values);
    Eigen::VectorXf S(2);
    cout << endl << "< T values: >" << endl;
    for (int i = 0; i < 12; i++){
        int temp;
        inFile >> temp;
        cout << "\t" << temp << endl;
        T(i,0) = temp;
        T(i,1) = 1;
    }
    cout << endl << "< Z values: >" << endl;
    for (int i = 0; i < 12; i++){
        int temp;
        inFile >> temp;
        cout << "\t" << temp << endl;
        Z(i) = log(temp);
    }

    S = T.colPivHouseholderQr().solve(Z); // The solution
    cout << endl << "< The solution using the QR decomposition is: >\n"
         << S << endl;

    vector<float> diff;
    float mid_diff = 0;
    for(int i=0; i<12; i++) {
        float value = S(0, 0)*T(i, 0) + S(1, 0);
        float temp = abs(Z(i) - value);
        diff.push_back(temp);
        mid_diff += temp;
    }
    mid_diff /= 12;
    cout << "< Mid difference: " << mid_diff << " >" << endl << endl;

    for(int i=0; i<12; i++) {
        if(diff[i] > mid_diff) {
            cout << "< Value number |" << i << "| is a miss >" << endl;
            cout << "< Point: " << T(i, 0) << " / " << Z(i) << " >" << endl;
        }
    }
}