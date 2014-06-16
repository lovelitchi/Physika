/*
 * @file  bvh_base.cpp
 * @base class of bounding volume hierarchy (BVH)
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

#include "Physika_Geometry/Bounding_Volume/bvh_base.h"
#include "Physika_Geometry/Bounding_Volume/bvh_node_base.h"
#include "Physika_Geometry/Bounding_Volume/bounding_volume_kdop18.h"
#include "Physika_Core/Vectors/vector_3d.h"
#include <stdio.h>

namespace Physika{

template <typename Scalar,int Dim>
BVHBase<Scalar, Dim>::BVHBase():
	root_node_(NULL),
	bv_type_(BoundingVolume<Scalar, Dim>::KDOP18)
{
}

template <typename Scalar,int Dim>
BVHBase<Scalar, Dim>::~BVHBase()
{
	if(root_node_ != NULL)
		delete root_node_;
}

template <typename Scalar,int Dim>
void BVHBase<Scalar, Dim>::setRootNode(BVHNodeBase<Scalar, Dim>* root_node)
{
	if(root_node_ == root_node)
		return;
	if(root_node_ != NULL)
		delete root_node_;
	root_node_ = root_node;
}

template <typename Scalar,int Dim>
const BVHNodeBase<Scalar, Dim>* const BVHBase<Scalar, Dim>::getRootNode() const
{
	return root_node_;
}

template <typename Scalar,int Dim>
void BVHBase<Scalar, Dim>::setBVType(typename BoundingVolume<Scalar, Dim>::BVType bv_type)
{
	bv_type_ = bv_type;
}

template <typename Scalar,int Dim>
typename BoundingVolume<Scalar, Dim>::BVType BVHBase<Scalar, Dim>::getBVType() const
{
	return bv_type_;
}


template <typename Scalar,int Dim>
void BVHBase<Scalar, Dim>::refit()
{
	root_node_->refit();
}

template <typename Scalar,int Dim>
void BVHBase<Scalar, Dim>::rebuild()
{
	cleanInternalNodes();
	root_node_ = buildFromLeafList(0, (int)leaf_node_list_.size());
}

template <typename Scalar,int Dim>
void BVHBase<Scalar, Dim>::clean()
{
	if(root_node_ != NULL)
	{
		root_node_->clean();
		delete root_node_;
		root_node_ = NULL;
	}
	unsigned int list_size = static_cast<unsigned int>(leaf_node_list_.size());
	for(unsigned int i = 0; i < list_size; ++i)
	{
		delete leaf_node_list_[i];
	}
	leaf_node_list_.clear();
}

template <typename Scalar,int Dim>
void BVHBase<Scalar, Dim>::cleanInternalNodes()
{
	if(root_node_ != NULL)
	{
		root_node_->cleanInternalNodes();
		delete root_node_;
		root_node_ = NULL;	
	}
}

template <typename Scalar,int Dim>
bool BVHBase<Scalar, Dim>::selfCollide()
{
	return root_node_->selfCollide();
}

template <typename Scalar,int Dim>
bool BVHBase<Scalar, Dim>::collide(const BVHBase<Scalar, Dim>* const target)
{
	if(target == NULL)
		return false;
	return root_node_->collide(target->getRootNode());
}

template <typename Scalar,int Dim>
BVHNodeBase<Scalar, Dim>* BVHBase<Scalar, Dim>::buildFromLeafList(const int start_position, const int end_position)
{
	//*****empty leaf node list*****
	if(start_position == 0 && end_position == 0)
		return NULL;

	//*****Leaf node*****
	if(start_position + 1 == end_position)
	{
		return leaf_node_list_[start_position];
	}
	//*****Internal node*****
	//basic construction
	BVHNodeBase<Scalar, Dim>* thisNode = new BVHNodeBase<Scalar, Dim>();
	thisNode->setLeaf(false);
	thisNode->setBVType(bv_type_);
	BoundingVolume<Scalar, Dim>* bounding_volume = NULL;
	switch(bv_type_)
	{
		case BoundingVolume<Scalar, Dim>::KDOP18: bounding_volume = new BoundingVolumeKDOP18<Scalar, Dim>();
		default: bounding_volume = new BoundingVolumeKDOP18<Scalar, Dim>();
	}
	thisNode->setBoundingVolume(bounding_volume);

	//get bounding volume
	for (int i = start_position; i < end_position; ++i)
	{
		bounding_volume->unionWith(leaf_node_list_[i]->getBoundingVolume());
	}

	//partite the BV according to its longest axis
	BVAxisPartition<Scalar, Dim> partition(bounding_volume);
	int left_buf = start_position, right_buf = end_position - 1;
	bool left_flag = false, right_flag = false, is_swaped = false;
	while(left_buf < right_buf)
	{
		if(partition.isLeftHandSide(leaf_node_list_[left_buf]->getBoundingVolume()->center()))
		{
			left_buf++;
		}
		else
		{
			left_flag = true;
		}
		if(!partition.isLeftHandSide(leaf_node_list_[right_buf]->getBoundingVolume()->center()))
		{
			right_buf--;
		}
		else
		{
			right_flag = true;
		}
		if(left_flag && right_flag)
		{
			BVHNodeBase<Scalar, Dim>* temp = leaf_node_list_[left_buf];
			leaf_node_list_[left_buf] = leaf_node_list_[right_buf];
			leaf_node_list_[right_buf] = temp;
			left_flag = false;
			right_flag = false;
			left_buf++;
			right_buf--;
			is_swaped = true;
		}
	}
	if (!is_swaped)
	{
		left_buf = (start_position + end_position)/2 ;
	}

	//set child nodes
	thisNode->setLeftChild(buildFromLeafList(start_position, left_buf)) ; 
	thisNode->setRightChild(buildFromLeafList(left_buf, end_position)) ;

	return thisNode;
}

template <typename Scalar,int Dim>
BVAxisPartition<Scalar, Dim>::BVAxisPartition(BoundingVolume<Scalar, Dim>* bounding_volume)
{
	if(bounding_volume == NULL)
		return;
	Vector<Scalar, Dim> center = bounding_volume->center();
	int longest_axis_index = 2;
	if (bounding_volume->width() >= bounding_volume->height() && bounding_volume->width() >= bounding_volume->depth())
	{
		longest_axis_index = 0;
	}
	else
	{
		if (bounding_volume->height() >= bounding_volume->width() && bounding_volume->height() >= bounding_volume->depth())
			longest_axis_index = 1;
	}

	longest_axis_index_ = longest_axis_index;
	axis_mid_point_ = center[longest_axis_index];
}

template <typename Scalar,int Dim>
bool BVAxisPartition<Scalar, Dim>::isLeftHandSide(const Vector<Scalar, Dim>& point) const
{
	return point[longest_axis_index_] <= axis_mid_point_;
}

//explicit instantitation
template class BVHBase<float, 3>;
template class BVHBase<double, 3>;

}