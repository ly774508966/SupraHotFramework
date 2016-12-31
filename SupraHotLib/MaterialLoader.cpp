#include "MaterialLoader.h"
#include "FileReader.h"
#include "StringUtil.h"

namespace SupraHot
{
	namespace Utils
	{
		MaterialLoader* MaterialLoader::GetInstance()
		{
			static MaterialLoader* instance(new MaterialLoader);
			return instance;
		}

		MaterialLoader::MaterialLoader()
		{
		}

		MaterialLoader::~MaterialLoader()
		{
		}
	};
};