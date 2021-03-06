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
#ifndef CSCANANIMATION_H
#define CSCANANIMATION_H

//(*Headers(CScanAnimation)
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/checkbox.h>
#include <wx/spinctrl.h>
#include <wx/radiobut.h>
#include "MyGLCanvas.h"
#include <wx/slider.h>
#include <wx/button.h>
#include <wx/dialog.h>
//*)

// The "custom class" mpWindow, from the wxMathPlot libray by David Schalig
//  See http://sourceforge.net/projects/wxmathplot
#include <mrpt/otherlibs/mathplot/mathplot.h>

#include <mrpt/slam/CSensoryFrame.h>
#include <mrpt/opengl/CPointCloudColoured.h>

class CScanAnimation: public wxDialog
{
	public:

		CScanAnimation(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~CScanAnimation();

		//(*Declarations(CScanAnimation)
		wxBoxSizer* BoxSizer4;
		wxStaticText* StaticText22;
		wxButton* btnStop;
		wxRadioButton* rbLoaded;
		wxSlider* slPos;
		wxBoxSizer* BoxSizer5;
		wxSpinCtrl* edDelay;
		wxStaticText* StaticText2;
		wxButton* btnClose;
		wxStaticText* StaticText1;
		wxButton* btnJump;
		wxStaticText* lbNumScans;
		wxButton* btnPickInput;
		wxSpinCtrl* edIndex;
		wxFlexGridSizer* FlexGridSizer8;
		wxCheckBox* cbAllowMix;
		wxRadioButton* rbFile;
		wxStaticBoxSizer* StaticBoxSizer1;
		wxStaticText* lbNumPoints;
		wxButton* btnPlay;
		CMyGLCanvas* m_plot3D;
		wxTextCtrl* edFile;
		//*)

	protected:

		//(*Identifiers(CScanAnimation)
		static const long ID_RADIOBUTTON1;
		static const long ID_RADIOBUTTON2;
		static const long ID_STATICTEXT22;
		static const long ID_TEXTCTRL11;
		static const long ID_BUTTON5;
		static const long ID_BUTTON1;
		static const long ID_BUTTON2;
		static const long ID_STATICTEXT4;
		static const long ID_SPINCTRL2;
		static const long ID_CHECKBOX1;
		static const long ID_BUTTON3;
		static const long ID_XY_GLCANVAS;
		static const long ID_SLIDER1;
		static const long ID_STATICTEXT1;
		static const long ID_SPINCTRL1;
		static const long ID_BUTTON4;
		static const long ID_STATICTEXT2;
		static const long ID_STATICTEXT3;
		//*)

	private:

		//(*Handlers(CScanAnimation)
		void OnbtnPlayClick(wxCommandEvent& event);
		void OnbtnStopClick(wxCommandEvent& event);
		void OnbtnCloseClick(wxCommandEvent& event);
		void OnslPosCmdScrollChanged(wxScrollEvent& event);
		void OnbtnJumpClick(wxCommandEvent& event);
		void OnslPosCmdScroll(wxScrollEvent& event);
		void OnbtnPickInputClick(wxCommandEvent& event);
		void OnInit(wxInitDialogEvent& event);
		void OnrbLoadedSelect(wxCommandEvent& event);
		void OnrbFile(wxCommandEvent& event);
		void OncbAllowMixClick(wxCommandEvent& event);
		//*)

		DECLARE_EVENT_TABLE()

		bool			m_stop;
		bool			m_mixlasers;

		struct TRenderObject
		{
			mrpt::system::TTimeStamp       timestamp;
			mrpt::opengl::CRenderizablePtr obj;
		};
		typedef std::map<std::string,TRenderObject> TListGlObjects;
		TListGlObjects  m_gl_objects;  //!< All the observations added to the map.

		void RebuildMaps();
		void BuildMapAndRefresh( mrpt::slam::CSensoryFrame *sf);

};

#endif
