#include "Vec4.h"
#include "Vec2.h"
#include "Vec3.h"

namespace SupraHot
{
	namespace Math
	{
		Vec4::Vec4()
		{
			this->x = 0;
			this->y = 0;
			this->z = 0;
			this->w = 0;
		}

		Vec4::Vec4(float x, float y, float z, float w)
		{
			this->x = x;
			this->y = y;
			this->z = z;
			this->w = w;
		}

		float Vec4::Dot(const Vec4& v)
		{
			return x * v.x + y * v.y + z * v.z + w * v.w;
		}

		Vec4::~Vec4()
		{
		}

		// --
		// Swizzles

		// vec2 swizzles
		Vec2 Vec4::xx(){ return Vec2(x, x); }
		Vec2 Vec4::yx(){ return Vec2(y, x); }
		Vec2 Vec4::zx(){ return Vec2(z, x); }
		Vec2 Vec4::wx(){ return Vec2(w, x); }
		Vec2 Vec4::xy(){ return Vec2(x, y); }
		Vec2 Vec4::yy(){ return Vec2(y, y); }
		Vec2 Vec4::zy(){ return Vec2(z, y); }
		Vec2 Vec4::wy(){ return Vec2(w, y); }
		Vec2 Vec4::xz(){ return Vec2(x, z); }
		Vec2 Vec4::yz(){ return Vec2(y, z); }
		Vec2 Vec4::zz(){ return Vec2(z, z); }
		Vec2 Vec4::wz(){ return Vec2(w, z); }
		Vec2 Vec4::xw(){ return Vec2(x, w); }
		Vec2 Vec4::yw(){ return Vec2(y, w); }
		Vec2 Vec4::zw(){ return Vec2(z, w); }
		Vec2 Vec4::ww(){ return Vec2(w, w); }

