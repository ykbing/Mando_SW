// 이동평균 필터: 구간을 이동시키면서 구간에 있는 값들의 평균값을 구하여 실제값을 보다 정확하게 추정하는 필터
// ex) 냉장고

#include <stdio.h>
#include <stdlib.h>

#define N 10

double mov_avg(double pre_avg, double n_data);

double data[N+1] = {0, };
	
int main()
{
	double pre_avg = 0; // 이전 평균
	double n_data = 0; // 새로 입력된 데이터
	
	while(1)
	{
		printf("\n");
		printf("데이터 입력 : ");
		scanf("%lf", &n_data);
		pre_avg = mov_avg(pre_avg, n_data);
		printf("\n");
		printf("평균 값 : %.2f\n", pre_avg);
	}
	
	return 0;
}

double mov_avg(double pre_avg, double n_data)
{
	double avg = 0;
	data[N] = n_data;

	avg = pre_avg + data[N] / N - data[0] / N; // 재귀식

	for(int i = 1; i < N+1; i++)
	{
		printf("[%.2lf]", data[i]);
	}
	
	for(int i = 1; i < N+1; i++)
	{
		data[i-1] = data[i];
	}
	data[N]=0;
	
	return avg;
}