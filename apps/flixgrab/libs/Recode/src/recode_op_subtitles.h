#pragma once

#include <string>
#include <vector>

#include "operation.h"

namespace recode {

    enum ERECODESUBTITLESErrorCode {
        RECODESUBTITLES_ERROR_CODE_UNKNOWN = 0,
        RECODESUBTITLES_ERROR_NO_INPUT_FILE = 1,
        RECODESUBTITLES_ERROR_FORMAT_NOT_SUPPORTED = 2,
        RECODESUBTITLES_ERROR_OPEN_INPUT_FILE = 3,
        RECODESUBTITLES_ERROR_OPEN_INPUT_FILE_REWRITE = 4
    };

class RecodeOpSubtitles :
    public Operation {

public:
    RecodeOpSubtitles(const std::string& stream);
    virtual ~RecodeOpSubtitles();

    Status                                              Do(OperationListener* listener) override;
    virtual void                                        Cancel() override;

private:
    Status                                              clearVTT(std::wstring source);
    int                                                 findLast(std::wstring& str, std::wstring find_str);
    std::wstring                                        getFormatFile(std::wstring fileName);
    bool                                                checkFormatIsSupported(std::wstring fileName);
    void                                                removeTag(std::wstring& source);

private:
    std::wstring                                        stream_;

};
}
