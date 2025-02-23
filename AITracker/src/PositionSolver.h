#pragma once

#include "opencv2/core/matx.hpp"
#include "opencv.hpp"
#include "data.h"

/**
	This class is responsible of mapping 2D facial landmarks to its
	corresponding 3D world coordinates for pose estimation (Position and rotation).
*/
class PositionSolver
{

public:

	double prior_pitch = -2.f, prior_yaw = -2.f, prior_distance = -1.f;

	PositionSolver(
		int im_width, 
		int im_height,
		float prior_pitch = -2.f,
		float prior_yaw = -2.f,
		float prior_distance = -1.f,
		bool complex = false,
		float fov = 56.0f );

	/**
		Stores solved translation/rotation on the face_data object
		@param face_data FaceData instance with 2D landmark coordinates detected
	*/
	void solve_rotation(FaceData* face_data);
	void set_prior_pitch(float new_pitch);
	void set_prior_yaw(float new_yaw);
	void set_prior_distance(float new_distance);

private:
	static const int NB_CONTOUR_POINTS_COMPLEX = 29;
	static const int NB_CONTOUR_POINTS_BASE = 18;
	const double TO_DEG = (180.0 / 3.14159265);
	const double TO_RAD = (3.14159265 / 180.0);

	cv::Mat mat3dface;
	cv::Mat mat3dcontour;
	std::vector<int> contour_indices;  // Facial landmarks that interest us

	//Buffer so we dont have to allocate a list on every solve_rotation call.
	cv::Mat landmark_points_buffer;
	//Prior rotations and translation as a vector so we dont have to allocate them.
	std::vector<double> rv, tv;  

	int width, height;
	
	cv::Mat camera_matrix, camera_distortion;

	bool complex;

	/**
		Gets euler angles from rotation matrix.
	*/
	void get_euler(cv::Mat& rvec, cv::Mat& tvec);

	/**
	* Lateral/Vertical offset adds an error to the calculated rotation.
	* This method corrects them.
	*/
	void correct_rotation(FaceData& face_data);
};

