#include "OrbiterElevationTileSource.h"
#include "OrbiterSkyboltClient/ThirdParty/ztreemgr.h"
#include <SkyboltVis/Renderable/Planet/Tile/HeightMap.h>

#include <osgDB/Registry>

using namespace skybolt;

OrbiterElevationTileSource::OrbiterElevationTileSource(const std::string& directory)
{
	mTreeMgr = std::make_unique<ZTreeMgr>(directory.c_str(), ZTreeMgr::LAYER_ELEV);
}

OrbiterElevationTileSource::~OrbiterElevationTileSource() = default;

#pragma pack(push,1)
//! From Orbiter developer documentation 'PlanetTextures.odt'
struct ELEVFILEHEADER { // file header for patch elevation data file
	char id[4];            // ID string + version ('E','L','E',1)
	int hdrsize;           // header size (100 expected)
	int dtype;             // data format (0=flat, no data block;
						   // 8=uint8; -8=int8; 16=uint16; -16=int16)
	int xgrd, ygrd;         // data grid size (259 x 259 expected)
	int xpad, ypad;         // horizontal, vertical padding width
						   // (1, 1 expected)
	double scale;          // data scaling factor (1.0 expected)
	double offset;         // data offset (elevation =
						   // raw value * scale + offset)
	double latmin, latmax; // latitude range [rad]
	double lngmin, lngmax; // longitude range [rad]
	double emin, emax, emean; // min, max, mean elevation [m]
};
#pragma pack(pop)

osg::ref_ptr<osg::Image> OrbiterElevationTileSource::createImage(const skybolt::QuadTreeTileKey& key, std::function<bool()> cancelSupplier) const
{
	BYTE *buf;

	std::scoped_lock<std::mutex> lock(mTreeMgrMutex); // MTODO: support concurrent calls to ReadData
	DWORD ndata = mTreeMgr->ReadData(key.level + 4, key.y, key.x, &buf);

	if (ndata < sizeof(ELEVFILEHEADER))
	{
		return nullptr;
	}

	const ELEVFILEHEADER& header = reinterpret_cast<const ELEVFILEHEADER&>(*buf);

	osg::ref_ptr<osg::Image> image = new osg::Image();
	image->allocateImage(256, 256, 1, GL_LUMINANCE, GL_UNSIGNED_SHORT);
	image->setInternalTextureFormat(GL_R16);
	uint16_t* ptr = (uint16_t*)image->getDataPointer();

	constexpr int expectedWidth = 259;
	constexpr int expectedHeight = 259;
	if (header.xgrd != expectedWidth && header.ygrd != expectedHeight)
	{
		return nullptr;
	}

	if (header.dtype == 8) // uint8
	{
		if ((int)ndata > header.hdrsize + expectedWidth * expectedHeight)
		{
			return nullptr;
		}

		std::uint8_t* source = buf + header.hdrsize;
		for (int y = 1; y < 257; ++y)
		{
			for (int x = 1; x < 257; ++x)
			{
				*ptr++ = vis::getHeightmapSeaLevelValueInt() + source[y * 259 + x] + header.offset;
			}
		}
	}
	else if (header.dtype == -16) // int16
	{
		if ((int)ndata > header.hdrsize + expectedWidth * expectedHeight * sizeof(std::int16_t))
		{
			return nullptr;
		}

		std::int16_t* source = reinterpret_cast<std::int16_t*>(buf + header.hdrsize);
		for (int y = 1; y < 257; ++y)
		{
			for (int x = 1; x < 257; ++x)
			{
				*ptr++ = vis::getHeightmapSeaLevelValueInt() + source[y * 259 + x] + header.offset;
			}
		}
	}
	else // if (header.dtype == 0) // flat
	{
		for (int i = 0; i < 256 * 256; ++i)
		{
			*ptr++ = vis::getHeightmapSeaLevelValueInt() + header.emean;
		}
	}

	// MTODO: scoped exit
	mTreeMgr->ReleaseData(buf);

	return image;
}