#include <iostream>
#include <string>
#include <cmath>
#include <fstream>
#include <sstream>
#include <openssl/md4.h>
#include "ClientPuzzle.h"

using namespace std;


ClientPuzzle::ClientPuzzle():
  mx_l(""), my(""), mlen(0), msublen(0)
{
}

void ClientPuzzle::setData(std::string x_l, std::string y, int len, int sublen)
{
  mx_l = x_l;
  my = y;
  mlen = len;
  msublen = sublen;
}

int ClientPuzzle::getLackLength()
{
  return mlen - msublen;
}

std::string ClientPuzzle::get_mx_l()
{
  return mx_l;
}

std::string ClientPuzzle::get_my()
{
  return my;
}


std::string substrBackOne(std::string str, size_t pos)
{
  const size_t strLen = str.length();
  return str.substr(strLen - pos, 1);
}

std::string binInc(std::string bin)
{
  const size_t length = bin.length();
  for (int i = 1; i <= (int)length; i++)
  {
    if (substrBackOne(bin, i).compare("0") == 0)
      return bin.replace(length - i, 1, "1");
    else
      bin.replace(length - i, 1, "0");
  }
  return "BIT OVERFLOW";
}

std::string dectobin(int dec)
{
  int d = dec;
  std::string bin = "";
  while (d)
  {
    bin = std::to_string(d % 2) + bin;
    d /= 2;
  }
  while ((int)bin.length() < 4)
    bin = "0" + bin;
  return bin;
}

std::string hextobin(std::string &hex)
{
  int i, hex_length;
  hex_length = (int)hex.length();
  std::string bin = "";

  for (i = 0; i < hex_length; ++i)
  {
    bin += dectobin(std::stoi(std::string(1, hex[i]), nullptr, 16));
  }
  return bin;
}

std::string hexdigest(std::string &key, int length)
{
  static char hex[32 + 1];
  unsigned char digest[16];
  const char *hexformat =
      "%02x%02x%02x%02x%02x%02x%02x%02x"
      "%02x%02x%02x%02x%02x%02x%02x%02x";

  MD4((const unsigned char *)key.c_str(), length, digest);

  snprintf(hex,
           sizeof(hex) / sizeof(hex[0]),
           hexformat,
           digest[0], digest[1], digest[2], digest[3],
           digest[4], digest[5], digest[6], digest[7],
           digest[8], digest[9], digest[10], digest[11],
           digest[12], digest[13], digest[14], digest[15]);

  std::string hex_str = hex;
  return hex_str;
}

std::string bindigest(std::string &key, int length)
{
  std::string digest_hex = hexdigest(key, length);
  return hextobin(digest_hex);
}

std::string puzzleMD4(ClientPuzzle &C)
{
  int lack_length = C.getLackLength();
  string x_l = C.get_mx_l();
  string y = C.get_my();

  string x;
  string x_k = std::string(lack_length, '0');

  for (int i = 0; i <= pow(2, lack_length); i++)
  {
    x = x_k + x_l;
    if (y.compare(bindigest(x, (int)x.length())) == 0)
      return x_k;
    x_k = binInc(x_k);
  }
  return "NOT FOUND";
}

void parsePuzzle(ifstream& ifs, ClientPuzzle& C)
{
  string tmp, x_l, y;
  int len, sublen;
  while (getline(ifs, tmp))
  {
    if (tmp.compare(0, 2, "cp") == 0)
    {
      istringstream iss(tmp);
      string cp, len_str, sublen_str;
      iss >> cp >> len_str >> sublen_str;
      len = std::stoi(len_str);
      sublen = std::stoi(sublen_str);
    }
    else
    {
      if ((int)tmp.length() == len)
        y = tmp;
      else if ((int)tmp.length() == sublen)
        x_l = tmp;
      else {
        cerr << "ERROR: FAILED IN PARSING." << endl;
        exit(1);
      }
    }
  }
  C.setData(x_l, y, len, sublen);
  return;
}
