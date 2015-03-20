#ifndef MATH_H
#define MATH_H

// just making the 2/3/4f vectors & matrix's a bit more clear and modular by
// holding them in this file
typedef XMVECTOR Vector4f;
typedef XMMATRIX Matrix4x4f;
//typedef XMFLOAT3 Vector3f;
typedef XMFLOAT2 Vector2f;

template <class T>
class Vector3 {
public:
	union {
		struct {
			T x, y, z;
		};
		
		T v[3];
	};

	Vector3(T _x, T _y, T _z) {
		Set(_x, _y, _z);
	}

	Vector3(const Vector3<T>& other) {
		Set(other.x, other.y, other.z);
	}

	Vector3(Vector4f& other) {
		Set(other.m128_f32[0], other.m128_f32[1], other.m128_f32[2]);
	}

	Vector3() {
		SetZero();
	}

	inline void SetZero() {
		Set(0.f, 0.f, 0.f);
	}

	void Set(T _x, T _y, T _z) {
		x = _x;
		y = _y;
		z = _z;
	}

	void Set(XMVECTOR& other) {
		x = other.m128_f32[0];
		y = other.m128_f32[1];
		z = other.m128_f32[2];
	}

	void Set(Vector3<T>& other) {
		x = other.x;
		y = other.y;
		z = other.z;
	}

	float Dot(Vector3<T>& other) {
		return (this->x * other.x) + (this->y * other.y) + (this->z * other.z);
	}

	float Length() {
		return((float)sqrt(x * x + y * y + z * z));
	}

	Vector3<T>& Cross(Vector3<T>& other) {
		Vector3<T> vCrossProduct;

		vCrossProduct.x = this->y * other.z - this->z * other.y;
		vCrossProduct.y = -this->x * other.z + this->z * other.x;
		vCrossProduct.z = this->x * other.y - this->y * other.x;

		return vCrossProduct;
	}

	Vector3<T> operator - (Vector3<T>& other) {
		Vector3<T> res;

		res.x = x - other.x;
		res.y = y - other.y;
		res.z = z - other.z;

		return res;
	}

	Vector3<T> operator + (Vector3<T>& other) {
		Vector3<T> res;

		res.x = x + other.x;
		res.y = y + other.y;
		res.z = z + other.z;

		return res;
	}

	Vector3<T> operator * (float value) {
		Vector3<T> res;

		res.x = x * value;
		res.y = y * value;
		res.z = z * value;

		return res;
	}

	inline Vector3<T>& operator = (Vector3<T>& other) {
		x = other.x;
		y = other.y;
		z = other.z;
		return *this;
	}

	inline Vector3<T>& operator = (XMVECTOR& other) {
		Set(other);
		return *this;
	}
};

typedef Vector3<float> Vector3f;

class Ray3f {
public:
	Vector3f m_Point;
	Vector3f m_Direction;
};

class ICollidable {
public:
	virtual int IntersectRay(Ray3f& ray, Vector3f& intersection) = 0;
};

// Can use either Sphere or Triangle as Ray detection
// Sphere is used to check if fire hits the house
class Sphere : public ICollidable {
public:
	Vector3f m_Position;
	float m_Radius;

	int IntersectRay(Ray3f& ray, Vector3f& intersection);
};

class Triangle {
public:
	Vector3f m_Point0;
	Vector3f m_Point1;
	Vector3f m_Point2;

	int IntersectRay(Ray3f& ray, Vector3f& intersection) {
		return IntersectRay(m_Point0, m_Point1, m_Point2, ray, intersection);
	}

	/**
	 * I will be intersection point
	 * @returns 0 when no intersection, 2 when line is on same plane with triangle, 1 when intersected
	 */
	static int IntersectRay(Vector3f& v0, Vector3f& v1, Vector3f& v2, Ray3f& ray, Vector3f& I);

};

#endif