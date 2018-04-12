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
    //ƒ~ƒŠ•b‚ÌŽæ“¾
    string ms = d[6];
    //•b‚ÌŽæ“¾
    string s = d[5];
    //•ª‚ÌŽæ“¾
    string m = d[4];
    //Žž‚ÌŽæ“¾
    string h = d[3];
    string time = h + splitstr + m + splitstr + s + splitstr + ms;
    return time;
  }

  string getDateString(string splitstr = ":") {
    vector<string> d = split(ofGetTimestampString(), '-');
    //ƒ~ƒŠ•b‚ÌŽæ“¾
    string ms = d[6];
    //•b‚ÌŽæ“¾
    string s = d[5];
    //•ª‚ÌŽæ“¾
    string m = d[4];
    //Žž‚ÌŽæ“¾
    string h = d[3];
    //“ú‚ÌŽæ“¾
    string D = d[2];
    //ŒŽ‚ÌŽæ“¾
    string M = d[1];
    //”N‚ÌŽæ“¾
    string Y = d[0];
    string time = Y + splitstr + M + splitstr+ D;
    return time;
  }

  string getClockString(string splitstr = ":") {
    vector<string> d = split(ofGetTimestampString(), '-');
    //ƒ~ƒŠ•b‚ÌŽæ“¾
    string ms = d[6];
    //•b‚ÌŽæ“¾
    string s = d[5];
    //•ª‚ÌŽæ“¾
    string m = d[4];
    //Žž‚ÌŽæ“¾
    string h = d[3];
    //“ú‚ÌŽæ“¾
    string D = d[2];
    //ŒŽ‚ÌŽæ“¾
    string M = d[1];
    //”N‚ÌŽæ“¾
    string Y = d[0];
    string time = h + splitstr + m + splitstr + s;
    return time;
  }

  string getDateTimeString(string splitstr = ":") {
    vector<string> d = split(ofGetTimestampString(), '-');
    //ƒ~ƒŠ•b‚ÌŽæ“¾
    string ms = d[6];
    //•b‚ÌŽæ“¾
    string s = d[5];
    //•ª‚ÌŽæ“¾
    string m = d[4];
    //Žž‚ÌŽæ“¾
    string h = d[3];
    //“ú‚ÌŽæ“¾
    string D = d[2];
    //ŒŽ‚ÌŽæ“¾
    string M = d[1];
    //”N‚ÌŽæ“¾
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

