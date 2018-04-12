#pragma once

#include "ofMain.h"

#include <stdio.h>
#include <vector>
#include <algorithm> 
#include <cmath>
#include <string>
#include <string.h>
#include <iterator>
#include <sstream>

class DateUtil {
private:
  DateUtil() {
  };

public:
  static DateUtil& DateUtil::getInstance() {
    static DateUtil du;
    return du;
  }

  
  string getTimeString(string splitstr = ":") {
    vector<string> d = split(ofGetTimestampString(), '-');
    //�~���b�̎擾
    string ms = d[6];
    //�b�̎擾
    string s = d[5];
    //���̎擾
    string m = d[4];
    //���̎擾
    string h = d[3];
    string time = h + splitstr + m + splitstr + s + splitstr + ms;
    return time;
  }

  string getDateString(string splitstr = ":") {
    vector<string> d = split(ofGetTimestampString(), '-');
    //�~���b�̎擾
    string ms = d[6];
    //�b�̎擾
    string s = d[5];
    //���̎擾
    string m = d[4];
    //���̎擾
    string h = d[3];
    //���̎擾
    string D = d[2];
    //���̎擾
    string M = d[1];
    //�N�̎擾
    string Y = d[0];
    string time = Y + splitstr + M + splitstr+ D;
    return time;
  }

  string getClockString(string splitstr = ":") {
    vector<string> d = split(ofGetTimestampString(), '-');
    //�~���b�̎擾
    string ms = d[6];
    //�b�̎擾
    string s = d[5];
    //���̎擾
    string m = d[4];
    //���̎擾
    string h = d[3];
    //���̎擾
    string D = d[2];
    //���̎擾
    string M = d[1];
    //�N�̎擾
    string Y = d[0];
    string time = h + splitstr + m + splitstr + s;
    return time;
  }

  string getDateTimeString(string splitstr = ":") {
    vector<string> d = split(ofGetTimestampString(), '-');
    //�~���b�̎擾
    string ms = d[6];
    //�b�̎擾
    string s = d[5];
    //���̎擾
    string m = d[4];
    //���̎擾
    string h = d[3];
    //���̎擾
    string D = d[2];
    //���̎擾
    string M = d[1];
    //�N�̎擾
    string Y = d[0];
    string time = Y + splitstr + M + splitstr + D + splitstr + h + splitstr + m + splitstr + s;
    return time;
  }

  std::vector<std::string> split(const std::string &str, char sep)
  {
    std::vector<std::string> v;
    std::stringstream ss(str);
    std::string buffer;
    while (std::getline(ss, buffer, sep)) {
      v.push_back(buffer);
    }
    return v;
  }


};

