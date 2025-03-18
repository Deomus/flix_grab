//DebugMemoryCheck.h

#ifndef DEBUG_MEMORY_CHECK_H
#define DEBUG_MEMORY_CHECK_H

#ifdef DEBUG_MEMORY_CHECK

#include <memory>

//#pragma comment(lib,"c:\\Program Files (x86)\\Visual Leak Detector\\lib\\vld.lib")
//#include "c:\\Program Files (x86)\\Visual Leak Detector\\include\\vld.h"

class MemoryCheckInitializer
{
public:
  MemoryCheckInitializer()
  {
      _CrtSetReportFile( _CRT_WARN | _CRT_ERROR, _CRTDBG_FILE_STDOUT);
      _CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
/*
      _CrtSetReportMode( _CRT_WARN, _CRTDBG_MODE_FILE );
      _CrtSetReportFile( _CRT_WARN, _CRTDBG_FILE_STDOUT );
      _CrtSetReportMode( _CRT_ERROR, _CRTDBG_MODE_FILE );
      _CrtSetReportFile( _CRT_ERROR, _CRTDBG_FILE_STDOUT );
      _CrtSetReportMode( _CRT_ASSERT, _CRTDBG_MODE_FILE );
      _CrtSetReportFile( _CRT_ASSERT, _CRTDBG_FILE_STDOUT );*/
  }
};
static MemoryCheckInitializer mci_;

#define meNew new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new meNew

#endif //DEBUG_MEMORY_CHECK
#endif //DEBUG_MEMORY_CHECK_H
