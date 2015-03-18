/****************************************************************************
 *
 * $Id: vpMbtKltXmlParser.cpp 5126 2015-01-05 22:07:11Z fspindle $
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
 * Load XML parameters of the Model based tracker (using point features).
 *
 * Authors:
 * Aurelien Yol
 *
 *****************************************************************************/
#include <visp/vpConfig.h>

#ifdef VISP_HAVE_XML2

#include <iostream>
#include <map>

#include <libxml/xmlmemory.h>      /* Fonctions de la lib XML.                */

#include <visp/vpMbtKltXmlParser.h>


/*!
  Default constructor. 
  
*/
vpMbtKltXmlParser::vpMbtKltXmlParser()
  : maskBorder(0), maxFeatures(0), winSize(0), qualityValue(0.), minDist(0.),
    harrisParam(0.), blockSize(0), pyramidLevels(0)
{
  init();
}

/*!
  Default destructor.
*/
vpMbtKltXmlParser::~vpMbtKltXmlParser()
{
}

/*!
  Initialise internal variables (including the map).
*/
void 
vpMbtKltXmlParser::init()
{
  vpMbXmlParser::init();

  nodeMap["klt"] = klt;
  nodeMap["mask_border"] = mask_border;
  nodeMap["max_features"] = max_features;
  nodeMap["window_size"] = window_size;
  nodeMap["quality"] = quality;
  nodeMap["min_distance"] = min_distance;
  nodeMap["harris"] = harris;
  nodeMap["size_block"] = size_block;
  nodeMap["pyramid_lvl"] = pyramid_lvl;
}

/*!
  Parse the file in parameters.
  This method is deprecated, use parse() instead.
  
  \param filename : File to parse.
*/
void
vpMbtKltXmlParser::parse(const char * filename)
{
  std::string file = filename;
  vpXmlParser::parse(file);
}

/*!
  Write info to file.
  
  \warning Useless, so not yet implemented => Throw exception.
*/
void 
vpMbtKltXmlParser::writeMainClass(xmlNodePtr /*node*/)
{
  throw vpException(vpException::notImplementedError, "Not yet implemented." );
}

/*!
  Read the parameters of the class from the file given by its document pointer 
  and by its root node. 
  
  \param doc : Document to parse.
  \param node : Root node. 
*/
void
vpMbtKltXmlParser::readMainClass(xmlDocPtr doc, xmlNodePtr node)
{
  bool camera_node = false;
  bool face_node = false;
  bool klt_node = false;
  
  for(xmlNodePtr dataNode = node->xmlChildrenNode; dataNode != NULL;  dataNode = dataNode->next)  {
    if(dataNode->type == XML_ELEMENT_NODE){
      std::map<std::string, int>::iterator iter_data= this->nodeMap.find((char*)dataNode->name);
      if(iter_data != nodeMap.end()){
        switch (iter_data->second){
        case camera:{
          this->read_camera (doc, dataNode);
          camera_node = true;
          }break;
        case face:{
          this->read_face(doc, dataNode);
          face_node = true;
          }break;
        case klt:{
          this->read_klt(doc, dataNode);
          klt_node = true;
          }break;
        default:{
//          vpTRACE("unknown tag in read_sample : %d, %s", iter_data->second, (iter_data->first).c_str());
          }break;
        }
      }
    }
  }

  if(!camera_node) {
    std::cout << "camera : u0 : "<< this->cam.get_u0() << " (default)" <<std::endl;
    std::cout << "camera : v0 : "<< this->cam.get_v0() << " (default)" <<std::endl;
    std::cout << "camera : px : "<< this->cam.get_px() << " (default)" <<std::endl;
    std::cout << "camera : py : "<< this->cam.get_py() << " (default)" <<std::endl;
  }

  if(!face_node) {
    std::cout << "face : Angle Appear : "<< angleAppear <<" (default)" <<std::endl;
    std::cout << "face : Angle Disappear : "<< angleDisappear <<" (default)" <<std::endl;
  }

  if(!klt_node) {
    std::cout << "klt : Mask Border : "<< maskBorder <<" (default)" <<std::endl;
    std::cout << "klt : Max Features : "<< maxFeatures <<" (default)" <<std::endl;
    std::cout << "klt : Windows Size : "<< winSize <<" (default)" <<std::endl;
    std::cout << "klt : Quality : "<< qualityValue <<" (default)" <<std::endl;
    std::cout << "klt : Min Distance : "<< minDist <<" (default)" <<std::endl;
    std::cout << "klt : Harris Parameter : "<< harrisParam <<" (default)" <<std::endl;
    std::cout << "klt : Block Size : "<< blockSize <<" (default)" <<std::endl;
    std::cout << "klt : Pyramid Levels : "<< pyramidLevels <<" (default)" <<std::endl;
  }
}

