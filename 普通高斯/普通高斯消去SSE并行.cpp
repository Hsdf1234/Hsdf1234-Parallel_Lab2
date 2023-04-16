#include <bits/stdc++.h>
#include <nmmintrin.h>  // SSE4.2
#include "random.h"

using namespace std;

// ��ͨ��˹��ԪSSE�����㷨
void LU(float** A, int N) {
    for (int k=0; k<N; k++) {
        // ���л����г���
        // float dup[4] = {A[k][k], A[k][k], A[k][k], A[k][k]};
        // vt�������Ĵ���Ϊ��{A[k][k], A[k][k], A[k][k], A[k][k]}
        __m128 vt = _mm_load1_ps(&A[k][k]);  // ����4��A[k][k]���vt��
        int j;
        for (j=k+1; j+4<=N; j+=4) {
            // va�������Ĵ���Ϊ��{A[k][j], A[k][j+1], A[k][j+2], A[k][j+3]}
            __m128 va = _mm_loadu_ps(&A[k][j]);  // loadu���ڴ���Բ�����

            // va = va / vt
            va = _mm_div_ps(va, vt);

            // ��va�Ĵ����洢��ԭλ�ã����4�����ĳ���
            _mm_storeu_ps(&A[k][j], va);
        }

        // ����ʣ�µ�Ԫ�أ�������಻����3��������ֱ�Ӵ��г����Ϳ�����
        for (; j<N; j++) {
            A[k][j] = A[k][j] / A[k][k];
        }
        A[k][k] = 1.0;
        for (int i=k+1; i<N; i++) {
            // �Ե�i�е�Ԫ�ؼ�����в��л�����
            // A[i][j]��A[k][j]��ʼ�����ĸ�Ԫ�طֱ��γɼĴ���
            // A[i][k]Ϊ�̶�ֵ������4�ݴ�����һ���Ĵ�����
            __m128 vaik = _mm_load1_ps(&A[i][k]);
            int j;
            for (j=k+1; j+4<=N; j+=4) {
                // ԭʼ��ʽ��A[i][j] = A[i][j] - A[k][j]*A[i][k];
                __m128 vakj = _mm_loadu_ps(&A[k][j]);
                __m128 vaij = _mm_loadu_ps(&A[i][j]);
                __m128 vx = _mm_mul_ps(vakj, vaik);
                vaij = _mm_sub_ps(vaij, vx);
                _mm_storeu_ps(&A[i][j], vaij);
            }

            // ʣ�µ�Ԫ��
            for (; j<N; j++) {
                A[i][j] = A[i][j] - A[k][j]*A[i][k];
            }
            A[i][k] = 0;
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