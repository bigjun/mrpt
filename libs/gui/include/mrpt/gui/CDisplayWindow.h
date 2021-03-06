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
#ifndef  CDisplayWindow_H
#define  CDisplayWindow_H

#include <mrpt/gui/CBaseGUIWindow.h>
#include <mrpt/utils/CImage.h>

namespace mrpt
{
	/** Classes for creating GUI windows for 2D and 3D visualization.   \ingroup mrpt_gui_grp */
	namespace gui
	{
		using namespace mrpt::utils;

		DEFINE_MRPT_OBJECT_PRE_CUSTOM_BASE_LINKAGE(CDisplayWindow, mrpt::gui::CBaseGUIWindow, GUI_IMPEXP)

		/** This class creates a window as a graphical user interface (GUI) for displaying images to the user.
		 *
		 *  For a list of supported events with the observer/observable pattern, see the discussion in mrpt::gui::CBaseGUIWindow.
		 * \ingroup mrpt_gui_grp
		 */
		class GUI_IMPEXP CDisplayWindow : public mrpt::gui::CBaseGUIWindow
		{
			// This must be added to any CSerializable derived class:
			DEFINE_MRPT_OBJECT( CDisplayWindow )

		protected:

			/** Enables or disables the visualization of cursor coordinates on the window caption.
			  */
			bool			m_enableCursorCoordinates;

		public:
			/** Constructor
			 */
			CDisplayWindow( const std::string &windowCaption = std::string(), unsigned int initWidth = 400, unsigned int initHeight = 400 );

			/** Class factory returning a smart pointer */
			static CDisplayWindowPtr Create(
				const std::string	&windowCaption,
				unsigned int initWidth = 400,
				unsigned int initHeight = 400  )
			{
				return CDisplayWindowPtr(new CDisplayWindow(windowCaption,initWidth,initHeight));
			}

			/** Destructor
			 */
			virtual ~CDisplayWindow();

			/** Gets the last x,y pixel coordinates of the mouse. \return False if the window is closed. */
			virtual bool getLastMousePosition(int &x, int &y) const;

			/** Set cursor style to default (cursorIsCross=false) or to a cross (cursorIsCross=true) */
			virtual void setCursorCross(bool cursorIsCross);

			/** Show a given color or grayscale image on the window and print a set of points on it.
			 *  It adapts the size of the window to that of the image.
			 */
			void  showImageAndPoints( const CImage &img, const vector_float &x, const vector_float &y, const TColor &color = TColor::red, const bool &showNumbers = false );

			/** Show a given color or grayscale image on the window and print a set of points on it.
			 *  It adapts the size of the window to that of the image.
			 *  The class of FEATURELIST can be: mrpt::vision::CFeatureList or any STL container of entities having "x","y" and "ID" fields.
			 */
			template <class FEATURELIST>
			void  showImageAndPoints( const CImage &img, const FEATURELIST &list, const TColor &color = TColor::red, const bool &showIDs = false )
			{
				MRPT_START
				CImage imgColor(1,1,CH_RGB);
				img.colorImage( imgColor );	// Create a colorimage
				imgColor.drawFeatures(list,color,showIDs);
				showImage(imgColor);
				MRPT_END
			}

			/** Show a given color or grayscale image on the window and print a set of points on it and a set of lines splitting the image in tiles.
			 *  It adapts the size of the window to that of the image.
			 *  The class of FEATURELIST can be: mrpt::vision::CFeatureList
			 */
			template <class FEATURELIST>
			void  showTiledImageAndPoints( const CImage &img, const FEATURELIST &list, const TColor &color = TColor::red )
			{
				MRPT_START

				CImage imgColor(1,1,3);
				img.colorImage( imgColor ); // Create a colorimage

				// Print the 4 tile lines
				unsigned int w = imgColor.getWidth();
				unsigned int h = imgColor.getHeight();
				imgColor.line( 0, h/2, w-1, h/2, TColor::green );
				imgColor.line( w/4, 0, w/4, h, TColor::green );
				imgColor.line( w/2, 0, w/2, h, TColor::green );
				imgColor.line( 3*w/4, 0, 3*w/4, h, TColor::green );

				showImageAndPoints( imgColor, list, color );

				MRPT_END
			}

