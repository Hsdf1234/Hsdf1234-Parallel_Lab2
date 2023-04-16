#include <bits/stdc++.h>  // ʹ������ͷ�ļ�stdc++.h��ע�⣬�е�IDE���ܲ�֧��
#include "random.h"  // ʹ�����Ƶ������

using namespace std;

// �½�һ������B�����A��ת��
float B[5000][5000];

// ��ͨ��˹��ȥCache�Ż�
void LU(float** A, int N) {
    // ����ת�õ�ʱ�临�Ӷ�ΪO(n^2)�������㷨ΪO(n^3)�����Կ��Ժ���
    for (int i=0; i<N; i++) {
        for (int j=0; j<i; j++) {
            B[j][i] = A[i][j];
            A[i][j] = 0;
        }
    }
    for (int k=0; k<N; k++) {
        for (int j=k+1; j<N; j++) {
            A[k][j] = A[k][j] / A[k][k];
        }
        A[k][k] = 1.0;
        for (int i=k+1; i<N; i++) {
            for (int j=k+1; j<N; j++) {
                A[i][j] = A[i][j] - A[k][j]*B[k][i];
            }
        }
    }
}

int main() {
    int N;
    vector<int> size = {200, 500, 1000, 2000, 3000};
    for (int i=0; i<5; i++) {
        // ���������ģ
        N = size[i];

        // ��ʼ����ά����
        float** A = new float*[N];
        for (int i=0; i<N; i++) {
            A[i] = new float[N];
        }

        // ʹ���������������
        reset(A, N);

        // ʹ��C++11��chrono������ʱ
        auto start = chrono::high_resolution_clock::now();
        LU(A, N);
        auto end = chrono::high_resolution_clock::now();
        auto diff = chrono::duration_cast<chrono::duration<double, milli>>(end - start);
        cout << "Size = " << N << ": " << diff.count() << "ms" << endl;
    }
    return 0;
}