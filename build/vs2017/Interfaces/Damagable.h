#pragma once

/// <summary>
/// Interface to be added, when we want the specific class to be able to take damage
/// </summary>
class IDamagable
{
	virtual void TakeDamage(float damageAmount) = 0;
};