namespace SupraHot
{
	namespace Math
	{
		#ifndef M_PI
			#define M_PI 3.14159265358979323846f
		#endif

		static const float PI = M_PI;
		static const float TWO_PI = 2.0f * PI;
		static const float HALF_PI = 0.5f * PI;
		static const float ONEFOURTH_PI = 0.25f * PI;
		static const float ONEOVER_PI = 1.0f / PI;
		static const float ONEOVER_TWOPI = 1.0f / TWO_PI;
		static const float DEGREES_TO_RADIANS = PI / 180.0f;
		static const float RADIANS_TO_DEGREES = 180.0f / PI;
		static const float ONEOVER_180_PI = ONEOVER_PI * 180.0f;

		#define ToRadians(x) (float)(((x) * DEGREES_TO_RADIANS))
		#define ToDegrees(x) (float)(((x) * RADIANS_TO_DEGREES))
	};
};