		// vec3 swizzles
		Vec3 Vec4::xxx(){ return Vec3(x, x, x); }
		Vec3 Vec4::yxx(){ return Vec3(y, x, x); }
		Vec3 Vec4::zxx(){ return Vec3(z, x, x); }
		Vec3 Vec4::wxx(){ return Vec3(w, x, x); }
		Vec3 Vec4::xyx(){ return Vec3(x, y, x); }
		Vec3 Vec4::yyx(){ return Vec3(y, y, x); }
		Vec3 Vec4::zyx(){ return Vec3(z, y, x); }
		Vec3 Vec4::wyx(){ return Vec3(w, y, x); }
		Vec3 Vec4::xzx(){ return Vec3(x, z, x); }
		Vec3 Vec4::yzx(){ return Vec3(y, z, x); }
		Vec3 Vec4::zzx(){ return Vec3(z, z, x); }
		Vec3 Vec4::wzx(){ return Vec3(w, z, x); }
		Vec3 Vec4::xwx(){ return Vec3(x, w, x); }
		Vec3 Vec4::ywx(){ return Vec3(y, w, x); }
		Vec3 Vec4::zwx(){ return Vec3(z, w, x); }
		Vec3 Vec4::wwx(){ return Vec3(w, w, x); }
		Vec3 Vec4::xxy(){ return Vec3(x, x, y); }
		Vec3 Vec4::yxy(){ return Vec3(y, x, y); }
		Vec3 Vec4::zxy(){ return Vec3(z, x, y); }
		Vec3 Vec4::wxy(){ return Vec3(w, x, y); }
		Vec3 Vec4::xyy(){ return Vec3(x, y, y); }
		Vec3 Vec4::yyy(){ return Vec3(y, y, y); }
		Vec3 Vec4::zyy(){ return Vec3(z, y, y); }
		Vec3 Vec4::wyy(){ return Vec3(w, y, y); }
		Vec3 Vec4::xzy(){ return Vec3(x, z, y); }
		Vec3 Vec4::yzy(){ return Vec3(y, z, y); }
		Vec3 Vec4::zzy(){ return Vec3(z, z, y); }
		Vec3 Vec4::wzy(){ return Vec3(w, z, y); }
		Vec3 Vec4::xwy(){ return Vec3(x, w, y); }
		Vec3 Vec4::ywy(){ return Vec3(y, w, y); }
		Vec3 Vec4::zwy(){ return Vec3(z, w, y); }
		Vec3 Vec4::wwy(){ return Vec3(w, w, y); }
		Vec3 Vec4::xxz(){ return Vec3(x, x, z); }
		Vec3 Vec4::yxz(){ return Vec3(y, x, z); }
		Vec3 Vec4::zxz(){ return Vec3(z, x, z); }
		Vec3 Vec4::wxz(){ return Vec3(w, x, z); }
		Vec3 Vec4::xyz(){ return Vec3(x, y, z); }
		Vec3 Vec4::yyz(){ return Vec3(y, y, z); }
		Vec3 Vec4::zyz(){ return Vec3(z, y, z); }
		Vec3 Vec4::wyz(){ return Vec3(w, y, z); }
		Vec3 Vec4::xzz(){ return Vec3(x, z, z); }
		Vec3 Vec4::yzz(){ return Vec3(y, z, z); }
		Vec3 Vec4::zzz(){ return Vec3(z, z, z); }
		Vec3 Vec4::wzz(){ return Vec3(w, z, z); }
		Vec3 Vec4::xwz(){ return Vec3(x, w, z); }
		Vec3 Vec4::ywz(){ return Vec3(y, w, z); }
		Vec3 Vec4::zwz(){ return Vec3(z, w, z); }
		Vec3 Vec4::wwz(){ return Vec3(w, w, z); }
		Vec3 Vec4::xxw(){ return Vec3(x, x, w); }
		Vec3 Vec4::yxw(){ return Vec3(y, x, w); }
		Vec3 Vec4::zxw(){ return Vec3(z, x, w); }
		Vec3 Vec4::wxw(){ return Vec3(w, x, w); }
		Vec3 Vec4::xyw(){ return Vec3(x, y, w); }
		Vec3 Vec4::yyw(){ return Vec3(y, y, w); }
		Vec3 Vec4::zyw(){ return Vec3(z, y, w); }
		Vec3 Vec4::wyw(){ return Vec3(w, y, w); }
		Vec3 Vec4::xzw(){ return Vec3(x, z, w); }
		Vec3 Vec4::yzw(){ return Vec3(y, z, w); }
		Vec3 Vec4::zzw(){ return Vec3(z, z, w); }
		Vec3 Vec4::wzw(){ return Vec3(w, z, w); }
		Vec3 Vec4::xww(){ return Vec3(x, w, w); }
		Vec3 Vec4::yww(){ return Vec3(y, w, w); }
		Vec3 Vec4::zww(){ return Vec3(z, w, w); }
		Vec3 Vec4::www(){ return Vec3(w, w, w); }

