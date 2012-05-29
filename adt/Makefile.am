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

lib_LTLIBRARIES = libadt.la
libadt_la_SOURCES = list.c arr.c avl.c stack.c queue.c
libadt_la_LDFLAGS = -version-info 0:1:0

#TEST SECTION
check_PROGRAMS = list arr avl stack queue
TESTS = $(check_PROGRAMS)
list_SOURCES = test/list.c
list_LDADD = libadt.la
arr_SOURCES = test/arr.c
arr_LDADD = libadt.la
avl_SOURCES = test/avl.c
avl_LDADD = libadt.la
stack_SOURCES = test/stack.c
stack_LDADD = libadt.la
queue_SOURCES = test/queue.c
queue_LDADD = libadt.la
