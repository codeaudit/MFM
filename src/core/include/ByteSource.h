/*                                              -*- mode:C++ -*-
  ByteSource.h Abstract stream
  Copyright (C) 2014 The Regents of the University of New Mexico.  All rights reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301
  USA
*/

/**
  \file ByteSource.h Abstract stream
  \author David H. Ackley.
  \date (C) 2014 All rights reserved.
  \lgpl
 */
#ifndef IO_H
#define IO_H

#include "itype.h"
#include "Fail.h"
#include <stdarg.h>    /* For ... */

namespace MFM {

  class ByteSource {
  public:
    ByteSource() : m_read(0), m_lastRead(-1), m_unread(false) { }

    s32 Read() {
      if (m_unread) m_unread = false;
      else m_lastRead = ReadByte();

      ++m_read;
      return m_lastRead;
    }

    void Unread() {
      if (m_unread) FAIL(ILLEGAL_STATE);
      m_unread = true;
      --m_read;
    }

    s32 Peek() {
      s32 read = Read();
      Unread();
      return read;
    }

    virtual int ReadByte() = 0;
    virtual ~ByteSource() { }

    u32 GetBytesRead() {
      return m_read;
    }

    s32 Scanf(const char * format, ...) ;
    s32 Vscanf(const char * format, va_list & ap) ;

  private:
    u32 m_read;
    s32 m_lastRead;
    bool m_unread;
  };

}

#endif /* IO_H */