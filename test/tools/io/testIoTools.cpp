/****************************************************************************
 *
 * $Id: testIoTools.cpp 5210 2015-01-26 10:51:11Z strinh $
 *
 * This file is part of the ViSP software.
 * Copyright (C) 2005 - 2014 by INRIA. All rights reserved.
 * 
 * This software is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * ("GPL") version 2 as published by the Free Software Foundation.
 * See the file LICENSE.txt at the root directory of this source
 * distribution for additional information about the GNU GPL.
 *
 * For using ViSP with software that can not be combined with the GNU
 * GPL, please contact INRIA about acquiring a ViSP Professional 
 * Edition License.
 *
 * See http://www.irisa.fr/lagadic/visp/visp.html for more information.
 * 
 * This software was developed at:
 * INRIA Rennes - Bretagne Atlantique
 * Campus Universitaire de Beaulieu
 * 35042 Rennes Cedex
 * France
 * http://www.irisa.fr/lagadic
 *
 * If you have questions regarding the use of this file, please contact
 * INRIA at visp@inria.fr
 * 
 * This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
 * WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 *
 * Description:
 * Test functions in vpIoTools.
 *
 * Authors:
 * Souriya Trinh
 *
 *****************************************************************************/

/*!

  \example testIoTools.cpp

  \brief Test functions in IoTools.

*/

#include <stdio.h>
#include <string.h>
#include <iostream>
#include <visp/vpIoTools.h>


