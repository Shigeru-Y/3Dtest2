/**
 * 
 */

#include "stdafx.h"
#include "TransMatrix.h"

// In order to use M_PI.
#define _USE_MATH_DEFINES
#include <math.h>

TransMatrix::TransMatrix()
{
}


TransMatrix::~TransMatrix()
{
}

/**
 * @fn void TransMatrix::SetIdentityMatrix(void)
 * @brief Set identity matrix to the class.
 */
void TransMatrix::SetIdentityMatrix(void)
{
	for (int j = 0; j < 4; j++) {
		for (int i = 0; i < 4; i++) {
			element[j][i] = 0;
		}
	}
	for (int i = 0; i < 4; i++) {
		element[i][i] = 1;
	}
}

/**
 * @fn bool TransMatrix::SetRotation(int axis, double degree)
 * @brief Set rotation angle to the class.
 * @param[in] axis Integer index of axis.
 * @param[in] degree Degree of rotation.
 * @return bool True of axis index is in the range. False otherwise.
 */
bool TransMatrix::SetRotation(int axis, double degree)
{
	double angle = M_PI / 180. * degree;
	bool result = true;

	switch (axis) {
	case 0:
		// Rotate around X axis. YZ plane.
		element[0][0] = 1;
		element[0][1] = 0;
		element[0][2] = 0;
		element[1][0] = 0;
		element[1][1] = cos(angle);
		element[1][2] = -sin(angle);
		element[2][0] = 0;
		element[2][1] = sin(angle);
		element[2][2] = cos(angle);
		break;
	case 1:
		// Rotate around Y axis. XZ plane.
		element[0][0] = cos(angle);
		element[0][1] = 0;
		element[0][2] = -sin(angle);
		element[1][0] = 0;
		element[1][1] = 1;
		element[1][2] = 0;
		element[2][0] = sin(angle);
		element[2][1] = 0;
		element[2][2] = cos(angle);
		break;
	case 2:
		// Rotate around Z axis. XY plane.
		element[0][0] = cos(angle);
		element[0][1] = -sin(angle);
		element[0][2] = 0;
		element[1][0] = sin(angle);
		element[1][1] = cos(angle);
		element[1][2] = 0;
		element[2][0] = 0;
		element[2][1] = 0;
		element[2][2] = 1;
		break;
	default:
		result = false;
		break;
	}
	return result;
}

/**
 * @fn bool TransMatrix::SetOffset(double *offsetVector)
 * @brief Set traverse offset to the class.
 * @param[in] offsetVector Vector of traverse offset.
 */
bool TransMatrix::SetOffset(double *offsetVector)
{
	for (int i = 0; i < 3; i++) {
		element[i][0] = offsetVector[i];
	}
	return true;
}


/**
* @fn int TransMatrix::Product(TransMatrix* src)
* @brief Calculate product of matrix.
* @param[in] src TransferMatrix class as multiplier.
* @details Result overwrite on the (this) class.
*/
int TransMatrix::Product(TransMatrix* src)
{
	TransMatrix temp;

	// calculate product.
	for (int j = 0; j < 4; j++) {
		for (int i = 0; i < 4; i++) {
			temp.element[j][i] = 0;
			for (int k = 0; k < 4; k++) {
				temp.element[j][i] += this->element[j][k] * src->element[k][i];
			}
		}
	}
	// copy array.
	for (int j = 0; j < 4; j++) {
		for (int i = 0; i < 4; i++) {
			this->element[j][i] = temp.element[j][i];
		}
	}
	return 0;
}


/**
 * @fn int TransMatrix::TransferCoordinate(double * src)
 * @brief Transfer coordinate array with rotation matrix.
 * @param[in] src Coordinate array.
 * @param[out] src Overwritten with transfer result.
 * @detail Transfer coordinate array. This function is called from timer message handler.
 */
int TransMatrix::TransferCoordinate(double * src)
{
	double temp[4];

	// Calculate product.
	for (int i = 0; i < 4; i++) {
		temp[i] =
			src[0] * this->element[i][0] +
			src[1] * this->element[i][1] +
			src[2] * this->element[i][2] +
			src[3] * this->element[i][3];
	}
	// Copy coordinate.
	for (int i = 0; i < 4; i++) {
		src[i] = temp[i];
	}
	return 0;
}
