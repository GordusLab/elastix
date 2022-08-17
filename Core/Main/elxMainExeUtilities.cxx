/*=========================================================================
 *
 *  Copyright UMC Utrecht and contributors
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *        http://www.apache.org/licenses/LICENSE-2.0.txt
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *=========================================================================*/

// Its own header file:
#include "elxMainExeUtilities.h"

#include "xoutmain.h"
#include <Core/elxGitRevisionInfo.h>
#include <Core/elxVersionMacros.h>
#include <itkMacro.h>

// Standard Library header files:
#include <cassert>
#include <exception>
#include <limits>
#include <sstream>
#include <typeinfo>


void
elastix::ReportTerminatingException(const char * const executableName, const std::exception & stdException) noexcept
{
  try
  {
    std::ostringstream outputStringStream;

    outputStringStream << "ERROR: " << executableName
                       << " terminated because of the following exception:\nException type: "
                       << typeid(stdException).name();

    const auto itkExceptionObject = dynamic_cast<const itk::ExceptionObject *>(&stdException);

    if (itkExceptionObject)
    {
      // itk::ExceptionObject provides the most information by inserting the object directly, instead of calling what().
      outputStringStream << *itkExceptionObject;
    }
    else
    {
      outputStringStream << "\nWhat message: " << stdException.what() << '\n';
    }

    const std::string message = outputStringStream.str();

    // Insert the message into the standard error stream, as well as into xout, because xout might not yet be set up.
    std::cerr << message;
    xl::xout["error"] << message << std::flush;
  }
  catch (...)
  {
    // Enforce that this function itself will never throw any exception.
    assert(!"Unhandled exception!");
  }
}


std::string
elastix::GetExtendedVersionInformation(const char * const executableName, const char * const indentation)
{
  std::ostringstream outputStringStream;
  outputStringStream << indentation << executableName << " version: " ELASTIX_VERSION_STRING;

  const std::string newline = '\n' + std::string(indentation);
  static_assert(gitRevisionSha, "gitRevisionSha should never be null!");
  static_assert(gitRevisionDate, "gitRevisionDate should never be null!");

  if (*gitRevisionSha != '\0')
  {
    outputStringStream << newline << "Git revision SHA: " << gitRevisionSha;
  }

  if (*gitRevisionDate != '\0')
  {
    outputStringStream << newline << "Git revision date: " << gitRevisionDate;
  }

  outputStringStream << newline << "Build date: " << __DATE__ << ' ' << __TIME__
#ifdef _MSC_FULL_VER
                     << newline << "Compiler: Visual C++ version " << _MSC_FULL_VER << '.' << _MSC_BUILD
#endif
#ifdef __clang__
                     << newline << "Compiler: Clang"
#  ifdef __VERSION__
                     << " version " << __VERSION__
#  endif
#endif
#if defined(__GNUC__)
                     << newline << "Compiler: GCC"
#  ifdef __VERSION__
                     << " version " << __VERSION__
#  endif
#endif
                     << newline << "Memory address size: " << std::numeric_limits<std::size_t>::digits << "-bit"
                     << newline << "CMake version: " ELX_CMAKE_VERSION << newline
                     << "ITK version: " << ITK_VERSION_MAJOR << '.' << ITK_VERSION_MINOR << '.' << ITK_VERSION_PATCH
                     << '\n';

  return outputStringStream.str();
}


void
elastix::PrintArguments(xoutlibrary::xoutbase & output, const char * const * const arguments)
{
  if ((arguments == nullptr) || (*arguments == nullptr) || (*(arguments + 1) == nullptr))
  {
    assert(!"There should be at least two arguments!");
  }
  else
  {
    const auto AddDoubleQuotesIfStringHasSpace = [](const std::string & str) {
      return (str.find(' ') == std::string::npos) ? str : ('"' + str + '"');
    };

    // Skip the first argument, as it is usually just the executable name.
    output << "\nCommand-line arguments: \n  " << AddDoubleQuotesIfStringHasSpace(*(arguments + 1));

    auto argument = arguments + 2;
    while (*argument != nullptr)
    {
      output << ' ' << AddDoubleQuotesIfStringHasSpace(*argument);
      ++argument;
    }
    output << '\n' << std::endl;
  }
}
