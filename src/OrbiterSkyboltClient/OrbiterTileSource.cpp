#include "OrbiterTileSource.h"
#include "ThirdParty/ztreemgr.h"

#include <osgDB/Registry>

OrbiterTileSource::OrbiterTileSource(const std::string& directory)
{
	mTreeMgr = std::make_unique<ZTreeMgr>(directory.c_str(), ZTreeMgr::LAYER_SURF);
}

OrbiterTileSource::~OrbiterTileSource()
{
}

// From https://stackoverflow.com/questions/7781898/get-an-istream-from-a-char
struct Membuf : std::streambuf
{
	Membuf(char* begin, char* end)
	{
		setg(begin, begin, end);
	}
};

osg::ref_ptr<osg::Image> OrbiterTileSource::createImage(const skybolt::QuadTreeTileKey& key, std::function<bool()> cancelSupplier) const
{
	BYTE *buf;
	DWORD ndata = mTreeMgr->ReadData(key.level + 4, key.y, key.x, &buf);

	char* charBuf = reinterpret_cast<char*>(buf);
	Membuf membuf(charBuf, charBuf + ndata);
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