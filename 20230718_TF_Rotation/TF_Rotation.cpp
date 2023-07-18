// TF_Rotation.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#define _USE_MATH_DEFINES
#include "math.h"

#define RAD2DEG(x)   x*180/M_PI 
#define DEG2RAD(x)   x/180*M_PI

#define Map_x -1.0
#define Map_y 1.0

#define BaseLink_x 1.0
#define BaseLink_y 1.0

typedef struct 
{
	double x;
	double y;
	double theta;
} Pose2D;

typedef struct
{
	double x;
	double y;	
} Point2D;

Pose2D    base_link_origin;
Point2D   base_link_Point2D, base_link_map_Point2D;

double cal_x, cal_y;
double angle_degree;
double angle_radian;
double Rotation_matrix[2][2];
double Rotation_matrix_inverse[2][2]; 

void set_rotation_matrix(double m_angle_degree)
{
	angle_radian = DEG2RAD(m_angle_degree);

	Rotation_matrix[0][0] = cos(angle_radian);	Rotation_matrix[0][1] = -sin(angle_radian);
	Rotation_matrix[1][0] = sin(angle_radian);	Rotation_matrix[1][1] = cos(angle_radian);

	printf("***Rotation_matrix***\n");
	printf("%6.3lf  %6.3lf\n", Rotation_matrix[0][0], Rotation_matrix[0][1]);
	printf("%6.3lf  %6.3lf\n", Rotation_matrix[1][0], Rotation_matrix[1][1]);
}

void set_rotation_matrix_inverse(double m_angle_degree)
{
	angle_radian = DEG2RAD(m_angle_degree);
	
	Rotation_matrix_inverse[0][0] = cos(angle_radian);  Rotation_matrix_inverse[0][1] = sin(angle_radian);
	Rotation_matrix_inverse[1][0] = -sin(angle_radian); Rotation_matrix_inverse[1][1] = cos(angle_radian);

    printf("***Rotation_matrix_inverse***\n");
	printf("%6.3lf  %6.3lf\n", Rotation_matrix_inverse[0][0], Rotation_matrix_inverse[0][1]);
	printf("%6.3lf  %6.3lf\n", Rotation_matrix_inverse[1][0], Rotation_matrix_inverse[1][1]);
}

void TF_base_link_map_link_base(Point2D base_link_point2d)
{
	base_link_point2d.x = (Map_x * Rotation_matrix[0][0]) + (Map_y * Rotation_matrix[0][1]);
	base_link_point2d.y = (Map_x * Rotation_matrix[1][0]) + (Map_y * Rotation_matrix[1][1]);


	printf("\n\n");
	printf("****base_link_point2d***\n");
	printf("X: %6.3lf Y: %6.3lf\n", base_link_point2d.x, base_link_point2d.y);
}

void TF_base_link_base_link_map(Point2D base_link_map_Point2D)
{
	base_link_map_Point2D.x = (BaseLink_x * Rotation_matrix_inverse[0][0]) + (BaseLink_y * Rotation_matrix_inverse[0][1]);
	base_link_map_Point2D.y = (BaseLink_x * Rotation_matrix_inverse[1][0]) + (BaseLink_y * Rotation_matrix_inverse[1][1]);

	printf("\n\n");
	printf("***base_link_map_point2d***\n");
	printf("X: %6.3lf Y: %6.3lf\n", base_link_map_Point2D.x, base_link_map_Point2D.y);
}

void Input_Data()
{
	printf("base_link_origin_x: ");
	scanf_s("%lf", &base_link_origin.x);
	printf("base_link_origin_y: ");
	scanf_s("%lf", &base_link_origin.y);
	printf("base_link_origin_angle: ");
	scanf_s("%lf", &base_link_origin.theta);
}

int _tmain(int argc, _TCHAR* argv[])
{	
	Input_Data();

	set_rotation_matrix(base_link_origin.theta);
	set_rotation_matrix_inverse(base_link_origin.theta);

	TF_base_link_map_link_base(base_link_map_Point2D);
	TF_base_link_base_link_map(base_link_Point2D);

	return 0;
}

