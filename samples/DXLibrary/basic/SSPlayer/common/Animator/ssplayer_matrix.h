#ifndef __SSPLAYER_MATRIX__
#define __SSPLAYER_MATRIX__


#include <array>

namespace ss
{
/**
* 行列計算
*	vec * matrix --> vec の形。vecは横ベクトル
*/
class SSMatrix {
public:
	SSMatrix();

	std::array<float, 16> _m;


	//各種行列に整える
	SSMatrix& setupTranslation(float x, float y, float z = 0);
	SSMatrix& setupRotationX(float radian);
	SSMatrix& setupRotationY(float radian);
	SSMatrix& setupRotationZ(float radian);
	SSMatrix& setupScale(float x, float y, float z = 1);
	SSMatrix& setupIdentity();

	//平行移動だけは簡単な演算で済むので別枠で定義しておく
	void setTranslation(float x, float y, float z = 0);			//平行移動成分をxyzで置き換え
	void zeroTranslation();										//平行移動成分を0移動に置き換え
	void addTranslation(float x, float y, float z = 0);			//平行移動させる
	void getTranslation(float *x, float *y, float *z) const;	//平行移動成分を取り出す
	void getTranslation(float *x, float *y) const;				//平行移動成分を取り出す

																//スケール値の抽出
	void getScale(float *x, float *y, float *z) const;

	//回転の抽出
	void getRotation(float *x, float *y, float *z) const; //ラジアンでかえります
	SSMatrix getRotationMatrix() const;		//回転成分だけ行列として取り出す

	SSMatrix operator*(const SSMatrix &o) const;
	SSMatrix& operator*=(const SSMatrix &o);

};






void	IdentityMatrix( float* matrix );
void    ScaleMatrix( float* _matrix , const float x , const float y , const float z);
void    TranslationMatrix( float* _matrix , const float x , const float y , const float z );
void	MultiplyMatrix(const float *m0, const float *m1, float *matrix);
void    Matrix4RotationX( float* _matrix ,const float radians );
void    Matrix4RotationY( float* _matrix ,const float radians );
void    Matrix4RotationZ( float* _matrix ,const float radians );
void	MatrixCopy(float* src, float* dst);

inline	void	TranslationMatrixM(  float* _matrix , const float x , const float y , const float z )
{
	float	_m[16];
	IdentityMatrix( _m );
	TranslationMatrix( _m , x , y , z );

	MultiplyMatrix( _m , _matrix , _matrix );
}

inline	void	ScaleMatrixM(  float* _matrix , const float x , const float y , const float z )
{

	float	_m[16];
	IdentityMatrix( _m );
	ScaleMatrix( _m , x , y , z );
	MultiplyMatrix( _m , _matrix , _matrix );
}

inline	void	RotationXYZMatrixM(  float* _matrix , const float x , const float y , const float z )
{

	if ( x != 0.0f )
	{
		float	_m[16];
		IdentityMatrix( _m );
		Matrix4RotationX( _m , x );

		MultiplyMatrix( _m , _matrix , _matrix );
	}

	if ( y != 0.0f )
	{
		float	_m[16];
		IdentityMatrix( _m );
		Matrix4RotationY( _m , y );

		MultiplyMatrix( _m , _matrix , _matrix );
	}

	if ( z != 0.0f )
	{
		float	_m[16];
		IdentityMatrix( _m );
		Matrix4RotationZ( _m , z );

		MultiplyMatrix( _m , _matrix , _matrix );
	}
}
inline	void	MatrixCopy(float* src, float* dst)
{
	int i;
	for (i = 0; i < 16; i++)
	{
		dst[i] = src[i];
	}
}


};

#endif