int
main(int argc, const char ** argv)
{
	const char c = vpIoTools::separator;
	if(c == '\\')
	{
		std::cout << "The directory separator character is '" << c << "' (Windows platform)." << std::endl;
	}
	else {
		std::cout << "The directory separator character is '" << c << "' (Unix like platform)." << std::endl;
	}


	std::string pathname = "";
#if defined(_WIN32)
	pathname = "C:\\Program Files (x86)\\Java\\jre7";
#else
	pathname = "/usr/bin/java";
#endif

	std::cout << "Parent of " << pathname << " is " << vpIoTools::getParent(pathname) << std::endl;
	std::cout << "Name of " << pathname << " is " << vpIoTools::getName(pathname) << std::endl;


	if(argc == 3 && std::string(argv[1]) == std::string("-i"))
	{
		std::cout << "Parent of " << argv[2] << " is " << vpIoTools::getParent(argv[2]) << std::endl;
		std::cout << "Name of " << argv[2] << " is " << vpIoTools::getName(argv[2]) << std::endl;
	}

	std::string windowsPathnameStyle = "\\usr\\bin\\java";
	std::cout << "Parent of " << windowsPathnameStyle << " is " << vpIoTools::getParent(windowsPathnameStyle) << std::endl;
	std::cout << "Name of " << windowsPathnameStyle << " is " << vpIoTools::getName(windowsPathnameStyle) << std::endl;

	std::string parent = "/usr/toto/", child = "\\blabla\\java";
	std::cout << "parent=" << vpIoTools::path(parent) << " ; child=" << vpIoTools::path(child) << std::endl;
	std::cout << "Create file path from parent=" << parent << " and child=" << child << " is "
			<< vpIoTools::createFilePath(parent, child) << std::endl;

	std::string expandPath = "~/Documents/fictional directory/fictional file";
	std::cout << "Path for " << expandPath << " is " << vpIoTools::path(expandPath) << std::endl;

	std::cout << "Test get name with an empty pathname=" << vpIoTools::getName("") << std::endl;
	std::cout << "Get parent with an empty pathname=" << vpIoTools::getParent("") << std::endl;
	std::cout << "Get parent with a filename=" << vpIoTools::getParent("my_file.txt") << std::endl;
	expandPath = "~/Documents/fictional dir/fictional file.txt";
	std::cout << "Get name with a unix expand pathname " << expandPath << "=" << vpIoTools::getName(expandPath) << std::endl;
	std::cout << "Get parent with a unix expand pathname " << expandPath << "=" << vpIoTools::getParent(expandPath) << std::endl;


	pathname = "c:/dir";
	std::cout << "pathname=" << vpIoTools::splitDrive(pathname).first << " ; " << vpIoTools::splitDrive(pathname).second << std::endl;

	std::cout << "isAbsolutePath of " << pathname << "=" << vpIoTools::isAbsolutePathname(pathname) << std::endl;

	pathname = "c:/dir/fictional directory/fictional file.txt";
	std::cout << "isAbsolutePath of " << pathname << "=" << vpIoTools::isAbsolutePathname(pathname) << std::endl;

	pathname = "/home/user/Documents/fictional directory/fictional file.txt";
	std::cout << "isAbsolutePath of " << pathname << "=" << vpIoTools::isAbsolutePathname(pathname) << std::endl;

	pathname = "~/Documents/fictional directory/fictional file.txt";
	std::cout << "isAbsolutePath of " << pathname << "=" << vpIoTools::isAbsolutePathname(pathname) << std::endl;

	pathname = "fictional directory/fictional file.txt";
	std::cout << "isAbsolutePath of " << pathname << "=" << vpIoTools::isAbsolutePathname(pathname) << std::endl;


	//Test vpIoTools::splitDrive
  unsigned int nbFail = 0, nbOk = 0;
#if defined(_WIN32)
	if(strcmp(vpIoTools::splitDrive("c:\\foo\\bar").first.c_str(), "c:") == 0) {
	  nbOk++;
	}
	else {
	  nbFail++;
	  std::cout << "Fail=" << vpIoTools::splitDrive("c:\\foo\\bar").first << " should be=c:" << std::endl;
  }
	if(strcmp(vpIoTools::splitDrive("c:\\foo\\bar").second.c_str(), "\\foo\\bar") == 0) {
    nbOk++;
  }
  else {
    nbFail++;
    std::cout << "Fail=" << vpIoTools::splitDrive("c:\\foo\\bar").second << " should be=\\foo\\bar" << std::endl;
  }

	if(strcmp(vpIoTools::splitDrive("c:/foo/bar").first.c_str(), "c:") == 0) {
    nbOk++;
  }
  else {
    nbFail++;
    std::cout << "Fail=" << vpIoTools::splitDrive("c:/foo/bar").first << " should be=c:" << std::endl;
  }
  if(strcmp(vpIoTools::splitDrive("c:/foo/bar").second.c_str(), "/foo/bar") == 0) {
    nbOk++;
  }
  else {
    nbFail++;
    std::cout << "Fail=" << vpIoTools::splitDrive("c:/foo/bar").second << " should be=/foo/bar" << std::endl;
  }

  if(strcmp(vpIoTools::splitDrive("\\\\conky\\mountpoint\\foo\\bar").first.c_str(), "\\\\conky\\mountpoint") == 0) {
    nbOk++;
  }
  else {
    nbFail++;
    std::cout << "Fail=" << vpIoTools::splitDrive("\\\\conky\\mountpoint\\foo\\bar").first << " should be=\\\\conky\\mountpoint" << std::endl;
  }
  if(strcmp(vpIoTools::splitDrive("\\\\conky\\mountpoint\\foo\\bar").second.c_str(), "\\foo\\bar") == 0) {
    nbOk++;
  }
  else {
    nbFail++;
    std::cout << "Fail=" << vpIoTools::splitDrive("\\\\conky\\mountpoint\\foo\\bar").second << " should be=\\foo\\bar" << std::endl;
  }

  if(strcmp(vpIoTools::splitDrive("//conky/mountpoint/foo/bar").first.c_str(), "//conky/mountpoint") == 0) {
    nbOk++;
  }
  else {
    nbFail++;
    std::cout << "Fail=" << vpIoTools::splitDrive("//conky/mountpoint/foo/bar").first << " should be=//conky/mountpoint" << std::endl;
  }
  if(strcmp(vpIoTools::splitDrive("//conky/mountpoint/foo/bar").second.c_str(), "/foo/bar") == 0) {
    nbOk++;
  }
  else {
    nbFail++;
    std::cout << "Fail=" << vpIoTools::splitDrive("//conky/mountpoint/foo/bar").second << " should be=/foo/bar" << std::endl;
  }

  if(strcmp(vpIoTools::splitDrive("\\\\\\conky\\mountpoint\\foo\\bar").first.c_str(), "") == 0) {
    nbOk++;
  }
  else {
    nbFail++;
    std::cout << "Fail=" << vpIoTools::splitDrive("\\\\\\conky\\mountpoint\\foo\\bar").first << " should be=" << std::endl;
  }
  if(strcmp(vpIoTools::splitDrive("\\\\\\conky\\mountpoint\\foo\\bar").second.c_str(),
      "\\\\\\conky\\mountpoint\\foo\\bar") == 0) {
    nbOk++;
  }
  else {
    nbFail++;
    std::cout << "Fail=" << vpIoTools::splitDrive("\\\\\\conky\\mountpoint\\foo\\bar").second << " should be=\\\\\\conky\\mountpoint\\foo\\bar" << std::endl;
  }

  if(strcmp(vpIoTools::splitDrive("///conky/mountpoint/foo/bar").first.c_str(), "") == 0) {
    nbOk++;
  }
  else {
    nbFail++;
    std::cout << "Fail=" << vpIoTools::splitDrive("///conky/mountpoint/foo/bar").first << " should be=" << std::endl;
  }
  if(strcmp(vpIoTools::splitDrive("///conky/mountpoint/foo/bar").second.c_str(), "///conky/mountpoint/foo/bar") == 0) {
    nbOk++;
  }
  else {
    nbFail++;
    std::cout << "Fail=" << vpIoTools::splitDrive("///conky/mountpoint/foo/bar").second << " should be=///conky/mountpoint/foo/bar" << std::endl;
  }

  if(strcmp(vpIoTools::splitDrive("\\\\conky\\\\mountpoint\\foo\\bar").first.c_str(), "") == 0) {
    nbOk++;
  }
  else {
    nbFail++;
    std::cout << "Fail=" << vpIoTools::splitDrive("\\\\conky\\\\mountpoint\\foo\\bar").first << " should be=" << std::endl;
  }
  if(strcmp(vpIoTools::splitDrive("\\\\conky\\\\mountpoint\\foo\\bar").second.c_str(),
      "\\\\conky\\\\mountpoint\\foo\\bar") == 0) {
    nbOk++;
  }
  else {
    nbFail++;
    std::cout << "Fail=" << vpIoTools::splitDrive("\\\\conky\\\\mountpoint\\foo\\bar").second << " should be=\\\\conky\\\\mountpoint\\foo\\bar" << std::endl;
  }

  if(strcmp(vpIoTools::splitDrive("//conky//mountpoint/foo/bar").first.c_str(), "") == 0) {
    nbOk++;
  }
  else {
    nbFail++;
    std::cout << "Fail=" << vpIoTools::splitDrive("//conky//mountpoint/foo/bar").first << " should be=" << std::endl;
  }
  if(strcmp(vpIoTools::splitDrive("//conky//mountpoint/foo/bar").second.c_str(), "//conky//mountpoint/foo/bar") == 0) {
    nbOk++;
  }
  else {
    nbFail++;
    std::cout << "Fail=" << vpIoTools::splitDrive("//conky//mountpoint/foo/bar").second << " should be=//conky//mountpoint/foo/bar" << std::endl;
  }

  std::cout << "Test vpIoTools::splitDrive (Win32) - passed: " << nbOk << "/" << (nbOk+nbFail) << std::endl;
#endif


  //Test vpIoTools::getFileExtension
#if defined(_WIN32)
  nbFail = 0;
  nbOk = 0;

  if(strcmp(vpIoTools::getFileExtension("foo.ext").c_str(), ".ext") == 0) {
    nbOk++;
  }
  else {
    nbFail++;
    std::cout << "Fail=" << vpIoTools::getFileExtension("foo.ext") << " should be=.ext" << std::endl;
  }

  if(strcmp(vpIoTools::getFileExtension("/foo/foo.ext").c_str(), ".ext") == 0) {
    nbOk++;
  }
  else {
    nbFail++;
    std::cout << "Fail=" << vpIoTools::getFileExtension("/foo/foo.ext") << " should be=.ext" << std::endl;
  }

  if(strcmp(vpIoTools::getFileExtension(".ext").c_str(), "") == 0) {
    nbOk++;
  }
  else {
    nbFail++;
    std::cout << "Fail=" << vpIoTools::getFileExtension(".ext") << " should be=" << std::endl;
  }

  if(strcmp(vpIoTools::getFileExtension("\\foo.ext\\foo").c_str(), "") == 0) {
    nbOk++;
  }
  else {
    nbFail++;
    std::cout << "Fail=" << vpIoTools::getFileExtension("\\foo.ext\\foo") << " should be=" << std::endl;
  }

  if(strcmp(vpIoTools::getFileExtension("foo.ext\\").c_str(), "") == 0) {
    nbOk++;
  }
  else {
    nbFail++;
    std::cout << "Fail=" << vpIoTools::getFileExtension("foo.ext\\") << " should be=" << std::endl;
  }

  if(strcmp(vpIoTools::getFileExtension("").c_str(), "") == 0) {
    nbOk++;
  }
  else {
    nbFail++;
    std::cout << "Fail=" << vpIoTools::getFileExtension("") << " should be=" << std::endl;
  }

  if(strcmp(vpIoTools::getFileExtension("foo.bar.ext").c_str(), ".ext") == 0) {
    nbOk++;
  }
  else {
    nbFail++;
    std::cout << "Fail=" << vpIoTools::getFileExtension("foo.bar.ext") << " should be=.ext" << std::endl;
  }

  if(strcmp(vpIoTools::getFileExtension("xx/foo.bar.ext").c_str(), ".ext") == 0) {
    nbOk++;
  }
  else {
    nbFail++;
    std::cout << "Fail=" << vpIoTools::getFileExtension("xx/foo.bar.ext") << " should be=.ext" << std::endl;
  }

  if(strcmp(vpIoTools::getFileExtension("xx\\foo.bar.ext").c_str(), ".ext") == 0) {
    nbOk++;
  }
  else {
    nbFail++;
    std::cout << "Fail=" << vpIoTools::getFileExtension("xx\\foo.bar.ext") << " should be=.ext" << std::endl;
  }

  if(strcmp(vpIoTools::getFileExtension("c:a/b\\c.d").c_str(), ".d") == 0) {
    nbOk++;
  }
  else {
    nbFail++;
    std::cout << "Fail=" << vpIoTools::getFileExtension("c:a/b\\c.d") << " should be=.d" << std::endl;
  }

  std::cout << "Test vpIoTools::getFileExtension (WIN32 platform) - passed: " << nbOk << "/" << (nbOk+nbFail) << std::endl;
#else
  nbFail = 0;
  nbOk = 0;

  if(strcmp(vpIoTools::getFileExtension("foo.bar").c_str(), ".bar") == 0) {
    nbOk++;
  }
  else {
    nbFail++;
    std::cout << "Fail=" << vpIoTools::getFileExtension("foo.bar") << " should be=.bar" << std::endl;
  }

  if(strcmp(vpIoTools::getFileExtension("foo.boo.bar").c_str(), ".bar") == 0) {
    nbOk++;
  }
  else {
    nbFail++;
    std::cout << "Fail=" << vpIoTools::getFileExtension("foo.boo.bar") << " should be=.bar" << std::endl;
  }

  if(strcmp(vpIoTools::getFileExtension("foo.boo.biff.bar").c_str(), ".bar") == 0) {
    nbOk++;
  }
  else {
    nbFail++;
    std::cout << "Fail=" << vpIoTools::getFileExtension("foo.boo.biff.bar") << " should be=.bar" << std::endl;
  }

  if(strcmp(vpIoTools::getFileExtension(".csh.rc").c_str(), ".rc") == 0) {
    nbOk++;
  }
  else {
    nbFail++;
    std::cout << "Fail=" << vpIoTools::getFileExtension(".csh.rc") << " should be=.rc" << std::endl;
  }

  if(strcmp(vpIoTools::getFileExtension("nodots").c_str(), "") == 0) {
    nbOk++;
  }
  else {
    nbFail++;
    std::cout << "Fail=" << vpIoTools::getFileExtension("nodots") << " should be=" << std::endl;
  }

  if(strcmp(vpIoTools::getFileExtension(".cshrc").c_str(), "") == 0) {
    nbOk++;
  }
  else {
    nbFail++;
    std::cout << "Fail=" << vpIoTools::getFileExtension(".cshrc") << " should be=" << std::endl;
  }

  if(strcmp(vpIoTools::getFileExtension("...manydots").c_str(), "") == 0) {
    nbOk++;
  }
  else {
    nbFail++;
    std::cout << "Fail=" << vpIoTools::getFileExtension("...manydots") << " should be=" << std::endl;
  }

  if(strcmp(vpIoTools::getFileExtension("...manydots.ext").c_str(), ".ext") == 0) {
    nbOk++;
  }
  else {
    nbFail++;
    std::cout << "Fail=" << vpIoTools::getFileExtension("...manydots.ext") << " should be=.ext" << std::endl;
  }

  if(strcmp(vpIoTools::getFileExtension(".").c_str(), "") == 0) {
    nbOk++;
  }
  else {
    nbFail++;
    std::cout << "Fail=" << vpIoTools::getFileExtension(".") << " should be=" << std::endl;
  }

  if(strcmp(vpIoTools::getFileExtension("..").c_str(), "") == 0) {
    nbOk++;
  }
  else {
    nbFail++;
    std::cout << "Fail=" << vpIoTools::getFileExtension("..") << " should be=" << std::endl;
  }

  if(strcmp(vpIoTools::getFileExtension("........").c_str(), "") == 0) {
    nbOk++;
  }
  else {
    nbFail++;
    std::cout << "Fail=" << vpIoTools::getFileExtension("........") << " should be=" << std::endl;
  }

  if(strcmp(vpIoTools::getFileExtension("").c_str(), "") == 0) {
    nbOk++;
  }
  else {
    nbFail++;
    std::cout << "Fail=" << vpIoTools::getFileExtension("") << " should be=" << std::endl;
  }

  std::cout << "Test vpIoTools::getFileExtension (Unix-like platform) - passed: " << nbOk << "/" << (nbOk+nbFail) << std::endl;
#endif


	std::cout << std::endl << "End" << std::endl;

	return 0;
}
