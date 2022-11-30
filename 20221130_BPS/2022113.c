#include <stdio.h>

int main(void)
{
    int BPS_value, B_value;
    float result_msec, time, BPS, B_input, B;

    printf("BPS를 입력해주세요: ");
    scanf("%d", &BPS_value);

    printf("통신 Byte를 입력해주세요: ");
    scanf("%d", &B_value);

    // msec <- sec 변환
    BPS = BPS_value / 1000;

    // 전송 속도(S) = 데이터의 양(A) / 전송 시간(T)
    result_msec = 1 / BPS;
    B_input = result_msec * B_value;

    printf("1Byte 당 통신 소요시간: %0.3f msec \n", result_msec);
    printf("입력 바이트에 맞는 통신 시간: %0.3f msec", B_input);
}
