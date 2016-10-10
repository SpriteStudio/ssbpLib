/**
*  SS5PlayerTypes.h
*/

#ifndef SS5PlayerTypes_h
#define SS5PlayerTypes_h

#include <float.h>
#include <math.h>
#include <assert.h>

namespace ss
{
class SSSize;
class SSPoint;
class DataArrayReader;


#ifdef _DEBUG
	#define SSLOG(...)       do {} while (0)
	#define SS_ASSERT(cond)    assert(cond)
	#define SS_ASSERT2(cond, msg) SS_ASSERT(cond)
	#define SSLOGERROR(format,...)  do {} while (0)
#else
	#define SSLOG(...)       do {} while (0)
	#define SS_ASSERT(cond)
	#define SS_ASSERT2(cond, msg) ((void)(cond))
	#define SSLOGERROR(format,...)  do {} while (0)
#endif

/**
* 座標クラス
*/
class SSPoint
{
public:
	float x;
	float y;

public:
	SSPoint(void) : x(0), y(0)
	{
	}

	SSPoint(float x, float y) : x(x), y(y)
	{
	}

	SSPoint(const SSPoint& other) : x(other.x), y(other.y)
	{
	}
/*
	SSPoint(const SSSize& size) : x(size.width), y(size.height)
	{
	}
*/
	SSPoint& operator= (const SSPoint& other)
	{
		setPoint(other.x, other.y);
		return *this;
	}
/*
	SSPoint& operator= (const SSSize& size)
	{
		setPoint(size.width, size.height);
		return *this;
	}
*/
	SSPoint operator+(const SSPoint& right) const
	{
		return SSPoint(this->x + right.x, this->y + right.y);
	}

	SSPoint operator-(const SSPoint& right) const
	{
		return SSPoint(this->x - right.x, this->y - right.y);
	}

	SSPoint operator-() const
	{
		return SSPoint(-x, -y);
	}

	SSPoint operator*(float a) const
	{
		return SSPoint(this->x * a, this->y * a);
	}

	SSPoint operator/(float a) const
	{
		SS_ASSERT2(a, "SSPoint division by 0.");
		return SSPoint(this->x / a, this->y / a);
	}

	void setPoint(float x, float y)
	{
		this->x = x;
		this->y = y;
	}

	bool equals(const SSPoint& target) const
	{
		return (fabs(this->x - target.x) < FLT_EPSILON)
			&& (fabs(this->y - target.y) < FLT_EPSILON);
	}

	bool fuzzyEquals(const SSPoint& b, float var) const
	{
		if (x - var <= b.x && b.x <= x + var)
			if (y - var <= b.y && b.y <= y + var)
				return true;
		return false;
	}
/*
	float getAngle(const SSPoint& other) const
	{
		SSPoint a2 = normalize();
		SSPoint b2 = other.normalize();
		float angle = atan2f(a2.cross(b2), a2.dot(b2));
		if (fabs(angle) < FLT_EPSILON) return 0.f;
		return angle;
	}

	SSPoint rotateByAngle(const SSPoint& pivot, float angle) const
	{
		return pivot + (*this - pivot).rotate(SSPoint::forAngle(angle));
	}
*/

	inline float getLength() const {
		return sqrtf(x*x + y*y);
	};
	inline float getLengthSq() const {
		return dot(*this); //x*x + y*y;
	};
	inline float getDistanceSq(const SSPoint& other) const {
		return (*this - other).getLengthSq();
	};
	inline float getDistance(const SSPoint& other) const {
		return (*this - other).getLength();
	};
	inline float getAngle() const {
		return atan2f(y, x);
	};
	float getAngle(const SSPoint& other) const;
	inline float dot(const SSPoint& other) const {
		return x*other.x + y*other.y;
	};
	inline float cross(const SSPoint& other) const {
		return x*other.y - y*other.x;
	};
	inline SSPoint getPerp() const {
		return SSPoint(-y, x);
	};
	inline SSPoint getRPerp() const {
		return SSPoint(y, -x);
	};
	inline SSPoint project(const SSPoint& other) const {
		return other * (dot(other) / other.dot(other));
	};
	inline SSPoint rotate(const SSPoint& other) const {
		return SSPoint(x*other.x - y*other.y, x*other.y + y*other.x);
	};
	inline SSPoint unrotate(const SSPoint& other) const {
		return SSPoint(x*other.x + y*other.y, y*other.x - x*other.y);
	};
	inline SSPoint normalize() const {
		float length = getLength();
		if (length == 0.) return SSPoint(1.f, 0);
		return *this / getLength();
	};
	inline SSPoint lerp(const SSPoint& other, float alpha) const {
		return *this * (1.f - alpha) + other * alpha;
	};
	SSPoint rotateByAngle(const SSPoint& pivot, float angle) const;

	static inline SSPoint forAngle(const float a)
	{
		return SSPoint(cosf(a), sinf(a));
	}
};

/**
* サイズクラス
*/
class SSSize
{
public:
	float width;
	float height;

public:
	SSSize(void) : width(0), height(0)
	{
	}

	SSSize(float width, float height) : width(width), height(height)
	{
	}