/*!
  Read klt information.
  
  \throw vpException::fatalError if there was an unexpected number of data. 
  
  \param doc : Pointer to the document.
  \param node : Pointer to the node of the camera information.
*/
void 
vpMbtKltXmlParser::read_klt(xmlDocPtr doc, xmlNodePtr node)
{
  bool mask_border_node = false;
  bool max_features_node = false;
  bool window_size_node = false;
  bool quality_node = false;
  bool min_distance_node = false;
  bool harris_node = false;
  bool size_block_node = false;
  bool pyramid_lvl_node = false;
  
  for(xmlNodePtr dataNode = node->xmlChildrenNode; dataNode != NULL;  dataNode = dataNode->next)  {
    if(dataNode->type == XML_ELEMENT_NODE){
      std::map<std::string, int>::iterator iter_data= this->nodeMap.find((char*)dataNode->name);
      if(iter_data != nodeMap.end()){
        switch (iter_data->second){
        case mask_border:{
          maskBorder = xmlReadUnsignedIntChild(doc, dataNode);
          mask_border_node = true;
          }break;
        case max_features:{
          maxFeatures = xmlReadUnsignedIntChild(doc, dataNode);
          max_features_node = true;
          }break;
        case window_size:{
          winSize = xmlReadUnsignedIntChild(doc, dataNode);
          window_size_node = true;
          }break;
        case quality:{
          qualityValue = xmlReadDoubleChild(doc, dataNode);
          quality_node = true;
          }break;
        case min_distance:{
          minDist = xmlReadDoubleChild(doc, dataNode);
          min_distance_node = true;
          }break;
        case harris:{
          harrisParam = xmlReadDoubleChild(doc, dataNode);
          harris_node = true;
          }break;
        case size_block:{
          blockSize = xmlReadUnsignedIntChild(doc, dataNode);
          size_block_node = true;
          }break;
        case pyramid_lvl:{
          pyramidLevels = xmlReadUnsignedIntChild(doc, dataNode);
          pyramid_lvl_node = true;
          }break; 
        default:{
//          vpTRACE("unknown tag in read_camera : %d, %s", iter_data->second, (iter_data->first).c_str());
          }break;
        }
      }
    }
  }
  
  if(!mask_border_node)
    std::cout << "klt : Mask Border : "<< maskBorder <<" (default)" <<std::endl;
  else
    std::cout << "klt : Mask Border : "<< maskBorder <<std::endl;
  
  if(!max_features_node)
    std::cout << "klt : Max Features : "<< maxFeatures <<" (default)" <<std::endl;
  else
    std::cout << "klt : Max Features : "<< maxFeatures <<std::endl;
  
  if(!window_size_node)
    std::cout << "klt : Windows Size : "<< winSize <<" (default)" <<std::endl;
  else
    std::cout << "klt : Windows Size : "<< winSize <<std::endl;
  
  if(!quality_node)
    std::cout << "klt : Quality : "<< qualityValue <<" (default)" <<std::endl;
  else
    std::cout << "klt : Quality : "<< qualityValue <<std::endl;
  
  if(!min_distance_node)
    std::cout << "klt : Min Distance : "<< minDist <<" (default)" <<std::endl;
  else
    std::cout << "klt : Min Distance : "<< minDist <<std::endl;
  
  if(!harris_node)
    std::cout << "klt : Harris Parameter : "<< harrisParam <<" (default)" <<std::endl;
  else
    std::cout << "klt : Harris Parameter : "<< harrisParam <<std::endl;
  
  if(!size_block_node)
    std::cout << "klt : Block Size : "<< blockSize <<" (default)" <<std::endl;
  else
    std::cout << "klt : Block Size : "<< blockSize <<std::endl;
  
  if(!pyramid_lvl_node)
    std::cout << "klt : Pyramid Levels : "<< pyramidLevels <<" (default)" <<std::endl;
  else
    std::cout << "klt : Pyramid Levels : "<< pyramidLevels <<std::endl;
}

#endif

