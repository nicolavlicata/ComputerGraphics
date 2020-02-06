#ifndef MATRIX4F_UTIL_H
#define MATRIX4F_UTIL_H

// For purposes of equality, two floating point values within Vector4f are equivalent
// if their difference is less than this value.
#define Vector4f_EPSILON 0.00001f

// compare 2 floating point numbers for equality.
bool fcomp(float f1, float f2, float epsilon) {
	return fabs(f1 - f2) < epsilon;
}

bool fcomp(float f1, float f2) {
	return fcomp(f1, f2, Vector4f_EPSILON);
}

#endif
