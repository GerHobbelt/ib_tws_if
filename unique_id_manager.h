
// Copyright (c) 2011 Ger Hobbelt
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#ifndef UNIQUE_ID_MANAGER_HEADER_INCLUDED
#define UNIQUE_ID_MANAGER_HEADER_INCLUDED

#include "system-includes.h"



template <typename T, unsigned int suggested_bucket_count = 0> class unique_id_manager
{
protected:
    /*
    Use buckets to count the number of 'active' (generated) IDs in that range segment.

    As we ASSUME that most, if not all, (very) long-living IDs are produced in the beginning
    of the run-time life, we can get away with just keeping track which subranges (one per
    bucket) are 'currently unused' in order to remain able to produce unique IDs once the
    id value 'wraps around'.
    */
    typedef unsigned int bucket_int;
    struct bucket_t;
    {
        bucket_int m_counter;
    }

    bucket_t *m_buckets;
    unsigned int m_bucket_count;

    T m_bucket_divisor;
    bucket_int m_last_bucket_full;
    unsigned int m_last_bucket_idx;

    T m_last_id;


protected:
    unique_id_manager() :
        m_last_id(0)
    {
        m_bucket_count = suggested_bucket_count;
        // just a bit of heuristics to produce a usable bucket count: small ID ranges
        // will get relatively more (much more) buckets to track the still-alive IDs
        // out there, since smaller ID range implies wrap-around happens sooner and
        // fragmentation of the range due to long-lived IDs mingling with the
        // short-lived ones is worse.
        if (m_bucket_count < 16)
        {
            m_bucket_count = 256 * sizeof(T);
        }
        m_buckets = new bucket_t[m_bucket_count];
        memset(m_buckets, 0, m_bucket_count * sizeof(m_buckets[0]));

        T r = 0 - 1; // eqv. to ~0 for both signed and unsigned T types
        r /= m_bucket_count;
        r++; // round up; for the pedantic: we ignore cases where (2^N-1 MOD bucket_count == 0) as we've bigger fish to fry here.
        m_bucket_divisor = (T)r;

        // now calculate what the max. count is in the last bucket,
        // and who that 'last bucket' may be:
        T l = 0 - 1;
        l /= r;

        T f = 0 - 1;
        f -= m_bucket_count * r; // T type value overflow will very probably happen! Don't worry.
        f += r * (m_bucket_count - l); // as we now 'overflow' in the other direction again, iff we 'overflowed' just before...

        if (f == 0)
        {
            f += m_bucket_count;
            l--;
        }
        m_last_bucket_idx = (unsigned int)l;
        m_last_bucket_full = (bucket_int)f;
    }
    virtual ~unique_id_manager()
    {
        delete m_buckets;
    }

public:
    // singleton for N singletons: obtain the reference to your desired instance
    static unique_id_manager &get_manager(int manager_id)
    {
        static unique_id_manager *mgr = NULL;

        if (!mgr)
        {
            mgr = new unique_id_manager();
        }
        return mgr;
    }

public:
    void release_unique_id(T id)
    {
        unsigned int pos = (unsigned int)(id / m_bucket_divisor);

        // freak out when we wrap here!
        if (m_buckets[pos].m_counter == 0)
        {
            throw "UniqueID Manager usage fault: calling code appears to 'release' IDs twice or more is 'releasing' IDs it didn't receive during this run-time!";
        }
        m_buckets[pos].m_counter--;
    }

    T obtain_next_unique_id(void)
    {
        /*
        The trick is that we can remain certain that we're handing out proper 'unique' IDs as long as
        we'll have buckets with '0' entries when we 'enter' them while generating the new next 'new' ID.
    
        The final straw won't be reached unless we've a very mixed lifetime distribution of IDs
        across our history (i.e. few or even no buckets ever reach zero(0) due to 'released' IDs)
        and that'd be pretty darn weird. Or rather: it would mean our initial design ASSUMPTION
        for this mechanism is fatally flawed and it's back to the drawing board anyhow!
        */
        unsigned int b1 = (unsigned int)(m_last_id / m_bucket_divisor);
        last_id++;
        unsigned int b2 = (unsigned int)(m_last_id / m_bucket_divisor);
        if (b2 != b1)
        {
            // we crossed the bucket boundary: see whether this bucket is really available to us:
            b1 = b2;
            while (m_buckets[b2].m_counter)
            {
                b2++;
                b2 %= last_bucket_idx; // NOT: b2=(b2 MOD bucket_count) despite that's what would be the initial reaction here
                if (b2 == b1)
                {
                    throw "UniqueID Manager internal fault: no more empty buckets available on this round. Distribution of ID lifetimes does not match our design assumption!";
                }
            }
            // determine the first free ID in this empty bucket:
            last_id = b2 * bucket_divisor;
            if (last_id == 0)
            {
                // never give out ID 0!
                //b2 = (unsigned int)(1 / bucket_divisor);
                // extra work when divisor == 1:
                //if (b2 != 0)
                if (bucket_divisor == 1)
                {
                    b1 = b2 = 1;
                    while (buckets[b2].counter)
                    {
                        b2++;
                        // NOT: b2=(b2 MOD bucket_count) despite that's what would be the initial reaction here.
                        // we've scanned the upper part of the bucket strip already so we don't allow wrapping in order to prevent infinite cycling here:
                        if (b2 == b1 || b2 > last_bucket_idx)
                        {
                            throw "UniqueID Manager internal fault: no more empty buckets available on this round. Distribution of ID lifetimes does not match our design assumption!";
                        }
                    }
                    last_id = b2 * bucket_divisor;
                }
                else
                {
                    last_id  = 1;
                }
            }
        }
        buckets[b2].counter++;
        return last_id;
    }
};


#endif // UNIQUE_ID_MANAGER_HEADER_INCLUDED
