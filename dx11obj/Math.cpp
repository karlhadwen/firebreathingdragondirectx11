#include "stdafx.h"
#include "Math.h"

#define SMALL_NUM  0.00000001 // anything that avoids division overflow

int Triangle::IntersectRay(Vector3f& v0, Vector3f& v1, Vector3f& v2, Ray3f& ray, Vector3f& I) {
	Vector3f u, v, n; // triangle vectors
	Vector3f w0, w; // ray vectors
	float r, a, b; // params to calc ray-plane intersect

	// get triangle edge vectors and plane normal
	u = v1 - v0;
	v = v2 - v0;

	n = u.Cross(v); // cross product
	if ((n.x == 0) && (n.y == 0) && (n.z == 0)) return -1;

	w0 = ray.m_Point - v0;

	a = -n.Dot(w0);
	b = n.Dot(ray.m_Direction);

	if (fabs(b) < SMALL_NUM) {     // ray is parallel to triangle plane
		if (a == 0)                // ray lies in triangle plane
			return 2;
		else return 0;             // ray disjoint from plane
	}

	// get intersect point of ray with triangle plane
	r = a / b; // split the two to get the middle point
	if (r < 0.0) {        
		// ray goes away from triangle
		return 0;          
	} // => no intersect
	// for a segment, also test if (r > 1.0) => no intersect

	Vector3f tr, tz;
	tr = ray.m_Direction * r;
	tz = ray.m_Point + tr;
	I.x = tz.x;
	I.y = tz.y;
	I.z = tz.z;

	// is I inside T?
	float uu, uv, vv, wu, wv, D;
	uu = u.Dot(u);
	uv = u.Dot(v);
	vv = v.Dot(v);
	w.x = I.x - v0.x;
	w.y = I.y - v0.y;
	w.z = I.z - v0.z;
	wu = w.Dot(u);
	wv = w.Dot(v);
	D = uv * uv - uu * vv;

	// get and test parametric coords
	float s, t;
	s = (uv * wv - vv * wu) / D;

	if (s < 0.0 || s > 1.0)        // I is outside T
		return 0;
	t = (uv * wu - uu * wv) / D;

	if (t < 0.0 || (s + t) > 1.0)  // I is outside T
		return 0;

	return 1;                      // I is in T
}

int Sphere::IntersectRay(Ray3f& ray, Vector3f& intersection) {
	float ZERO_TOLERANCE = 1e-06f;

	Vector3f diff = ray.m_Point - m_Position;
	float a0 = diff.Dot(diff) - m_Radius*m_Radius;
	float a1, discr, root;
	float mRayParameter[2];

	if (a0 <= (float)0) {
		// P is inside the sphere
		a1 = ray.m_Direction.Dot(diff);
		discr = a1*a1 - a0;
		root = sqrt(discr);
		mRayParameter[0] = -a1 + root;
		intersection = ray.m_Point + (ray.m_Direction * mRayParameter[0]);
		return 1;
	}
	// else: P is outside the sphere

	a1 = ray.m_Direction.Dot(diff);
	if (a1 >= (float)0.0) {
		return 0;
	}

	discr = a1*a1 - a0;
	if (discr < (float)0.0) {
		return 0;
	} else if (discr >= ZERO_TOLERANCE) {
		root = sqrt(discr);
		mRayParameter[0] = -a1 - root;
		//mRayParameter[1] = -a1 + root;
		intersection = ray.m_Point + (ray.m_Direction * mRayParameter[0]);
		//mPoint[1] = ray.m_Point + mRayParameter[1] * ray.m_Direction;
		return 1;
	} else {
		mRayParameter[0] = -a1;
		intersection = ray.m_Point + (ray.m_Direction * mRayParameter[0]);
		return 1;
	}

	return 0;
}
