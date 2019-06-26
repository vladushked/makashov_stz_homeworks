#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>

#include "../../Eigen/Dense"

using namespace std;

int main(){

    cout << "< Program started >" << endl;
    int values;
    ifstream inFile;
    inFile.open("data.txt");
    if (inFile.is_open()){
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
    for (int i = 0; i < values; i++){
        int temp;
        inFile >> temp;
        cout << "\t" << temp << endl;
        T(i,0) = temp;
        T(i,1) = 1;
    }
    cout << endl << "< Z values: >" << endl;
    for (int i = 0; i < values; i++){
        int temp;
        inFile >> temp;
        cout << "\t" << temp << endl;
        Z(i) = log(temp);
    }
    inFile.close();

    S = T.colPivHouseholderQr().solve(Z); // The solution
    cout << endl << "< The solution using the QR decomposition is: >\n"
         << S << endl;

    vector<float> diff;
    float mid_diff = 0;
    for(int i=0; i<values; i++) {
        float value = S(0, 0)*T(i, 0) + S(1, 0);
        float temp = abs(Z(i) - value);
        diff.push_back(temp);
        mid_diff += temp;
    }
    mid_diff /= values;
    cout << "< Mid difference: " << mid_diff << " >" << endl << endl;

    float max_diff = 0;
    int max_diff_value = 0;
    for(int i=0; i<12; i++) {
        if(diff[i] > mid_diff) {
            if (diff[i] > max_diff) {
                max_diff = diff[i];
                max_diff_value = i;
            }
            cout << "< Value number |" << i << "| is a miss >" << endl;
            cout << "< Point: " << T(i, 0) << " / " << Z(i) << " >" << endl;
        }
    }
    cout << "< Value |" << max_diff_value << "| is a maximum miss: " << max_diff << " >" << endl;

    //Remove point with max miss

    /*
    Eigen::MatrixXf T_new(values - 1,2);
    Eigen::VectorXf Z_new(values - 1);
    Eigen::VectorXf S_new(2);

    int k = 0;
    for (int i = 0; i < values - 1; i++) {
        if (i == max_diff_value) {
            k++;
            T_new(i, 0) = T(k, 0);
            T(i, 1) = 1;
            Z_new(i) = Z(k);
            k++;
        }
        else  {
            T_new(i,0) = T(k,0);
            T(i,1) = 1;
            Z_new(i) = Z(k);
            k++;
        }
    }
    cout << endl << "< New T values: >" << endl;
    for (int i = 0; i < values - 1; i++)
        cout << "\t" << T_new(i,0) << endl;
    cout << endl << "< New log(Z) values: >" << endl;
    for (int i = 0; i < values - 1; i++)
        cout << "\t" << Z_new(i) << endl;

    S_new = T_new.colPivHouseholderQr().solve(Z_new); // The solution
    cout << endl << "< The solution using the QR decomposition is: >\n"
         << S_new << endl;

    vector<float> new_diff;
    mid_diff = 0;
    for(int i = 0; i < values - 1; i++) {
        float value = S_new(0, 0)*T_new(i, 0) + S_new(1, 0);
        float temp = abs(Z_new(i) - value);
        new_diff.push_back(temp);
        mid_diff += temp;
    }
    mid_diff /= (values - 1);
    cout << "< Mid difference: " << mid_diff << " >" << endl << endl;

    max_diff = 0;
    max_diff_value = 0;
    for(int i = 0; i < values - 1; i++) {
        if(new_diff[i] > mid_diff) {
            if (new_diff[i] > max_diff) {
                max_diff = diff[i];
                max_diff_value = i;
            }
            cout << "< Value number |" << i << "| is a miss >" << endl;
            cout << "< Point: " << T(i, 0) << " / " << Z(i) << " >" << endl;
        }
    }
     */

    inFile.open("data1.txt");
    if (inFile.is_open()){
        cout << "< File opened > " << endl;
    } else{
        cerr << "Warning! File didn't opened!" << endl;
        return 1;
    }
    inFile >> values;
    cout << "< Number of values: " << values << " >" << endl;

    Eigen::MatrixXf T_new(values - 1,2);
    Eigen::VectorXf Z_new(values - 1);
    Eigen::VectorXf S_new(2);
    cout << endl << "< T values: >" << endl;
    for (int i = 0; i < values - 1; i++){
        int temp;
        inFile >> temp;
        cout << "\t" << temp << endl;
        T_new(i,0) = temp;
        T_new(i,1) = 1;
    }
    cout << endl << "< Z values: >" << endl;
    for (int i = 0; i < values - 1; i++){
        int temp;
        inFile >> temp;
        cout << "\t" << temp << endl;
        Z_new(i) = log(temp);
    }
    inFile.close();

    S_new = T_new.colPivHouseholderQr().solve(Z_new); // The solution
    cout << endl << "< The solution using the QR decomposition is: >\n"
         << S_new << endl;

    vector<float> diff_new;
    mid_diff = 0;
    for(int i=0; i<values - 1; i++) {
        float value = S_new(0, 0)*T_new(i, 0) + S_new(1, 0);
        float temp = abs(Z_new(i) - value);
        diff_new.push_back(temp);
        mid_diff += temp;
    }
    mid_diff /= values;
    cout << "< Mid difference: " << mid_diff << " >" << endl << endl;

    max_diff = 0;
    max_diff_value = 0;
    for(int i=0; i<12; i++) {
        if(diff_new[i] > mid_diff) {
            if (diff_new[i] > max_diff) {
                max_diff = diff_new[i];
                max_diff_value = i;
            }
            cout << "< Value number |" << i << "| is a miss >" << endl;
            cout << "< Point: " << T_new(i, 0) << " / " << Z_new(i) << " >" << endl;
        }
    }
    cout << "< Value |" << max_diff_value << "| is a maximum miss: " << max_diff << " >" << endl;

    for (int i = 0;i < 10; i++){
        float value;
        cin >> value;
        value = log(value);
        cout << (value - S(1, 0)) / S(0, 0) << endl;
    }

    return 0;
}