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

#ifndef CHeightGridMap2D_H
#define CHeightGridMap2D_H

#include <mrpt/utils/CDynamicGrid.h>
#include <mrpt/utils/CSerializable.h>
#include <mrpt/math/CMatrixD.h>
#include <mrpt/math/geometry.h>
#include <mrpt/system/os.h>
#include <mrpt/utils/CLoadableOptions.h>
#include <mrpt/utils/stl_extensions.h>
#include <mrpt/utils/color_maps.h>

#include <mrpt/slam/CMetricMap.h>

#include <mrpt/maps/link_pragmas.h>

namespace mrpt
{
	namespace poses
	{
		class CPose2D;
	}
	namespace slam
	{
		using namespace mrpt::utils;

		class CObservation;

		DEFINE_SERIALIZABLE_PRE_CUSTOM_BASE_LINKAGE( CHeightGridMap2D, CMetricMap, MAPS_IMPEXP  )

		/** The contents of each cell in a CHeightGridMap2D map.
		 **/
		struct MAPS_IMPEXP THeightGridmapCell
		{
			/** Constructor
			  */
			THeightGridmapCell() : h(0), w(0)
			{}

			/** The current average height (in meters).
			  */
			float	h;

			/** The current standard deviation of the height (in meters).
			  */
			float	var;

			/** Auxiliary variable for storing the incremental mean value (in meters).
			  */
			float	u;

			/** Auxiliary (in meters).
			  */
			float	v;


			/** [mrSimpleAverage model] The accumulated weight: initially zero if un-observed, increased by one for each observation.
			  */
			uint32_t	w;
		};

		/** A mesh representation of a surface which keeps the estimated height for each (x,y) location.
		  *  Important implemented features are the insertion of 2D laser scans (from arbitrary 6D poses) and the exportation as 3D scenes.
		  *
		  *   Each cell contains the up-to-date average height from measured falling in that cell. Algorithms that can be used:
		  *		- mrSimpleAverage: Each cell only stores the current average value.
		  */
		class MAPS_IMPEXP CHeightGridMap2D : public CMetricMap, public utils::CDynamicGrid<THeightGridmapCell>
		{
			// This must be added to any CSerializable derived class:
			DEFINE_SERIALIZABLE( CHeightGridMap2D )
		public:

			/** Calls the base CMetricMap::clear
			  * Declared here to avoid ambiguity between the two clear() in both base classes.
			  */
			inline void clear() { CMetricMap::clear(); }

			float cell2float(const THeightGridmapCell& c) const
			{
				return float(c.h);
			}

			/** The type of map representation to be used.
			  *  See mrpt::slam::CHeightGridMap2D for discussion.
			  */
			enum TMapRepresentation
			{
				mrSimpleAverage = 0
//				mrSlidingWindow
			};

			/** Constructor
			  */
			CHeightGridMap2D(
				TMapRepresentation	mapType = mrSimpleAverage,
				float				x_min = -2,
				float				x_max = 2,
				float				y_min = -2,
				float				y_max = 2,
				float				resolution = 0.1
				);

			 /** Returns true if the map is empty/no observation has been inserted.
			   */
			 bool  isEmpty() const;

			// See docs in base class
			double	 computeObservationLikelihood( const CObservation *obs, const CPose3D &takenFrom );

			/** Parameters related with inserting observations into the map.
			  */
			struct MAPS_IMPEXP TInsertionOptions : public utils::CLoadableOptions
			{
				/** Default values loader:
				  */
				TInsertionOptions();

				/** See utils::CLoadableOptions
				  */
				void  loadFromConfigFile(
					const mrpt::utils::CConfigFileBase  &source,
					const std::string &section);

				/** See utils::CLoadableOptions
				  */
				void  dumpToTextStream(CStream	&out) const;

				/** Wether to perform filtering by z-coordinate (default=false): coordinates are always RELATIVE to the robot for this filter.
				  */
				bool	filterByHeight;

				/** Only when filterByHeight is true: coordinates are always RELATIVE to the robot for this filter.
				  */
				float	z_min,z_max;

				float	minDistBetweenPointsWhenInserting;	//!< When inserting a scan, a point cloud is first created with this minimum distance between the 3D points (default=0).

				mrpt::utils::TColormap colorMap;

			} insertionOptions;

			/** See docs in base class: in this class it always returns 0 */
			float  compute3DMatchingRatio(
					const CMetricMap						*otherMap,
					const CPose3D							&otherMapPose,
					float									maxDistForCorr = 0.10f,
					float									maxMahaDistForCorr = 2.0f
					) const
			{
				return 0;
			}

			/** The implementation in this class just calls all the corresponding method of the contained metric maps.
			  */
			void  saveMetricMapRepresentationToFile(
				const std::string	&filNamePrefix
				) const;

			/** Returns a 3D object representing the map: by default, it will be a mrpt::opengl::CMesh object, unless
			  *   it is specified otherwise in mrpt::
			  */
			void  getAs3DObject ( mrpt::opengl::CSetOfObjectsPtr	&outObj ) const;

			/** Return the type of the gas distribution map, according to parameters passed on construction.
			  */
			TMapRepresentation	 getMapType();


			/** Gets the intersection between a 3D line and a Height Grid map (taking into account the different heights of each individual cell).
			  */
			bool intersectLine3D(const TLine3D &r1, TObject3D &obj) const;

			/** Computes the minimum and maximum height in the grid.
			  * \return False if there is no observed cell yet.
			  */
			bool getMinMaxHeight(float &z_min, float &z_max) const;

			/** Return the number of cells with at least one height data inserted. */
			size_t countObservedCells() const;

		protected:

			/** The map representation type of this map.
			  */
			TMapRepresentation		m_mapType;

			 /** Erase all the contents of the map
			  */
			 virtual void  internal_clear();

			 /** Insert the observation information into this map. This method must be implemented
			  *    in derived classes.
			  * \param obs The observation
			  * \param robotPose The 3D pose of the robot mobile base in the map reference system, or NULL (default) if you want to use CPose2D(0,0,deg)
			  *
			  * \sa CObservation::insertObservationInto
			  */
			 virtual bool  internal_insertObservation( const CObservation *obs, const CPose3D *robotPose = NULL );

		};


	} // End of namespace

	namespace global_settings
	{
		/** If set to true (default), mrpt::slam::CHeightGridMap2D will be exported as a opengl::CMesh, otherwise, as a opengl::CPointCloudColoured
		  * Affects to:
		  *		- CHeightGridMap2D::getAs3DObject
		  */
		extern MAPS_IMPEXP bool HEIGHTGRIDMAP_EXPORT3D_AS_MESH;
	}

	// Specializations MUST occur at the same namespace:
	namespace utils
	{
		template <>
		struct TEnumTypeFiller<slam::CHeightGridMap2D::TMapRepresentation>
		{
			typedef slam::CHeightGridMap2D::TMapRepresentation enum_t;
			static void fill(bimap<enum_t,std::string>  &m_map)
			{
				m_map.insert(slam::CHeightGridMap2D::mrSimpleAverage,     "mrSimpleAverage");
			}
		};
	} // End of namespace

} // End of namespace

#endif
