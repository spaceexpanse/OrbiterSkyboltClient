#pragma once

#include <SkyboltVis/Renderable/Planet/Tile/TileSource/TileSource.h>

class OrbiterImageTileSource : public skybolt::vis::TileSource
{
public:
	OrbiterImageTileSource(const std::string& directory);
	~OrbiterImageTileSource() override;

	osg::ref_ptr<osg::Image> createImage(const skybolt::QuadTreeTileKey& key, std::function<bool()> cancelSupplier) const;

private:
	std::unique_ptr<class ZTreeMgr> mTreeMgr;
	mutable std::mutex mTreeMgrMutex;
};