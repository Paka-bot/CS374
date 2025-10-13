#include <stdio.h>
#include <math.h>

// 파이(PI) 상수 정의 [cite: 58]
#define PI 3.14159265359

/**
 * @brief 이 프로그램은 사용자로부터 2에서 10개의 구형 조각(spherical segment)에 대한
 * 치수(반지름 R, 높이 ha, hb)를 입력받아, 각 조각의 총 표면적과 부피를 계산합니다.
 * 모든 계산이 끝난 후에는 전체 표면적과 부피의 평균값을 출력합니다.
 * 입력값은 유효성 검사를 거치며, 유효한 데이터가 입력될 때까지 재입력을 요청합니다.
 */
int main() {
    // --- 변수 선언 ---
    // 계산할 구형 조각의 개수 [cite: 35]
    int n = 0;
    // 유효한 데이터 세트의 개수를 세는 카운터
    int valid_sets_count = 0;

    // 사용자로부터 입력받을 변수들 (double 타입 사용) [cite: 34, 38]
    double R, ha, hb;

    // 계산에 사용될 중간 변수들
    double a, b, h;

    // 각 구형 조각의 계산 결과를 저장할 변수들
    double top_surface_area, bottom_surface_area, lateral_surface_area;
    double total_surface_area, volume;

    // 평균 계산을 위한 합계 변수들
    double total_area_sum = 0.0;
    double total_volume_sum = 0.0;


    // --- 1. 계산할 구형 조각 개수 입력 (2-10 사이) ---
    // 유효한 값이 입력될 때까지 반복 
    do {
        printf("How many spherical segments you want to evaluate [2-10]?\n");
        scanf("%d", &n);
    } while (n < 2 || n > 10);


    // --- 2. n개의 유효한 데이터 세트에 대해 계산 반복 ---
    // valid_sets_count가 n보다 작을 동안 반복하여 n개의 유효한 세트를 처리 [cite: 37, 46]
    while (valid_sets_count < n) {
        printf("Obtaining data for spherical segment number %d\n", valid_sets_count + 1);

        // R, ha, hb 값 입력받기 
        printf("What is the radius of the sphere (R)?\n");
        scanf("%lf", &R);
        printf("What is the height of the top area of the spherical segment (ha)?\n");
        scanf("%lf", &ha);
        printf("What is the height of the bottom area of the spherical segment (hb)?\n");
        scanf("%lf", &hb);

        // 입력받은 값 확인을 위해 출력 [cite: 40]
        printf("Entered data: R=%.2f ha=%.2f hb=%.2f\n", R, ha, hb);

        // 입력값 유효성 검증 [cite: 42, 43, 44]
        if (R > 0 && ha > 0 && hb > 0 && ha <= R && hb <= R && ha >= hb) {
            // --- 3. 유효한 데이터일 경우 계산 수행 --- 
            
            // 중간값 a, b, h 계산
            a = sqrt(R * R - ha * ha); // [cite: 28, 61]
            b = sqrt(R * R - hb * hb); // [cite: 28, 61]
            h = ha - hb;               // [cite: 28]

            // 넓이와 부피 계산
            top_surface_area = PI * a * a;                                 // [cite: 28]
            bottom_surface_area = PI * b * b;                              // [cite: 29]
            lateral_surface_area = 2 * PI * R * h;                         // [cite: 30]
            total_surface_area = top_surface_area + bottom_surface_area + lateral_surface_area; // [cite: 32]
            
            // 샘플 실행 결과와 일치하는 수정된 부피 공식 사용
            volume = (1.0 / 6.0) * PI * h * (3 * a * a + 3 * b * b + h * h);

            // 계산 결과 출력 (소수점 둘째 자리까지) [cite: 49, 135]
            printf("Total Surface Area = %.2f Volume = %.2f\n", total_surface_area, volume);

            // 평균 계산을 위해 합계 누적
            total_area_sum += total_surface_area;
            total_volume_sum += volume;

            // 유효한 세트 카운터 증가
            valid_sets_count++;
        } else {
            // 유효하지 않은 데이터일 경우 에러 메시지 출력 
            printf("Invalid Input.\n");
            // valid_sets_count가 증가하지 않으므로, 다음 루프에서 같은 번호의 데이터를 다시 입력받게 됨
        }
    }

    // --- 4. 최종 평균값 계산 및 출력 --- 
    if (n > 0) {
        double average_surface_area = total_area_sum / n;
        double average_volume = total_volume_sum / n;

        printf("Total average results:\n");
        printf("Average Surface Area = %.2f Average Volume = %.2f\n", average_surface_area, average_volume);
    }

    return 0; // 프로그램 정상 종료
}
