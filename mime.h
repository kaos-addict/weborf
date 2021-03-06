/*
Weborf
Copyright (C) 2010  Salvo "LtWorf" Tomaselli

Weborf is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

@author Salvo "LtWorf" Tomaselli <tiposchi@tiscali.it>
*/

#ifndef WEBORF_MIME_H
#define WEBORF_MIME_H


#include "options.h"
#include "types.h"


int mime_init(magic_t *token);
void mime_release(magic_t token);
const char* mime_get_fd (magic_t token,int fd,struct stat *sb);

/* This is to support old versions of libmagic that do not define
 * MAGIC_MIME_TYPE
 */
#ifdef SEND_MIMETYPES
#ifndef MAGIC_MIME_TYPE
#define MAGIC_MIME_TYPE MAGIC_MIME
#endif
#endif

#endif