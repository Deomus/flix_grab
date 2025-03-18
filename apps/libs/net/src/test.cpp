
#include <QtWidgets>
#include <net/http_downloader.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    net::HttpDownloader down("test.mp4");

    
    QUrl url("http://video.playground.ru/178893/md/178893.mp4");
    //https://www.youtube.com/watch?v=G6WGUbnBLWw

    //QUrl url("https://www.youtube.com/watch?v=G6WGUbnBLWw");
    //down.download(url, Downloader::Append);
    down.setChunkSize();
    down.download(url, net::HttpDownloader::Truncate);
           
    return a.exec();
}