			/** Show a pair of given color or grayscale images (put together) on the window and print a set of matches on them.
			 *  It adapts the size of the window to that of the image.
			 *  MATCHEDLIST can be of the class: mrpt::vision::CMatchedFeatureList, or any STL container of pairs of anything having ".x" and ".y" (e.g. mrpt::math::TPoint2D)
			 */
			template <class MATCHEDLIST>
			void  showImagesAndMatchedPoints( const	CImage &img1, const	CImage &img2, const MATCHEDLIST &mList, const TColor &color = TColor::red, bool showNumbers = false )
			{
				MRPT_START

				CImage imgColor;

				//img1.colorImage( imgColor ); // Create a colorimage
				imgColor.joinImagesHorz( img1, img2 );

				unsigned int w = img1.getWidth();
                unsigned int nf = 0;

				for( typename MATCHEDLIST::const_iterator i = mList.begin(); i != mList.end(); ++i, ++nf )
				{
					imgColor.drawCircle( round( i->first->x ), round( i->first->y ), 4, color );
					imgColor.drawCircle( round( i->second->x + w ), round( i->second->y ), 4, color );
					//imgColor.line( round( i->first->x ), round( i->first->y ), round( i->second->x + w ), round( i->second->y ), color );
					if( showNumbers )
					{
					    char buf[15];
					    mrpt::system::os::sprintf( buf, 15, "%d[%u]", nf, (unsigned int)i->first->ID );
                        imgColor.textOut( round( i->first->x ) - 10, round( i->first->y ), buf, color );
                        mrpt::system::os::sprintf( buf, 15, "%d[%u]", nf, (unsigned int)i->second->ID );
                        imgColor.textOut( round( i->second->x + w ) + 10, round( i->second->y ), buf, color );
                    }
				}
				showImage(imgColor);

				MRPT_END
			}

			/** Show a pair of given color or grayscale images (put together) on the window and print a set of matches on them.
			 *  It adapts the size of the window to that of the image.
			 *  FEATURELIST can be of the class: mrpt::vision::CFeatureList
			 */
			template <class FEATURELIST>
			void  showImagesAndMatchedPoints( const	CImage &img1, const	CImage &img2, const FEATURELIST &leftList, const FEATURELIST &rightList, const TColor &color = TColor::red )
			{
				MRPT_START

				CImage imgColor;

				//img1.colorImage( imgColor ); // Create a colorimage
				ASSERT_( leftList.size() == rightList.size() );
				imgColor.joinImagesHorz( img1, img2 );

				unsigned int w = img1.getWidth();

				for( typename FEATURELIST::const_iterator iL = leftList.begin(), iR = rightList.begin(); iL != leftList.end(); ++iL, ++iR )
				{
					imgColor.drawCircle( round( (*iL)->x ), round( (*iL)->y ), 4, color );
					imgColor.drawCircle( round( (*iR)->x + w ), round( (*iR)->y ), 4, color );
					imgColor.line( round( (*iL)->x ), round( (*iL)->y ), round( (*iR)->x + w ), round( (*iR)->y ), color );
				}
				showImage(imgColor);

				MRPT_END
			}

			/** Show a given color or grayscale image on the window.
			 *  It adapts the size of the window to that of the image.
			 */
			void  showImage( const CImage	&img );

			/** Plots a graph in MATLAB-like style.
			 */
			void  plot( const vector_float &x, const vector_float &y );

			/** Plots a graph in MATLAB-like style.
			 */
			void  plot( const vector_float &y );

			/** Resizes the window, stretching the image to fit into the display area.
			 */
			void  resize( unsigned int width, unsigned int height );

			/** Changes the position of the window on the screen.
			 */
			void  setPos( int x, int y );

			/** Enables or disables the visualization of cursor coordinates on the window caption (default = enabled).
			  */
			inline void  enableCursorCoordinatesVisualization(bool enable)
			{
				m_enableCursorCoordinates = enable;
			}

			/** Changes the window title text.
			  */
			void  setWindowTitle( const std::string &str );

		}; // End of class def.

	} // End of namespace

} // End of namespace

#endif
