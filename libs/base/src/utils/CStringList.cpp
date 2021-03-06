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

#include <mrpt/base.h>  // Precompiled headers


#include <mrpt/utils/CStringList.h>
#include <mrpt/utils/CFileInputStream.h>
#include <mrpt/utils/CFileOutputStream.h>
#include <mrpt/system/filesystem.h>
#include <mrpt/system/string_utils.h>

using namespace mrpt::utils;
using namespace mrpt::system;
using namespace std;

// This must be added to any CSerializable class implementation file.
IMPLEMENTS_SERIALIZABLE(CStringList, CSerializable, mrpt::utils)

/*---------------------------------------------------------------
						Constructor
  ---------------------------------------------------------------*/
CStringList::CStringList()
{

}

/*---------------------------------------------------------------
						Constructor
  ---------------------------------------------------------------*/
CStringList::CStringList(const string& text)
{
	MRPT_START
	setText( text );
	MRPT_END
}


/*---------------------------------------------------------------
						add
  ---------------------------------------------------------------*/
void  CStringList::add( const string &str )
{
	m_strings.push_back( str );
}

/*---------------------------------------------------------------
						insert
  ---------------------------------------------------------------*/
void  CStringList::insert( size_t index, const string &str )
{
	MRPT_START
	if (index>=m_strings.size()) THROW_EXCEPTION("index out of bounds!");

	m_strings.insert( m_strings.begin()+index, str);

	MRPT_END
}

/*---------------------------------------------------------------
						set
  ---------------------------------------------------------------*/
void  CStringList::set( size_t index, const string &str )
{
	MRPT_START
	if (index>=m_strings.size()) THROW_EXCEPTION("index out of bounds!");

	m_strings[index]= str;

	MRPT_END
}


/*---------------------------------------------------------------
						clear
  ---------------------------------------------------------------*/
void  CStringList::clear()
{
	m_strings.clear();
}


/*---------------------------------------------------------------
						remove
  ---------------------------------------------------------------*/
void  CStringList::remove(size_t index)
{
	MRPT_START
	if (index>=m_strings.size()) THROW_EXCEPTION("index out of bounds!");

	m_strings.erase( m_strings.begin()+index );

	MRPT_END
}


/*---------------------------------------------------------------
						find
  ---------------------------------------------------------------*/
bool  CStringList::find(
	const string		&compareText,
	size_t					foundIndex,
	bool					caseSensitive) const
{
	MRPT_START

	foundIndex = 0;
	if (caseSensitive)
	{
		for (deque<string>::const_iterator it=m_strings.begin();it!=m_strings.end();++it,foundIndex++)
			if (!os::_strcmp(compareText.c_str(),it->c_str()))
				return true;
	}
	else
	if (caseSensitive)
	{
		for (deque<string>::const_iterator it=m_strings.begin();it!=m_strings.end();++it,foundIndex++)
			if (!os::_strcmpi(compareText.c_str(),it->c_str()))
				return true;
	}

	return false;
	MRPT_END
}

/*---------------------------------------------------------------
						getText
  ---------------------------------------------------------------*/
void  CStringList::getText(string &outText) const
{
	MRPT_START
	deque<string>::const_iterator	it;
	size_t								curPos = 0,totalLen = 0;

	// 1) Compute overall length, including 2 chars per new-line:
	// ----------------------------------------------------------------
	for (it=m_strings.begin();it!=m_strings.end();it++)
		totalLen += it->size() + 2;

	outText.resize(totalLen);

	// 2) Copy the text out:
	// ----------------------------------------------------------------
	for (it=m_strings.begin();it!=m_strings.end();it++)
	{
		os::memcpy(&outText[curPos],totalLen,it->c_str(),it->size());
		curPos+=it->size();
		outText[curPos++]='\r';
		outText[curPos++]='\n';
	}

	MRPT_END
}

/*---------------------------------------------------------------
						setText
  ---------------------------------------------------------------*/
void  CStringList::setText(const string &inText)
{
	MRPT_START
	mrpt::system::tokenize( inText,"\r\n",m_strings );
	MRPT_END
}

/*---------------------------------------------------------------
						loadFromFile
  ---------------------------------------------------------------*/
void  CStringList::loadFromFile(const string &fileName)
{
	MRPT_START

	ASSERT_( mrpt::system::fileExists(fileName) );

	CFileInputStream	fil(fileName.c_str());

	// Load the whole file into a string:
	size_t nBytes = fil.getTotalBytesCount();
	string		wholeStr;
	wholeStr.resize( nBytes );

	// "Rewind" :-)
	fil.Seek(0);
	if ( nBytes != fil.ReadBuffer( &wholeStr[0],nBytes ) )
		THROW_EXCEPTION("Error reading text from file!");

	// Parse:
	setText(wholeStr);

	MRPT_END
}

/*---------------------------------------------------------------
						saveToFile
  ---------------------------------------------------------------*/
void  CStringList::saveToFile(const string &fileName) const
{
	MRPT_START

	CFileOutputStream	fil(fileName.c_str());
	deque<string>::const_iterator	it;

	for (it=m_strings.begin();it!=m_strings.end();it++)
	{
		fil.WriteBuffer( it->c_str(), it->size() );
		fil.WriteBuffer( "\r\n",2 );
	}

	MRPT_END
}


