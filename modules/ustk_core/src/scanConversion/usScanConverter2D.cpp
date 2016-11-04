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
 * Pierre Chatelain
 *
 *****************************************************************************/

#include <visp3/ustk_core/usScanConverter2D.h>
#include <visp/vpMath.h>

usScanConverter2D::usScanConverter2D() {}

usScanConverter2D::~usScanConverter2D() {}

/**
* Initialize the scan-converter.
* @param AN Number of samples along a scan-line in the pre-scan image.
* @param LN Number of scanlines in the pre-scan image.
* @param speedOfSound Speed of sound in meters per seconds (normally 1540 m/s).
* @param resolution Resolution of the post-scan image you want (size of a pixel in meters).
* @param radius Transducer radius in meters (0 if linear transducer).
* @param inputHeight Distance in meters between first and last pixel on a scanline.
* @param pitch Probe pitch.
* @param nElements Number of elements on the probe.
*/
void usScanConverter2D::init(unsigned int AN, unsigned int LN, double speedOfSound, double resolution,
                             double radius, int inputHeight, double pitch, int nElements)
{
  m_AN = AN;
  m_LN = LN;
  m_speedOfSound = speedOfSound;
  m_curved = radius != 0;
  if (m_curved) {
    m_radius = radius;
  } else {
    std::cerr << "Error: In usScanConverter2D::init(): "
	      << "Probe radius is 0 (linear probe). "
	      << "Cannot initialize scan converter." << std::endl;
    exit(EXIT_FAILURE);
  }
  m_APitch = (m_speedOfSound * inputHeight) / (4.0e7 * AN); // RF is sampled at 40MHz
  m_LPitch = (pitch * nElements) / LN;
  m_resolution = resolution;

  double r_min = m_radius;
  double r_max = (m_radius + m_APitch * AN);
  double t_min = - (LN * m_LPitch) / (2.0 * m_radius);
  double t_max = - t_min;
  m_x_min = r_min * cos(t_min);
  m_x_max = r_max;
  m_y_min = r_max * sin(t_min);
  m_y_max = r_max * sin(t_max);

  m_height = vpMath::round((m_x_max - m_x_min) / resolution);
  m_width = vpMath::round((m_y_max - m_y_min) / resolution);

  m_rMap.resize(m_height, m_width);
  m_tMap.resize(m_height, m_width);
  
  double x, y;
  for (unsigned int i = 0; i < m_height; ++i) {
    for (unsigned int j = 0; j < m_width; ++j) {
      x = m_x_min + i * resolution;
      y = m_y_min + j * resolution;
      m_rMap[i][j] = (sqrt(x * x + y * y) - m_radius) / m_APitch;
      m_tMap[i][j] = atan2(y, x) * m_radius / m_LPitch + LN / 2.0;
    }
  }
  //saving settings in postScanImage
  m_postScanImage.setHeightResolution(m_resolution);
  m_postScanImage.setWidthResolution(m_resolution);
  m_postScanImage.setScanLineNumber(m_LN);
  m_postScanImage.setTransducerConvexity(true);
  m_postScanImage.setScanLinePitch(m_LPitch/m_radius);
  m_postScanImage.setTransducerRadius(m_radius);
}

