#pragma once

#include <maths/Matrix44.h>
#include <maths/quaternion.h>
#include "Utils.h"

/************************************************************************/
/*                         FORWARD DECLARATIONS                         */
/************************************************************************/
class b2Body;

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

	~WorldObject();

	/// <summary>
	/// This function initializes the world object, should only be called by the spawn object scene function.
	/// </summary>
	/// <param name="scene">The owninig scene of the object</param>
	/// <param name="owner">The owning object of this object</param>
	void Init(WorldObject* owner = nullptr);

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

	/// <summary>
	/// Function to respond to collision
	/// </summary>
	virtual void OnCollision(b2Body* OtherBody);

	int ID = NO_ID;

protected:

	b2Body* collisionBody = nullptr;
	gef::Matrix44 transformMat;
	WorldObject* owningObject;

	gef::Vector4 translation;
	gef::Vector4 scale;
	gef::Vector4 rotationVec;

	bool bMarkedForDelete = false;
	bool bDisableUpdate = false;

	void BuildTransform();
	
public:

	/************************************************************************/
	/*                              SETTERS                                 */
	/************************************************************************/
	virtual void SetTranslation(const gef::Vector4& inTranslation);
	virtual void SetRotation(const gef::Vector4& inRotation);
	virtual void SetRotation(const float inRotation);
	virtual void SetScale(const gef::Vector4& inScale);
	void SetCollisionBody(b2Body* inCollisionBody);

	/************************************************************************/
	/*                              GETTERS                                 */
	/************************************************************************/
	__forceinline const virtual gef::Vector4& GetTranslation() const { return transformMat.GetTranslation(); }
	__forceinline const virtual gef::Vector4& GetRotation() const { return rotationVec; };
	__forceinline const virtual float GetRotationF() const { return 0.f; }
	__forceinline const virtual gef::Vector4& GetScale() const { return transformMat.GetScale(); }
	__forceinline const gef::Matrix44& GetTransform() const { return transformMat; }
	__forceinline WorldObject* GetOwner() const { return owningObject; }
	__forceinline b2Body* GetCollisionBody() const { return collisionBody; }


	__forceinline void MarkForDelete() { bMarkedForDelete = true; }
	__forceinline void DisableUpdate() { bDisableUpdate = true; }
	__forceinline void EnableUpdate() { bDisableUpdate = false; }

	__forceinline bool ShouldUpdate() { return !bDisableUpdate; }
	__forceinline const bool GetIsMarkedForDelete() const { return bMarkedForDelete; }
};

