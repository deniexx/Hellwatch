#pragma once

#include "GameFramework/WorldObject.h"

#include "graphics/mesh.h"
#include "Interfaces/Damagable.h"


class MeshActor : public WorldObject, public IDamagable
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

	virtual void Update(float deltaTime) override;

	virtual void TakeDamage(float damageAmount) {};

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

	virtual void SetRotation(const gef::Vector4& inRotation) override;


	/************************************************************************/
	/*                              GETTERS                                 */
	/************************************************************************/
	__forceinline const gef::Mesh* GetMesh() const { return mesh; }
};

