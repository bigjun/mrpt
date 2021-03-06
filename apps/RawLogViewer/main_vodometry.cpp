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

#include "xRawLogViewerMain.h"

#include <wx/msgdlg.h>
#include <wx/filedlg.h>
#include <wx/app.h>
#include <wx/progdlg.h>
#include <wx/imaglist.h>
#include <wx/busyinfo.h>
#include <wx/log.h>
#include <wx/textdlg.h>

// General global variables:
#include <mrpt/slam.h>

#ifdef RAWLOGVIEWER_HAS_STEREOSLAM
	#include <mrpt/stereoslam.h>
#endif

using namespace mrpt;
using namespace mrpt::slam;
using namespace mrpt::opengl;
using namespace mrpt::system;
using namespace mrpt::math;
using namespace mrpt::gui;
using namespace mrpt::utils;
using namespace mrpt::vision;
using namespace std;

#ifdef RAWLOGVIEWER_HAS_STEREOSLAM
	using namespace mrpt::stereoslam;
#endif


void xRawLogViewerFrame::OnMenuVisualOdometry(wxCommandEvent& event)
{
	WX_START_TRY

#ifdef RAWLOGVIEWER_HAS_STEREOSLAM

	wxBusyCursor        waitCursor;
	unsigned int		nEntries = rawlog.size();

	wxString            auxStr;
	wxProgressDialog    progDia(
		wxT("Progress"),
		wxT("Computing visual odometry from rawlog..."),
		nEntries, // range
		this, // parent
		wxPD_CAN_ABORT |
		wxPD_APP_MODAL |
		wxPD_SMOOTH |
		wxPD_AUTO_HIDE |
		wxPD_ELAPSED_TIME |
		wxPD_ESTIMATED_TIME |
		wxPD_REMAINING_TIME);

	wxTheApp->Yield();  // Let the app. process messages

	string					errorMsg;
	CVisualOdometryStereo	vOd;

	// VOdometer options
	// TO DO: read from .ini file??
	// Take the
	// Stereo parameters
	vOd.stereoParams.stdPixel			= 0.5f;
	vOd.stereoParams.stdDisp			= 1.5f;
	vOd.stereoParams.maxZ				= 6.0f;
	vOd.stereoParams.minZ				= 0.0f;
	vOd.stereoParams.maxY				= 2.0f;

	// Odometry
	vOd.odometryOptions.covMethod		= TOdometryOptions::TCov_Linear;
	//vOd.odometryOptions.covMethod		= TOdometryOptions::TCov_UT;
	vOd.odometryOptions.minNumFeatures	= 25;
	vOd.odometryOptions.SAVE_FILES		= false;

	// Matching
	vOd.matchingOptions.matching_method	= TMatchingOptions::mmCorrelation;
	vOd.matchingOptions.epipolar_TH		= 1.5f;
	vOd.odometryOptions.rowCheck_TH		= 1.5f;
	vOd.matchingOptions.maxEDD_TH		= 90.0f;
	vOd.matchingOptions.EDD_RATIO		= 0.7f;
	vOd.matchingOptions.minCC_TH		= 0.92f;
	vOd.matchingOptions.minDCC_TH		= 0.025f;
	vOd.matchingOptions.rCC_TH			= 0.90f;

	// Create LOG Directory and Delete files
	if( !mrpt::system::fileExists("LOG_VODOMETRY") && vOd.odometryOptions.SAVE_FILES )
		mrpt::system::createDirectory("LOG_VODOMETRY");									// Create LOG directory
	mrpt::system::deleteFiles("./LOG_VODOMETRY/*.txt");									// Clear LOG directory

	for (unsigned int countLoop=0;countLoop<nEntries-2;countLoop++)
	{
		if (countLoop % 5 == 0)
		{
			auxStr.sprintf(wxT("Computing visual odometry from... %u objects"),countLoop );
			if (!progDia.Update( countLoop, auxStr ))
				break;
			wxTheApp->Yield();  // Let the app. process messages
		}

		try
		{
			switch (rawlog.getType(countLoop))
			{
				case CRawlog::etSensoryFrame:
				{
					ASSERT_( rawlog.getType( countLoop + 1 ) == CRawlog::etActionCollection );
					ASSERT_( rawlog.getType( countLoop + 2 ) == CRawlog::etSensoryFrame );

					CSensoryFramePtr SF = rawlog.getAsObservations(countLoop);
					CObservationStereoImagesPtr obsSt = SF->getObservationByClass<CObservationStereoImages>();

					// ----------------------------------------
					// VISUAL ODOMETRY OPTIONS:
					// ----------------------------------------
					// Stereo
					vOd.stereoParams.baseline			= obsSt->rightCameraPose.x();
					vOd.stereoParams.K = obsSt->leftCamera.intrinsicParams;

					CActionRobotMovement3D newAction;
					newAction.estimationMethod	= CActionRobotMovement3D::emVisualOdometry;
					vOd.process( obsSt, newAction.poseChangeQuat );

					CActionCollectionPtr AC = rawlog.getAsAction( countLoop + 1 );
					AC->clear();
					AC->insert( newAction );

					// Unload previous images!
					obsSt->imageLeft.unload();
					obsSt->imageRight.unload();

				}
				break;

				//case CRawlog::etObservation:
				//{
				//	CObservationPtr o = rawlog.getAsObservation(countLoop);

				//	if (IS_CLASS(o,CObservationStereoImages) )
				//	{
				//		CObservationStereoImagesPtr obsSt = CObservationStereoImagesPtr(o);
				//		obsSt->imageLeft.saveToFile( format( "%s/img_stereo_%s_left_%05u.%s",outDir.c_str(), obsSt->sensorLabel.c_str() ,imgSaved, imgFileExtension.c_str() ) );

				//		obsSt->imageRight.saveToFile( format("%s/img_stereo_%s_right_%05u.%s",outDir.c_str(),obsSt->sensorLabel.c_str(),imgSaved, imgFileExtension.c_str()) );
				//		imgSaved++;
				//	}
				//	else
				//	if (IS_CLASS(o,CObservationImage) )
				//	{
				//		CObservationImagePtr obsIm = CObservationImagePtr(o);
				//		obsIm->image.saveToFile( format("%s/img_monocular_%s_%05u.%s",outDir.c_str(),obsIm->sensorLabel.c_str(),imgSaved, imgFileExtension.c_str()) );
				//		imgSaved++;
				//	}
				//}
				//break;

				default:
					break;
			} // end for each entry

		}
		catch (exception &e)
		{
			errorMsg = e.what();
			break;
		}
		catch (...)
		{
			break;
		}
	} // end while keep loading

	progDia.Update( nEntries );
	this->rebuildTreeView();

	// Set error msg:
	//wxMessageBox(_U(format("Images saved: %i",imgSaved).c_str()),_("Done"),wxOK,this);

#else
	THROW_EXCEPTION("MRPT has been compiled without the library mrpt-stereoslam")
#endif
	WX_END_TRY
}
