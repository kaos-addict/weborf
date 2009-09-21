/*
Weborf
Copyright (C) 2007  Salvo "LtWorf" Tomaselli

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

#ifndef WEBORF_OPTIONS_H
#define WEBORF_OPTIONS_H

#define VERSION "0.11"
#define SIGNATURE "Weborf/0.11 (GNU/Linux)"

//----------System
//Uid for superuser
#define ROOTUID 0

//----------Network
//Queue for connect requests
#define MAXQ 40
//Default port
#define PORT "8080"
//Delete the following line to use IPv4 instead.
#define IPV6
//-----------Threads
//Max threads
#define MAXTHREAD 200
//Thread started when free threads are low and when starting
#define INITIALTHREAD 12
//If there are LOWTHREAD or less free threads, will start some new ones
#define LOWTHREAD 3
//If there are more than MAXFREETHREAD, one will be closed
#define MAXFREETHREAD 12
//Polling frequence
#define THREADCONTROL 30

//------------Server
#define INDEX "index.html"
#define BASEDIR "/var/www"
#define READ_TIMEOUT 5000

//------------Buffers
#define INBUFFER 6048
#define FILEBUF 50000
//Maximum size for a page generated by a script or internally
#define MAXSCRIPTOUT  1024000
//Buffer for headers
#define HEADBUF 2048
//Max size for password
#define PWDLIMIT 300
#define INDEXMAXLEN 30
//Buffer to contain the string representation of an integer
#define NBUFFER 15
//buffer to contain a range
#define RBUFFER 128
#define BUFFERED_READER_SIZE 512	//256
//Buffer for text date
#define DATEBUFFER 50
#define URI_LEN 256

//Number of index pages allowed to search
#define MAXINDEXCOUNT 10

//-------------LIMITS
#define POST_MAX_SIZE 2000000

//-------------HTML
#define HTMLHEAD "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\"><html><head><title>Weborf</title></head><body>"
#define HTMLFOOT "<p>Generated by Weborf/0.11 (GNU/Linux)</p></body></html>"

//-------------SCRIPTS
//Timeout for the scripts, in seconds
#define SCRPT_TIMEOUT 15
#define PY_WRAPPER "weborf.pywrap.py"
#define CGI_PHP "/usr/lib/cgi-bin/php5"
//Hides all the errors.
#define HIDE_CGI_ERRORS

//-------------COMPRESSING PAGES
//#define __COMPRESSION //enables support for compressing pages, comment to disable
#ifdef __COMPRESSION
#define SIZE_COMPRESS_MIN 4000
#define SIZE_COMPRESS_MAX 4000000000
#define GZIPNICE 4
#endif

//The following header can be disabled to increase a little the speed
//#define SEND_DATE_HEADER

//-------------RANGE
#define __RANGE

//-------------Logging options
//#define THREADDBG
//#define SOCKETDBG
//#define SERVERDBG
//#define SENDINGDBG
//#define REQUESTDBG
#endif
