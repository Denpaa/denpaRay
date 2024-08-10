//  matrix.hpp
//  Contains all the functions related to matrices.
//  Created by 電波

#pragma once

// -----------------------------------------------
// @denpa: All the matrix structures below are assumed to be row-major.
// -----------------------------------------------

#define DEFINE_MATRIX_STRUCTURE(N) \
typedef struct matrix##N##x##N { \
	f32 v[N*N]; \
} matrix##N##x##N;

DEFINE_MATRIX_STRUCTURE(2);
DEFINE_MATRIX_STRUCTURE(3);
DEFINE_MATRIX_STRUCTURE(4);

STATIC_ASSERT(sizeof(matrix2x2) == 2 * 2 * sizeof(f32), "Unexpected padding for matrix2x2.");
STATIC_ASSERT(sizeof(matrix3x3) == 3 * 3 * sizeof(f32), "Unexpected padding for matrix3x3.");
STATIC_ASSERT(sizeof(matrix4x4) == 4 * 4 * sizeof(f32), "Unexpected padding for matrix4x4.");

// -----------------------------------------------
// @denpa: Finds the submatrix of a 4 by 4 matrix.
// -----------------------------------------------
INTERNAL DINLINE matrix3x3 submatrixOf4x4(matrix4x4 a, u32 row, u32 column) {
	if (row == 0) {
		if (column == 0) {
			return matrix3x3 {a.v[5], a.v[6], a.v[7], a.v[9], a.v[10], a.v[11], a.v[13], a.v[14], a.v[15]};
		} else if (column == 1) {
			return matrix3x3 {a.v[4], a.v[6], a.v[7], a.v[8], a.v[10], a.v[11], a.v[12], a.v[14], a.v[15]};
		} else if (column == 2) {
			return matrix3x3 {a.v[4], a.v[5], a.v[7], a.v[8], a.v[9], a.v[11], a.v[12], a.v[13], a.v[15]};
		} else if (column == 3) {
			return matrix3x3 {a.v[4], a.v[5], a.v[6], a.v[8], a.v[9], a.v[10], a.v[12], a.v[13], a.v[14]};
		}
	} else if (row == 1) {
		if (column == 0) {
			return matrix3x3 {a.v[1], a.v[2], a.v[3], a.v[9], a.v[10], a.v[11], a.v[13], a.v[14], a.v[15]};
		} else if (column == 1) {
			return matrix3x3 {a.v[0], a.v[2], a.v[3], a.v[8], a.v[10], a.v[11], a.v[12], a.v[14], a.v[15]};
		} else if (column == 2) {
			return matrix3x3 {a.v[0], a.v[1], a.v[3], a.v[8], a.v[9], a.v[11], a.v[12], a.v[13], a.v[15]};
		} else if (column == 3) {
			return matrix3x3 {a.v[0], a.v[1], a.v[2], a.v[8], a.v[9], a.v[10], a.v[12], a.v[13], a.v[14]};
		}
	} else if (row == 2) {
		if (column == 0) {
			return matrix3x3 {a.v[1], a.v[2], a.v[3], a.v[5], a.v[6], a.v[7], a.v[13], a.v[14], a.v[15]};
		} else if (column == 1) {
			return matrix3x3 {a.v[0], a.v[2], a.v[3], a.v[4], a.v[6], a.v[7], a.v[12], a.v[14], a.v[15]};
		} else if (column == 2) {
			return matrix3x3 {a.v[0], a.v[1], a.v[3], a.v[4], a.v[5], a.v[7], a.v[12], a.v[13], a.v[15]};
		} else if (column == 3) {
			return matrix3x3 {a.v[0], a.v[1], a.v[2], a.v[4], a.v[5], a.v[6], a.v[12], a.v[13], a.v[14]};
		}
	} else if (row == 3) {
		if (column == 0) {
			return matrix3x3 {a.v[1], a.v[2], a.v[3], a.v[5], a.v[6], a.v[7], a.v[9], a.v[10], a.v[11]};
		} else if (column == 1) {
			return matrix3x3 {a.v[0], a.v[2], a.v[3], a.v[4], a.v[6], a.v[7], a.v[8], a.v[10], a.v[11]};
		} else if (column == 2) {
			return matrix3x3 {a.v[0], a.v[1], a.v[3], a.v[4], a.v[5], a.v[7], a.v[8], a.v[9], a.v[11]};
		} else if (column == 3) {
			return matrix3x3 {a.v[0], a.v[1], a.v[2], a.v[4], a.v[5], a.v[6], a.v[8], a.v[9], a.v[10]};
		}
	}
	return {};
}

