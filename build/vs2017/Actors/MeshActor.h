#pragma once

#include "GameFramework/WorldObject.h"

#include "graphics/mesh.h"
#include "Interfaces/Damagable.h"

class ActorComponent;

/// <summary>
/// An object that has a mesh
/// </summary>
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

	std::vector<ActorComponent*> components;
	gef::Mesh* mesh;

public:

	/************************************************************************/
	/*                              SETTERS                                 */
	/************************************************************************/
	void SetMesh(gef::Mesh* inMesh) { mesh = inMesh; }
	void SetMeshName(std::string newName);

	void SetMaterial(gef::Material newMaterial) 
	{
		material = newMaterial; 
		bOverrideMaterial = true;
	}
	virtual void SetRotation(const gef::Vector4& inRotation) override;


	/************************************************************************/
	/*                              GETTERS                                 */
	/************************************************************************/
	__forceinline const gef::Mesh* GetMesh() const { return mesh; }
	__forceinline const std::vector<ActorComponent*>& GetComponents() { return components; }
	__forceinline const std::string& GetMeshName() { return meshName; }

	/************************************************************************/
	/*                             TEMPLATES                                */
	/************************************************************************/

protected:

	/// <summary>
	/// Creates and return the specified component, if no component is specified it will create the base class of ActorComponent
	/// </summary>
	template<typename ComponentType = ActorComponent>
	ComponentType* CreateComponent()
	{
		ComponentType* component = new ComponentType();
		component->Init(this);
		components.push_back(component);

		return component;
	}


public:

	/// <summary>
	/// Gets the first component of the specified type from the Actor
	/// </summary>
	template<typename ComponentType = ActorComponent>
	ComponentType* GetComponent()
	{
		for (const auto component : components)
		{
			if (ComponentType* foundComponent = reinterpret_cast<ComponentType>(component))
			{
				return foundComponent;
			}
		}
	}

	/// <summary>
	/// Gets all the components of the specified type from the Actor
	/// </summary>
	template<typename ComponentType = ActorComponent>
	std::vector<ComponentType*>& GetComponents()
	{
		std::vector<ComponentType*> componentsFound;
		for (const auto component : components)
		{
			if (ComponentType* foundComponent = reinterpret_cast<ComponentType>(component))
			{
				componentsFound.push_back(foundComponent);
			}
		}

		return componentsFound;
	}
};

