#ifndef COLOR4F_H
#define COLOR4F_H

class Color4f {
public:
	union {
		struct {
			float R;
			float G;
			float B;
			float A;
		};

		float Values[4];
	};

	Color4f() {
		Set(0.0f, 0.0f, 0.0f, 1.0f);
	}

	Color4f(float r, float g, float b, float a) {
		Set(r, g, b, a);
	}

	void Set(float r, float g, float b, float a) {
		this->R = r;
		this->G = g;
		this->B = b;
		this->A = a;
	}
};

#endif