// -----------------------------------------------
// @denpa: Finds the submatrix of a 3 by 3 matrix.
// -----------------------------------------------
INTERNAL DINLINE matrix2x2 submatrixOf3x3(matrix3x3 a, u32 row, u32 column) {
	if (row == 0) {
		if (column == 0) {
			return matrix2x2 {a.v[4], a.v[5], a.v[7], a.v[8]};
		} else if (column == 1) {
			return matrix2x2 {a.v[3], a.v[5], a.v[6], a.v[8]};
		} else if (column == 2) {
			return matrix2x2 {a.v[3], a.v[4], a.v[6], a.v[7]};
		}
	} else if (row == 1) {
		if (column == 0) {
			return matrix2x2 {a.v[1], a.v[2], a.v[7], a.v[8]};
		} else if (column == 1) {
			return matrix2x2 {a.v[0], a.v[2], a.v[6], a.v[8]};
		} else if (column == 2) {
			return matrix2x2 {a.v[0], a.v[1], a.v[6], a.v[7]};
		}
	} else if (row == 2) {
		if (column == 0) {
			return matrix2x2 {a.v[1], a.v[2], a.v[4], a.v[5]};
		} else if (column == 1) {
			return matrix2x2 {a.v[0], a.v[2], a.v[3], a.v[5]};
		} else if (column == 2) {
			return matrix2x2 {a.v[0], a.v[1], a.v[3], a.v[4]};
		}
	}
	return {};
}

// -----------------------------------------------
// @denpa: Finds the determinant of a 2 by 2 matrix.
// -----------------------------------------------
INTERNAL DINLINE f32 determinant2x2(matrix2x2 a) {
	return (a.v[0]*a.v[3]) - (a.v[1]*a.v[2]);
}

// -----------------------------------------------
// @denpa: Finds the minor of a 3 by 3 matrix.
// -----------------------------------------------
INTERNAL DINLINE f32 minorOf3x3(matrix3x3 a, u32 row, u32 column) {
	return determinant2x2(submatrixOf3x3(a, row, column));
}

// -----------------------------------------------
// @denpa: Finds the cofactor of a 3 by 3 matrix.
// -----------------------------------------------
INTERNAL DINLINE f32 cofactorOf3x3(matrix3x3 a, u32 row, u32 column) {
	if ((row + column) % 2 == 0) {return minorOf3x3(a, row, column);}
	else {return -(minorOf3x3(a, row, column));}
}

// -----------------------------------------------
// @denpa: Finds the determinant of a 3 by 3 matrix.
// -----------------------------------------------
INTERNAL DINLINE f32 determinant3x3(matrix3x3 a) {
	return (a.v[0] * cofactorOf3x3(a, 0, 0)) + (a.v[1] * cofactorOf3x3(a, 0, 1)) + (a.v[2] * cofactorOf3x3(a, 0, 2));
}

// -----------------------------------------------
// @denpa: Finds the minor of a 4 by 4 matrix.
// -----------------------------------------------
INTERNAL DINLINE f32 minorOf4x4(matrix4x4 a, u32 row, u32 column) {
	return determinant3x3(submatrixOf4x4(a, row, column));
}

// -----------------------------------------------
// @denpa: Finds the cofactor of a 4 by 4 matrix.
// -----------------------------------------------
INTERNAL DINLINE f32 cofactorOf4x4(matrix4x4 a, u32 row, u32 column) {
	if ((row + column) % 2 == 0) {return minorOf4x4(a, row, column);}
	else {return -(minorOf4x4(a, row, column));}
}

// -----------------------------------------------
// @denpa: Finds the determinant of a 4 by 4 matrix.
// -----------------------------------------------
INTERNAL DINLINE f32 determinant4x4(matrix4x4 a) {
	return (a.v[0] * cofactorOf4x4(a, 0, 0)) + (a.v[1] * cofactorOf4x4(a, 0, 1)) + (a.v[2] * cofactorOf4x4(a, 0, 2) + (a.v[3]) * cofactorOf4x4(a, 0, 3));
}


