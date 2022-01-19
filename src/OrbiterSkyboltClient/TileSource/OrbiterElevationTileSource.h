#pragma once

#include <SkyboltVis/Renderable/Planet/Tile/TileSource/TileSource.h>

class OrbiterElevationTileSource : public skybolt::vis::TileSource
{
public:
	OrbiterElevationTileSource(const std::string& directory);
	~OrbiterElevationTileSource() override;

	osg::ref_ptr<osg::Image> createImage(const skybolt::QuadTreeTileKey& key, std::function<bool()> cancelSupplier) const;

private:
	std::unique_ptr<class ZTreeMgr> mTreeMgr;
};