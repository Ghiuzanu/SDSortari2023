#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <cmath>
#include <fstream>

using namespace std;

ifstream fin("input.in");
ofstream fout("results.out");

void radixsort(vector<long long> &v, long long radixBase, long long nrDigits){
    vector<vector<long long>> B(radixBase);
    long long x = 1, X = 9;
    for (long long i = 0; i < nrDigits - 1; ++i) {
        X = X * 10 + 9;
    }
    while(x < X) {
        B.clear();
        B.resize(radixBase);
        for (long long j = 0; j < v.size(); ++j) {
            B[(v[j] / x) % radixBase].push_back(v[j]);
        }
        v.clear();
        for (long long i = 0; i < radixBase; ++i) {
            for (long long j = 0; j < B[i].size(); j++) {
                v.push_back(B[i][j]);
            }
        }
        x = x * radixBase;
    }
}

void merge(vector<long long>& v, long long start, long long mid, long long end) {
    long long i = start, j = mid + 1, k = 0;
    vector<long long> temp(end - start + 1);
    while (i <= mid && j <= end) {
        if (v[i] <= v[j]) {
            temp[k++] = v[i++];
        } else {
            temp[k++] = v[j++];
        }
    }
    while (i <= mid) {
        temp[k++] = v[i++];
    }
    while (j <= end) {
        temp[k++] = v[j++];
    }
    for (i = start, k = 0; i <= end; ++i, ++k) {
        v[i] = temp[k];
    }
}

void mergesort(vector<long long>& v, long long start, long long end) {
    if (start < end) {
        long long mid = (start + end) / 2;
        mergesort(v, start, mid);
        mergesort(v, mid + 1, end);
        merge(v, start, mid, end);
    }
}

void shellSort(vector<long long>& v) {
    long long n = v.size();

    // Gap sequence
    for (long long gap = n/2; gap > 0; gap /= 2) {
        // Do an insertion sort for each gap-sized subarray
        for (long long i = gap; i < n; ++i) {
            long long temp = v[i];
            long long j;
            for (j = i; j >= gap && v[j - gap] > temp; j -= gap) {
                v[j] = v[j - gap];
            }
            v[j] = temp;
        }
    }
}

void countSort(vector<long long> &v, long long nrDigits){
    long long maxx = 1;
    for (long long i = 0; i < nrDigits; ++i) {
        maxx = maxx * 10;
    }
    vector<long long>counter(maxx);
    for (long long i = 0; i < v.size(); ++i) {
        counter[v[i]]++;
    }
    v.clear();
    for (long long i = 0; i < maxx; ++i) {
        for (long long j = 0; j < counter[i]; ++j) {
            v.push_back(i);
        }
    }
}

long long partition(vector<long long>& v, long long start, long long end) {
    long long pivot = v[end];
    long long i = start - 1;
    for (long long j = start; j < end; j++) {
        if (v[j] <= pivot) {
            i++;
            swap(v[i], v[j]);
        }
    }
    swap(v[i + 1], v[end]);
    return i + 1;
}

void quicksort(vector<long long>& v, long long start, long long end) {
    if (start < end) {
        long long pivot = partition(v, start, end);
        quicksort(v, start, pivot - 1);
        quicksort(v, pivot + 1, end);
    }
}