// -----------------------------------------------
// @denpa: Checks if two 4 by 4 matrices are equal to each other.
// -----------------------------------------------
INTERNAL DINLINE bool are4x4MatricesEqual(matrix4x4 a, matrix4x4 b) {
	for (u8 i = 0; i < 16; i++) {
		if (!areFloatsEqual(a.v[i], b.v[i])) {return false;}
	}
	return true;
}

// -----------------------------------------------
// @denpa: Translates an objected by the specified coordinates.
// -----------------------------------------------
INTERNAL DINLINE matrix4x4 createTranslationMatrix(f32 x, f32 y, f32 z) {
	return matrix4x4 {1.f, 0.f, 0.f, x,
					0.f, 1.f, 0.f, y,
					0.f, 0.f, 1.f, z,
					0.f, 0.f, 0.f, 1.f};
}

// -----------------------------------------------
// @denpa: Scales an object by the provided scale factors.
// -----------------------------------------------
INTERNAL DINLINE matrix4x4 createScaleMatrix(f32 x, f32 y, f32 z) {
	return matrix4x4 {x, 0.f, 0.f, 0.f,
					0.f, y, 0.f, 0.f,
					0.f, 0.f, z, 0.f,
					0.f, 0.f, 0.f, 1.f};
}

// -----------------------------------------------
// @denpa: Rotates an object around the X axis. (Euler angle)
// -----------------------------------------------
INTERNAL DINLINE matrix4x4 createRotationMatrixXAxis(f32 radian) {
	return matrix4x4 {1.f, 0.f, 0.f, 0.f,
					0.f, cosf(radian), -sinf(radian), 0.f,
					0.f, sinf(radian), cosf(radian), 0.f,
					0.f, 0.f, 0.f, 1.f};
}

// -----------------------------------------------
// @denpa: Rotates an object around the Y axis. (Euler angle)
// -----------------------------------------------
INTERNAL DINLINE matrix4x4 createRotationMatrixYAxis(f32 radian) {
	return matrix4x4 {cosf(radian), 0.f, sinf(radian), 0.f,
					0.f, 1.f, 0.f, 0.f,
					-sinf(radian), 0.f, cosf(radian), 0.f,
					0.f, 0.f, 0.f, 1.f};
}

// -----------------------------------------------
// @denpa: Rotates an object around the Z axis. (Euler angle)
// -----------------------------------------------
INTERNAL DINLINE matrix4x4 createRotationMatrixZAxis(f32 radian) {
	return matrix4x4 {cosf(radian), -sinf(radian), 0.f, 0.f,
					sinf(radian), cosf(radian), 0.f, 0.f,
					0.f, 0.f, 1.f, 0.f,
					0.f, 0.f, 0.f, 1.f};
}

// -----------------------------------------------
// @denpa: Stretches out an object using a shear matrix.
// -----------------------------------------------
INTERNAL DINLINE matrix4x4 createShearMatrix(f32 xy, f32 xz, f32 yx, f32 yz, f32 zx, f32 zy) {
	return matrix4x4 {1.f, xy, xz, 0.f,
					yx, 1, yz, 0.f,
					zx, zy, 1.f, 0.f,
					0.f, 0.f, 0.f, 1.f};
}

// -----------------------------------------------
// @denpa: Transposes the matrix provided.
// A transposed matrix has its rows and columns switched.
// -----------------------------------------------
INTERNAL DINLINE matrix4x4 transposeMatrix4x4(matrix4x4 a) {
	return matrix4x4 {a.v[0], a.v[4], a.v[8], a.v[12],
					a.v[1], a.v[5], a.v[9], a.v[13],
					a.v[2], a.v[6], a.v[10], a.v[14],
					a.v[3], a.v[7], a.v[11], a.v[15]};
}

// -----------------------------------------------
// @denpa: Finds the inverse of a 4x4 matrix.
// j is used as the row in cofactorOf4x4 in order to transpose the matrix.
// -----------------------------------------------
INTERNAL DINLINE matrix4x4 inverseMatrix4x4(matrix4x4 a) {
	f32 determinant = determinant4x4(a);
	
	if (areFloatsEqual(determinant, 0.f)) {return matrix4x4 {};}
	
	matrix4x4 result;
	for (u32 i = 0, j = 0; i < 16; i+= 4, j++) {
		result.v[i] = cofactorOf4x4(a, j, 0) / determinant;
		result.v[i+1] = cofactorOf4x4(a, j, 1) / determinant;
		result.v[i+2] = cofactorOf4x4(a, j, 2) / determinant;
		result.v[i+3] = cofactorOf4x4(a, j, 3) / determinant;
	}
	return transposeMatrix4x4(result);
}

