#pragma once

class Vec2 {
public:
	Vec2 operator+(Vec2 vec) {
		vec.x += x;
		vec.y += y;
		return vec;
	}
	Vec2 operator-(Vec2 vec) {
		vec.x -= x;
		vec.y -= y;
		return vec;
	}

	union {
		struct {
			float x;
			float y;
		};
		float components[2];
	};
};

class Vec3 {
public:
	Vec3 operator+(Vec3 vec) {
		vec.x += x;
		vec.y += y;
		vec.z += z;
		return vec;
	}
	Vec3 operator-(Vec3 vec) {
		vec.x -= x;
		vec.y -= y;
		vec.z -= z;
		return vec;
	}
	union {
		struct {
			float x;
			float y;
			float z;
		};
		float components[3];
	};
};

class Vec4 {
public:
	Vec4 operator+(Vec4 vec) {
		vec.x += x;
		vec.y += y;
		vec.z += z;
		vec.w += w;
		return vec;
	}
	Vec4 operator-(Vec4 vec) {
		vec.x -= x;
		vec.y -= y;
		vec.z -= z;
		vec.w -= w;
		return vec;
	}
	union {
		struct {
			float x;
			float y;
			float z;
			float w;
		};
		float components[4];
	};
};

class Mat2x2 {
	public:
		Mat2x2() = default;
		Mat2x2(float n) {
			v0.x = n;
			v1.y = n;
		}
		Vec2 operator*(Vec2& vec) {
			Vec2 result;
			result.x = (v0.x * vec.x) + (v0.y * vec.y);
			result.y = (v1.x * vec.x) + (v1.y * vec.y);

			return result;
		}

	union {
		struct {
			Vec2 v0;
			Vec2 v1;
		};
		float vectors[2];
	};
};

class Mat3x3 {
	public:
		Mat3x3() = default;
		Mat3x3(float n) {
			v0.x = n;
			v1.y = n;
			v2.z = n;
		}
		Vec3 operator*(Vec3& vec) {
			Vec3 result;
			result.x = (v0.x * vec.x) + (v0.y * vec.y) + (v0.z * vec.z);
			result.y = (v1.x * vec.x) + (v1.y * vec.y) + (v1.z * vec.z);
			result.z = (v2.x * vec.x) + (v2.y * vec.y) + (v2.z * vec.z);

			return result;
		}

	union {
		struct {
			Vec4 v0;
			Vec4 v1;
			Vec4 v2;
		};
		float vectors[3];
	};
};

class Mat4x4 {
	public:
		Mat4x4() = default;
		Mat4x4(float n) {
			v0.x = n;
			v1.y = n;
			v2.z = n;
			v3.w = n;
		}
		Vec4 operator*(Vec4& vec) {
			Vec4 result;
			result.x = (v0.x * vec.x) + (v0.y * vec.y) + (v0.z * vec.z) + (v0.w * vec.w);
			result.y = (v1.x * vec.x) + (v1.y * vec.y) + (v1.z * vec.z) + (v1.w * vec.w);
			result.z = (v2.x * vec.x) + (v2.y * vec.y) + (v2.z * vec.z) + (v2.w * vec.w);
			result.w = (v3.x * vec.x) + (v3.y * vec.y) + (v3.z * vec.z) + (v3.w * vec.w);

			return result;
		}

	union {
		struct {
			Vec4 v0;
			Vec4 v1;
			Vec4 v2;
			Vec4 v3;
		};
		float vectors[4];
	};
};
