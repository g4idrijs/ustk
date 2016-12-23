/****************************************************************************
 *
 * This file is part of the UsTk software.
 * Copyright (C) 2014 by Inria. All rights reserved.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License ("GPL") as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 * See the file COPYING at the root directory of this source
 * distribution for additional information about the GNU GPL.
 *
 * This software was developed at:
 * INRIA Rennes - Bretagne Atlantique
 * Campus Universitaire de Beaulieu
 * 35042 Rennes Cedex
 * France
 * http://www.irisa.fr/lagadic
 *
 * If you have questions regarding the use of this file, please contact the
 * authors at Alexandre.Krupa@inria.fr
 *
 * This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
 * WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 *
 * Authors:
 * Marc Pouliquen
 *
 *****************************************************************************/

/**
 * @file usImageIo.h
 * @brief Input/output operations between ultrasound data and image files and their associated header
 * (containing usTransducerSettings, usMotorSettings, and pre-scan or post-scan info).
 */

#ifndef US_IMAGE_IO_H
#define US_IMAGE_IO_H

#include <string>

#include <visp3/ustk_core/usImageRF2D.h>
#include <visp3/ustk_core/usImageRF3D.h>
#include <visp3/ustk_core/usImagePreScan2D.h>
#include <visp3/ustk_core/usImagePreScan3D.h>
#include <visp3/ustk_core/usImagePostScan2D.h>
#include <visp3/ustk_core/usImagePostScan3D.h>
#include <visp3/ustk_io/usMetaHeaderParser.h>
#include <visp3/ustk_io/usImageSettingsXmlParser.h>


/**
 * @class usImageIo
 * @brief Input/output operations between ultrasound data and files (header + image file).
 * @ingroup module_ustk_io
 *
 */
class VISP_EXPORT usImageIo
{
private:

  static std::string getExtension(const std::string &filename);

public:

  /** @name RF io */
  //@{
  static void read(usImageRF2D<unsigned char> &rfImage,const std::string &headerFileName);
  static void read(usImageRF3D<unsigned char> &rfImage3D, const std::string &headerFileName);

  static void write(const usImageRF2D<unsigned char> &rfImage, const std::string &headerFileName,
                    const std::string &imageExtension2D);
  static void write(const usImageRF2D<unsigned char> &rfImage, const std::string &headerFileName);
  static void write(const usImageRF3D<unsigned char> &rfImage3D, const std::string &headerFileName,
                    const std::string &imageExtension2D);
  static void write(const usImageRF3D<unsigned char> &rfImage3D, const std::string &headerFileName);

  //@}

  /** @name Pre-scan io */
  //@{
  static void read(usImagePreScan2D<unsigned char> &preScanImage,const std::string &headerFileName);
  static void read(usImagePreScan3D<unsigned char> &preScanImage3D, const std::string &headerFileName);

  //Doubles types writing not implemented
  static void read(usImagePreScan2D<double> &preScanImage,const std::string &headerFileName);
  static void read(usImagePreScan3D<double> &preScanImage3D,const std::string &headerFileName);

  static void write(const usImagePreScan2D<unsigned char> &preScanImage, const std::string &headerFileName,
                    const std::string &imageExtension2D);
  static void write(const usImagePreScan2D<unsigned char> &preScanImage, const std::string &headerFileName);

  static void write(const usImagePreScan3D<unsigned char> &preScanImage3D, const std::string &headerFileName,
                    const std::string &imageExtension2D);
  static void write(const usImagePreScan3D<unsigned char> &preScanImage3D, const std::string &headerFileName);

  //Doubles types writing not implemented
  static void write(const usImagePreScan2D<double> &preScanImage, const std::string &headerFileName,
                    const std::string &imageExtension2D);
  static void write(const usImagePreScan2D<double> &preScanImage, const std::string &headerFileName);
  static void write(const usImagePreScan3D<double> &preScanImage3D, const std::string &headerFileName,
                    const std::string &imageExtension2D);
  static void write(const usImagePreScan3D<double> &preScanImage3D, const std::string &headerFileName);
  //@}

  /** @name Post-scan io */
  //@{
  static void read(usImagePostScan2D<unsigned char> &postScanImage, const std::string &headerFileName);
  static void read(usImagePostScan3D<unsigned char> &postScanImage3D, const std::string &headerFileName);

  static void write(const usImagePostScan2D<unsigned char> &postScanImage, const std::string &headerFileName,
                    const std::string &imageExtension2D);
  static void write(const usImagePostScan2D<unsigned char> &postScanImage, const std::string &headerFileName);
  static void write(const usImagePostScan3D<unsigned char> &postScanImage3D, const std::string &headerFileName,
                    const std::string &imageExtension2D);
  static void write(const usImagePostScan3D<unsigned char> &postScanImage3D, const std::string &headerFileName);
  //@}

#ifndef DOXYGEN_SHOULD_SKIP_THIS
  struct VolHeader
  {
    int type;     // Data type(0: prescan B, 1 : postscan B, 2 : rf, 3 : vel / var, 5 = B + flow RGB)
    int volumes;  // Number of volumes in the file
    int fpv;      // Number of frames per volumes
    int w;        // frame width
    int h;        // frame height
    int ss;       // sample size in bits
    int degPerFr; // degree step between frames
  };
#endif //DOXYGEN_SHOULD_SKIP_THIS

  typedef enum
  {
    FORMAT_XML,
    FORMAT_MHD,
    FORMAT_VOL,
    HEADER_FORMAT_UNKNOWN
  } usHeaderFormatType;

  static usHeaderFormatType getHeaderFormat(const std::string &headerfilename);
};

#endif //US_IMAGE_IO_H
