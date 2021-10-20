#include "AttachWeaponToBoneComponent.h"
#include "WeaponBase.h"
#include "SkeletalMeshComponent.h"

AttachWeaponToBoneComponent::AttachWeaponToBoneComponent(WeaponBase* _owner, SkeletalMeshComponent* _skelMesh, unsigned int _boneNum)
	:Component(_owner)
	,m_socketNum(_boneNum)
{
}

AttachWeaponToBoneComponent::~AttachWeaponToBoneComponent()
{
}

void AttachWeaponToBoneComponent::Update(float in_deltaTime)
{

}

void AttachWeaponToBoneComponent::SetBoneNum(unsigned int _boneNum)
{
}
