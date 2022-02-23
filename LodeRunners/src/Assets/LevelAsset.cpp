#include "LevelAsset.h"

LevelAsset::LevelAsset()
{
	for (size_t i = 0; i < m_Data.size(); i++)
	{
		m_Data[i] = MakeRef<Sprite>();
	}
}
