/* +---------------------------------------------------------------------------+
   |          The Mobile Robot Programming Toolkit (MRPT) C++ library          |
   |                                                                           |
   |                   http://mrpt.sourceforge.net/                            |
   |                                                                           |
   |   Copyright (C) 2005-2010  University of Malaga                           |
   |                                                                           |
   |    This software was written by the Machine Perception and Intelligent    |
   |      Robotics Lab, University of Malaga (Spain).                          |
   |    Contact: Jose-Luis Blanco  <jlblanco@ctima.uma.es>                     |
   |                                                                           |
   |  This file is part of the MRPT project.                                   |
   |                                                                           |
   |     MRPT is free software: you can redistribute it and/or modify          |
   |     it under the terms of the GNU General Public License as published by  |
   |     the Free Software Foundation, either version 3 of the License, or     |
   |     (at your option) any later version.                                   |
   |                                                                           |
   |   MRPT is distributed in the hope that it will be useful,                 |
   |     but WITHOUT ANY WARRANTY; without even the implied warranty of        |
   |     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         |
   |     GNU General Public License for more details.                          |
   |                                                                           |
   |     You should have received a copy of the GNU General Public License     |
   |     along with MRPT.  If not, see <http://www.gnu.org/licenses/>.         |
   |                                                                           |
   +---------------------------------------------------------------------------+ */

#ifndef mrpt_vision_pinhole_H
#define mrpt_vision_pinhole_H

#include <mrpt/utils/TCamera.h>
#include <mrpt/vision/utils.h>

namespace mrpt
{
	namespace vision
	{
		/** Functions related to pinhole camera models, point projections, etc. */
		namespace pinhole
		{
			/** Project a set of 3D points into a camera at an arbitrary 6D pose using its calibration matrix (undistorted projection model)
			  * \param in_points_3D [IN] The list of 3D points in world coordinates (meters) to project.
			  * \param cameraPose [IN] The pose of the camera in the world.
			  * \param intrinsicParams [IN] The 3x3 calibration matrix. See http://www.mrpt.org/Camera_Parameters
			  * \param projectedPoints [OUT] The list of image coordinates (in pixels) for the projected points. At output this list is resized to the same number of input points.
			  * \param accept_points_behind [IN] See the note below.
			  *
			  * \note Points "behind" the camera (which couldn't be physically seen in the real world) are marked with pixel coordinates (-1,-1) to detect them as invalid, unless accept_points_behind is true. In that case they'll be projected normally.
			  *
			  * \sa projectPoints_with_distortion
			  */
			void VISION_IMPEXP projectPoints_no_distortion(
				const std::vector<mrpt::poses::CPoint3D> &in_points_3D,
				const mrpt::poses::CPose3D &cameraPose,
				const mrpt::math::CMatrixDouble33 & intrinsicParams,
				std::vector<mrpt::vision::TPixelCoordf> &projectedPoints,
				bool accept_points_behind = false
				);

			/** Project a set of 3D points into a camera at an arbitrary 6D pose using its calibration matrix and distortion parameters (radial and tangential distortions projection model)
			  * \param in_points_3D [IN] The list of 3D points in world coordinates (meters) to project.
			  * \param cameraPose [IN] The pose of the camera in the world.
			  * \param intrinsicParams [IN] The 3x3 calibration matrix. See http://www.mrpt.org/Camera_Parameters
			  * \param distortionParams [IN] The 4-length vector with the distortion parameters [k1 k2 p1 p2]. See http://www.mrpt.org/Camera_Parameters
			  * \param projectedPoints [OUT] The list of image coordinates (in pixels) for the projected points. At output this list is resized to the same number of input points.
			  * \param accept_points_behind [IN] See the note below.
			  *
			  * \note Points "behind" the camera (which couldn't be physically seen in the real world) are marked with pixel coordinates (-1,-1) to detect them as invalid, unless accept_points_behind is true. In that case they'll be projected normally.
			  *
			  * \sa projectPoint_with_distortion, projectPoints_no_distortion
			  */
			void VISION_IMPEXP projectPoints_with_distortion(
				const std::vector<mrpt::poses::CPoint3D> &in_points_3D,
				const mrpt::poses::CPose3D &cameraPose,
				const mrpt::math::CMatrixDouble33 & intrinsicParams,
				const std::vector<double> & distortionParams,
				std::vector<mrpt::vision::TPixelCoordf> &projectedPoints,
				bool accept_points_behind = false
				);

