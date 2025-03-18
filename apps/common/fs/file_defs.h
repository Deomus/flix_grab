// file_defs.h

#ifndef __COMMON_FS_FILE_DEFS_H__
#define __COMMON_FS_FILE_DEFS_H__


namespace fs
{

 /*!
 * \enum    EDvsDirType
 * \brief   Values that represent dvs dir types.
 */
enum EDvsDirType
{
    eCurrentProcessDir  = 0,
    eCommonLibDir       = 1,
    eCommonBinDir       = 2,
    eLogDir             = 3,
    eSharedDir          = 4,
    //eRoamingDir         = 5
};


}

#endif // __COMMON_FS_FILE_DEFS_H__