/*---------------------------------------------------------------
						writeToStream
 ---------------------------------------------------------------*/
void  CStringList::writeToStream(CStream &out, int *out_Version) const
{
	if (out_Version)
		*out_Version = 0;
	else
	{
		uint32_t	i,N = (uint32_t) m_strings.size();

		out << N;

		for (i=0;i<N;i++)
			out << m_strings[i];
	}

}

/*---------------------------------------------------------------
						readFromStream
 ---------------------------------------------------------------*/
void  CStringList::readFromStream(CStream &in, int version)
{
	switch(version)
	{
	case 0:
		{
			uint32_t	i,N;

			in >> N;

			m_strings.resize(N);

			for (i=0;i<N;i++)
				in >> m_strings[i];

		} break;
	default:
		MRPT_THROW_UNKNOWN_SERIALIZATION_VERSION(version)

	};
}

/*---------------------------------------------------------------
						size
 ---------------------------------------------------------------*/
size_t  CStringList::size() const
{
	return m_strings.size();
}

/*---------------------------------------------------------------
						get
 ---------------------------------------------------------------*/
void  CStringList::get(size_t index, string &outText) const
{
	MRPT_START
	if (index>=m_strings.size()) THROW_EXCEPTION("index out of bounds!");

	outText = m_strings[index];

	MRPT_END
}

/*---------------------------------------------------------------
						operator ()
 ---------------------------------------------------------------*/
string  CStringList::operator ()(size_t index) const
{
	MRPT_START
	if (index>=m_strings.size()) THROW_EXCEPTION("index out of bounds!");

	return m_strings[index];
	MRPT_END
}

/*---------------------------------------------------------------
						operator ()
 ---------------------------------------------------------------*/
string&  CStringList::operator ()(size_t index)
{
	MRPT_START
	if (index>=m_strings.size()) THROW_EXCEPTION("index out of bounds!");

	return m_strings[index];
	MRPT_END
}

/*---------------------------------------------------------------
						get_string
 ---------------------------------------------------------------*/
string  CStringList::get_string( const string &keyName )
{
	MRPT_START
	string		strToLookFor(keyName + string("="));
	size_t			idx = string::npos;

	for (deque<string>::iterator it=m_strings.begin();it!=m_strings.end();++it)
	{
		idx = it->find(strToLookFor);
		if (idx==0)
			return	it->substr( strToLookFor.size() );
	}

	THROW_EXCEPTION( format("Key '%s' not found!",keyName.c_str()) );

	MRPT_END
}

/*---------------------------------------------------------------
						get_float
 ---------------------------------------------------------------*/
float CStringList::get_float( const string &keyName )
{
	MRPT_START
	string	s( get_string(keyName) );
	return (float)atof(s.c_str());
	MRPT_END
}

/*---------------------------------------------------------------
						get_int
 ---------------------------------------------------------------*/
int CStringList::get_int( const string &keyName )
{
	MRPT_START
	string	s( get_string(keyName) );
	return atoi(s.c_str());
	MRPT_END
}

/*---------------------------------------------------------------
						get_double
 ---------------------------------------------------------------*/
double CStringList::get_double( const string &keyName )
{
	MRPT_START
	string	s( get_string(keyName) );
	return atof(s.c_str());
	MRPT_END
}

/*---------------------------------------------------------------
						get_bool
 ---------------------------------------------------------------*/
bool CStringList::get_bool( const string &keyName )
{
	MRPT_START
	string	s( get_string(keyName) );
	return atoi(s.c_str())!=0;
	MRPT_END
}

/*---------------------------------------------------------------
						set
 ---------------------------------------------------------------*/
void  CStringList::set( const string &keyName, const string &value )
{
	MRPT_START
	string		strToLookFor(keyName + string("="));
	size_t			idx = string::npos;

	for (deque<string>::iterator it=m_strings.begin();it!=m_strings.end();++it)
	{
		idx = it->find(strToLookFor);
		if (idx==0)
		{
			// Replace existing string:
			(*it) = strToLookFor + value ;
			return;
		}
	}

	// It is a new key: Append it!
	m_strings.push_back( strToLookFor + value );

	MRPT_END
}
/*---------------------------------------------------------------
						set
 ---------------------------------------------------------------*/
void  CStringList::set( const string &keyName, const int &value )
{
	MRPT_START
	set(keyName,format("%i",value));
	MRPT_END
}
/*---------------------------------------------------------------
						set
 ---------------------------------------------------------------*/
void  CStringList::set( const string &keyName, const float &value )
{
	MRPT_START
	set(keyName,format("%.10e",value));
	MRPT_END
}

/*---------------------------------------------------------------
						set
 ---------------------------------------------------------------*/
void  CStringList::set( const string &keyName, const double &value )
{
	MRPT_START
	set(keyName,format("%.16e",value));
	MRPT_END
}

/*---------------------------------------------------------------
						set
 ---------------------------------------------------------------*/
void  CStringList::set( const string &keyName, const bool &value )
{
	MRPT_START
	set(keyName,string(value ? "1":"0"));
	MRPT_END
}

