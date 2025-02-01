package OpenSSL::safe::installdata;

use strict;
use warnings;
use Exporter;
our @ISA = qw(Exporter);
our @EXPORT = qw(
    @PREFIX
    @libdir
    @BINDIR @BINDIR_REL_PREFIX
    @LIBDIR @LIBDIR_REL_PREFIX
    @INCLUDEDIR @INCLUDEDIR_REL_PREFIX
    @APPLINKDIR @APPLINKDIR_REL_PREFIX
    @ENGINESDIR @ENGINESDIR_REL_LIBDIR
    @MODULESDIR @MODULESDIR_REL_LIBDIR
    @PKGCONFIGDIR @PKGCONFIGDIR_REL_LIBDIR
    @CMAKECONFIGDIR @CMAKECONFIGDIR_REL_LIBDIR
    $VERSION @LDLIBS
);

our @PREFIX                     = ( '/Users/wdazhi/Documents/Projects/httpserver/third/openssl' );
our @libdir                     = ( '/Users/wdazhi/Documents/Projects/httpserver/third/openssl/lib' );
our @BINDIR                     = ( '/Users/wdazhi/Documents/Projects/httpserver/third/openssl/bin' );
our @BINDIR_REL_PREFIX          = ( 'bin' );
our @LIBDIR                     = ( '/Users/wdazhi/Documents/Projects/httpserver/third/openssl/lib' );
our @LIBDIR_REL_PREFIX          = ( 'lib' );
our @INCLUDEDIR                 = ( '/Users/wdazhi/Documents/Projects/httpserver/third/openssl/include' );
our @INCLUDEDIR_REL_PREFIX      = ( 'include' );
our @APPLINKDIR                 = ( '/Users/wdazhi/Documents/Projects/httpserver/third/openssl/include/openssl' );
our @APPLINKDIR_REL_PREFIX      = ( 'include/openssl' );
our @ENGINESDIR                 = ( '/Users/wdazhi/Documents/Projects/httpserver/third/openssl/lib/engines-3' );
our @ENGINESDIR_REL_LIBDIR      = ( 'engines-3' );
our @MODULESDIR                 = ( '/Users/wdazhi/Documents/Projects/httpserver/third/openssl/lib/ossl-modules' );
our @MODULESDIR_REL_LIBDIR      = ( 'ossl-modules' );
our @PKGCONFIGDIR               = ( '/Users/wdazhi/Documents/Projects/httpserver/third/openssl/lib/pkgconfig' );
our @PKGCONFIGDIR_REL_LIBDIR    = ( 'pkgconfig' );
our @CMAKECONFIGDIR             = ( '/Users/wdazhi/Documents/Projects/httpserver/third/openssl/lib/cmake/OpenSSL' );
our @CMAKECONFIGDIR_REL_LIBDIR  = ( 'cmake/OpenSSL' );
our $VERSION                    = '3.4.0';
our @LDLIBS                     =
    # Unix and Windows use space separation, VMS uses comma separation
    $^O eq 'VMS'
    ? split(/ *, */, ' ')
    : split(/ +/, ' ');

1;
