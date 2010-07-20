#! /usr/bin/perl

########################################################
# Instruction:
# 1/ create a clean directory: mkdir oxygen-transparent
# 2/ go to this directory: cd oxygen-transparent
# 3/ copy and run the script:
#      cp download_path/oxygen-setup.pl .
#      ./oxygen-setup.pl
#    this should
#    - checkout the sources from svn,
#    - create a local 'build' directory
#    - run cmake, and compile the code locally
#    - give instructions on how to install
# 4/ logout/login to have the new oxygen applied
# 5/ that's it
# -----------------------------------
# Notes:
# 1/ you can type 'oxygen-settings' in a terminal or in krunner to have full access to oxygen
# configuration options, and notably the background opacity setting (for both the style and the decoration)
#
# 2/ to update the source code from kde svn repository, go to 'oxygen-transparent' and re-run the script.
# 3/ to recompile, go to 'oxygen-transparent/build' and type 'make', and 'make install' (as root, or with sudo)
# 4/ if you have a kde svn account you can replace the $repository variable below to you account.
########################################################

{
    use Term::ANSIColor;
    use Getopt::Long;

    GetOptions(
        'clean',
        'no-svn',
        'no-configure',
        'no-make',
        'help' );

    print( "--- oxygen easy setup script\n" );

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

    my $build_dir= "$work_dir/build";

    ###########################################
    if( !$opt_no_svn )
    {

        print( "--- checking out source code\n" );

        my $repository="svn://anonsvn.kde.org";

        # if you have your own svn account on kde, replace the line above by e.g.
        # my $repository="svn+ssh://username\@svn.kde.org";

        # base directory, where the sources are retrieved from
        my $base_directory="home/kde/trunk/playground/artwork/oxygen-transparent";

        ### common
        chdir_and_echo( $work_dir );
        print( "---  libs\n" );
        if( -d "libs" )
        {

            cmd_and_echo( "svn update libs" );

        } else {

            cmd_and_echo( "svn co $repository/$base_directory/libs" );

        }
        print( "\n" );

        ### decoration
        print( "---  decoration\n" );
        if( -d "decoration" )
        {

            cmd_and_echo( "svn update decoration" );

        } else {

            cmd_and_echo( "svn co $repository/$base_directory/decoration" );

        }
        print( "\n" );

        ### style
        print( "---  style\n" );
        if( -d "style" )
        {

            cmd_and_echo( "svn update style" );

        } else {

            cmd_and_echo( "svn co $repository/$base_directory/style" );

        }

        print( "\n" );

    }

    ###########################################
    if( !$opt_no_configure )
    {

        print( "--- creating CMakeLists.txt\n" );

        chdir_and_echo( $work_dir );
        open( FILE, ">CMakeLists.txt" );
        print FILE ( "find_package (KDE4 REQUIRED)\n" );
        print FILE ( "add_definitions (-DQT3_SUPPORT -DQT3_SUPPORT_WARNINGS)\n" );
        print FILE ( "include (KDE4Defaults)\n" );
        print FILE ( "include (MacroLibrary)\n" );
        print FILE ( "add_definitions (\${QT_DEFINITIONS} \${KDE4_DEFINITIONS})\n" );
        print FILE ( "include_directories (\${KDE4_INCLUDES} \${CMAKE_BINARY_DIR})\n" );
        print FILE ( "include_directories( libs )\n" );
        print FILE ( "add_subdirectory( libs )\n" );
        print FILE ( "add_subdirectory( decoration )\n" );
        print FILE ( "add_subdirectory( style )\n" );
        close FILE;

        print( "\n" );

        print( "--- running cmake\n" );
        chdir_and_echo( "$build_dir" );
        cmd_and_echo( "cmake -DCMAKE_INSTALL_PREFIX=\`kde4-config --prefix\` -DCMAKE_BUILD_TYPE=release $work_dir" );
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
        "common",
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
    print( "  --no-svn              do not check-out and/or update the code from svn\n" );
    print( "  --no-configure        do not generate CMakeLists.txt nor attempt to configure the compilation\n");
    print( "  --no-make             do not attempt to compile the code\n");
    print( "  --clean               removes everything for a fresh setup\n" );
    print( "  --help                display this help and exit\n");
    print( "  --branch=BRANCH       use given BRANCH to fetch sources. Default is trunk\n" );
    goto END;

}
END: {}
