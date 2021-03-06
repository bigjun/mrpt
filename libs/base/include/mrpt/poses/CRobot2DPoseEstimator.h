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
#ifndef CRobot2DPoseEstimator_H
#define CRobot2DPoseEstimator_H

#include <mrpt/synch/CCriticalSection.h>
#include <mrpt/math/lightweight_geom_data.h>

namespace mrpt
{
	namespace poses
	{
		using namespace mrpt::math;
		using namespace mrpt::system;

		/** A simple filter to estimate and extrapolate the robot 2D (x,y,phi) pose from asynchronous odometry and localization data.
		  *  The implemented model is a state vector:
		  *		- (x,y,phi,v,w)
		  *  for the robot pose (x,y,phi) and velocities (v,w).
		  *
		  *  The filter can be asked for an extrapolation for some arbitrary time "t'", and it'll do a simple linear prediction.
		  *  All methods are thread-safe.
		  * \ingroup poses_grp poses_pdf_grp
		  */
		class BASE_IMPEXP CRobot2DPoseEstimator
		{
		public:
			 CRobot2DPoseEstimator( ); //!< Default constructor
			 virtual ~CRobot2DPoseEstimator();	//!< Destructor
			 void reset();

			 /** Updates the filter so the pose is tracked to the current time */
			 void processUpdateNewPoseLocalization(
				 const TPose2D &newPose,
				 const CMatrixDouble33 &newPoseCov,
				 TTimeStamp cur_tim);

			 /** Updates the filter so the pose is tracked to the current time */
			 void processUpdateNewOdometry(
				 const TPose2D &newGlobalOdometry,
				 TTimeStamp cur_tim,
				 bool hasVelocities = false,
				 float v = 0,
				 float w = 0);

			 /** Get the current estimate, obtained as:
			   *
			   *   last_loc (+) [ last_odo (-) odo_ref ] (+) extrapolation_from_vw
			   *
			   * \return true is the estimate can be trusted. False if the real observed data is too old or there is no valid data yet.
			   * \sa getLatestRobotPose
			   */
			 bool getCurrentEstimate( TPose2D &pose, float &v, float &w, TTimeStamp tim_query = mrpt::system::now() ) const;

			 /** Get the current estimate, obtained as:
			   *
			   *   last_loc (+) [ last_odo (-) odo_ref ] (+) extrapolation_from_vw
			   *
			   * \return true is the estimate can be trusted. False if the real observed data is too old or there is no valid data yet.
			   * \sa getLatestRobotPose
			   */
			 bool getCurrentEstimate( CPose2D &pose, float &v, float &w, TTimeStamp tim_query = mrpt::system::now() ) const
			 {
			 	TPose2D  p;
			 	bool ret = getCurrentEstimate(p,v,w,tim_query);
			 	if (ret)
					pose = CPose2D(p);
			 	return ret;
			 }

			 /** Get the latest known robot pose, either from odometry or localization.
			   *  This differs from getCurrentEstimate() in that this method does NOT extrapolate as getCurrentEstimate() does.
			   * \return false if there is not estimation yet.
			   * \sa getCurrentEstimate
			   */
			 bool getLatestRobotPose(TPose2D &pose) const;

			 /** Get the latest known robot pose, either from odometry or localization.
			   * \return false if there is not estimation yet.
			   */
			 inline bool getLatestRobotPose(CPose2D &pose) const
			 {
			 	TPose2D p;
			 	bool v = getLatestRobotPose(p);
				if (v)
				{
			 		pose.x(p.x);
			 		pose.y(p.y);
			 		pose.phi(p.phi);
				}
			 	return v;
			 }


			 struct TOptions
			 {
				TOptions() :
					max_odometry_age	( 1.0 ),
					max_localiz_age		( 4.0 )
				{}

				double  max_odometry_age; //!< To consider data old, in seconds
				double  max_localiz_age; //!< To consider data old, in seconds
			 };

			 TOptions params; //!< parameters of the filter.

		private:
			mrpt::synch::CCriticalSection  m_cs;

			TTimeStamp		m_last_loc_time;
			TPose2D			m_last_loc;   //!< Last pose as estimated by the localization/SLAM subsystem.
			CMatrixDouble33 m_last_loc_cov;

			TPose2D			m_loc_odo_ref;  //!< The interpolated odometry position for the last "m_robot_pose" (used as "coordinates base" for subsequent odo readings)

			TTimeStamp		m_last_odo_time;
			TPose2D			m_last_odo;
			float			m_robot_v;
			float			m_robot_w;

			/** An auxiliary method to extrapolate the pose of a robot located at "p" with velocities (v,w) after a time delay "delta_time".
			  */
			static void extrapolateRobotPose(
				const TPose2D &p,
				const float v,
				const float w,
				const double delta_time,
				TPose2D &new_p);

		}; // end of class

	} // End of namespace
} // End of namespace

#endif
