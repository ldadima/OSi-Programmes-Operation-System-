#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void *thr_func(void *arg) {
	printf("Thread 2: string 0\n");
	printf("Thread 2: string 1\n");
	printf("Thread 2: string 2\n");
	printf("Thread 2: string 3\n");
	printf("Thread 2: string 4\n");
	printf("Thread 2: string 5\n");
	printf("Thread 2: string 6\n");
	printf("Thread 2: string 7\n");
	printf("Thread 2: string 8\n");
	printf("Thread 2: string 9\n");
	pthread_exit((void *) 0);
//	return ((void *) 0); // ���� 2 �������� ���������� ����, ����� ����� ����������, �� �����
};

int main(int argc, char* argv[]) {
	int err;
	void *stat; //2�� ������, ��� join �����
	pthread_t thr2;
	err = pthread_create(&thr2, NULL, thr_func, NULL);
	if (err != 0) {
		perror("Thread creation error");
	}
	err=pthread_join(thr2,&stat); //����� ��� 2�� ������, ����� ���� ������� � � 1��, �� � �������, ��� ��� ���������
	if (err != 0) {
		perror("Thread waiting error");
	}
	printf("Thr max: %d\n", sysconf(_SC_THREAD_THREADS_PROCESS));
	printf("Thread 1: string 0\n");//����� ��������� ����, �� ��� �������� �������
	printf("Thread 1: string 1\n");
	printf("Thread 1: string 2\n");
	printf("Thread 1: string 3\n");
	printf("Thread 1: string 4\n");
	printf("Thread 1: string 5\n");
	printf("Thread 1: string 6\n");
	printf("Thread 1: string 7\n");
	printf("Thread 1: string 8\n");
	printf("Thread 1: string 9\n");
	//sleep(1);//����� ��� �������� �������� ���� �������� �����(��� ��������� ������� ���� ������������� � ������� �������)
	return 0;
}
//��� ������������ ������������������ ����������? ���� ��� �������� �� ������� ���������� ����� (���� 2 ����, �� ������������ ����� ����������� ������ 2 ����;
//�����-�� ������������� ����������������� ����������� ��������� ������������� �����-������, �.�. ���� ���� ���� ������������� ��-�� �����, �� ������ ����� �����������
//���������� � ��������� �����(����������) ���� ����������, �� ����� � ����� ���� � ����������� ���������, ������� �� ���������� ���� �������� ����������, � �� ������� �� ���������� �����������
//���� 2 ���� ������ ��� �����. �������� ������ ��� ������� ������������ � ���� � ��������� ������������ 
//����������� ���� 2� ���� � ������� �� ��� 120 2�� ������ �����
//��� ����� alias, �� ����� ��� ������� ���������� �������, ��� ���-�� ������� �������, ���� � ����������, ���������� ���������� PATH
//����� ��������� �� ����� ��� �����, �� �������
