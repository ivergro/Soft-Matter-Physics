#pragma once
#include "std_lib_facilities.h"
#include "filewriter.h"

//Forbedringer
    //Plot marking hits and misses

//Const variable at runtime

//Box parameters
constexpr double a = 0.5;
constexpr double b = 3.5;
constexpr double c = 0.5;
constexpr double d = 1.5;

//Disk parameter (unit disk)
constexpr double r = 1;

//to be able to miss on random numbers
constexpr double ext = 0.5;

double hit_or_miss_rectangle(int N);
double hit_or_miss_disk(int N);


struct Point {
		double x = 0;
		double y = 0;
        Point(double a, double b) : x(a), y(b) {}
	};	

