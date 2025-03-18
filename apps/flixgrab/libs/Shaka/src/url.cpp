#include "url.h"
#include <string>
#include <algorithm>
#include <cctype>
#include <functional>
#include <iterator>

using namespace std;

// ctors, copy, equality, ...
Url::Url(const std::string& url_s)
{
    parse(url_s);
}

Url::Url(const Url& other) : protocol_(other.protocol_), host_(other.host_), path_(other.path_), query_(other.query_)
{

}

void Url::parse(const string& url_s)
{
    const string prot_end("://");
    string::const_iterator prot_i = search(url_s.begin(), url_s.end(),
        prot_end.begin(), prot_end.end());
    protocol_.reserve(distance(url_s.begin(), prot_i));
    transform(url_s.begin(), prot_i,
        back_inserter(protocol_),
        tolower); // protocol is icase
    if (prot_i == url_s.end())
        return;
    advance(prot_i, prot_end.length());
    string::const_iterator path_i = find(prot_i, url_s.end(), '/');
    host_.reserve(distance(prot_i, path_i));
    transform(prot_i, path_i,
        back_inserter(host_),
        tolower); // host is icase
    string::const_iterator query_i = find(path_i, url_s.end(), '?');
    path_.assign(path_i, query_i);
    if (query_i != url_s.end())
        ++query_i;
    query_.assign(query_i, url_s.end());
}