// -----------------------------------------------
// @denpa: Multiplies two 4 by 4 matrices.
// -----------------------------------------------
INTERNAL DINLINE matrix4x4 multiplyMatrices4x4(matrix4x4 a, matrix4x4 b) {
	return matrix4x4 {(a.v[0]*b.v[0]) + (a.v[1]*b.v[4]) + (a.v[2]*b.v[8]) + (a.v[3]*b.v[12]),
					(a.v[0]*b.v[1]) + (a.v[1]*b.v[5]) + (a.v[2]*b.v[9]) + (a.v[3]*b.v[13]),
					(a.v[0]*b.v[2]) + (a.v[1]*b.v[6]) + (a.v[2]*b.v[10]) + (a.v[3]*b.v[14]),
					(a.v[0]*b.v[3]) + (a.v[1]*b.v[7]) + (a.v[2]*b.v[11]) + (a.v[3]*b.v[15]),
		
					(a.v[4]*b.v[0]) + (a.v[5]*b.v[4]) + (a.v[6]*b.v[8]) + (a.v[7]*b.v[12]),
					(a.v[4]*b.v[1]) + (a.v[5]*b.v[5]) + (a.v[6]*b.v[9]) + (a.v[7]*b.v[13]),
					(a.v[4]*b.v[2]) + (a.v[5]*b.v[6]) + (a.v[6]*b.v[10]) + (a.v[7]*b.v[14]),
					(a.v[4]*b.v[3]) + (a.v[5]*b.v[7]) + (a.v[6]*b.v[11]) + (a.v[7]*b.v[15]),
		
					(a.v[8]*b.v[0]) + (a.v[9]*b.v[4]) + (a.v[10]*b.v[8]) + (a.v[11]*b.v[12]),
					(a.v[8]*b.v[1]) + (a.v[9]*b.v[5]) + (a.v[10]*b.v[9]) + (a.v[11]*b.v[13]),
					(a.v[8]*b.v[2]) + (a.v[9]*b.v[6]) + (a.v[10]*b.v[10]) + (a.v[11]*b.v[14]),
					(a.v[8]*b.v[3]) + (a.v[9]*b.v[7]) + (a.v[10]*b.v[11]) + (a.v[11]*b.v[15]),
		
					(a.v[12]*b.v[0]) + (a.v[13]*b.v[4]) + (a.v[14]*b.v[8]) + (a.v[15]*b.v[12]),
					(a.v[12]*b.v[1]) + (a.v[13]*b.v[5]) + (a.v[14]*b.v[9]) + (a.v[15]*b.v[13]),
					(a.v[12]*b.v[2]) + (a.v[13]*b.v[6]) + (a.v[14]*b.v[10]) + (a.v[15]*b.v[14]),
					(a.v[12]*b.v[3]) + (a.v[13]*b.v[7]) + (a.v[14]*b.v[11]) + (a.v[15]*b.v[15])};
}

// -----------------------------------------------
// @denpa: Returns a 4 by 4 identity matrix.
// -----------------------------------------------
INTERNAL DINLINE matrix4x4 identityMatrix4x4(void) {
	return matrix4x4 {1.f, 0.f, 0.f, 0.f,
					0.f, 1.f, 0.f, 0.f,
					0.f, 0.f, 1.f, 0.f,
					0.f, 0.f, 0.f, 1.f};
}

// -----------------------------------------------
// @denpa: Multiplies one 4 by 4 matrix with a 4 by 1 tuple.
// -----------------------------------------------
INTERNAL DINLINE tuple multiplyMatrix4x4Tuple(matrix4x4 a, tuple b) {
	return tuple {.x = (a.v[0]*b.x) + (a.v[1]*b.y) + (a.v[2]*b.z) + (a.v[3]*b.w),
				.y = (a.v[4]*b.x) + (a.v[5]*b.y) + (a.v[6]*b.z) + (a.v[7]*b.w),
				.z = (a.v[8]*b.x) + (a.v[9]*b.y) + (a.v[10]*b.z) + (a.v[11]*b.w),
				.w = (a.v[12]*b.x) + (a.v[13]*b.y) + (a.v[14]*b.z) + (a.v[15]*b.w)};
}
