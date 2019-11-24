package oshelper;

#**************************************************************
#  
#  Licensed to the Apache Software Foundation (ASF) under one
#  or more contributor license agreements.  See the NOTICE file
#  distributed with this work for additional information
#  regarding copyright ownership.  The ASF licenses this file
#  to you under the Apache License, Version 2.0 (the
#  "License"); you may not use this file except in compliance
#  with the License.  You may obtain a copy of the License at
#  
#    http://www.apache.org/licenses/LICENSE-2.0
#  
#  Unless required by applicable law or agreed to in writing,
#  software distributed under the License is distributed on an
#  "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
#  KIND, either express or implied.  See the License for the
#  specific language governing permissions and limitations
#  under the License.
#  
#**************************************************************



use English;
use warnings;
use strict;

BEGIN {
    use Exporter   ();
    our ($VERSION, @ISA, @EXPORT, @EXPORT_OK, %EXPORT_TAGS);

    $VERSION     = 1.00;
    # if using RCS/CVS, this may be preferred
    $VERSION = do { my @r = (q$Revision: 1.1 $ =~ /\d+/g); sprintf "%d."."%02d" x $#r, @r }; # must be all one line, for MakeMaker
    @ISA         = qw(Exporter);
    @EXPORT      = qw(&getEnvironment &isWindowsEnvironment &isUnixEnvironment &getUsername);
    %EXPORT_TAGS = ( );     # eg: TAG => [ qw!name1 name2! ],
    # your exported package globals go here,
    # as well as any optionally exported functions
    @EXPORT_OK   = ( ); # qw($Var1 %Hashit &func3);
}


# ------------------------------------------------------------------------------
sub getEnvironment()
{
    my $sEnvironment;
    if ($OSNAME eq "MSWin32" || $OSNAME eq "cygwin")
    {
        $sEnvironment = "wntmsci";
    }
    elsif ( $OSNAME eq "linux")
    {
        $sEnvironment = "unxlngi";
    }
    elsif ( $OSNAME eq "solaris")
    {
        $sEnvironment = "unxsoli";
    }
    else
    {
        print "Unknown Environment please check OSNAME: '$OSNAME'\n";
        $sEnvironment = "unknown";
    }
    return $sEnvironment;
}

# ------------------------------------------------------------------------------

sub isWindowsEnvironment()
{
    if ($OSNAME eq "MSWin32" ||
        $OSNAME eq "cygwin")
    {
        return 1;
    }
    return 0;
}

sub isUnixEnvironment()
{
    if ($OSNAME eq "linux" ||
        $OSNAME eq "solaris")
    {
        return 1;
    }
    return 0;
}

sub getUsername()
{
    my $sUser = $ENV{USER};
    if (!$sUser)
    {
        $sUser = $ENV{USERNAME};
    }
    if (!$sUser)
    {
        die "Username not set.\n";
    }
    return $sUser;
}

1;