			/** Project one 3D point into a camera using its calibration matrix and distortion parameters (radial and tangential distortions projection model)
			  * \param in_point_wrt_cam [IN] The 3D point wrt the camera focus, with +Z=optical axis, +X=righthand in the image plane, +Y=downward in the image plane.
			  * \param in_cam_params [IN] The camera parameters. See http://www.mrpt.org/Camera_Parameters
			  * \param out_projectedPoints [OUT] The projected point, in pixel units.
			  * \param accept_points_behind [IN] See the note below.
			  *
			  * \note Points "behind" the camera (which couldn't be physically seen in the real world) are marked with pixel coordinates (-1,-1) to detect them as invalid, unless accept_points_behind is true. In that case they'll be projected normally.
			  *
			  * \sa projectPoints_with_distortion
			  */
			void VISION_IMPEXP projectPoint_with_distortion(
				const mrpt::math::TPoint3D  &in_point_wrt_cam,
				const mrpt::utils::TCamera  &in_cam_params, 
				mrpt::vision::TPixelCoordf  &out_projectedPoints,
				bool accept_points_behind = false
				);

			void VISION_IMPEXP projectPoints_with_distortion(
				const std::vector<mrpt::math::TPoint3D>  &P,
				const mrpt::utils::TCamera  &params,
				const CPose3DQuat &cameraPose,
				std::vector<mrpt::vision::TPixelCoordf>  &pixels,
				bool accept_points_behind = false
				);


			/** Undistort a list of points given by their pixel coordinates, provided the camera matrix and distortion coefficients.
			  * \param srcDistortedPixels [IN] The pixel coordinates as in the distorted image.
			  * \param dstUndistortedPixels [OUT] The computed pixel coordinates without distortion.
			  * \param intrinsicParams [IN] The 3x3 calibration matrix. See http://www.mrpt.org/Camera_Parameters
			  * \param distortionParams [IN] The 4-length vector with the distortion parameters [k1 k2 p1 p2]. See http://www.mrpt.org/Camera_Parameters
			  */
			void VISION_IMPEXP undistort_points(
				const std::vector<mrpt::vision::TPixelCoordf>  &srcDistortedPixels,
				std::vector<mrpt::vision::TPixelCoordf> &dstUndistortedPixels,
				const mrpt::math::CMatrixDouble33 & intrinsicParams,
				const std::vector<double> & distortionParams );

			/** Undistort a list of points given by their pixel coordinates, provided the camera matrix and distortion coefficients.
			  * \param inputPixels [IN] The pixel coordinates as in the distorted image.
			  * \param dstUndistortedPixels [OUT] The computed pixel coordinates without distortion.
			  * \param intrinsicParams [IN] The 3x3 calibration matrix. See http://www.mrpt.org/Camera_Parameters
			  * \param distortionParams [IN] The 4-length vector with the distortion parameters [k1 k2 p1 p2]. See http://www.mrpt.org/Camera_Parameters
			  */
			//void VISION_IMPEXP undistort_points( 
			//	const std::vector<mrpt::vision::TPixelCoordf>	&inputPixels, 		/* distorted pixels in image */
			//	const mrpt::math::CMatrixDouble33				&intrinsicParams,	/* intrinsic parameters of the camera */
			//	const std::vector<double>						&distortionParams,	/* k1 k2 p1 p2 */
			//	const unsigned int								&resX,				/* X-resolution of the image */
			//	const unsigned int								&resY,				/* Y-resolution of the image */
			//	std::vector<mrpt::vision::TPixelCoordf>			&outputPixels		/* estimated undistorted pixels in image */
			//	);


		}
	}
}

#endif
