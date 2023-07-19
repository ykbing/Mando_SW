#include "stdafx.h"
#define _USE_MATH_DEFINES
#include "math.h"

#define RAD2DEG(x)   x*180/M_PI 
#define DEG2RAD(x)   x/180*M_PI

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
Point2D   base_link_Point2D, base_link_map_Point2D, map_link;

double angle_degree;
double angle_radian;
double Rotation_matrix[2][2];
double Rotation_matrix_inverse[2][2]; 

void set_rotation_matrix(double m_angle_degree)
{
	angle_radian = DEG2RAD(m_angle_degree);

	Rotation_matrix[0][0] = cos(angle_radian);	Rotation_matrix[0][1] = -sin(angle_radian);
	Rotation_matrix[1][0] = sin(angle_radian);	Rotation_matrix[1][1] = cos(angle_radian);

	/*
	printf("***Rotation_matrix***\n");
	printf("%6.3lf  %6.3lf\n", Rotation_matrix[0][0], Rotation_matrix[0][1]);
	printf("%6.3lf  %6.3lf\n", Rotation_matrix[1][0], Rotation_matrix[1][1]);
	*/
}

void set_rotation_matrix_inverse(double m_angle_degree)
{
	angle_radian = DEG2RAD(m_angle_degree);
	
	Rotation_matrix_inverse[0][0] = cos(angle_radian);  Rotation_matrix_inverse[0][1] = sin(angle_radian);
	Rotation_matrix_inverse[1][0] = -sin(angle_radian); Rotation_matrix_inverse[1][1] = cos(angle_radian);

	/*
	printf("\n");
    printf("***Rotation_matrix_inverse***\n");
	printf("%6.3lf  %6.3lf\n", Rotation_matrix_inverse[0][0], Rotation_matrix_inverse[0][1]);
	printf("%6.3lf  %6.3lf\n", Rotation_matrix_inverse[1][0], Rotation_matrix_inverse[1][1]);
	*/
}

void translational_base_link_map(Point2D base_link_Point2D, Point2D* base_link_map_point2d, Pose2D base_link_origin)
{
	base_link_map_point2d->x = (map_link.x - base_link_origin.x);
	base_link_map_point2d->y = (map_link.y - base_link_origin.y);
	printf("/// *************base_link_map -> base_link************* ///\n");
	printf("base_link_map_point2d.X: %lf\n", base_link_map_point2d->x);
	printf("base_link_map_point2d.Y: %lf\n", base_link_map_point2d->y);
}

//  base_link_map => base_link
void TF_base_link_map_base_link(Point2D base_link_Point2D, Point2D* base_link_map_point2d, Pose2D base_link_origin)
{
	set_rotation_matrix(base_link_origin.theta);

	base_link_Point2D.x = (base_link_map_point2d->x * Rotation_matrix[0][0]) + (base_link_map_point2d->y * Rotation_matrix[0][1]);
	base_link_Point2D.y = (base_link_map_point2d->x * Rotation_matrix[1][0]) + (base_link_map_point2d->y * Rotation_matrix[1][1]);

	printf("\n");
	printf("base_link_Point2D_X: %6.3lf\nbase_link_Point2D_Y: %6.3lf\n", base_link_Point2D.x, base_link_Point2D.y);
}

/*********************************************************************************************/

//  base_link => base_link_map
void TF_base_link_base_link_map(Point2D base_link_Point2D, Point2D* base_link_map_point2d, Pose2D base_link_origin)
{
	set_rotation_matrix_inverse(base_link_origin.theta);

	base_link_map_point2d->x = (base_link_Point2D.x * Rotation_matrix_inverse[0][0]) + (base_link_Point2D.y * Rotation_matrix_inverse[0][1]);
	base_link_map_point2d->y = (base_link_Point2D.x * Rotation_matrix_inverse[1][0]) + (base_link_Point2D.y * Rotation_matrix_inverse[1][1]);
	
	printf("\n");
	printf("/// *************base_Link -> map************* ///\n");
	printf("base_link_map_point2d_X: %6.3lf\nbase_link_map_point2d_Y: %6.3lf\n", base_link_map_point2d->x, base_link_map_point2d->y);
}
void translational_map_link(Point2D base_link_Point2D, Point2D* base_link_map_point2d, Pose2D base_link_origin)
{
	map_link.x = (base_link_map_point2d->x + base_link_origin.x);
	map_link.y = (base_link_map_point2d->y + base_link_origin.y);
	printf("\n");
	printf("map_link.X: %lf\n", map_link.x);
	printf("map_link.Y: %lf\n", map_link.y);
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
	//angle_degree = 90;
	//angle_radian = DEG2RAD(angle_degree);

	/*
	base_link_origin.x = 3.0;
	base_link_origin.y = 3.0;
	base_link_origin.theta = -90;
	*/

	map_link.x = 4.0;
	map_link.y = 4.0;

	base_link_Point2D.x = 1;
	base_link_Point2D.y = -1;

	base_link_map_Point2D.x = 1;
	base_link_map_Point2D.y = 1;

	Input_Data();

	translational_base_link_map(base_link_Point2D, &base_link_map_Point2D, base_link_origin);
	TF_base_link_map_base_link(base_link_Point2D, &base_link_map_Point2D, base_link_origin);
	
	TF_base_link_base_link_map(base_link_Point2D, &base_link_map_Point2D, base_link_origin);
	translational_map_link(base_link_Point2D, &base_link_map_Point2D, base_link_origin);

	return 0;
}
