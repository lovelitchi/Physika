/*
 * @file  object_bvh_node.cpp
 * @node of a collidable object's BVH
 * @author Tianxiang Zhang
 * 
 * This file is part of Physika, a versatile physics simulation library.
 * Copyright (C) 2013 Physika Group.
 *
 * This Source Code Form is subject to the terms of the GNU General Public License v2.0. 
 * If a copy of the GPL was not distributed with this file, you can obtain one at:
 * http://www.gnu.org/licenses/gpl-2.0.html
 *
 */

#include "Physika_Geometry/Bounding_Volume/bvh_node_base.h"
#include "Physika_Geometry/Bounding_Volume/object_bvh_node.h"
#include "Physika_Geometry/Bounding_Volume/bounding_volume_kdop18.h"
#include "Physika_Core/Vectors/vector_3d.h"

namespace Physika{

template <typename Scalar,int Dim>
ObjectBVHNode<Scalar, Dim>::ObjectBVHNode():
	object_type_(CollidableObject<Scalar, Dim>::MESH_BASED),
	object_(NULL),
	face_index_(0),
	has_face_(false)
{
}

template <typename Scalar,int Dim>
ObjectBVHNode<Scalar, Dim>::~ObjectBVHNode()
{
}

template <typename Scalar,int Dim>
bool ObjectBVHNode<Scalar, Dim>::isSceneNode() const
{
	return false;
}

template <typename Scalar,int Dim>
bool ObjectBVHNode<Scalar, Dim>::isObjectNode() const
{
	return true;
}

template <typename Scalar,int Dim>
typename CollidableObject<Scalar, Dim>::ObjectType ObjectBVHNode<Scalar, Dim>::getObjectType() const
{
	return object_type_;
}

template <typename Scalar,int Dim>
void ObjectBVHNode<Scalar, Dim>::setObject(CollidableObject<Scalar, Dim>* object)
{
	object_ = object;
	object_type_ = getObjectType();
}

template <typename Scalar,int Dim>
const CollidableObject<Scalar, Dim>* ObjectBVHNode<Scalar, Dim>::getObject() const
{
	return object_;
}

template <typename Scalar,int Dim>
void ObjectBVHNode<Scalar, Dim>::setFaceIndex(unsigned int face_index)
{
	face_index_ = face_index;
	has_face_ = true;
	object_type_ = CollidableObject<Scalar, Dim>::MESH_BASED;
	buildFromFace();
}

template <typename Scalar,int Dim>
unsigned int ObjectBVHNode<Scalar, Dim>::getFaceIndex() const
{
	return face_index_;
}

template <typename Scalar,int Dim>
void ObjectBVHNode<Scalar, Dim>::resize()
{
	if(object_type_ == CollidableObject<Scalar, Dim>::MESH_BASED)
		buildFromFace();
}

template <typename Scalar,int Dim>
bool ObjectBVHNode<Scalar, Dim>::elemTest(const BVHNodeBase<Scalar, Dim>* const target)
{
	//TO DO
	if(target == NULL || !target->isObjectNode())
		return false;
	if(target->getBVType() != this->bv_type_)
		return false;
	//if()
	return true;
}

template <typename Scalar,int Dim>
void ObjectBVHNode<Scalar, Dim>::buildFromFace()
{
	this->is_leaf_ = true;
	if(!has_face_)
		return;
	if(this->bounding_volume_ == NULL)
	{
		switch(this->bv_type_)
		{
			case BoundingVolume<Scalar, Dim>::KDOP18: this->bounding_volume_ = new BoundingVolumeKDOP18<Scalar, Dim>();
			default: this->bounding_volume_ = new BoundingVolumeKDOP18<Scalar, Dim>();
		}
	}
	this->bounding_volume_->setEmpty();
	//unsigned int point_num = face_->numVertices();
	//for(unsigned int i = 0; i < point_num; ++i)
	//{
	//	this->bounding_volume_->unionWith(face_->vertex(i).positionIndex());
	//}
}

template class ObjectBVHNode<float, 3>;
template class ObjectBVHNode<double, 3>;

}