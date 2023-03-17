#pragma once

#include "GameFramework/WorldObject.h"

#include "graphics/mesh.h"


class MeshActor : public WorldObject
{
	GENERATED_BODY(WorldObject, MeshActor)

public:

	/// <summary>
	/// Defaults constructor
	/// </summary>
	MeshActor();

	/// <summary>
	/// Constructor which assigns the mesh
	/// </summary>
	/// <param name="inMesh">Mesh to be assigned</param>
	MeshActor(gef::Mesh* inMesh);

	virtual void Render() override;

protected:

	std::string meshName;
	gef::Material material;
	bool bOverrideMaterial;

private:

	gef::Mesh* mesh;

public:

	/************************************************************************/
	/*                              SETTERS                                 */
	/************************************************************************/
	void SetMesh(gef::Mesh* inMesh) { mesh = inMesh; }
	void SetMaterial(gef::Material newMaterial) {
		material = newMaterial; bOverrideMaterial = true;
	}


	/************************************************************************/
	/*                              GETTERS                                 */
	/************************************************************************/
	__forceinline const gef::Mesh* GetMesh() const { return mesh; }
};