int main() {
    long long t;
    fin>>t;
    for (int j = 1; j < t + 1; ++j) {
        fout.tie(nullptr);
        fout << "--------------------------------------------------------------\n";
        fout << "Test " << j << endl << endl;
        srand(time(0));
        long long nrDigits, nrNumbers, x;
        vector<long long> v;
        fin >> nrNumbers;
        fout << "Numbers to sort: " << nrNumbers << endl;
        fin >> nrDigits;
        fout << "Maximum number of digits: " << nrDigits << endl << endl;
        for (long long i = 0; i < nrNumbers; ++i) {
            x = rand() % 10;
            for (long long i = 0; i < nrDigits - 1; ++i) {
                x = x * 10 + rand() % 10;
            }
            v.push_back(x);
        }
        countSort(v, nrDigits);
        fout << endl << "RADIXSORT: ";
        vector<long long> w(v), vv(v), ww(v), vw(v);
        const auto t1 = std::chrono::high_resolution_clock::now();
        long long i;
        for (i = 0; i < w.size() - 1; ++i) {
            if (w[i] > w[i + 1]) {
                break;
            }
        }
        if (i == w.size())
            fout<<"already sorted"<<endl;
        radixsort(w, 10, nrDigits);
        for (long long i = 0; i < w.size() - 1; ++i) {
            if (w[i] > w[i + 1]) {
                fout << "Radix did not sort properly" << endl;
                break;
            }
        }
//    mergesort(v, 0, nrNumbers - 1);
//    fout<<endl;
        const auto t2 = std::chrono::high_resolution_clock::now();
        fout << (std::chrono::duration_cast<std::chrono::minutes>(t2 - t1).count()) << " minutes // ";
        fout << (std::chrono::duration_cast<std::chrono::seconds>(t2 - t1).count()) << " seconds // ";
        fout << (std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()) << " milliseconds // ";
        fout << (std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count()) << " nanoseconds \n ";

        fout << endl << "MERGESORT: ";
        const auto t3 = std::chrono::high_resolution_clock::now();
//    radixsort(v, 10, nrDigits);
        mergesort(v, 0, nrNumbers - 1);
        for (long long i = 0; i < w.size() - 1; ++i) {
            if (w[i] > w[i + 1]) {
                fout << "Merge did not sort properly" << endl;
                break;
            }
        }
//    fout<<endl;
        const auto t4 = std::chrono::high_resolution_clock::now();
        fout << (std::chrono::duration_cast<std::chrono::minutes>(t4 - t3).count()) << " minutes // ";
        fout << (std::chrono::duration_cast<std::chrono::seconds>(t4 - t3).count()) << " seconds // ";
        fout << (std::chrono::duration_cast<std::chrono::milliseconds>(t4 - t3).count()) << " milliseconds // ";
        fout << (std::chrono::duration_cast<std::chrono::nanoseconds>(t4 - t3).count()) << " nanoseconds \n";

        fout << endl << "SHELLSORT: ";
        const auto t5 = std::chrono::high_resolution_clock::now();
        shellSort(vv);
        for (long long i = 0; i < vv.size() - 1; ++i) {
            if (vv[i] > vv[i + 1]) {
                fout << "Shell did not sort properly" << endl;
                break;
            }
        }
//    fout<<endl;
        const auto t6 = std::chrono::high_resolution_clock::now();
        fout << (std::chrono::duration_cast<std::chrono::minutes>(t6 - t5).count()) << " minutes // ";
        fout << (std::chrono::duration_cast<std::chrono::seconds>(t6 - t5).count()) << " seconds // ";
        fout << (std::chrono::duration_cast<std::chrono::milliseconds>(t6 - t5).count()) << " milliseconds // ";
        fout << (std::chrono::duration_cast<std::chrono::nanoseconds>(t6 - t5).count()) << " nanoseconds \n";


        fout << endl << "COUNTSORT: ";
        if (nrDigits >= 10) {
            fout << "Too many digits for this sort \n";
        } else {
            const auto t7 = std::chrono::high_resolution_clock::now();
            countSort(ww, nrDigits);
            for (long long i = 0; i < ww.size() - 1; ++i) {
                if (ww[i] > ww[i + 1]) {
                    fout << "Count did not sort properly" << endl;
                    break;
                }
            }
//        fout<<endl;
            const auto t8 = std::chrono::high_resolution_clock::now();
            fout << (std::chrono::duration_cast<std::chrono::minutes>(t8 - t7).count()) << " minutes // ";
            fout << (std::chrono::duration_cast<std::chrono::seconds>(t8 - t7).count()) << " seconds // ";
            fout << (std::chrono::duration_cast<std::chrono::milliseconds>(t8 - t7).count()) << " milliseconds // ";
            fout << (std::chrono::duration_cast<std::chrono::nanoseconds>(t8 - t7).count()) << " nanoseconds \n";
        }

        fout << endl << "QUICKSORT: ";
        if (nrDigits >= 10) {
            fout << "Too many digits for this sort \n";
        } else {
            const auto t9 = std::chrono::high_resolution_clock::now();
            quicksort(vw, 0, nrNumbers - 1);
//            for (long long i = 0; i < vw.size() - 1; ++i) {
//                if (vw[i] > vw[i + 1]) {
//                    fout << "Quick did not sort properly" << endl;
//                    break;
//                }
//            }
//        fout<<endl;
            const auto t10 = std::chrono::high_resolution_clock::now();
            fout << (std::chrono::duration_cast<std::chrono::minutes>(t10 - t9).count()) << " minutes // ";
            fout << (std::chrono::duration_cast<std::chrono::seconds>(t10 - t9).count()) << " seconds // ";
            fout << (std::chrono::duration_cast<std::chrono::milliseconds>(t10 - t9).count()) << " milliseconds // ";
            fout << (std::chrono::duration_cast<std::chrono::nanoseconds>(t10 - t9).count()) << " nanoseconds \n";
        }

    }
    return 0;
}