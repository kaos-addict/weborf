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

#define VERSION "0.12"
#define SIGNATURE "Weborf/0.12 (GNU/Linux)"

//----------System
#define ROOTUID 0               //Uid for superuser

//----------Network
#define MAXQ 40                 //Queue for connect requests
#define PORT "8080"             //Default port


#ifdef _POSIX_IPV6              //Enables ipv6 if supported
//Delete the following line to use IPv4 instead.
#define IPV6
#endif

//-----------Threads
#define MAXTHREAD 200           //Max threads
#define INITIALTHREAD 12        //Thread started when free threads are low and when starting
#define LOWTHREAD 3             //Minimum number of free threads, before starting new ones
#define MAXFREETHREAD 12        //Maximum number of free threads, before starting to slowly close them
#define THREADCONTROL 1        //Polling frequence in seconds

//------------Server
#define INDEX "index.html"      //Default index file that weborf will search
#define BASEDIR "/var/www"      //Default basedir
#define READ_TIMEOUT 5000       //Timeout before closing inactive keep-alive connections, in milliseconds

//------------Buffers
#define INBUFFER 2048
#define FILEBUF 4096
#define MAXSCRIPTOUT  512000    //Maximum size for a page generated by a script or internally
#define HEADBUF 2048            //Buffer for headers
#define PWDLIMIT 300            //Max size for password
#define INDEXMAXLEN 30
#define NBUFFER 15              //Buffer to contain the string representation of an integer
#define RBUFFER 128             //Buffer to contain a range
#define BUFFERED_READER_SIZE 1024
#define DATEBUFFER 50           //Buffer for text date
#define URI_LEN 256
#define PATH_LEN 1024

//Number of index pages allowed to search
#define MAXINDEXCOUNT 10

//-------------LIMITS
#define POST_MAX_SIZE 2000000   //Maximum allowed size for POST data

//-------------HTML
#define HTMLHEAD "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\"><html><head><title>Weborf</title></head><body>"
#define HTMLFOOT "<p>Generated by Weborf/0.12 (GNU/Linux)</p></body></html>"

//-------------SCRIPTS
#define SCRPT_TIMEOUT 15        //Timeout for the scripts, in seconds
#define HIDE_CGI_ERRORS         //Hides all the errors.
#define CGI_PHP "/usr/lib/cgi-bin/php5"
#define CGI_PY "/usr/lib/cgi-bin/weborf_py_wrapper"

//-------------COMPRESSING PAGES
//#define __COMPRESSION         //enables support for compressing pages, comment to disable
#ifdef __COMPRESSION
#define SIZE_COMPRESS_MIN 4000
#define SIZE_COMPRESS_MAX 4000000000
#define GZIPNICE 4              //Nice value for gzip process
#endif

//The following header can be disabled to increase a little the speed
//#define SEND_LAST_MODIFIED_HEADER

//-------------RANGE
#define __RANGE                 //Enables support to range (partial download)

//-------------WEBDAV
#define WEBDAV                  //Enables webdav support
#ifdef WEBDAV
#define MAXPROPCOUNT 40         //Number of supported properties
#define HIDE_HIDDEN_FILES       //Hides hidden files
#endif

//-------------Logging options
//#define THREADDBG
//#define SOCKETDBG
//#define SENDINGDBG
#define SERVERDBG
#define REQUESTDBG
#endif
