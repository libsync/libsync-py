# Copyright (C) 2012 William Kennington
#
# This file is part of Libsync.
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

AUTOMAKE_OPTIONS = subdir-objects
ACLOCAL_AMFLAGS = -I ../m4

lib_LTLIBRARIES = libsync.la
libsync_la_SOURCES = crypt.c openssl.c
libsync_la_LDFLAGS = -version-info 0:1:0
AM_CFLAGS = $(LIBDEPS_CFLAGS)
libsync_la_LIBADD = ../adt/libadt.la $(LIBDEPS_LIBS)
