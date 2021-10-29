#pragma once
class TransMatrix
{
public:
	TransMatrix();
	~TransMatrix();

	double element[4][4];
	void SetIdentityMatrix();
	bool SetRotation(int axis, double angle);
	bool SetOffset(double *offsetVector);

	int Product(TransMatrix* src);
	int TransferCoordinate(double * src);
};

