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
#ifndef  mrpt_bimap_H
#define  mrpt_bimap_H

// Note: This file is included from "stl_extensions.h"

#include <mrpt/utils/utils_defs.h>
#include <map>

namespace mrpt
{
	namespace utils
	{
		/** A bidirectional version of std::map, declared as bimap<KEY,VALUE> and which actually contains two std::map's, one for keys and another for values.
		  * To use this class, insert new pairs KEY<->VALUE with bimap::insert. Then, you can access the KEY->VALUE map with bimap::direct(), and the VALUE->KEY map with bimap::inverse(). The consistency of the two internal maps is assured at any time.
		  *
		  * \note This class can be accessed through iterators to the map KEY->VALUE only.
		  * \note Both typenames KEY and VALUE must be suitable for being employed as keys in a std::map, i.e. they must be comparable through a "< operator".
		  * \ingroup stlext_grp
		  */
		template <typename KEY,typename VALUE>
		class bimap
		{
		private:
			std::map<KEY,VALUE>	m_k2v;
			std::map<VALUE,KEY>	m_v2k;

		public:
			typedef typename std::map<KEY,VALUE>::const_iterator const_iterator;
			typedef typename std::map<KEY,VALUE>::iterator iterator;

			typedef typename std::map<VALUE,KEY>::const_iterator const_iterator_inverse;
			typedef typename std::map<VALUE,KEY>::iterator iterator_inverse;

			/** Default constructor - does nothing */
			bimap() { }

			inline const_iterator begin() const { return m_k2v.begin(); }
			inline iterator       begin() { return m_k2v.begin(); }
			inline const_iterator end() const { return m_k2v.end(); }
			inline iterator       end() { return m_k2v.end(); }

			inline const_iterator_inverse inverse_begin() const { return m_v2k.begin(); }
			inline iterator_inverse       inverse_begin() { return m_v2k.begin(); }
			inline const_iterator_inverse inverse_end() const { return m_v2k.end(); }
			inline iterator_inverse       inverse_end() { return m_v2k.end(); }

			inline size_t size() const { return m_k2v.size(); }
			inline bool empty() const { return m_k2v.empty(); }

			/** Return a read-only reference to the internal map KEY->VALUES */
			const std::map<KEY,VALUE> &getDirectMap() const { return m_k2v; }
			/** Return a read-only reference to the internal map KEY->VALUES */
			const std::map<VALUE,KEY> &getInverseMap() const { return m_v2k; }

			void clear() //!< Clear the contents of the bi-map.
			{
				m_k2v.clear();
				m_v2k.clear();
			}

			/** Insert a new pair KEY<->VALUE in the bi-map */
			void insert(const KEY &k,const VALUE &v)
			{
				m_k2v[k]=v;
				m_v2k[v]=k;
			}

			/**  Get the value associated the given key, KEY->VALUE, returning false if not present.
			  *  \sa inverse, hasKey, hasValue
			  * \return false on key not found.
			  */
			bool direct(const KEY &k, VALUE &out_v) const
			{
				const_iterator i=m_k2v.find(k);
				if (i==m_k2v.end()) return false;
				out_v = i->second;
				return true;
			}

			/** Return true if the given key 'k' is in the bi-map  \sa hasValue, direct, inverse */
			inline bool hasKey(const KEY& k) const {
				return m_k2v.find(k)!=m_k2v.end();
			}
			/** Return true if the given value 'v' is in the bi-map \sa hasKey, direct, inverse */
			inline bool hasValue(const VALUE& v) const {
				return m_v2k.find(v)!=m_v2k.end();
			}

			/**  Get the value associated the given key, KEY->VALUE, raising an exception if not present.
			  *  \sa inverse, hasKey, hasValue
			  * \exception std::exception On key not present in the bi-map.
			  */
			VALUE direct(const KEY &k) const
			{
				const_iterator i=m_k2v.find(k);
				if (i==m_k2v.end()) THROW_EXCEPTION("Key not found.");
				return i->second;
			}

			/**  Get the key associated the given value, VALUE->KEY, returning false if not present.
			  *  \sa direct, hasKey, hasValue
			  * \return false on value not found.
			  */
			bool inverse(const VALUE &v, KEY &out_k) const
			{
				const_iterator_inverse i=m_v2k.find(v);
				if (i==m_v2k.end()) return false;
				out_k = i->second;
				return true;
			}

			/**  Get the key associated the given value, VALUE->KEY, raising an exception if not present.
			  *  \sa direct, hasKey, hasValue
			  * \return false on value not found.
			  */
			KEY inverse(const VALUE &v) const
			{
				const_iterator_inverse i=m_v2k.find(v);
				if (i==m_v2k.end()) THROW_EXCEPTION("Value not found.");
				return i->second;
			}


			inline const_iterator find_key(const KEY& k) const  { return m_k2v.find(k); }
			inline iterator       find_key(const KEY& k)        { return m_k2v.find(k); }

			inline const_iterator_inverse find_value(const VALUE& v) const  { return m_v2k.find(v); }
			inline iterator_inverse       find_value(const VALUE& v)        { return m_v2k.find(v); }


		};  // end class bimap

	} // End of namespace
} // End of namespace
#endif
