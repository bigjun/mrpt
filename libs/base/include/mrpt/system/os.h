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
#ifndef  FILE_MRPT_OS_H
#define  FILE_MRPT_OS_H

#include <mrpt/config.h>

#include <cstdarg>
#include <cstdlib>
#include <cstring>
#include <deque>
#include <vector>

// Duplicated here since <mrpt/system/os.h> is the only header that cannot include "utils_defs.h"
#include <mrpt/base/link_pragmas.h>  // DLL import/export definitions

#include <mrpt/utils/types.h>  // This must be AFTER <utils_impexp.h>
#include <mrpt/utils/mrpt_macros.h>  // This must be AFTER <utils_impexp.h>

/** Represents an invalid timestamp, where applicable.
  */
#define INVALID_TIMESTAMP (0)

namespace mrpt
{
	/** This namespace provides a OS-independent interface to many useful functions: filenames manipulation, time and date, string parsing, file I/O, threading, memory allocation, etc.
	 *  \sa mrpt::system::os \ingroup mrpt_base_grp
	 */
	namespace system
	{
		/** \defgroup mrpt_system_os OS and compiler abstraction
		  * \ingroup mrpt_base_grp */

		/** This namespace provides a OS-independent interface to low-level functions.
		 *   Most of these functions are converted into calls to standard functions, unless we are into Visual Studio 2005 (or newer). In that case the secure version
		 *     of the standard library functions (prefix "_s") are used instead. \ingroup mrpt_base_grp mrpt_system_os
		 */
		namespace os
		{
			/** \addtogroup mrpt_system_os
			  * @{ */

			/** An OS-independent version of sprintf (Notice the bufSize param, which may be ignored in some compilers)
			  *  \sa utils::format
			  */
			int BASE_IMPEXP sprintf(char *buf, size_t bufSize, const char *format, ...) MRPT_NO_THROWS MRPT_printf_format_check(3,4);

			/** An OS-independent version of vsprintf (Notice the bufSize param, which may be ignored in some compilers)
			  */
			int BASE_IMPEXP vsprintf(char *buf, size_t bufSize, const char *format, va_list args) MRPT_NO_THROWS;

			/** An OS-independent version of vsnprintf (Notice the bufSize param, which may be ignored in some compilers)
			  */
			int BASE_IMPEXP vsnprintf(char *buf, size_t bufSize, const char *format, va_list args) MRPT_NO_THROWS;

			/** An OS-independent version of fopen.
			  * \return It will always return NULL on any error.
			  */
			FILE BASE_IMPEXP *fopen(const char *fileName,const char *mode) MRPT_NO_THROWS;

			/** An OS-independent version of fopen (std::string version)
			  * \return It will always return NULL on any error.
			  */
			FILE BASE_IMPEXP *fopen(const std::string &fileName,const char *mode) MRPT_NO_THROWS;

			/** An OS-independent version of fprintf
			  */
			int BASE_IMPEXP fprintf(FILE *fil, const char *format, ...)  MRPT_NO_THROWS MRPT_printf_format_check(2,3);

			/** An OS-independent version of fscanf
			  * \return The number of fields correctly assigned
			  */
			//int BASE_IMPEXP fscanf(FILE *fil, const char *format, ...)  MRPT_NO_THROWS MRPT_scanf_format_check(2,3);

			/** An OS-independent version of fclose.
			  * \exception std::exception On trying to close a NULL file descriptor.
			  */
			void BASE_IMPEXP fclose(FILE *f);

			/** An OS-independent version of strcat.
			  * \return It will always return the "dest" pointer.
			  */
			char BASE_IMPEXP * strcat(char *dest, size_t destSize, const char *source) MRPT_NO_THROWS;

			/** An OS-independent version of strcpy.
			  * \return It will always return the "dest" pointer.
			  */
			char  BASE_IMPEXP *strcpy(char *dest, size_t destSize, const char *source) MRPT_NO_THROWS;

			/** An OS-independent version of strcmp.
			  * \return It will return 0 when both strings are equal, casi sensitive.
			  */
			int BASE_IMPEXP _strcmp(const char*str1,const char*str2) MRPT_NO_THROWS;

			/** An OS-independent version of strcmpi.
			  * \return It will return 0 when both strings are equal, casi insensitive.
			  */
			int BASE_IMPEXP _strcmpi(const char*str1,const char*str2) MRPT_NO_THROWS;

			/** An OS-independent version of strncmp.
			  * \return It will return 0 when both strings are equal, casi sensitive.
			  */
			int BASE_IMPEXP _strncmp(const char*str,const char*subStr,size_t count) MRPT_NO_THROWS;

			/** An OS-independent version of strnicmp.
			  * \return It will return 0 when both strings are equal, casi insensitive.
			  */
			int BASE_IMPEXP _strnicmp(const char*str,const char*subStr,size_t count) MRPT_NO_THROWS;

			/** An OS-independent version of strtoll.
			  */
			int64_t BASE_IMPEXP _strtoll(const char *nptr, char **endptr, int base);

