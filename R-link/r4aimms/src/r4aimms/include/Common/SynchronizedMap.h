/*  This file is part of R-Link.

    R-Link is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    R-Link is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with R-Link.  If not, see <http://www.gnu.org/licenses/>.

*/
#ifndef _INCLUDED_Common_SynchronizedMap
#define _INCLUDED_Common_SynchronizedMap

#include "Common/Lock.h"
#include <map>

namespace Common{

    template <typename K, typename V, typename constKbyRef=const K&>
    class SynchronizedMap{
    public:
         typedef std::map<K, V> Key2ValueMap;
    public:
        SynchronizedMap(){};
        ~SynchronizedMap(){};

        bool lookup( constKbyRef key, V& value) const{
            AutoLock al(m_Lock);
            return _nolock_lookup(key,value);
        }

        bool store( constKbyRef key, const V& value, const bool& bAllowOverride = true)
        {
            AutoLock al(m_Lock);
            return _nolock_store(key, value, bAllowOverride);
        }

        void erase( constKbyRef key )
        {
            AutoLock al(m_Lock);
            _nolock_erase(key);
        }

        bool exists( constKbyRef key )
        {
            AutoLock al(m_Lock);
            return _nolock_exists(key);
        }

        void clear()
        {
            AutoLock al(m_Lock);
            _nolock_clear();
        }

        bool _nolock_lookup( constKbyRef key, V& value) const{
            typename Key2ValueMap::const_iterator itMap = m_mapKey2Value.lower_bound(key);
            if (itMap != m_mapKey2Value.end() && (*itMap).first == key) {
                value = (*itMap).second;
                return true;
            }
            return false;
        }

        bool _nolock_store( constKbyRef key, const V& value, const bool& bAllowOverride = true)
        {
            typename Key2ValueMap::iterator itMap = m_mapKey2Value.lower_bound(key);
            if (itMap != m_mapKey2Value.end() && (*itMap).first == key) {
                if (bAllowOverride){
                    (*itMap).second = value;
                    return true;
                }
            }
            else{
                m_mapKey2Value.insert( itMap, typename Key2ValueMap::value_type(key, value) );
                return true;
            }
            return false;
        }

        void _nolock_erase( constKbyRef key )
        {
            typename Key2ValueMap::iterator itMap = m_mapKey2Value.lower_bound(key);
            if (itMap != m_mapKey2Value.end() && (*itMap).first == key) {
                m_mapKey2Value.erase(itMap);
            }
        }

        bool _nolock_exists(constKbyRef key) const
        {
            typename Key2ValueMap::const_iterator itMap = m_mapKey2Value.lower_bound(key);
            return (itMap != m_mapKey2Value.end() && (*itMap).first == key);
        }

        void _nolock_clear()
        {
            m_mapKey2Value.clear();
        }

    public:
        mutable Lock m_Lock;
        Key2ValueMap m_mapKey2Value;
    };
}

#endif // #ifndef _INCLUDED_Common_SynchronizedMap