	SSSize(const SSSize& other) : width(other.width), height(other.height)
	{
	}

	SSSize(const SSPoint& point) : width(point.x), height(point.y)
	{
	}

	SSSize& operator= (const SSSize& other)
	{
		setSize(other.width, other.height);
		return *this;
	}

	SSSize& operator= (const SSPoint& point)
	{
		setSize(point.x, point.y);
		return *this;
	}

	SSSize operator+(const SSSize& right) const
	{
		return SSSize(this->width + right.width, this->height + right.height);
	}

	SSSize operator-(const SSSize& right) const
	{
		return SSSize(this->width - right.width, this->height - right.height);
	}

	SSSize operator*(float a) const
	{
		return SSSize(this->width * a, this->height * a);
	}

	SSSize operator/(float a) const
	{
		SS_ASSERT2(a, "SSSize division by 0.");
		return SSSize(this->width / a, this->height / a);
	}

	void setSize(float width, float height)
	{
		this->width = width;
		this->height = height;
	}

	bool equals(const SSSize& target) const
	{
		return (fabs(this->width - target.width)  < FLT_EPSILON)
			&& (fabs(this->height - target.height) < FLT_EPSILON);
	}
};

/**
* 矩形クラス
*/
class SSRect
{
public:
	SSPoint origin;
	SSSize  size;

public:

	SSRect(void)
	{
		setRect(0.0f, 0.0f, 0.0f, 0.0f);
	}

	SSRect(float x, float y, float width, float height)
	{
		setRect(x, y, width, height);
	}

	SSRect(const SSRect& other)
	{
		setRect(other.origin.x, other.origin.y, other.size.width, other.size.height);
	}

	SSRect& operator= (const SSRect& other)
	{
		setRect(other.origin.x, other.origin.y, other.size.width, other.size.height);
		return *this;
	}

	void setRect(float x, float y, float width, float height)
	{
		// CGRect can support width<0 or height<0
		SS_ASSERT2(width >= 0.0f && height >= 0.0f, "width and height of Rect must not less than 0.");

		origin.x = x;
		origin.y = y;

		size.width = width;
		size.height = height;
	}

	bool equals(const SSRect& rect) const
	{
		return (origin.equals(rect.origin) &&
			size.equals(rect.size));
	}

	float getMaxX() const
	{
		return (float)(origin.x + size.width);
	}

	float getMidX() const
	{
		return (float)(origin.x + size.width / 2.0);
	}

	float getMinX() const
	{
		return origin.x;
	}

	float getMaxY() const
	{
		return origin.y + size.height;
	}

	float getMidY() const
	{
		return (float)(origin.y + size.height / 2.0);
	}

	float getMinY() const
	{
		return origin.y;
	}

	bool containsPoint(const SSPoint& point) const
	{
		bool bRet = false;

		if (point.x >= getMinX() && point.x <= getMaxX()
			&& point.y >= getMinY() && point.y <= getMaxY())
		{
			bRet = true;
		}

		return bRet;
	}

	bool intersectsRect(const SSRect& rect) const
	{
		return !(getMaxX() < rect.getMinX() ||
			rect.getMaxX() <      getMinX() ||
			getMaxY() < rect.getMinY() ||
			rect.getMaxY() <      getMinY());
	}

};
	
/**
 * カラー構造体
 */
struct SSColor4B{
	unsigned char r;
	unsigned char g;
	unsigned char b;
	unsigned char a;

	using uchar = unsigned char;
	SSColor4B() :r(0), g(0), b(0), a(0){}
	SSColor4B(uchar r_, uchar g_, uchar b_, uchar a_) :r(r_), g(g_), b(b_), a(a_){}
};


/**
 * 頂点座標構造体
 */
struct SSVertex3F{
	float x;
	float y;
	float z;

	SSVertex3F() :x(0), y(0), z(0){}
	SSVertex3F(float x_, float y_, float z_) : x(x_), y(y_), z(z_){}
	SSVertex3F(const SSVertex3F &o) : x(o.x), y(o.y), z(o.z){}

	SSVertex3F& operator =(const SSVertex3F &o){
		x = o.x;	y = o.y;	z = o.z;
		return *this;
	}
	bool operator ==(const SSVertex3F &o) const{
		return (x == o.x && y == o.y && z == o.z);
	}
	bool operator !=(const SSVertex3F &o) const{
		return !(*this == o);
	}

	const SSVertex3F operator -() const{
		return SSVertex3F(-x, -y, -z);
	}
	const SSVertex3F operator +(const SSVertex3F &o) const{
		return SSVertex3F(x + o.x, y + o.y, z + o.z);
	}
	const SSVertex3F operator -(const SSVertex3F &o) const{
		return SSVertex3F(x - o.x, y - o.y, z - o.z);
	}
	const float operator *(const SSVertex3F &o) const{	//内積
		return x*o.x + y*o.y + z*o.z;
	}
	const SSVertex3F operator *(float s) const{
		return SSVertex3F(x*s, y*s, z*s);
	}
	const SSVertex3F operator /(float s) const{
		assert(s);								//ゼロ除算チェック
		float oneOverS = 1.0f / s;
		return SSVertex3F(x*oneOverS, y*oneOverS, z*oneOverS);
	}