			/** An OS-independent version of strtoull.
			  */
			uint64_t BASE_IMPEXP _strtoull(const char *nptr, char **endptr, int base);

			/** An OS-independent version of timegm (which is not present in all compilers): converts a time structure into an UTM time_t */
			time_t BASE_IMPEXP timegm(struct tm *tm);

			/** An OS and compiler independent version of "memcpy"
			  */
			void BASE_IMPEXP memcpy(
				void		*dest,
				size_t		destSize,
				const void	*src,
				size_t		copyCount ) MRPT_NO_THROWS;

			/** An OS-independent version of getch, which waits until a key is pushed.
			  * \return The pushed key code
			  */
			int BASE_IMPEXP getch() MRPT_NO_THROWS;

			/** An OS-independent version of kbhit, which returns true if a key has been pushed.
			  */
			bool BASE_IMPEXP kbhit() MRPT_NO_THROWS;

			/** @} */

		}	// end namespace "os"

		/** \addtogroup mrpt_system_os
		  * @{ */

		/** Shows the message "Press any key to continue" (or other custom message) to the current standard output and returns when a key is pressed.
		  */
		void BASE_IMPEXP pause(const std::string &msg = std::string("Press any key to continue...") ) MRPT_NO_THROWS;

		/** Clears the console window */
		void BASE_IMPEXP clearConsole();

		/** A useful function for debuging, which saves a std::vector into a text file (compat. with MATLAB)
		* \return Returns false on any error, true on everything OK.
		*/
		bool  BASE_IMPEXP vectorToTextFile( const std::vector<float> &vec, const std::string &fileName, bool append = false, bool byRows=false );
		//! \overload
		bool  BASE_IMPEXP vectorToTextFile( const std::vector<double> &vec, const std::string &fileName, bool append = false, bool byRows=false );
		//! \overload
		bool  BASE_IMPEXP vectorToTextFile( const std::vector<int> &vec, const std::string &fileName, bool append = false, bool byRows=false );
		//! \overload
		bool  BASE_IMPEXP vectorToTextFile( const std::vector<size_t> &vec, const std::string &fileName, bool append = false, bool byRows=false );
		//! \overload
		template <class Derived>
		bool vectorToTextFile( const Eigen::MatrixBase<Derived> &vec, const std::string &fileName ) {
			try {
				vec.saveToTextFile(fileName);
				return true;
			} catch(...) {return false;}
		}

		/** Load a std::vector from a text file (compat. with MATLAB)
		* \return Returns false on any error, true on everything OK.
		* \sa loadBinaryFile
		*/
		bool  BASE_IMPEXP vectorFromTextFile( std::vector<double> &vec, const std::string &fileName, const bool byRows=false );

		/** Saves a vector directly as a binary dump to a file:
		* \return Returns false on any error, true on everything OK.
		* \sa loadBinaryFile
		*/
		bool BASE_IMPEXP vectorToBinaryFile( const vector_byte &vec, const std::string &fileName );

		/** Loads a entire file as a vector of bytes.
		* \return Returns false on any error, true on everything OK.
		* \sa vectorToBinaryFile
		*/
		bool BASE_IMPEXP loadBinaryFile( vector_byte &out_data, const std::string &fileName );

		/** Returns the MRPT compilation date
		  */
		std::string BASE_IMPEXP MRPT_getCompilationDate();

		/** Returns a string describing the MRPT version including the SVN number.
		  */
		std::string BASE_IMPEXP MRPT_getVersion();

		/** Returns a const ref to a text with the same text that appears at the beginning of each MRPT file (useful for displaying the License text in GUIs) */
		const std::string BASE_IMPEXP & getMRPTLicense();

		/** Call this to register handlers for fatal erros (memory access,etc) that show useful debug information (It is called automatically normally, no need for the user to explicitly call this method.).
		  */
		void BASE_IMPEXP registerFatalExceptionHandlers();

		/** Dumps the current program stack with detailed information of source files and lines.
		  *  This function requires MRPT linked against wxWidgets. Otherwise, an empty string is returned.
		  *  File names and lines won't be available in release builds.
		  */
		std::string BASE_IMPEXP stack_trace(bool calling_from_exception = false);

		/** Only when built in debug (with _DEBUG), this function will be called just before raising any MRPT exception,
		  *  so the user can conveniently put a breakpoint here to explore the call stack, etc.
		  */
		void BASE_IMPEXP breakpoint(const std::string &exception_msg);

		/** For use in  setConsoleColor */
		enum TConsoleColor
		{
			CONCOL_NORMAL = 0,
			CONCOL_BLUE   = 1,
			CONCOL_GREEN  = 2,
			CONCOL_RED    = 4
		};

		/** Changes the text color in the console for the text written from now on.
		  * The parameter "color" can be any value in TConsoleColor.
		  *
		  * By default the color of "cout" is changed, unless changeStdErr=true, in which case "cerr" is changed.
		  */
		void BASE_IMPEXP setConsoleColor( TConsoleColor color, bool changeStdErr=false );

		/** @} */

	} // End of namespace

} // End of namespace

#endif
