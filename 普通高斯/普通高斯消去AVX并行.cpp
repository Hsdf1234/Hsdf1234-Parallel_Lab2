#include <bits/stdc++.h>
#include <immintrin.h>  // AVX��AVX2
#include "random.h"

using namespace std;

// ��ͨ��˹��ԪAVX�����㷨
void LU(float** A, int N) {
    for (int k=0; k<N; k++) {
        // ���л����г���
        // ��������AVXָ���û���ҵ�������SSE��_mm_load1_ps���������ָ�����ʹ���½�����ķ�ʽ������8��A[k][k]���vt��
        float dup[8] = {A[k][k], A[k][k], A[k][k], A[k][k], A[k][k], A[k][k], A[k][k], A[k][k]};

        // vt�������Ĵ���Ϊ��{A[k][k], A[k][k], A[k][k], A[k][k], A[k][k], A[k][k], A[k][k], A[k][k]}
        __m256 vt = _mm256_loadu_ps(dup);
        int j;
        for (j=k+1; j+8<=N; j+=8) {
            // va�������Ĵ���Ϊ��{A[k][j], A[k][j+1], A[k][j+2], A[k][j+3], A[k][j+4], A[k][j+5], A[k][j+6], A[k][j+7]}
            __m256 va = _mm256_loadu_ps(&A[k][j]);  // loadu���ڴ���Բ�����

            // va = va / vt
            va = _mm256_div_ps(va, vt);

            // ��va�Ĵ����洢��ԭλ�ã����8�����ĳ���
            _mm256_storeu_ps(&A[k][j], va);
        }

        // ����ʣ�µ�Ԫ��
        for (; j<N; j++) {
            A[k][j] = A[k][j] / A[k][k];
        }
        A[k][k] = 1.0;
        for (int i=k+1; i<N; i++) {
            // �Ե�i�е�Ԫ�ؼ�����в��л�����
            // A[i][j]��A[k][j]��ʼ�����ĸ�Ԫ�طֱ��γɼĴ���
            // A[i][k]Ϊ�̶�ֵ������8�ݴ�����һ���Ĵ�����
            float dupik[8] = {A[i][k], A[i][k], A[i][k], A[i][k], A[i][k], A[i][k], A[i][k], A[i][k]};
            __m256 vaik = _mm256_loadu_ps(dupik);
            int j;
            for (j=k+1; j+8<=N; j+=8) {
                // ԭʼ��ʽ��A[i][j] = A[i][j] - A[k][j]*A[i][k];
                __m256 vakj = _mm256_loadu_ps(&A[k][j]);
                __m256 vaij = _mm256_loadu_ps(&A[i][j]);
                __m256 vx = _mm256_mul_ps(vakj, vaik);
                vaij = _mm256_sub_ps(vaij, vx);
                _mm256_storeu_ps(&A[i][j], vaij);
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