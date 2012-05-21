/*
 * OpenGLES_Utility.h
 *
 *  Created on: 17.05.2012
 *      Author: yoshi252
 */

#ifndef OPENGLES_UTILITY_H_
#define OPENGLES_UTILITY_H_

#include "gl_includes.h"
#include "math.h"

typedef Eigen::Matrix<GLfloat, 4,4> ProjectionMatrix;

/** \brief Creates a perspective projection matrix
 *
 *	See gluPerspective() for details.
 */
inline ProjectionMatrix perspectiveProjection(GLfloat fovy, GLfloat aspect, GLfloat zNear, GLfloat zFar) {
	Eigen::Matrix<GLfloat, 4, 4> proj;
	proj = ProjectionMatrix::Zero();

	float f = 1.0 / tan(fovy / 2.0);
	proj(0,0) = f/aspect;
	proj(1,1) = f;
	proj(2,2) = (zFar + zNear) / (zNear - zFar);
	proj(2,3) = (2 * zFar * zNear) / (zNear - zFar);
	proj(3,2) = -1;

	return proj;
}

inline ProjectionMatrix lookAtMatrix(GLfloat eyex, GLfloat eyey, GLfloat eyez,
		                             GLfloat centerx, GLfloat centery, GLfloat centerz,
		                             GLfloat upx, GLfloat upy, GLfloat upz) {
	ProjectionMatrix mat;
	mat = ProjectionMatrix::Zero();
	Eigen::Vector3f f(centerx - eyex, centery - eyey, centerz - eyez);
	Eigen::Vector3f up(upx, upy, upz);
	f = f / f.norm();
	up = up / up.norm();
	Eigen::Vector3f s = f.cross(up);
	Eigen::Vector3f u = s.cross(f);

	mat(0,0) =  s(0); mat(0,1) =  s(1); mat(0,2) =  s(2);
	mat(1,0) =  u(0); mat(1,1) =  u(1); mat(1,2) =  u(2);
	mat(2,0) = -f(0); mat(2,1) = -f(1);	mat(2,2) = -f(2);
	mat(3,3) = 1;

	return mat;
}

inline ProjectionMatrix translateMatrix(GLfloat x, GLfloat y, GLfloat z) {
	ProjectionMatrix trans;
	trans << 1, 0, 0, x,
		     0, 1, 0, y,
			 0, 0, 1, z,
			 0, 0, 0, 1;
	return trans;
}

inline ProjectionMatrix rotateMatrix(GLfloat rotationAngle, GLfloat vecx, GLfloat vecy, GLfloat vecz) {
	Eigen::AngleAxis<float> aa(rotationAngle, Vector3f(vecx, vecy, vecz));
	ProjectionMatrix m;
	m = ProjectionMatrix::Zero();
	m.block<3,3>(0,0) = aa.matrix();
	return m;
}

inline ProjectionMatrix orthogonalProjection(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat nearVal, GLfloat farVal) {
	ProjectionMatrix ortho;
	ortho = ProjectionMatrix::Zero();
	GLfloat tx = -(right + left)/(right-left);
	GLfloat ty = -(top+bottom)/(top-bottom);
	GLfloat tz = -(farVal+nearVal)/(farVal-nearVal);

	ortho(0,0) = 2/(right - left);
	ortho(1,1) = 2/(top - bottom);
	ortho(2,2) = -2/(farVal - nearVal);
	ortho(3,3) = 1;
	ortho(0,3) = tx;
	ortho(1,3) = ty;
	ortho(2,3) = tz;

	return ortho;
}

#endif /* OPENGLES_UTILITY_H_ */
