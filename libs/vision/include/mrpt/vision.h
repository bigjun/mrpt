/* +---------------------------------------------------------------------------+
   |                 The Mobile Robot Programming Toolkit (MRPT)               |
   |                                                                           |
   |                          http://www.mrpt.org/                             |
   |                                                                           |
   | Copyright (c) 2005-2013, Individual contributors, see AUTHORS file        |
   | Copyright (c) 2005-2013, MAPIR group, University of Malaga                |
   | Copyright (c) 2012-2013, University of Almeria                            |
   | All rights reserved.                                                      |
   |                                                                           |
   | Redistribution and use in source and binary forms, with or without        |
   | modification, are permitted provided that the following conditions are    |
   | met:                                                                      |
   |    * Redistributions of source code must retain the above copyright       |
   |      notice, this list of conditions and the following disclaimer.        |
   |    * Redistributions in binary form must reproduce the above copyright    |
   |      notice, this list of conditions and the following disclaimer in the  |
   |      documentation and/or other materials provided with the distribution. |
   |    * Neither the name of the copyright holders nor the                    |
   |      names of its contributors may be used to endorse or promote products |
   |      derived from this software without specific prior written permission.|
   |                                                                           |
   | THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS       |
   | 'AS IS' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED |
   | TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR|
   | PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDERS BE LIABLE |
   | FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL|
   | DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR|
   |  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)       |
   | HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,       |
   | STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN  |
   | ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE           |
   | POSSIBILITY OF SUCH DAMAGE.                                               |
   +---------------------------------------------------------------------------+ */

#ifndef __mrpt_vision_H
#define __mrpt_vision_H

#include <mrpt/config.h>

// Only really include all headers if we come from a user program (anything
//  not defining mrpt_*_EXPORTS) or MRPT is being built with precompiled headers.
#if !defined(mrpt_vision_EXPORTS) || MRPT_ENABLE_PRECOMPILED_HDRS || defined(MRPT_ALWAYS_INCLUDE_ALL_HEADERS)

#include <mrpt/vision/utils.h>
#include <mrpt/vision/TSimpleFeature.h>
#include <mrpt/vision/multiDesc_utils.h>
#include <mrpt/vision/chessboard_camera_calib.h>
#include <mrpt/vision/chessboard_stereo_camera_calib.h>
#include <mrpt/vision/chessboard_find_corners.h>
#include <mrpt/vision/pinhole.h>
#include <mrpt/vision/CCamModel.h>
#include <mrpt/vision/CFeatureExtraction.h>
#include <mrpt/vision/CVideoFileWriter.h>
#include <mrpt/vision/tracking.h>
#include <mrpt/vision/descriptor_kdtrees.h>
#include <mrpt/vision/descriptor_pairing.h>
#include <mrpt/vision/bundle_adjustment.h>
#include <mrpt/vision/CUndistortMap.h>
#include <mrpt/vision/CStereoRectifyMap.h>
#include <mrpt/vision/CImagePyramid.h>
#include <mrpt/vision/robust_kernels.h>

// Maps:
#include <mrpt/slam/CLandmark.h>
#include <mrpt/slam/CLandmarksMap.h>

// Obs:
#include <mrpt/slam/CObservationVisualLandmarks.h>

#endif // end precomp.headers

#endif