/**
* Initialize the scan-converter.
* @param AN Number of samples along a scan-line in the pre-scan image.
* @param LN Number of scanlines in the pre-scan image.
* @param speedOfSound Speed of sound in meters per seconds (normally 1540 m/s).
* @param resolution Resolution of the post-scan image you want (size of a pixel in meters).
* @param radius Transducer radius in meters (0 if linear transducer).
* @param samplingFrequency Sampling frequency used to obtain the pre-scan image.
* @param pitch Probe pitch.
* @param nElements Number of elements on the probe.
*/
void usScanConverter2D::init(unsigned int AN, unsigned int LN, double speedOfSound, double resolution,
                             double radius, double samplingFrequency, double pitch, int nElements)
{
  m_AN = AN;
  m_LN = LN;
  m_speedOfSound = speedOfSound;
  m_curved = radius != 0;
  if (m_curved)
    m_radius = radius;
  m_APitch = m_speedOfSound / (2.0 * samplingFrequency);
  m_LPitch = (pitch * nElements) / (LN-1);
  m_resolution = resolution;

  double r_min = m_radius;
  double r_max = (m_radius + m_APitch * AN);
  double t_min = - ((LN-1) * m_LPitch) / (2.0 * m_radius);
  double t_max = - t_min;
  m_x_min = r_min * cos(t_min);
  m_x_max = r_max;
  m_y_min = r_max * sin(t_min);
  m_y_max = r_max * sin(t_max);


  std::cout << "r_min " << r_min << std::endl;
  std::cout << "r_max " << r_max << std::endl;
  std::cout << "t_min " <<  t_min<< std::endl;
  std::cout << "t_max " << t_max << std::endl;
  std::cout << "x_min" << m_x_min << std::endl;
  std::cout << "x_max " << m_x_max << std::endl;
  std::cout << "y_min " << m_y_min << std::endl;
  std::cout << "y_max " << m_y_max << std::endl;

  m_height = vpMath::round((m_x_max - m_x_min) / resolution);
  m_width = vpMath::round((m_y_max - m_y_min) / resolution);

  m_rMap.resize(m_height, m_width);
  m_tMap.resize(m_height, m_width);
  
  double x, y;
  for (unsigned int i = 0; i < m_height; ++i) {
    for (unsigned int j = 0; j < m_width; ++j) {
      x = m_x_min + i * resolution;
      y = m_y_min + j * resolution;
      m_rMap[i][j] = (sqrt(x * x + y * y) - m_radius) / m_APitch;
      m_tMap[i][j] = atan2(y, x) * m_radius / m_LPitch + (LN-1) / 2.0;
    }
  }
  //saving settings in postScanImage
  m_postScanImage.setHeightResolution(m_resolution);
  m_postScanImage.setWidthResolution(m_resolution);
  m_postScanImage.setScanLineNumber(m_LN);
  m_postScanImage.setTransducerConvexity(true);
  //m_postScanImage.setScanLinePitch(m_LPitch/m_radius);
  m_postScanImage.setScanLinePitch(0.00783337); // setting for sonosite, to improve (line above is not correct)
  m_postScanImage.setTransducerRadius(m_radius);


  std::cout << "Scan conversion parameters :" << std::endl;
  std::cout << "AN : " << AN << std::endl;
  std::cout << "LN : " << m_postScanImage.getScanLineNumber() << std::endl;
  std::cout << "line pitch : " << m_postScanImage.getScanLinePitch() << std::endl;
  std::cout << "LN : " << m_postScanImage.getScanLineNumber() << std::endl;
  std::cout << "radius : " << m_postScanImage.getTransducerRadius() << std::endl;
  std::cout << "APitch : " << m_APitch << std::endl;
  std::cout << "LPitch : " << m_LPitch << std::endl;
  std::cout << "pitch : " << pitch << std::endl;
  std::cout << "resolution : " << m_resolution << std::endl;
}

/**
* Run the scan-converter.
* @param [out] postScanImage Post-scan image : result of the scan conversion.
* @param [in] preScanImage Pre-scan image to convert.
*/
void usScanConverter2D::run(usImagePostScan2D<unsigned char> &postScanImage, const usImagePreScan2D<unsigned char> &preScanImage)
{
  //saving image settings
  postScanImage = m_postScanImage;
  postScanImage.resize(m_height, m_width);
  for (unsigned int i = 0; i < m_height; ++i)
    for (unsigned int j = 0; j < m_width; ++j) {
      double u = m_rMap[i][j];
      double v = m_tMap[i][j];
      postScanImage(i, j, interpolateLinear(preScanImage, u, v));
    }
}


double usScanConverter2D::interpolateLinear(const vpImage<unsigned char>& I, double x, double y)
{
  int x1 = floor(x);
  int x2 = ceil(x);
  int y1 = floor(y);
  int y2 = ceil(y);
  double val1, val2;

  if ((0 <= x) && (x < I.getHeight()) && (0 <= y) && (y < I.getWidth())) {
    // Check whether the indices are within the image extent
    if (x1 < 0) ++x1;
    if (y1 < 0) ++y1;
    if (x2 >= static_cast<int>(I.getHeight())) --x2;
    if (y2 >= static_cast<int>(I.getWidth())) --y2;

    // Check whether the target is on the grid
    if (x1==x2) {
      val1 = I(x1, y1);
      val2 = I(x1, y2);
    }
    else {
      val1 = (x2 - x) * I(x1, y1) + (x - x1) * I(x2, y1);
      val2 = (x2 - x) * I(x1, y2) + (x - x1) * I(x2, y2);
    }
    if (y1==y2)
      return val1;
    else
      return (y2 - y) * val1 + (y - y1) * val2;
  }

  return 0.0;
}