		// vec4 - swizzles
		Vec4 Vec4::xxxx(){ return Vec4(x, x, x, x); }
		Vec4 Vec4::yxxx(){ return Vec4(y, x, x, x); }
		Vec4 Vec4::zxxx(){ return Vec4(z, x, x, x); }
		Vec4 Vec4::wxxx(){ return Vec4(w, x, x, x); }
		Vec4 Vec4::xyxx(){ return Vec4(x, y, x, x); }
		Vec4 Vec4::yyxx(){ return Vec4(y, y, x, x); }
		Vec4 Vec4::zyxx(){ return Vec4(z, y, x, x); }
		Vec4 Vec4::wyxx(){ return Vec4(w, y, x, x); }
		Vec4 Vec4::xzxx(){ return Vec4(x, z, x, x); }
		Vec4 Vec4::yzxx(){ return Vec4(y, z, x, x); }
		Vec4 Vec4::zzxx(){ return Vec4(z, z, x, x); }
		Vec4 Vec4::wzxx(){ return Vec4(w, z, x, x); }
		Vec4 Vec4::xwxx(){ return Vec4(x, w, x, x); }
		Vec4 Vec4::ywxx(){ return Vec4(y, w, x, x); }
		Vec4 Vec4::zwxx(){ return Vec4(z, w, x, x); }
		Vec4 Vec4::wwxx(){ return Vec4(w, w, x, x); }
		Vec4 Vec4::xxyx(){ return Vec4(x, x, y, x); }
		Vec4 Vec4::yxyx(){ return Vec4(y, x, y, x); }
		Vec4 Vec4::zxyx(){ return Vec4(z, x, y, x); }
		Vec4 Vec4::wxyx(){ return Vec4(w, x, y, x); }
		Vec4 Vec4::xyyx(){ return Vec4(x, y, y, x); }
		Vec4 Vec4::yyyx(){ return Vec4(y, y, y, x); }
		Vec4 Vec4::zyyx(){ return Vec4(z, y, y, x); }
		Vec4 Vec4::wyyx(){ return Vec4(w, y, y, x); }
		Vec4 Vec4::xzyx(){ return Vec4(x, z, y, x); }
		Vec4 Vec4::yzyx(){ return Vec4(y, z, y, x); }
		Vec4 Vec4::zzyx(){ return Vec4(z, z, y, x); }
		Vec4 Vec4::wzyx(){ return Vec4(w, z, y, x); }
		Vec4 Vec4::xwyx(){ return Vec4(x, w, y, x); }
		Vec4 Vec4::ywyx(){ return Vec4(y, w, y, x); }
		Vec4 Vec4::zwyx(){ return Vec4(z, w, y, x); }
		Vec4 Vec4::wwyx(){ return Vec4(w, w, y, x); }
		Vec4 Vec4::xxzx(){ return Vec4(x, x, z, x); }
		Vec4 Vec4::yxzx(){ return Vec4(y, x, z, x); }
		Vec4 Vec4::zxzx(){ return Vec4(z, x, z, x); }
		Vec4 Vec4::wxzx(){ return Vec4(w, x, z, x); }
		Vec4 Vec4::xyzx(){ return Vec4(x, y, z, x); }
		Vec4 Vec4::yyzx(){ return Vec4(y, y, z, x); }
		Vec4 Vec4::zyzx(){ return Vec4(z, y, z, x); }
		Vec4 Vec4::wyzx(){ return Vec4(w, y, z, x); }
		Vec4 Vec4::xzzx(){ return Vec4(x, z, z, x); }
		Vec4 Vec4::yzzx(){ return Vec4(y, z, z, x); }
		Vec4 Vec4::zzzx(){ return Vec4(z, z, z, x); }
		Vec4 Vec4::wzzx(){ return Vec4(w, z, z, x); }
		Vec4 Vec4::xwzx(){ return Vec4(x, w, z, x); }
		Vec4 Vec4::ywzx(){ return Vec4(y, w, z, x); }
		Vec4 Vec4::zwzx(){ return Vec4(z, w, z, x); }
		Vec4 Vec4::wwzx(){ return Vec4(w, w, z, x); }
		Vec4 Vec4::xxwx(){ return Vec4(x, x, w, x); }
		Vec4 Vec4::yxwx(){ return Vec4(y, x, w, x); }
		Vec4 Vec4::zxwx(){ return Vec4(z, x, w, x); }
		Vec4 Vec4::wxwx(){ return Vec4(w, x, w, x); }
		Vec4 Vec4::xywx(){ return Vec4(x, y, w, x); }
		Vec4 Vec4::yywx(){ return Vec4(y, y, w, x); }
		Vec4 Vec4::zywx(){ return Vec4(z, y, w, x); }
		Vec4 Vec4::wywx(){ return Vec4(w, y, w, x); }
		Vec4 Vec4::xzwx(){ return Vec4(x, z, w, x); }
		Vec4 Vec4::yzwx(){ return Vec4(y, z, w, x); }
		Vec4 Vec4::zzwx(){ return Vec4(z, z, w, x); }
		Vec4 Vec4::wzwx(){ return Vec4(w, z, w, x); }
		Vec4 Vec4::xwwx(){ return Vec4(x, w, w, x); }
		Vec4 Vec4::ywwx(){ return Vec4(y, w, w, x); }
		Vec4 Vec4::zwwx(){ return Vec4(z, w, w, x); }
		Vec4 Vec4::wwwx(){ return Vec4(w, w, w, x); }
		Vec4 Vec4::xxxy(){ return Vec4(x, x, x, y); }
		Vec4 Vec4::yxxy(){ return Vec4(y, x, x, y); }
		Vec4 Vec4::zxxy(){ return Vec4(z, x, x, y); }
		Vec4 Vec4::wxxy(){ return Vec4(w, x, x, y); }
		Vec4 Vec4::xyxy(){ return Vec4(x, y, x, y); }
		Vec4 Vec4::yyxy(){ return Vec4(y, y, x, y); }
		Vec4 Vec4::zyxy(){ return Vec4(z, y, x, y); }
		Vec4 Vec4::wyxy(){ return Vec4(w, y, x, y); }
		Vec4 Vec4::xzxy(){ return Vec4(x, z, x, y); }
		Vec4 Vec4::yzxy(){ return Vec4(y, z, x, y); }
		Vec4 Vec4::zzxy(){ return Vec4(z, z, x, y); }
		Vec4 Vec4::wzxy(){ return Vec4(w, z, x, y); }
		Vec4 Vec4::xwxy(){ return Vec4(x, w, x, y); }
		Vec4 Vec4::ywxy(){ return Vec4(y, w, x, y); }
		Vec4 Vec4::zwxy(){ return Vec4(z, w, x, y); }
		Vec4 Vec4::wwxy(){ return Vec4(w, w, x, y); }
		Vec4 Vec4::xxyy(){ return Vec4(x, x, y, y); }
		Vec4 Vec4::yxyy(){ return Vec4(y, x, y, y); }
		Vec4 Vec4::zxyy(){ return Vec4(z, x, y, y); }
		Vec4 Vec4::wxyy(){ return Vec4(w, x, y, y); }
		Vec4 Vec4::xyyy(){ return Vec4(x, y, y, y); }
		Vec4 Vec4::yyyy(){ return Vec4(y, y, y, y); }
		Vec4 Vec4::zyyy(){ return Vec4(z, y, y, y); }
		Vec4 Vec4::wyyy(){ return Vec4(w, y, y, y); }
		Vec4 Vec4::xzyy(){ return Vec4(x, z, y, y); }
		Vec4 Vec4::yzyy(){ return Vec4(y, z, y, y); }
		Vec4 Vec4::zzyy(){ return Vec4(z, z, y, y); }
		Vec4 Vec4::wzyy(){ return Vec4(w, z, y, y); }
		Vec4 Vec4::xwyy(){ return Vec4(x, w, y, y); }
		Vec4 Vec4::ywyy(){ return Vec4(y, w, y, y); }
		Vec4 Vec4::zwyy(){ return Vec4(z, w, y, y); }
		Vec4 Vec4::wwyy(){ return Vec4(w, w, y, y); }
		Vec4 Vec4::xxzy(){ return Vec4(x, x, z, y); }
		Vec4 Vec4::yxzy(){ return Vec4(y, x, z, y); }
		Vec4 Vec4::zxzy(){ return Vec4(z, x, z, y); }
		Vec4 Vec4::wxzy(){ return Vec4(w, x, z, y); }
		Vec4 Vec4::xyzy(){ return Vec4(x, y, z, y); }
		Vec4 Vec4::yyzy(){ return Vec4(y, y, z, y); }
		Vec4 Vec4::zyzy(){ return Vec4(z, y, z, y); }
		Vec4 Vec4::wyzy(){ return Vec4(w, y, z, y); }
		Vec4 Vec4::xzzy(){ return Vec4(x, z, z, y); }
		Vec4 Vec4::yzzy(){ return Vec4(y, z, z, y); }
		Vec4 Vec4::zzzy(){ return Vec4(z, z, z, y); }
		Vec4 Vec4::wzzy(){ return Vec4(w, z, z, y); }
		Vec4 Vec4::xwzy(){ return Vec4(x, w, z, y); }
		Vec4 Vec4::ywzy(){ return Vec4(y, w, z, y); }
		Vec4 Vec4::zwzy(){ return Vec4(z, w, z, y); }
		Vec4 Vec4::wwzy(){ return Vec4(w, w, z, y); }
		Vec4 Vec4::xxwy(){ return Vec4(x, x, w, y); }
		Vec4 Vec4::yxwy(){ return Vec4(y, x, w, y); }
		Vec4 Vec4::zxwy(){ return Vec4(z, x, w, y); }
		Vec4 Vec4::wxwy(){ return Vec4(w, x, w, y); }
		Vec4 Vec4::xywy(){ return Vec4(x, y, w, y); }
		Vec4 Vec4::yywy(){ return Vec4(y, y, w, y); }
		Vec4 Vec4::zywy(){ return Vec4(z, y, w, y); }
		Vec4 Vec4::wywy(){ return Vec4(w, y, w, y); }
		Vec4 Vec4::xzwy(){ return Vec4(x, z, w, y); }
		Vec4 Vec4::yzwy(){ return Vec4(y, z, w, y); }
		Vec4 Vec4::zzwy(){ return Vec4(z, z, w, y); }
		Vec4 Vec4::wzwy(){ return Vec4(w, z, w, y); }
		Vec4 Vec4::xwwy(){ return Vec4(x, w, w, y); }
		Vec4 Vec4::ywwy(){ return Vec4(y, w, w, y); }
		Vec4 Vec4::zwwy(){ return Vec4(z, w, w, y); }
		Vec4 Vec4::wwwy(){ return Vec4(w, w, w, y); }
		Vec4 Vec4::xxxz(){ return Vec4(x, x, x, z); }
		Vec4 Vec4::yxxz(){ return Vec4(y, x, x, z); }
		Vec4 Vec4::zxxz(){ return Vec4(z, x, x, z); }
		Vec4 Vec4::wxxz(){ return Vec4(w, x, x, z); }
		Vec4 Vec4::xyxz(){ return Vec4(x, y, x, z); }
		Vec4 Vec4::yyxz(){ return Vec4(y, y, x, z); }
		Vec4 Vec4::zyxz(){ return Vec4(z, y, x, z); }
		Vec4 Vec4::wyxz(){ return Vec4(w, y, x, z); }
		Vec4 Vec4::xzxz(){ return Vec4(x, z, x, z); }
		Vec4 Vec4::yzxz(){ return Vec4(y, z, x, z); }
		Vec4 Vec4::zzxz(){ return Vec4(z, z, x, z); }
		Vec4 Vec4::wzxz(){ return Vec4(w, z, x, z); }
		Vec4 Vec4::xwxz(){ return Vec4(x, w, x, z); }
		Vec4 Vec4::ywxz(){ return Vec4(y, w, x, z); }
		Vec4 Vec4::zwxz(){ return Vec4(z, w, x, z); }
		Vec4 Vec4::wwxz(){ return Vec4(w, w, x, z); }
		Vec4 Vec4::xxyz(){ return Vec4(x, x, y, z); }
		Vec4 Vec4::yxyz(){ return Vec4(y, x, y, z); }
		Vec4 Vec4::zxyz(){ return Vec4(z, x, y, z); }
		Vec4 Vec4::wxyz(){ return Vec4(w, x, y, z); }
		Vec4 Vec4::xyyz(){ return Vec4(x, y, y, z); }
		Vec4 Vec4::yyyz(){ return Vec4(y, y, y, z); }
		Vec4 Vec4::zyyz(){ return Vec4(z, y, y, z); }
		Vec4 Vec4::wyyz(){ return Vec4(w, y, y, z); }
		Vec4 Vec4::xzyz(){ return Vec4(x, z, y, z); }
		Vec4 Vec4::yzyz(){ return Vec4(y, z, y, z); }
		Vec4 Vec4::zzyz(){ return Vec4(z, z, y, z); }
		Vec4 Vec4::wzyz(){ return Vec4(w, z, y, z); }
		Vec4 Vec4::xwyz(){ return Vec4(x, w, y, z); }
		Vec4 Vec4::ywyz(){ return Vec4(y, w, y, z); }
		Vec4 Vec4::zwyz(){ return Vec4(z, w, y, z); }
		Vec4 Vec4::wwyz(){ return Vec4(w, w, y, z); }
		Vec4 Vec4::xxzz(){ return Vec4(x, x, z, z); }
		Vec4 Vec4::yxzz(){ return Vec4(y, x, z, z); }
		Vec4 Vec4::zxzz(){ return Vec4(z, x, z, z); }
		Vec4 Vec4::wxzz(){ return Vec4(w, x, z, z); }
		Vec4 Vec4::xyzz(){ return Vec4(x, y, z, z); }
		Vec4 Vec4::yyzz(){ return Vec4(y, y, z, z); }
		Vec4 Vec4::zyzz(){ return Vec4(z, y, z, z); }
		Vec4 Vec4::wyzz(){ return Vec4(w, y, z, z); }
		Vec4 Vec4::xzzz(){ return Vec4(x, z, z, z); }
		Vec4 Vec4::yzzz(){ return Vec4(y, z, z, z); }
		Vec4 Vec4::zzzz(){ return Vec4(z, z, z, z); }
		Vec4 Vec4::wzzz(){ return Vec4(w, z, z, z); }
		Vec4 Vec4::xwzz(){ return Vec4(x, w, z, z); }
		Vec4 Vec4::ywzz(){ return Vec4(y, w, z, z); }
		Vec4 Vec4::zwzz(){ return Vec4(z, w, z, z); }
		Vec4 Vec4::wwzz(){ return Vec4(w, w, z, z); }
		Vec4 Vec4::xxwz(){ return Vec4(x, x, w, z); }
		Vec4 Vec4::yxwz(){ return Vec4(y, x, w, z); }
		Vec4 Vec4::zxwz(){ return Vec4(z, x, w, z); }
		Vec4 Vec4::wxwz(){ return Vec4(w, x, w, z); }
		Vec4 Vec4::xywz(){ return Vec4(x, y, w, z); }
		Vec4 Vec4::yywz(){ return Vec4(y, y, w, z); }
		Vec4 Vec4::zywz(){ return Vec4(z, y, w, z); }
		Vec4 Vec4::wywz(){ return Vec4(w, y, w, z); }
		Vec4 Vec4::xzwz(){ return Vec4(x, z, w, z); }
		Vec4 Vec4::yzwz(){ return Vec4(y, z, w, z); }
		Vec4 Vec4::zzwz(){ return Vec4(z, z, w, z); }
		Vec4 Vec4::wzwz(){ return Vec4(w, z, w, z); }
		Vec4 Vec4::xwwz(){ return Vec4(x, w, w, z); }
		Vec4 Vec4::ywwz(){ return Vec4(y, w, w, z); }
		Vec4 Vec4::zwwz(){ return Vec4(z, w, w, z); }
		Vec4 Vec4::wwwz(){ return Vec4(w, w, w, z); }
		Vec4 Vec4::xxxw(){ return Vec4(x, x, x, w); }
		Vec4 Vec4::yxxw(){ return Vec4(y, x, x, w); }
		Vec4 Vec4::zxxw(){ return Vec4(z, x, x, w); }
		Vec4 Vec4::wxxw(){ return Vec4(w, x, x, w); }
		Vec4 Vec4::xyxw(){ return Vec4(x, y, x, w); }
		Vec4 Vec4::yyxw(){ return Vec4(y, y, x, w); }
		Vec4 Vec4::zyxw(){ return Vec4(z, y, x, w); }
		Vec4 Vec4::wyxw(){ return Vec4(w, y, x, w); }
		Vec4 Vec4::xzxw(){ return Vec4(x, z, x, w); }
		Vec4 Vec4::yzxw(){ return Vec4(y, z, x, w); }
		Vec4 Vec4::zzxw(){ return Vec4(z, z, x, w); }
		Vec4 Vec4::wzxw(){ return Vec4(w, z, x, w); }
		Vec4 Vec4::xwxw(){ return Vec4(x, w, x, w); }
		Vec4 Vec4::ywxw(){ return Vec4(y, w, x, w); }
		Vec4 Vec4::zwxw(){ return Vec4(z, w, x, w); }
		Vec4 Vec4::wwxw(){ return Vec4(w, w, x, w); }
		Vec4 Vec4::xxyw(){ return Vec4(x, x, y, w); }
		Vec4 Vec4::yxyw(){ return Vec4(y, x, y, w); }
		Vec4 Vec4::zxyw(){ return Vec4(z, x, y, w); }
		Vec4 Vec4::wxyw(){ return Vec4(w, x, y, w); }
		Vec4 Vec4::xyyw(){ return Vec4(x, y, y, w); }
		Vec4 Vec4::yyyw(){ return Vec4(y, y, y, w); }
		Vec4 Vec4::zyyw(){ return Vec4(z, y, y, w); }
		Vec4 Vec4::wyyw(){ return Vec4(w, y, y, w); }
		Vec4 Vec4::xzyw(){ return Vec4(x, z, y, w); }
		Vec4 Vec4::yzyw(){ return Vec4(y, z, y, w); }
		Vec4 Vec4::zzyw(){ return Vec4(z, z, y, w); }
		Vec4 Vec4::wzyw(){ return Vec4(w, z, y, w); }
		Vec4 Vec4::xwyw(){ return Vec4(x, w, y, w); }
		Vec4 Vec4::ywyw(){ return Vec4(y, w, y, w); }
		Vec4 Vec4::zwyw(){ return Vec4(z, w, y, w); }
		Vec4 Vec4::wwyw(){ return Vec4(w, w, y, w); }
		Vec4 Vec4::xxzw(){ return Vec4(x, x, z, w); }
		Vec4 Vec4::yxzw(){ return Vec4(y, x, z, w); }
		Vec4 Vec4::zxzw(){ return Vec4(z, x, z, w); }
		Vec4 Vec4::wxzw(){ return Vec4(w, x, z, w); }
		Vec4 Vec4::xyzw(){ return Vec4(x, y, z, w); }
		Vec4 Vec4::yyzw(){ return Vec4(y, y, z, w); }
		Vec4 Vec4::zyzw(){ return Vec4(z, y, z, w); }
		Vec4 Vec4::wyzw(){ return Vec4(w, y, z, w); }
		Vec4 Vec4::xzzw(){ return Vec4(x, z, z, w); }
		Vec4 Vec4::yzzw(){ return Vec4(y, z, z, w); }
		Vec4 Vec4::zzzw(){ return Vec4(z, z, z, w); }
		Vec4 Vec4::wzzw(){ return Vec4(w, z, z, w); }
		Vec4 Vec4::xwzw(){ return Vec4(x, w, z, w); }
		Vec4 Vec4::ywzw(){ return Vec4(y, w, z, w); }
		Vec4 Vec4::zwzw(){ return Vec4(z, w, z, w); }
		Vec4 Vec4::wwzw(){ return Vec4(w, w, z, w); }
		Vec4 Vec4::xxww(){ return Vec4(x, x, w, w); }
		Vec4 Vec4::yxww(){ return Vec4(y, x, w, w); }
		Vec4 Vec4::zxww(){ return Vec4(z, x, w, w); }
		Vec4 Vec4::wxww(){ return Vec4(w, x, w, w); }
		Vec4 Vec4::xyww(){ return Vec4(x, y, w, w); }
		Vec4 Vec4::yyww(){ return Vec4(y, y, w, w); }
		Vec4 Vec4::zyww(){ return Vec4(z, y, w, w); }
		Vec4 Vec4::wyww(){ return Vec4(w, y, w, w); }
		Vec4 Vec4::xzww(){ return Vec4(x, z, w, w); }
		Vec4 Vec4::yzww(){ return Vec4(y, z, w, w); }
		Vec4 Vec4::zzww(){ return Vec4(z, z, w, w); }
		Vec4 Vec4::wzww(){ return Vec4(w, z, w, w); }
		Vec4 Vec4::xwww(){ return Vec4(x, w, w, w); }
		Vec4 Vec4::ywww(){ return Vec4(y, w, w, w); }
		Vec4 Vec4::zwww(){ return Vec4(z, w, w, w); }
		Vec4 Vec4::wwww(){ return Vec4(w, w, w, w); }
	};
};