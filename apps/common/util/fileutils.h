#ifndef __FILEUTILS_H
#define __FILEUTILS_H

#include <QString>

class FileUtilApi
{
public:
	static  QString  GetMyVideoFolderPath(const QString & sInPathDef);
    static  QString  GetMyMusicFolderPath(const QString & sInPathDef);
    static  QString  GetMyPictureFolderPath(const QString & sInPathDef);
    static  void     ShowFileInExplorer(const QString & sInPath);
};

//enum Permission 
//{
//    ReadOwner = 0x4000, WriteOwner = 0x2000, ExeOwner = 0x1000,
//    ReadUser  = 0x0400, WriteUser  = 0x0200, ExeUser  = 0x0100,
//    ReadGroup = 0x0040, WriteGroup = 0x0020, ExeGroup = 0x0010,
//    ReadOther = 0x0004, WriteOther = 0x0002, ExeOther = 0x0001
//};
//check access for write to folder
bool checkAccessRigth(const std::wstring &sPathName);
#endif // FILEUTILS_H