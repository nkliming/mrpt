/* +---------------------------------------------------------------------------+
   |                     Mobile Robot Programming Toolkit (MRPT)               |
   |                          http://www.mrpt.org/                             |
   |                                                                           |
   | Copyright (c) 2005-2017, Individual contributors, see AUTHORS file        |
   | See: http://www.mrpt.org/Authors - All rights reserved.                   |
   | Released under BSD License. See details in http://www.mrpt.org/License    |
   +---------------------------------------------------------------------------+ */

#include "base-precomp.h"  // Precompiled headers

#include <mrpt/poses/FrameTransformer.h>
#include <mrpt/poses/CPose3D.h>

using namespace mrpt::poses;

// ------- FrameTransformerInterface --------
template <int DIM>
FrameTransformerInterface<DIM>::FrameTransformerInterface()
{
}
template <int DIM>
FrameTransformerInterface<DIM>::~FrameTransformerInterface()
{
}

// Explicit instantations:
template class BASE_IMPEXP FrameTransformerInterface<2>;
template class BASE_IMPEXP FrameTransformerInterface<3>;


// ------- FrameTransformer --------
template <int DIM>
FrameTransformer<DIM>::FrameTransformer()
{

}
template <int DIM>
FrameTransformer<DIM>::~FrameTransformer()
{
	
}

template <int DIM>
void FrameTransformer<DIM>::sendTransform(
	const std::string & parent_frame, 
	const std::string & child_frame, 
	const pose_t & child_wrt_parent,
	const mrpt::system::TTimeStamp & timestamp)
{
	m_pose_edges_buffer[parent_frame][child_frame] = TF_TreeEdge(child_wrt_parent, timestamp);
}

MRPT_TODO("future work: allow graph traversal and do pose chain composition"); // We dont need to replicate the full functionality of ROS tf, though... just what we really need.

template <int DIM>
FrameLookUpStatus FrameTransformer<DIM>::lookupTransform(
	const std::string & target_frame, 
	const std::string & source_frame, 
	pose_t & child_wrt_parent,
	const mrpt::system::TTimeStamp query_time,
	const double timeout_secs)
{
	ASSERTMSG_(timeout_secs == .0, "timeout_secs!=0: Blocking calls not supported yet!");
	ASSERTMSG_(query_time == INVALID_TIMESTAMP, "`query_time` different than 'latest' not supported yet!");

	const auto &it_src = m_pose_edges_buffer.find(source_frame);
	if (it_src == m_pose_edges_buffer.end()) {
		return LKUP_UNKNOWN_FRAME;
	}

	const auto &it_dst = it_src->second.find(target_frame);
	if (it_dst == it_src->second.end()) {
		return LKUP_NO_CONNECTIVITY;
	}

	const TF_TreeEdge & te = it_dst->second;
	child_wrt_parent = te.pose;

	return LKUP_GOOD;
}

// Explicit instantations:
template class BASE_IMPEXP FrameTransformer<2>;
template class BASE_IMPEXP FrameTransformer<3>;
