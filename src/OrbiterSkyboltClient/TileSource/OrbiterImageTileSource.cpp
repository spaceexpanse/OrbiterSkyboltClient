#include "OrbiterImageTileSource.h"
#include "MemoryStreamBuf.h"
#include "OrbiterSkyboltClient/ThirdParty/ztreemgr.h"

#include <osgDB/Registry>

OrbiterImageTileSource::OrbiterImageTileSource(const std::string& directory)
{
	mTreeMgr = std::make_unique<ZTreeMgr>(directory.c_str(), ZTreeMgr::LAYER_SURF);
}

OrbiterImageTileSource::~OrbiterImageTileSource() = default;

osg::ref_ptr<osg::Image> OrbiterImageTileSource::createImage(const skybolt::QuadTreeTileKey& key, std::function<bool()> cancelSupplier) const
{
	BYTE *buf;
	DWORD ndata = mTreeMgr->ReadData(key.level + 4, key.y, key.x, &buf);

	MemoryStreamBuf membuf(reinterpret_cast<char*>(buf), ndata);
	std::istream istream(&membuf);

	osgDB::ReaderWriter *rw = osgDB::Registry::instance()->getReaderWriterForExtension("dds");
	osgDB::ReaderWriter::ReadResult res = rw->readImage(istream);
	osg::ref_ptr<osg::Image> image = res.getImage();

	if (image)
	{
		image->flipVertical();
	}
	return image;
}