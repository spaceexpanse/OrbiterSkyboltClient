/*
Copyright 2021 Matthew Reid

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#pragma once

#include <SkyboltVis/Renderable/Model/Model.h>

#include "OrbiterAPI.h"

#include <osg/Texture2D>

struct OrbiterModelConfig : public skybolt::vis::ModelConfig
{
	OBJHANDLE owningObject;
	int meshId;
	int meshVisibilityCategoryFlags;
	std::vector<int> meshGroupToGeometryIndex; //!< Maps orbiter mesh group ID to osg geometry ID. Index is -1 if the mesh group has no geometry
};

class OrbiterModel : public skybolt::vis::Model
{
public:
	OrbiterModel(const OrbiterModelConfig& config);

	void setMeshTexture(int groupId, const osg::ref_ptr<osg::Texture2D>& texture);
	void useMeshAsMfd(int groupId, const osg::ref_ptr<osg::Program>& program, bool alphaBlend);

	OBJHANDLE getOwningObject() const { return mOwningObject; }
	int getMeshId() const { return mMeshId; }

	//! Gets the orbiter visibility category flags for the mesh. Values can be MESHVIS_NEVER, MESHVIS_EXTERNAL etc.
	int getMeshVisibilityCategoryFlags() const { return mMeshVisibilityCategoryFlags; }

private:
	osg::Drawable* getDrawable(int groupId) const; //!< Returns null if drawable not found for groupId

private:
	OBJHANDLE mOwningObject;
	int mMeshId;
	int mMeshVisibilityCategoryFlags;
	std::vector<int> mMeshGroupToGeometryIndex;
	std::set<int> mMfdGroupIds;
};
