#pragma once

#include <maths/Matrix44.h>
#include <maths/quaternion.h>
#include "Utils.h"

/************************************************************************/
/*                         FORWARD DECLARATIONS                         */
/************************************************************************/
class SceneApp;

/************************************************************************/
/*                          Base Object Class                           */
/************************************************************************/
class WorldObject
{

public:

	/// <summary>
	/// Default constructor
	/// </summary>
	WorldObject();

	/// <summary>
	/// This function initializes the world object, should only be called by the spawn object scene function.
	/// </summary>
	/// <param name="scene">The owninig scene of the object</param>
	/// <param name="owner">The owning object of this object</param>
	void Init(SceneApp* scene, WorldObject* owner);

	/// <summary>
	/// Override this to Initialize variables or do any necessary work when the object is created
	/// </summary>
	virtual void PostInit();

	/// <summary>
	/// Update function, called every frame
	/// </summary>
	/// <param name="deltaTime">Float variable for the frame time</param>
	virtual void Update(float deltaTime);

	/// <summary>
	/// Renders the object
	/// </summary>
	virtual void Render();
	
protected:

	gef::Matrix44 transformMat;
	WorldObject* owningObject;
	SceneApp* owningScene;

	gef::Quaternion rotationVec;
	
public:

	/************************************************************************/
	/*                              SETTERS                                 */
	/************************************************************************/
	virtual void SetTranslation(const gef::Vector4& inTranslation);
	virtual void SetRotation(const gef::Quaternion& inRotation);
	virtual void SetRotation(const float inRotation);
	virtual void SetScale(const gef::Vector4& inScale);


	/************************************************************************/
	/*                              GETTERS                                 */
	/************************************************************************/
	__forceinline const virtual gef::Vector4& GetTranslation() const { return transformMat.GetTranslation(); }
	__forceinline const virtual gef::Quaternion& GetRotation() const { return rotationVec; };
	__forceinline const virtual float GetRotationF() const { return 0.f; }
	__forceinline const virtual gef::Vector4& GetScale() const { return transformMat.GetScale(); }
	__forceinline const gef::Matrix44& GetTransform() const { return transformMat; }
	__forceinline const SceneApp* GetScene() const { return owningScene; }
	__forceinline WorldObject* GetOwner() const { return owningObject; }
};

