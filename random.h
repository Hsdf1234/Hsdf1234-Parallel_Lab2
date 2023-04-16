#include <bits/stdc++.h>

using namespace std;

// ����0��1֮��ĸ��������
float get_random_float() {
    static mt19937 gen(time(nullptr));  // ��ʱ����Ϊ����
    uniform_real_distribution<> dis(0.0, 1.0);  // ���ȷֲ�
    return dis(gen);
}

// ���²������������
// �����Ȳ��������Ǿ���Ȼ��ͬ��֮��������ϵĲ��ԣ���֤��������
void reset(float** m, int n) {
    // �Ȳ���һ�������Ǿ���
    for (int i=0; i<n; i++) {
        for (int j=0; j<n; j++) {
            m[i][j] = 0;
        }
        m[i][i] = 1.0;
        for (int j=i+1; j<n; j++) {
            m[i][j] = get_random_float();
        }
    }
    // ���½����������
    for (int k=0; k<n; k++) {
        for (int i=k+1; i<n; i++) {
            // ��i�� += ��k��
            for (int j=0; j<n; j++) {
                m[i][j] += m[k][j];
            }
        }
    }
}

void print(float** m, int n) {
    for (int i=0; i<n; i++) {
        for (int j=0; j<n; j++) {
            cout << m[i][j] << ' ';
        }
        cout << endl;
    }
}