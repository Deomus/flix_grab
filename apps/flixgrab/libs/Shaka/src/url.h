#ifndef URL_HH_
#define URL_HH_

#include <string>
struct Url {
    Url(const std::string& url_s); // omitted copy, ==, accessors, ...
    Url(const Url& other);
    Url() {}

    std::string protocol() const { return protocol_; }
    std::string host() const { return host_; }
    std::string path() const { return path_; }
    std::string query() const { return query_; }

private:
    void parse(const std::string& url_s);
private:
    std::string protocol_, host_, path_, query_;
};
#endif /* URL_HH_ */