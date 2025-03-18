#pragma once

enum EStatusCode {
    Success = 0,
    UnknownError = 1,
    NetError = 2,
    Aborted = 3,
//Error while parsing url 
    ParseError = 4,
    WrongUrl = 5,
    RegistrationFailed = 6,
    CheckProfileFailed = 7,
    ParseTimeout = 8,
    StreamError = 9,
    EncoderError = 10,
    CdmError = 11,
    PxError = 12,
    HttpError = 13,
    NoMemory = 14,
    FileError = 15,
    ConvError = 16,
    ConvSubtitlesError = 17,
    RecodeSubtitlesError = 18,
    DecompressSubtitlesError = 19,
    WriteMetadataError = 20,
    LiveStream = 21,
    VideoBlockedInLocation = 22,
    PrivateVideo = 23,
    ProtectedByPassword = 24,
    VideoUnavailable = 25,
    StopPage = 26, //Amazon
    ForcedReloadPage = 27,//Amazon
    EmptyData = 28, //Amazon
    PaidVideo = 29,
    YouTubeMovies = 30,
};
