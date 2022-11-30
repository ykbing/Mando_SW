// 1 회전 당 4 pulse 출력 
// 모터 최대 회전 수 3000RPM

#define MAX_RPM 3000

#include <stdio.h>

int main(void)
{
	int input_RPM = 0;
	int input_pulse = 0;
	float period = 0;
	float result_period = 0.0;

	while (1)
	{
		printf("----Period(ms) 계산---- \n");
		printf("모터 회전수(RPM): ");
		scanf("%d", &input_RPM);
		if (input_RPM > MAX_RPM)
		{
			printf("RPM값 초과 다시 입력해주세요");
			continue;
		}

		printf("1회 회전당 pulse 수:");
		scanf("%d", &input_pulse);

		period = ((input_RPM / 60) * input_pulse);
		result_period = 1 / period * 1000;

		printf("계산된 주기(ms): %lf", result_period);
	}

	return 0;
}
