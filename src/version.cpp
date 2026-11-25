#include "uincampus/version.hpp"

#include "uincampus/config.hpp" // généré par CMake

#include <sstream>

namespace uin {

namespace {

std::string compilateur()
{
    std::ostringstream flux;
    // Clang définit aussi __GNUC__ : il doit être testé en premier.
#if defined(__clang__)
    flux << "Clang " << __clang_major__ << '.' << __clang_minor__ << '.' << __clang_patchlevel__;
#elif defined(__GNUC__)
    flux << "GCC " << __GNUC__ << '.' << __GNUC_MINOR__ << '.' << __GNUC_PATCHLEVEL__;
#elif defined(_MSC_VER)
    flux << "MSVC " << _MSC_VER;
#else
    flux << "compilateur inconnu";
#endif
    return flux.str();
}

} // namespace

std::string_view version()
{
    return config::version;
}

std::string informationsConstruction()
{
    const std::string_view configuration = config::typeConstruction.empty()
                                               ? std::string_view{"multi-configuration"}
                                               : config::typeConstruction;

    std::ostringstream flux;
    flux << config::nomProjet << ' ' << config::version << " — " << compilateur() << " — C++ ("
         << __cplusplus << ") — " << configuration;
    return flux.str();
}

} // namespace uin
