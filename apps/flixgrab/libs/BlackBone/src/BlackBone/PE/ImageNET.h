#pragma once
#include "../Config.h"
#ifdef COMPILER_MSVC

#include "../Include/Winheaders.h"
#include <string>
#include <map>

#pragma warning(disable : 4091)
#include <cor.h>
#include <CorError.h>
#include <atlbase.h>
#pragma warning(default : 4091)

namespace blackbone
{

/// <summary>
/// .NET metadata parser
/// </summary>
class ImageNET
{
public:
    typedef std::map<std::pair<std::wstring, std::wstring>, uintptr_t> mapMethodRVA;

public:
    BLACKBONE_API ImageNET(void);
    BLACKBONE_API ~ImageNET(void);

    /// <summary>
    /// Initialize COM classes
    /// </summary>
    /// <param name="path">Image file path</param>
    /// <returns>true on success</returns>
    BLACKBONE_API bool Init( const std::wstring& path );

    /// <summary>
    /// Extract methods from image
    /// </summary>
    /// <param name="methods">Found Methods</param>
    /// <returns>true on success</returns>
    BLACKBONE_API bool Parse( mapMethodRVA* methods = nullptr );

    /// <summary>
    /// Get image .NET runtime version
    /// </summary>
    /// <returns>runtime version, "n/a" if nothing found</returns>
    BLACKBONE_API static std::wstring GetImageRuntimeVer( const wchar_t* ImagePath );

private:
    std::wstring _path;         // Image path
    mapMethodRVA _methods;      // Image methods

    // COM helpers
    CComPtr<IMetaDataDispenserEx>    _pMetaDisp;
    CComPtr<IMetaDataImport>         _pMetaImport;
    CComPtr<IMetaDataAssemblyImport> _pAssemblyImport;
};

}

#endif