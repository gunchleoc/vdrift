/************************************************************************/
/*                                                                      */
/* This file is part of VDrift.                                         */
/*                                                                      */
/* VDrift is free software: you can redistribute it and/or modify       */
/* it under the terms of the GNU General Public License as published by */
/* the Free Software Foundation, either version 3 of the License, or    */
/* (at your option) any later version.                                  */
/*                                                                      */
/* VDrift is distributed in the hope that it will be useful,            */
/* but WITHOUT ANY WARRANTY; without even the implied warranty of       */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the        */
/* GNU General Public License for more details.                         */
/*                                                                      */
/* You should have received a copy of the GNU General Public License    */
/* along with VDrift.  If not, see <http://www.gnu.org/licenses/>.      */
/*                                                                      */
/************************************************************************/

#include "roadpatch.h"
#include "graphics/scenenode.h"

bool ROADPATCH::Collide(
	const MATHVECTOR <float, 3> & origin,
	const MATHVECTOR <float, 3> & direction,
	float seglen, MATHVECTOR <float, 3> & outtri,
	MATHVECTOR <float, 3> & normal) const
{
	bool col = patch.CollideSubDivQuadSimpleNorm(origin, direction, outtri, normal);
	float len = (outtri - origin).Magnitude();
	return col && len <= seglen;
}

void ROADPATCH::AddRacinglineScenenode(
	SCENENODE & node,
	const ROADPATCH & nextpatch,
	std::tr1::shared_ptr<TEXTURE> racingline_texture)
{
	//Create racing line scenenode
	keyed_container <DRAWABLE>::handle drawhandle = node.GetDrawlist().normal_blend.insert(DRAWABLE());
	DRAWABLE & draw = node.GetDrawlist().normal_blend.get(drawhandle);

	draw.SetDiffuseMap(racingline_texture);
	draw.SetDecal(true);
	draw.SetVertArray(&racingline_vertexarray);

	const MATHVECTOR <float, 3> & r0 = patch.GetRacingLine();
	const MATHVECTOR <float, 3> & r1 = nextpatch.patch.GetRacingLine();
	MATHVECTOR <float, 3> v0 = r0 + (patch.GetPoint(0,0) - r0).Normalize()*0.1;
	MATHVECTOR <float, 3> v1 = r0 + (patch.GetPoint(0,3) - r0).Normalize()*0.1;
	MATHVECTOR <float, 3> v2 = r1 + (nextpatch.GetPatch().GetPoint(0,3) - r1).Normalize()*0.1;
	MATHVECTOR <float, 3> v3 = r1 + (nextpatch.GetPatch().GetPoint(0,0) - r1).Normalize()*0.1;

	float trackoffset = 0.1;
	v0[2] += trackoffset;
	v1[2] += trackoffset;
	v2[2] += trackoffset;
	v3[2] += trackoffset;

	float vcorners[12];
	float uvs[8];
	int bfaces[6];

	vcorners[0] = v0[0]; vcorners[1] = v0[1]; vcorners[2] = v0[2];
	vcorners[3] = v1[0]; vcorners[4] = v1[1]; vcorners[5] = v1[2];
	vcorners[6] = v2[0]; vcorners[7] = v2[1]; vcorners[8] = v2[2];
	vcorners[9] = v3[0]; vcorners[10] = v3[1]; vcorners[11] = v3[2];

	uvs[0] = 0;
	uvs[1] = 0;
	uvs[2] = 1;
	uvs[3] = 0;
	uvs[4] = 1;
	uvs[5] = (v2-v1).Magnitude();
	uvs[6] = 0;
	uvs[7] = (v2-v1).Magnitude();

	bfaces[0] = 0;
	bfaces[1] = 2;
	bfaces[2] = 1;
	bfaces[3] = 0;
	bfaces[4] = 3;
	bfaces[5] = 2;

	racingline_vertexarray.SetFaces(bfaces, 6);
	racingline_vertexarray.SetVertices(vcorners, 12);
	racingline_vertexarray.SetTexCoordSets(1);
	racingline_vertexarray.SetTexCoords(0, uvs, 8);
}
