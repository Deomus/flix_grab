#include "main_window.h"
#include <QtCore>

#include "mpd_parser.h"

#include <iostream>

#include "decryptor.h"

int main(int argc, char *argv[])
{
    //Decryptor::TestUrl("http://http-v-darwin.hulustream.com/922/60643922/agave50863177_39858284_H264_1500_39860777_video.mp4?end=20161201093929&authToken=04ff37962c86c170b6769");

    //Decryptor::TestUrl("d:/hulu/hulu.mp4");
    //return 0; 
    //MpdParser parser;

    //QFile file_out("test.mpd");
    //
    ///*  if (file_out.open(QIODevice::WriteOnly | QIODevice::Text)) {
    //QTextStream stream_injected(&file_out);
    //stream_injected << script;
    //}*/
    //if (file_out.open(QIODevice::ReadOnly)) {
    //    QTextStream stream_injected(&file_out);
    //    parser.read(stream_injected.readAll());
    //}


    ////https://ipv4_1-lagg0-c001.1.waw001.retn.isp.nflxvideo.net/?o=AQGHaJd5MGf3i6qnGFpl05JAjTmhkJJeBLIYJG3JP51MWc5S_9vXFb2uPzPwtuLbTxlFHn8oiSjHGFoKj9fbdseIm-ZLDm-T3lY797VclAuwlj4gEme2aYfrIDUmbTv_kGMlOqTmBJP04pponCdGzJQ&v=3&e=1469832898&t=SdkCXFJs-rdVWfmvBIKq_7USA9Q&random=80510897161146384

    //shaka::media::test_demuxer("https://ipv4_1-lagg0-c001.1.waw001.retn.isp.nflxvideo.net/?o=AQGHaJd5MGf3i6qnGFpl05JAjTmhkJJeBLIYJG3JP51MWc5S_9vXFb2uPzPwtuLbTxlFHn8oiSjHGFoKj9fbdseIm-ZLDm-T3lY797VclAuwlj4gEme2aYfrIDUmbTv_kGMlOqTmBJP04pponCdGzJQ&v=3&e=1469832898&t=SdkCXFJs-rdVWfmvBIKq_7USA9Q&random=80510897161146384");
    //return 0;
    

    //bool result = InstallPhylloxera();

    ////DecryptProvider::Instance()->Start();

    //std::cout << "Phylloxera installed: " << result << std::endl;

    //std::string sHost = "http://www.netflix.com/WiPlayer?movieid=70143836&trkid=50263249&tctx=0%2C8%2Cee2d7017-6ca8-4016-97c6-decd3dff138d-116404963#episodeId=70196253&trackId=50503433";
    //PANDA
    //QString web_url = "https://www.netflix.com/watch/70075480?trackId=13752289";
   // QString web_url = "https://www.netflix.com/watch/70094483?trackId=14183416&tctx=0%2C0%2C84556178-eab7-4d09-9ade-4b9f9d63027c-10618145";
    //Poo
    //QString web_url = "https://www.netflix.com/watch/70253397?trackId=13752289&tctx=0%2C0%2Ce6e6cfcc4d5b86d3cb7ede59f43f2d73911fb674%3A7335ed6f92580783ac3e104e33d456406ba5f98a";
    //Charlie
    QString web_url = "https://www.netflix.com/watch/70021648?trackId=14183187&tctx=3%2C4%2C57993e9f-938e-49d1-8a98-cc6777da9069-7775719";
    //QString web_url = "https://www.netflix.com/watch/80077368?trackId=13462986&tctx=0%2C0%2Ce1dd20e6-59dc-4dc4-a10a-a2f8dd75aefd-5573631";
    //std::string sHost = "file:///d://cadmium.html";
    //std::string sHost = "file:///d://test.html";
    //std::string sHost = "http://www.dvdvideosoft.com/Files/Scripts/netflix/test.html";

    QApplication a(argc, argv);

    //QString loc = QLibraryInfo::location(QLibraryInfo::DataPath);
    
 //--load-extension=d:/hola_plugin.  
    // Prepare the main window
    MainWindow w;

//    QLibrary        phylloxera_lib("Phylloxera.dll");

  //  bool b = phylloxera_lib.load();

  //  w.gotoPage(web_url);

    Gethulu         hulu;

    //hulu.add("http://www.hulu.com/watch/876124");

    //hulu.add("http://www.hulu.com/watch/988103");

    hulu.add("http://www.hulu.com/watch/999283");


    //w.show();

    a.exec();

   // DecryptProvider::Instance()->Stop();

    //result = UninstallPhylloxera();

    //std::cout << "Phylloxera uninstalled: " << result << std::endl;

    return 0;
}
