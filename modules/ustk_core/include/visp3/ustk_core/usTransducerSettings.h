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
 * Marc Pouliquen
 *
 *****************************************************************************/

/**
 * @file usTransducerSettings.h
 * @brief Generic ultrasound transducer settings.
 */

#ifndef US_TRANSDUCER_SETTINGS_H
#define US_TRANSDUCER_SETTINGS_H

//std includes
#include <iostream>

//visp includes
#include <visp3/core/vpConfig.h>

//ustk includes

/**
 * @class usTransducerSettings
 * @brief Generic class for 2D ultrasound data common settings associated to the type of probe transducer used during acquisition.
 * @ingroup module_ustk_core
 *
 * This class represents ultrasound transducer common settings which are:
 * - the transducer radius \f$R_T\f$ in meters (value set to zero for a linear transducer)
 * - the scan line pitch that corresponds to the angle \f$\alpha_{SC}\f$ (in radians) between
 *   to successive scan line beams when the transducer is convex, or to the distance \f$d_{SC}\f$
 *   (in meters) when the transducer is linear
 * - the number of scan lines \f$n_{SC}\f$
 * - the type of ultrasound transducer used for data acquisition: convex or linear.
 *
 * The following figure summerize these transducer settings.
 *
 * \image html img-usTransducerSettings.png
 */
class VISP_EXPORT usTransducerSettings {
public:
  usTransducerSettings();
  usTransducerSettings(double transducerRadius, double scanLinePitch, unsigned int scanLineNumber, bool isTransducerConvex);
  usTransducerSettings(const usTransducerSettings &other);

  virtual ~usTransducerSettings();

  /** @name Inherited functionalities from usTransducerSettings */
  //@{

  double getTransducerRadius() const;
  unsigned int getScanLineNumber() const;
  double getScanLinePitch() const;

  bool isTransducerConvex() const;

  usTransducerSettings& operator=(const usTransducerSettings& other);
  bool operator==(usTransducerSettings const& other);

  //Settings for the probe transducer
  void setTransducerConvexity(const bool isTransducerConvex);
  void setTransducerSettings(const usTransducerSettings& other);
  void setTransducerRadius(const double transducerRadius);
  void setScanLineNumber(unsigned int scanLineNumber);
  void setScanLinePitch(const double scanLinePitch);

  //@}

  friend VISP_EXPORT std::ostream& operator<<(std::ostream& out, const usTransducerSettings &other);

private:
  //Settings for the probe transducer
  double m_transducerRadius;
  double m_scanLinePitch;
  unsigned int m_scanLineNumber;
  bool m_isTransducerConvex;
};

#endif // US_TRANSDUCER_SETTINGS_H