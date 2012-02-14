#! /usr/bin/perl

########################################################
# Instruction:
# 1/ create a clean directory: mkdir oxygen-transparent
# 2/ go to this directory: cd oxygen-transparent
# 3/ copy and run the script:
#      cp download_path/oxygen-setup.pl .
#      ./oxygen-setup.pl
#    this should
#    - checkout the sources from git,
#    - create a local 'build' directory
#    - run cmake, and compile the code locally
#    - give instructions on how to install
# 4/ select the "Oxygen Transparent" style and window decoration from oxygen settings
# 5/ that's it
# -----------------------------------
# Notes:
# 1/ you can type 'oxygen-transparent-settings' in a terminal or in krunner to have full access to oxygen
# configuration options, and notably the background opacity setting (for both the style and the decoration)
#
# 2/ to update the source code from git repository, cd to 'oxygen-transparent' and re-run the script. Alternatively
#    cd to oxygen-transparent/src, and run 'git pull'
# 3/ to recompile, go to 'oxygen-transparent/build' and type 'make', and 'make install' (as root, or with sudo)
########################################################

{
    use Term::ANSIColor;
    use Getopt::Long;

    GetOptions(
        'clean',
        'no-git',
        'no-configure',
        'no-make',
        'branch:s',
        'help' );

    print( "--- oxygen transparent easy setup script\n" );

    ###########################################
    if($opt_help)
    {
        usage();
        exit(0);
    }

    ###########################################
    if($opt_clean)
    {
        do_clean();
        exit(0);
    }

    print( "\n" );
    my $work_dir = `pwd`;
    chop($work_dir);

    my $source_dir = "$work_dir/src";
    my $build_dir= "$work_dir/build";

    ###########################################
    if( !$opt_no_git )
    {

        print( "--- checking out source code\n" );
        my $repository="git://anongit.kde.org";

        # if you have your own svn account on kde, replace the line above by e.g.
        # my $repository="svn+ssh://username\@svn.kde.org";

        # base directory, where the sources are retrieved from
        my $base_directory="oxygen-transparent";

        ### common
        if( -d $source_dir )
        {

            # directory exists, simply update (pull)
            chdir_and_echo( "$source_dir" );

            # switch to branch
            if( $opt_branch )
            { cmd_and_echo( "git checkout $opt_branch" ); }

            # change to source directory and update
            cmd_and_echo( "git pull" );

        } else {

            # checkout sources
            my $repository="$repository/$base_directory";
            my $command = "git clone ";

            if( $opt_branch )
            { $command =  $command." -b ".$opt_branch." "; }

            $command = $command.$repository;
            cmd_and_echo( $command );

            # checkout sources and rename
            cmd_and_echo( "mv oxygen-transparent src" );

        }

        chdir_and_echo( "$work_dir" );
        print( "\n" );

    }

    ###########################################
    if( !$opt_no_configure )
    {

        print( "--- running cmake\n" );
        chdir_and_echo( "$build_dir" );
        cmd_and_echo( "cmake -DCMAKE_INSTALL_PREFIX=\`kde4-config --prefix\` -DCMAKE_BUILD_TYPE=release $source_dir" );
        chdir_and_echo( "$work_dir" );
        print( "\n" );

    }

    ###########################################
    if( !$opt_no_make )
    {
        print( "--- compiling\n" );
        chdir_and_echo( "$build_dir" );
        cmd_and_echo( "make" );
        chdir_and_echo( "$work_dir" );

        print( "--- done\n" );
        print( "\n" );
    }

    print( "type \"" );
    print colored ['cyan'], 'cd build; make install';
    print( "\" (as root, or using sudo) to install.\n" );
    print( "report problems to \"hugo\@oxygen-icons.org\".\n" );

}

###########################################
# clean everything
sub do_clean
{
    print( "--- cleaning.\n" );

    ### directories
    my @dirs = (
        "oxygen",
        "libs",
        "style",
        "decoration",
        "CMakeFiles",
        "CMakeTmp",
        "lib" );

    foreach $dir (@dirs)
    { if( -d "$dir" ) { cmd_and_echo( "rm -rf $dir" ); } }

    ### files
    my @files = (
        "CMakeCache.txt",
        "cmake_install.cmake",
        "CMakeLists.txt",
        "cmake_uninstall.cmake",
        "CTestTestfile.cmake",
        "install_manifest.txt",
        "Makefile" );
    foreach $file (@files)
    { if( -e "$file" ) { cmd_and_echo( "rm -f $file" ); } }
    print( "--- done.\n" );
    print( "report problems to \"hugo\@oxygen-icons.org\".\n" );

}

###########################################
# run command and log
sub cmd_and_echo
{
  print( "$_[0]\n" );
  goto END if &doSystemFail("$_[0]");
}

###########################################
# create directory and log
sub mkdir_and_echo
{
    if( ! -d $_[0] ) { cmd_and_echo( "mkdir -p $_[0]" ) };
}

###########################################
# change directory and log
sub chdir_and_echo
{
    mkdir_and_echo( "$_[0]" );
    print("cd $_[0]\n");
    chdir "$_[0]";
}

###########################################
# run command and check command return
sub doSystemFail
{
  my $arg = shift(@_) . ">&1";
  my $status = system($arg);
  if ($status)
  { print( "system $arg failed: $?\n" ); }
  return $status;
}

###########################################
# usage
sub usage
{
    print( "usage: oxygen-setup.pl [options]\n");
    print( "\n");
    print( "oxygen-setup.pl checks out the relevant oxygen code from svn; configures it and compiles it.\n");
    print( "installation is left to the user as it usually requires root privileges.\n" );
    print( "\n");

    print( "where options are one or several of the following:\n");
    print( "  --no-git              do not check-out and/or update the code from git\n" );
    print( "  --no-configure        do not generate CMakeLists.txt nor attempt to configure the compilation\n");
    print( "  --no-make             do not attempt to compile the code\n");
    print( "  --clean               removes everything for a fresh setup\n" );
    print( "  --help                display this help and exit\n");
    print( "  --branch=BRANCH       use given BRANCH to fetch sources. Default is trunk\n" );
    goto END;

}
END: {}
