#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>

using namespace std;
using hashMap = unordered_map<string, string>;

void parsePerLine(string str, hashMap &palette) {
  size_t idx0 = str.find('"');
  size_t idx1 = str.find('"', idx0 + 1);
  size_t idx2 = str.find('"', idx1 + 1);
  size_t idx3 = str.find('"', idx2 + 1);

  string key = str.substr(idx0 + 1, idx1 - idx0 - 1);
  string val = str.substr(idx2, idx3 - idx2 + 1);
  // insert pound for hex val
  val.reserve(val.size() + 1);
  val.insert(val.begin() + 1, '#');

  // only setup base16 pairs
  if (key.rfind("base", 0) == 0) {
    palette[key] = val;
  }
}

// main fileIn.json
int main(int argc, char **argv) {
  hashMap palette;

  ifstream fin(argv[1]);
  string line;
  while (getline(fin, line)) {
    // skip brackets
    if (line.length() > 1) {
      parsePerLine(line, palette);
    }
  }
  fin.close();

  ofstream fout("../palette.h");
  fout << "#ifndef " << "PALETTE_H" << endl;
  fout << "#define " << "PALETTE_H" << endl;
  for (const auto [key, val] : palette) {
    fout << '\t' << "static const char " << key << "[] = " << val << ";" << endl;
  }
  fout << "#endif" << endl;

  return 0;
}
