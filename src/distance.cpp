#include "miscela.hpp"

double deg2rad(double);
double lat2meter(double);
double lon2meter(double);

double dist1(double lat1, double lon1, double lat2, double lon2){
	/* calculates distance using hubeny formula */

	double R_E = 6378137.0; // Equatorial radius
	double R_P = 6356752.314140356; // Polar radius
	double E = (R_E*R_E - R_P*R_P)/(R_E*R_E); //(First eccentricity)^2
	double CR_MA = R_E*(1.0-E); // Curvature radius of meridian on equator

	double delta_lat = deg2rad(lat1)-deg2rad(lat2);
	double delta_lon = deg2rad(lon1)-deg2rad(lon2);
	double lat_ave = (deg2rad(lat1)+deg2rad(lat2))/2.0;
	double sin_lat = sin(lat_ave);

	double w = 1.0 - E*(sin_lat*sin_lat);
	double CR_M = CR_MA/(sqrt(w)*w); // Curvature radius of meridian
	double CR_P = R_E/sqrt(w); // Curvature radius of prime vertical

	double t1 = CR_M*delta_lat;
	double t2 = CR_P*cos(lat_ave)*delta_lon;

	return sqrt((t1*t1)+(t2*t2))/1000.0;
}

double dist2(double lat1, double lon1, double lat2, double lon2){
	double lat = fabs(lat2meter(lat1)-lat2meter(lat2));
	double lon = fabs(lon2meter(lon1)-lon2meter(lon2));
	return sqrt(lat*lat+lon*lon);
}

double deg2rad(double deg){
	return (deg*M_PI/180.0);
}

double lat2meter(double lat){
	// santander city : lat=43.45, lon=-3.816667
	// km scale
	double kmParLat = 111.319490793;
	return lat*kmParLat;
}

double lon2meter(double lon){
	// santander city : lat=43.45, lon=-3.816667
	// km scale
	double kmParLon = 80.815144778;
	return lon*kmParLon;
}

