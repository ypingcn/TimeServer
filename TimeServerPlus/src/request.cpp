#include "request.h"


tsp_request_t * tsp_request_new(void)
{
  tsp_request_t * res = (tsp_request_t *) new tsp_request_t;
  if(res == NULL)
  {
    perror("tsp_httpheaders_new");
    exit(-1);
  }
  return res;
}

int tsp_request_delete(tsp_request_t * header)
{
  if(header == NULL)
    return -1;
  delete header;
  return 0;
}

using std::cout;
using std::endl;

int tsp_request_parse(const std::string& raw_request,tsp_request_t * parse_result)
{
  if(raw_request.empty())
  {
    perror("tsp_httpheaders_parse:empty raw_request");
    return -1;
  }
  if(parse_result == NULL)
  {
    perror("tsp_httpheaders_parse:parse_result NULL");
    return -1;
  }

  std::string rn("\r\n"),rnrn("\r\n\r\n");
  size_t pre = 0 ,nxt = 0 ,rnsize = 2;

  if( (nxt = raw_request.find(rn,pre)) != std::string::npos )
  {
    std::string line(raw_request.substr(pre, nxt-pre));
    cout<<line<<endl;
    std::stringstream s(line);
    s>>parse_result->method;
    s>>parse_result->url;
    s>>parse_result->version;

    if(parse_result->version.find("HTTP",0) == std::string::npos)
      return -1;

    std::string url_line(parse_result->url);
    size_t arg_begin = url_line.find('?',0);

    if(arg_begin != std::string::npos)
    {
      arg_begin++;
      size_t i=arg_begin,j=arg_begin;
      std::string key,value;

      size_t arg_next =  url_line.find("&&",arg_begin);
      while(arg_next != std::string::npos)
      {
        while(url_line[j] != '=')
          j++;

        key = url_line.substr(i,j-i);

        j++;
        i = j;

        j = arg_next;

        value = url_line.substr(i,j-i);

        i = j = arg_next + 2;
        arg_next = url_line.find("&&",i);
      }

      while(url_line[j] != '=' && j < url_line.size())
        j++;

      if( j == url_line.size() )
        return -1;

      key = url_line.substr(i,j-i);

      j++;
      i = j;

      j = url_line.size();

      value = url_line.substr(i,j-i);
    }

    pre = nxt;
  }
  else
  {
    perror("tsp_httpheaders_parse: raw_request without '\r\n'");
    return -1;
  }

  size_t headers_end_pos = 0;

  if( (headers_end_pos = raw_request.find(rnrn,pre)) != std::string::npos)
  {
    std::string line,key,value;
    nxt = raw_request.find(rn, pre+rnsize);

    size_t i,j;

    while(nxt <= headers_end_pos)
    {
      line = raw_request.substr(pre+rnsize,nxt-pre-rnsize);
      i = 0,j = 0;

      while( isblank(line[j]) )
        j++;
      i = j;

      while( !isblank(line[j]) && line[j] != ':')
        j++;

      key = line.substr(i, j-i);

      while( !isblank(line[j]) )
        j++;

      i = j;

      while( j != nxt )
        j++;

      value = line.substr(i, j-i);

      parse_result->headers.insert(new_httpheader(key, value));

      pre = nxt;
      nxt = raw_request.find(rn, pre+rnsize);

    }
  }

  parse_result->body = raw_request.substr(headers_end_pos + rnsize + rnsize,
    raw_request.size() - rnsize - rnsize);


  return 0;
}

using std::cout;
using std::endl;

void tsp_request_handle(int sockfd,tsp_request_t * request)
{
  tsp_response(sockfd,request);
}
