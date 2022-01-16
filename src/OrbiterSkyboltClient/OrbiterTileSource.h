#pragma once

#include <SkyboltVis/Renderable/Planet/Tile/TileSource/TileSource.h>

class OrbiterTileSource : public skybolt::vis::TileSource
{
public:
	OrbiterTileSource(const std::string& directory);
	~OrbiterTileSource() override;

	osg::ref_ptr<osg::Image> createImage(const skybolt::QuadTreeTileKey& key, std::function<bool()> cancelSupplier) const;

private:
	std::unique_ptr<class ZTreeMgr> mTreeMgr;
};