	SSVertex3F& operator +=(const SSVertex3F &o){
		x += o.x;	y += o.y;	z += o.z;
		return *this;
	}
	SSVertex3F &operator -=(const SSVertex3F &o){
		x -= o.x;	y -= o.y;	z -= o.z;
		return *this;
	}
	SSVertex3F &operator *=(float s){
		x *= s;	y *= s;	z *= s;
		return *this;
	}
	SSVertex3F &operator /=(float s){
		assert(s);
		float oneOverS = 1.0f / s;
		x *= oneOverS;	y *= oneOverS;	z *= oneOverS;
		return *this;
	}
};

//! スカラー乗算の対称性のために定義しておく
inline const SSVertex3F operator *(float s, const SSVertex3F &o){
	return SSVertex3F(s*o.x, s*o.y, s*o.z);
}



/**
 * ４頂点座標構造体
 */
struct SSQuad3 {
	SSVertex3F        bl;
	SSVertex3F        br;
	SSVertex3F        tl;
	SSVertex3F        tr;

	//頂点オフセットの読み取り
	void readVertexTransform(DataArrayReader &reader);
};


/**
 * UV構造体
 *  2Dベクトルの演算が使えます
 */
struct SSTex2F {
	float u;
	float v;

	SSTex2F() :u(0), v(0){}
	SSTex2F(float u_, float v_) :u(u_), v(v_){}
	SSTex2F(const SSTex2F& o) : u(o.u), v(o.v) {}

	SSTex2F &operator =(const SSTex2F &o){
		u = o.u;
		v = o.v;
		return *this;
	}
	bool operator ==(const SSTex2F &o) const{
		return (u == o.u && v == o.v);
	}
	bool operator !=(const SSTex2F &o) const{
		return (u != o.u || v != o.v);
	}

	const SSTex2F operator -() const{
		return SSTex2F(-u, -v);
	}
	const SSTex2F operator +(const SSTex2F &o) const{
		return SSTex2F(u + o.u, v + o.v);
	}
	const SSTex2F operator -(const SSTex2F &o) const{
		return SSTex2F(u - o.u, v - o.v);
	}
	const float operator *(const SSTex2F &o) const{	//内積
		return u*o.u + v*o.v;
	}
	const SSTex2F operator *(float s) const{
		return SSTex2F(u*s, v*s);
	}
	const SSTex2F operator /(float s) const{
		assert(s);								//ゼロ除算チェック
		float oneOverS = 1.0f / s;
		return SSTex2F(u*oneOverS, v*oneOverS);
	}

	SSTex2F &operator +=(const SSTex2F &o){
		u += o.u;	v += o.v;
		return *this;
	}
	SSTex2F &operator -=(const SSTex2F &o){
		u -= o.u;	v -= o.v;
		return *this;
	}
	SSTex2F &operator *=(float s){
		u *= s;		v *= s;
		return *this;
	}
	SSTex2F &operator /=(float s){
		assert(s);
		float oneOverS = 1.0f / s;
		u *= oneOverS;	v *= oneOverS;
		return *this;
	}

	//! 回転させる
	void rotate(float angleRadian){
		float tu = u;
		u = tu * cos(angleRadian) - v *sin(angleRadian);
		v = tu * sin(angleRadian) + v * cos(angleRadian);
	}
	//! 基準点を中心として回転させる
	void rotate(float angleRadian, const SSTex2F &offset){
		*this -= offset;
		rotate(angleRadian);
		*this += offset;
	}
};



/**
* 頂点構造体
*/
typedef struct _ccV3F_C4B_T2F
{
	SSVertex3F			vertices;	/// 座標
	SSColor4B			colors;		/// カラー
	SSTex2F				texCoords;	/// UV
} SSV3F_C4B_T2F;

/**
* ４頂点構造体
*/
struct SSV3F_C4B_T2F_Quad{
	SSV3F_C4B_T2F    tl;
	SSV3F_C4B_T2F    bl;
	SSV3F_C4B_T2F    tr;
	SSV3F_C4B_T2F    br;

	//4頂点の加算
	SSV3F_C4B_T2F_Quad& operator +=(const SSQuad3 &o){
		tl.vertices += o.tl;
		bl.vertices += o.bl;
		tr.vertices += o.tr;
		br.vertices += o.br;
		return *this;
	}

	//vertex各要素に演算を適用させる
	template<class F>
	void vertexForeach(F func){
		func(tl.vertices);
		func(tr.vertices);
		func(bl.vertices);
		func(br.vertices);
	}

	//uv各要素に演算を適用させる
	template<class F>
	void uvForeach(F func){
		func(tl.texCoords);
		func(tr.texCoords);
		func(bl.texCoords);
		func(br.texCoords);
	}

	//color各要素に演算を適用させる
	template<class F>
	void colorsForeach(F func){
		func(tl.colors);
		func(tr.colors);
		func(bl.colors);
		func(br.colors);
	}
};

//テクスチャデータ
struct TextuerData
{
	long handle;
	int size_w;
	int size_h;
};


};	// namespace ss

#endif
