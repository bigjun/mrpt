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
#ifndef CSimplePointsMap_H
#define CSimplePointsMap_H

#include <mrpt/slam/CPointsMap.h>
#include <mrpt/slam/CObservation2DRangeScan.h>
#include <mrpt/slam/CObservation3DRangeScan.h>
#include <mrpt/utils/CSerializable.h>
#include <mrpt/math/CMatrix.h>

#include <mrpt/maps/link_pragmas.h>

namespace mrpt
{
	namespace slam
	{

		DEFINE_SERIALIZABLE_PRE_CUSTOM_BASE_LINKAGE( CSimplePointsMap , CPointsMap, MAPS_IMPEXP )

		/** A cloud of points in 2D or 3D, which can be built from a sequence of laser scans.
		 *    This class only stores the coordinates (x,y,z) of each point.
		 *
		 *  See mrpt::slam::CPointsMap and derived classes for other point cloud classes.
		 *
		 * \sa CMetricMap, CWeightedPointsMap, CPoint, mrpt::utils::CSerializable
	  	 * \ingroup mrpt_maps_grp
		 */
		class MAPS_IMPEXP CSimplePointsMap : public CPointsMap
		{
			// This must be added to any CSerializable derived class:
			DEFINE_SERIALIZABLE( CSimplePointsMap )

		 public:
			 CSimplePointsMap();          //!< Default constructor
			 virtual ~CSimplePointsMap(); //!< Destructor

			// --------------------------------------------
			/** @name Pure virtual interfaces to be implemented by any class derived from CPointsMap
				@{ */

			/** Reserves memory for a given number of points: the size of the map does not change, it only reserves the memory.
			  *  This is useful for situations where it is approximately known the final size of the map. This method is more
			  *  efficient than constantly increasing the size of the buffers. Refer to the STL C++ library's "reserve" methods.
			  */
			virtual void reserve(size_t newLength);

			/** Resizes all point buffers so they can hold the given number of points: newly created points are set to default values,
			  *  and old contents are not changed.
			  * \sa reserve, setPoint, setPointFast, setSize
			  */
			virtual void resize(size_t newLength);

			/** Resizes all point buffers so they can hold the given number of points, *erasing* all previous contents
			  *  and leaving all points to default values.
			  * \sa reserve, setPoint, setPointFast, setSize
			  */
			virtual void setSize(size_t newLength);

			/** Changes the coordinates of the given point (0-based index), *without* checking for out-of-bounds and *without* calling mark_as_modified()  \sa setPoint */
			virtual void  setPointFast(size_t index,float x, float y, float z);

			/** The virtual method for \a insertPoint() *without* calling mark_as_modified()   */
			virtual void  insertPointFast( float x, float y, float z = 0 );

			 /** Virtual assignment operator, to be implemented in derived classes.
			   */
			 virtual void  copyFrom(const CPointsMap &obj);

			/** Get all the data fields for one point as a vector: [X Y Z]
			  *  Unlike getPointAllFields(), this method does not check for index out of bounds
			  * \sa getPointAllFields, setPointAllFields, setPointAllFieldsFast
			  */
			virtual void  getPointAllFieldsFast( const size_t index, std::vector<float> & point_data ) const {
				point_data.resize(3);
				point_data[0] = x[index];
				point_data[1] = y[index];
				point_data[2] = z[index];
			}

			/** Set all the data fields for one point as a vector: [X Y Z]
			  *  Unlike setPointAllFields(), this method does not check for index out of bounds
			  * \sa setPointAllFields, getPointAllFields, getPointAllFieldsFast
			  */
			virtual void  setPointAllFieldsFast( const size_t index, const std::vector<float> & point_data ) {
				ASSERTDEB_(point_data.size()==3)
				x[index] = point_data[0];
				y[index] = point_data[1];
				z[index] = point_data[2];
			}

			/** See CPointsMap::loadFromRangeScan() */
			virtual void  loadFromRangeScan(
					const CObservation2DRangeScan &rangeScan,
					const CPose3D				  *robotPose = NULL );

			/** See CPointsMap::loadFromRangeScan() */
			virtual void  loadFromRangeScan(
					const CObservation3DRangeScan &rangeScan,
					const CPose3D				  *robotPose = NULL );


		protected:

			/** Auxiliary method called from within \a addFrom() automatically, to finish the copying of class-specific data  */
			virtual void  addFrom_classSpecific(const CPointsMap &anotherMap, const size_t nPreviousPoints) {
				// No extra data.
			}

			// Friend methods:
			template <class Derived> friend struct detail::loadFromRangeImpl;
			template <class Derived> friend struct detail::pointmap_traits;

		public:


			/** @} */
			// --------------------------------------------


			/** If the map is a simple points map or it's a multi-metric map that contains EXACTLY one simple points map, return it.
				* Otherwise, return NULL
				*/
			virtual const CSimplePointsMap * getAsSimplePointsMap() const { return this; }
			virtual       CSimplePointsMap * getAsSimplePointsMap()       { return this; }

		protected:
			/** Clear the map, erasing all the points.
			 */
			virtual void  internal_clear();

			/** @name PLY Import virtual methods to implement in base classes
			    @{ */
			/** In a base class, reserve memory to prepare subsequent calls to PLY_import_set_vertex */
			virtual void PLY_import_set_vertex_count(const size_t N);
			/** @} */

		}; // End of class def.
	} // End of namespace
	
	namespace utils
	{
		/** Specialization mrpt::utils::PointCloudAdapter<mrpt::slam::CSimplePointsMap>  \ingroup mrpt_adapters_grp*/
		template <>
		class PointCloudAdapter<mrpt::slam::CSimplePointsMap> : public detail::PointCloudAdapterHelperNoRGB<mrpt::slam::CSimplePointsMap,float>
		{
		private:
			mrpt::slam::CSimplePointsMap &m_obj;
		public:
			typedef float  coords_t;         //!< The type of each point XYZ coordinates
			static const int HAS_RGB   = 0;  //!< Has any color RGB info?
			static const int HAS_RGBf  = 0;  //!< Has native RGB info (as floats)?
			static const int HAS_RGBu8 = 0;  //!< Has native RGB info (as uint8_t)?

			/** Constructor (accept a const ref for convenience) */
			inline PointCloudAdapter(const mrpt::slam::CSimplePointsMap &obj) : m_obj(*const_cast<mrpt::slam::CSimplePointsMap*>(&obj)) { }
			/** Get number of points */
			inline size_t size() const { return m_obj.size(); }
			/** Set number of points (to uninitialized values) */
			inline void resize(const size_t N) { m_obj.resize(N); }

			/** Get XYZ coordinates of i'th point */
			template <typename T>
			inline void getPointXYZ(const size_t idx, T &x,T &y, T &z) const {
				m_obj.getPointFast(idx,x,y,z);
			}
			/** Set XYZ coordinates of i'th point */
			inline void setPointXYZ(const size_t idx, const coords_t x,const coords_t y, const coords_t z) {
				m_obj.setPointFast(idx,x,y,z);
			}
		}; // end of PointCloudAdapter<mrpt::slam::CPointsMap>

	}
	
} // End of namespace

#endif
