#ifndef CLIENTPUZZLE_H
#define CLIENTPUZZLE_H

#include <string>
#include <fstream>

using namespace std;


class ClientPuzzle {
public:
  ClientPuzzle();

  void setData(std::string x_l, std::string y, int len, int sublen);
  int getLackLength();
  std::string get_mx_l();
  std::string get_my();

private:
  std::string mx_l;
  std::string my;
  int mlen;
  int msublen;
};

std::string substrBackOne(std::string str, size_t pos);
std::string binInc(std::string bin);
std::string dectobin(int dec);
std::string hextobin(std::string &hex);
std::string hexdigest(std::string &key, int length);
std::string bindigest(std::string &key, int length);
std::string puzzleMD4(ClientPuzzle &C);
void parsePuzzle(ifstream &ifs, ClientPuzzle &C);

#endif