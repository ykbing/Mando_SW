#include <stdio.h>

int main(void)
{
    int BPS_value, B_value, BPS_choice;
    float result_msec, BPS, B_input;

    int BPS_LIST[] = {1200, 2400, 4800, 9600, 14400, 19200, 38400, 57600, 115200, 230400, 460800, 921600};

    printf("(1)1200  (2)2400 (3)4800 (4)9600 (5)14400 (6)19200 \n");
    printf("(7)38400  (8)57600 (9)115200 (10)230400 (11)460800 (12)921600 \n");
    printf("BPS를 입력해주세요(괄호 안 숫자 입력): ");
    scanf("%d", &BPS_choice);

    BPS_value = BPS_LIST[BPS_choice - 1]; 

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
