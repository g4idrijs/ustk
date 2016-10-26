/****************************************************************************
 *
 * Author:
 * Marc Pouliquen
 *
 *****************************************************************************/


/*!
  \example testUsImagePreScan3DMhdParser.cpp

  USTK MHD parser example.
  
  This example contains the declaration of a class used to read and write data
  in a mhd file like:
  \code
NDims = 2
DimSize = 186 233 
ElementType = MET_UCHAR
ElementSpacing = 1 1 
ElementByteOrderMSB = False
ElementDataFile = prescan3d.raw
UltrasoundImageType = PRESCAN_3D
  \endcode
  
*/


#include <visp3/core/vpConfig.h>

#include <iostream>

#include <visp3/core/vpXmlParser.h>
#include <visp3/core/vpDebug.h>
#include <visp3/core/vpIoTools.h>
#include <visp3/io/vpParseArgv.h>

#include <visp3/ustk_io/usMetaHeaderParser.h>

#include <visp3/ustk_io/usImageIo.h>

#include <string>

/* -------------------------------------------------------------------------- */
/*                         COMMAND LINE OPTIONS                               */
/* -------------------------------------------------------------------------- */

// List of allowed command line options
#define GETOPTARGS	"cdo:h"

void usage(const char *name, const char *badparam, const std::string& opath, const std::string& user);
bool getOptions(int argc, const char **argv, std::string &opath, const std::string& user);

/*!

Print the program options.

\param name : Program name.
\param badparam : Bad parameter name.
\param opath : Output image path.
\param user : Username.

 */
void usage(const char *name, const char *badparam, const std::string& opath, const std::string& user)
{
  fprintf(stdout, "\n\
Write and read data in a mhd file.\n\
\n\
SYNOPSIS\n\
  %s [-o <output image path>] [-h]\n", name);

  fprintf(stdout, "\n\
OPTIONS:                                               Default\n\
  -o <output data path>                                %s\n\
    Set data output path.\n\
    From this directory, creates the \"%s\"\n\
    subdirectory depending on the username, where \n\
    prescan3D.mhd file is written.\n\
\n\
  -h\n\
    Print the help.\n\n", opath.c_str(), user.c_str());

  if (badparam) {
    fprintf(stderr, "ERROR: \n" );
    fprintf(stderr, "\nBad parameter [%s]\n", badparam);
  }
}

/*!
  Set the program options.

  \param argc : Command line number of parameters.
  \param argv : Array of command line parameters.
  \param opath : Output data path.
  \param user : Username.
  \return false if the program has to be stopped, true otherwise.
*/
bool getOptions(int argc, const char **argv, std::string &opath, const std::string& user)
{
  const char *optarg_;
  int	c;
  while ((c = vpParseArgv::parse(argc, argv, GETOPTARGS, &optarg_)) > 1) {

    switch (c) {
    case 'o': opath = optarg_; break;
    case 'h': usage(argv[0], NULL, opath, user); return false; break;

    case 'c':
    case 'd':
      break;

    default:
      usage(argv[0], optarg_, opath, user); return false; break;
    }
  }

  if ((c == 1) || (c == -1)) {
    // standalone param or error
    usage(argv[0], NULL, opath, user);
    std::cerr << "ERROR: " << std::endl;
    std::cerr << "  Bad argument " << optarg_ << std::endl << std::endl;
    return false;
  }

  return true;
}

/* -------------------------------------------------------------------------- */
/*                               MAIN FUNCTION                                */
/* -------------------------------------------------------------------------- */

int main(int argc, const char** argv)
{
  try {
    std::string opt_opath;
    std::string ipath;
    std::string opath;
    std::string filename;
    std::string username;

    usMetaHeaderParser testReferenceSettings;

    std::cout <<  "-------------------------------------------------------" << std::endl ;
    std::cout <<  "  testUsImagePreScan3DMhdParser.cpp" <<std::endl << std::endl ;
    std::cout <<  "  writing and reading ultrasound data using a the US mhd parser" << std::endl ;
    std::cout <<  "-------------------------------------------------------" << std::endl ;
    std::cout << std::endl ;

    // Set the default output path
#if !defined(_WIN32) && (defined(__unix__) || defined(__unix) || (defined(__APPLE__) && defined(__MACH__))) // UNIX
    opt_opath = "/tmp";
#elif defined(_WIN32)
    opt_opath = "C:\\temp";
#endif

    // Get the user login name
    vpIoTools::getUserName(username);

    // Read the command line options
    if (getOptions(argc, argv, opt_opath, username) == false) {
      exit (-1);
    }

    // Get the option values
    if (!opt_opath.empty())
      opath = opt_opath;

    // Append to the output path string, the login name of the user
    std::string dirname = vpIoTools::createFilePath(opath, username);

    // Test if the output path exist. If no try to create it
    if (vpIoTools::checkDirectory(dirname) == false) {
      try {
        // Create the dirname
        vpIoTools::makeDirectory(dirname);
      }
      catch (...) {
        usage(argv[0], NULL, opath, username);
        std::cerr << std::endl
                  << "ERROR:" << std::endl;
        std::cerr << "  Cannot create " << dirname << std::endl;
        std::cerr << "  Check your -o " << opath << " option " << std::endl;
        exit(-1);
      }
    }
    filename = dirname + vpIoTools::path("/") + "prescan3d.mhd";

    //Init values in reference parser (same values in file read in test)
    usImagePreScan3D<unsigned char> prescan3DReference;
    prescan3DReference.resize(186,233,163);
    prescan3DReference.setScanLinePitch(0.0145);
    prescan3DReference.setTransducerRadius(0.554);
    prescan3DReference.setTransducerConvexity(true);
    prescan3DReference.setFramePitch(0.258);
    prescan3DReference.setMotorRadius(0.025);
    prescan3DReference.setMotorType(usMotorSettings::TiltingMotor);
    prescan3DReference.setAxialResolution(0.0058);

    std::cout << "Written in " << filename << std::endl ;
    std::cout << prescan3DReference;

    //write image
    usImageIo::write(prescan3DReference,filename);

    //read the image we just wrote
    usImagePreScan3D<unsigned char> prescan3D;
    filename = dirname + vpIoTools::path("/") + "prescan3d.mhd";
    usImageIo::read(prescan3D,filename);

    std::cout << "Read from " << filename << std::endl ;
    std::cout << prescan3D;

    if(prescan3D == prescan3DReference) {
      std::cout << "Test passed !" << std::endl;
      return 0;
    }

    std::cout << "Test failed !" << std::endl;
    return 1;
  }
  catch(const vpException &e) {
    std::cout << "Catch an exception: " << e.getMessage() << std::endl;
    return 1;
  }
}