//********************************** Banshee Engine (www.banshee3d.com) **************************************************//
//**************** Copyright (c) 2016 Marko Pintera (marko.pintera@gmail.com). All rights reserved. **********************//
#include "BsFPhysXJoint.h"
#include "BsPhysXRigidbody.h"
#include "PxRigidDynamic.h"

using namespace physx;

namespace BansheeEngine
{
	PxJointActorIndex::Enum toJointActor(JointBody body)
	{
		if (body == JointBody::A)
			return PxJointActorIndex::eACTOR0;

		return PxJointActorIndex::eACTOR1;
	}

	FPhysXJoint::FPhysXJoint(physx::PxJoint* joint)
		:mJoint(joint)
	{
		
	}

	FPhysXJoint::~FPhysXJoint()
	{
		mJoint->release();
	}

	Rigidbody* FPhysXJoint::getBody(JointBody body) const
	{
		PxRigidActor* actorA = nullptr;
		PxRigidActor* actorB = nullptr;

		mJoint->getActors(actorA, actorB);

		PxRigidActor* wantedActor = body == JointBody::A ? actorA : actorB;
		if (wantedActor == nullptr)
			return nullptr;

		return (Rigidbody*)wantedActor->userData;
	}

	void FPhysXJoint::setBody(JointBody body, Rigidbody* value)
	{
		PxRigidActor* actorA = nullptr;
		PxRigidActor* actorB = nullptr;

		mJoint->getActors(actorA, actorB);

		PxRigidActor* actor = nullptr;
		if (value != nullptr)
			actor = static_cast<PhysXRigidbody*>(value)->_getInternal();

		if (body == JointBody::A)
			actorA = actor;
		else
			actorB = actor;

		mJoint->setActors(actorA, actorB);
	}

	Vector3 FPhysXJoint::getPosition(JointBody body) const
	{
		PxVec3 position = mJoint->getLocalPose(toJointActor(body)).p;

		return fromPxVector(position);
	}

	Quaternion FPhysXJoint::getRotation(JointBody body) const
	{
		PxQuat rotation = mJoint->getLocalPose(toJointActor(body)).q;

		return fromPxQuaternion(rotation);
	}

	void FPhysXJoint::setTransform(JointBody body, const Vector3& position, const Quaternion& rotation)
	{
		PxTransform transform = toPxTransform(position, rotation);

		mJoint->setLocalPose(toJointActor(body), transform);
	}

	float FPhysXJoint::getBreakForce() const
	{
		float force = 0.0f;
		float torque = 0.0f;

		mJoint->getBreakForce(force, torque);
		return force;
	}

	void FPhysXJoint::setBreakForce(float force)
	{
		float dummy = 0.0f;
		float torque = 0.0f;

		mJoint->getBreakForce(dummy, torque);
		mJoint->setBreakForce(force, torque);
	}

	float FPhysXJoint::getBreakTorque() const
	{
		float force = 0.0f;
		float torque = 0.0f;

		mJoint->getBreakForce(force, torque);
		return torque;
	}

	void FPhysXJoint::setBreakToque(float torque)
	{
		float force = 0.0f;
		float dummy = 0.0f;

		mJoint->getBreakForce(force, dummy);
		mJoint->setBreakForce(force, torque);
	}
}