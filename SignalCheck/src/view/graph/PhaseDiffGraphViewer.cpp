//
//  ofxGraphViewer.cpp
//
//  Created by motoi ishibashi on 12/04/05.
//

#include <iostream>

#include "PhaseDiffGraphViewer.h"
vector<float> last4PlotsAlpha;
vector<float> last4PlotsBeta;
vector<string> timer;
bool saveCapFlg;
PhaseDiffGraphViewer::PhaseDiffGraphViewer()
{
  dataAlpha = NULL;
  dataBeta = NULL;
  data5chAlpha = NULL;
  data5chBeta = NULL;
  saveCapFlg = false;
  bufferLength = 0;
  min = -1.0;
  max = 1.0;
  width = 200.0;
  height = 100.0;
  lineColor1 = 0xffffff;
  lineColor2 = 0xffffff;
  idx = 0;
  offset = 0;

  last4PlotsAlpha.resize(4);
  last4PlotsBeta.resize(4);
  timer.resize(0);
  swingWidthAlpha = 0;
  swingWidthBeta = 0;

  prevValueAlpha = 0;
  prevValueBeta = 0;

  for (int i = 0; i < 4; i++) {
    last4PlotsAlpha.push_back(0.1);
    last4PlotsBeta.push_back(0.1);
  }
}

PhaseDiffGraphViewer::~PhaseDiffGraphViewer()
{
  if (dataAlpha != NULL) {
    delete dataAlpha;
  }

  if (dataBeta != NULL) {
    delete dataBeta;
  }

  if (data5chAlpha != NULL) {
	  delete data5chAlpha;
  }

  if (data5chBeta != NULL) {
	  delete data5chBeta;
  }

}

void PhaseDiffGraphViewer::setup(int _bufferLength, int _offset)
{
  bufferLength = _bufferLength;
  offset = _offset;

  this->allocate(bufferLength);

}

void PhaseDiffGraphViewer::allocate(int num)
{
  if (dataAlpha != NULL) {
    delete dataAlpha;
  }

  if (dataBeta != NULL) {
    delete dataBeta;
  }

  if (data5chAlpha != NULL) {
	  delete data5chAlpha;
  }

  if (data5chBeta != NULL) {
	  delete data5chBeta;
  }

  bufferLength = num;

  dataAlpha = new float[bufferLength];
  memset(dataAlpha, 0, sizeof(float)*bufferLength);

  dataBeta = new float[bufferLength];
  memset(dataBeta, 0, sizeof(float)*bufferLength);

  data5chAlpha = new float[bufferLength];
  memset(data5chAlpha, 0, sizeof(float)*bufferLength);

  data5chBeta = new float[bufferLength];
  memset(data5chBeta, 0, sizeof(float)*bufferLength);

}

void PhaseDiffGraphViewer::pushData(float _alpha, float _beta, float _alpha5ch, float _beta5ch, float _peekFreq)
{
  if (peekFreq != _peekFreq) {
    peekFreq += (_peekFreq - peekFreq) / 2;
  }
  else {
    peekFreq = _peekFreq;
  }

  if (0 > peekFreq || peekFreq > 5000) {
    peekFreq = 3000.0f;
  }

  float diffSampling = 1 / (44643.0f * 8.0f) * peekFreq * 360.0f;


  /*
  1-5ch 12
  */
  _alpha += diffSampling * (Const::getInstance().EAST_ANT - Const::getInstance().CENTER_ANT); //20  1-5ch 12
  _beta += diffSampling * (Const::getInstance().SOUTH_ANT - Const::getInstance().CENTER_ANT);

  _alpha += Const::getInstance().antPhaseDiff[Const::getInstance().EAST_ANT] - Const::getInstance().antPhaseDiff[Const::getInstance().WEST_ANT];
  _beta += Const::getInstance().antPhaseDiff[Const::getInstance().SOUTH_ANT] - Const::getInstance().antPhaseDiff[Const::getInstance().NORTH_ANT];


  _alpha5ch += diffSampling * (Const::getInstance().EAST_ANT - Const::getInstance().CENTER_ANT); //20  1-5ch 12
  _beta5ch += diffSampling * (Const::getInstance().SOUTH_ANT - Const::getInstance().CENTER_ANT);

  _alpha5ch += Const::getInstance().antPhaseDiff[Const::getInstance().EAST_ANT] + Const::getInstance().antPhaseDiff[Const::getInstance().WEST_ANT];
  _beta5ch += Const::getInstance().antPhaseDiff[Const::getInstance().SOUTH_ANT] + Const::getInstance().antPhaseDiff[Const::getInstance().NORTH_ANT];


  // 位相反転の時に
  float widAlpha = 0;
  for (int i = 0; i < last4PlotsAlpha.size(); i++) {
    widAlpha += last4PlotsAlpha[i];
  }
  swingWidthAlpha = widAlpha / 4;

  float widBeta = 0;
  for (int i = 0; i < last4PlotsBeta.size(); i++) {
    widBeta += last4PlotsBeta[i];
  }
  swingWidthBeta = widBeta / 4;

  //std::cerr << swingWidthBeta << std::endl;

  if (abs(swingWidthAlpha) < 20) {
    if (abs(_alpha - prevValueAlpha) > 150) {
      if (_alpha > 150 || _alpha < -150) {
        if (prevValueAlpha > 0) {
          _alpha = abs(_alpha);
        }
        else {
          _alpha = abs(_alpha) * -1;
        }
      }
      
    }
  }

  if (abs(swingWidthBeta) < 20) {
    if (abs(_beta - prevValueBeta) > 150) {
      if (_beta > 150 || _beta < -150) {
        if (prevValueBeta > 0) {
          _beta = abs(_beta);
        }
        else {
          _beta = abs(_beta) * -1;
        }
      }
    }
  }

  last4PlotsAlpha.push_back(abs(_alpha) - abs(prevValueAlpha));
  if (last4PlotsAlpha.size() > 4) last4PlotsAlpha.erase(last4PlotsAlpha.begin());

  last4PlotsBeta.push_back(abs(_beta) - abs(prevValueBeta));
  if (last4PlotsBeta.size() > 4) last4PlotsBeta.erase(last4PlotsBeta.begin());

  for (int i = bufferLength - 1; i > 0; i--) {
    dataAlpha[i] = dataAlpha[i - 1];
    dataBeta[i] = dataBeta[i - 1];
	data5chAlpha[i] = data5chAlpha[i - 1];
	data5chBeta[i] = data5chBeta[i - 1];
  }
  idx++;
  prevValueAlpha = _alpha;
  prevValueBeta = _beta;

  dataAlpha[0] = round(_alpha);
  dataBeta[0] = round(_beta);
  data5chAlpha[0] = round(_alpha5ch);
  data5chBeta[0] = round(_beta5ch);


  // タイマー進める
  
  if (timer.size() > 0) {
    for (int i = 0; i < timer.size(); i++) {
      
      int cnt = ofToInt(ofSplitString(timer[i], "__")[0]);
      //std::cerr << ofSplitString(timer[i], "__")[1] << std::endl;
      cnt++;
      timer[i] = ofToString(cnt) + "__" + ofSplitString(timer[i], "__")[1];
      //std::cerr << ofToString(cnt) + ofSplitString(timer[i], "__")[1] << std::endl;
    }

    int cnt = ofToInt(ofSplitString(timer[0], "__")[0]);

    if (cnt > Const::getInstance().delayCapTime / 0.025) {
      saveCapFlg = true;
    }
    else {
      saveCapFlg = false;
    }
  }
}

void PhaseDiffGraphViewer::draw()
{
  draw(0.0, 0.0);
}

void PhaseDiffGraphViewer::draw(float posx, float posy)
{
  draw(posx, posy, width, height, min, max);
}


void PhaseDiffGraphViewer::draw(float posx, float posy, float w, float h, float min, float max)
{
  if (dataAlpha == NULL) return;

  if (idx < offset) return;

  //ofPushStyle();
  ofNoFill();
  ofSetLineWidth(0.5);
  ofSetHexColor(0x555555);

  ofSetColor(30, 30, 30);
  for (int i = 0; i < 6; i++) {
    ofLine(posx, posy + h * (i + 1) / 6, posx + w, posy + h * (i + 1) / 6);
  }

  ofSetColor(120, 120, 120);
  ofRect(posx, posy, w, h);
  ofSetColor(120, 120, 120);
  ofLine(posx, posy + h / 2.0, posx + w, posy + h / 2.0);

  ofSetHexColor(lineColor1);
  glBegin(GL_LINE_STRIP);
  for (int i = 0; i < bufferLength; i++) {
    //glVertex3d(posx + (w / (float)bufferLength)*i, posy + h - ofMap(dataAlpha[i + offset], min, max, 0.0, h), 0.0);
    if (bufferLength < i + offset) {
      glVertex3d(posx + (w / (float)bufferLength)*i, posy + h - ofMap(0.0, min, max, 0.0, h), 0.0);
    }
    else {
      glVertex3d(posx + (w / (float)bufferLength)*i, posy + h - ofMap(dataAlpha[i + offset], min, max, 0.0, h), 0.0);
    }
  }
  glEnd();

  ofSetHexColor(lineColor2);
  glBegin(GL_LINE_STRIP);
  for (int i = 0; i < bufferLength; i++) {
    //glVertex3d(posx + (w / (float)bufferLength)*i, posy + h - ofMap(dataAlpha[i + offset], min, max, 0.0, h), 0.0);
    if (bufferLength < i + offset) {
      glVertex3d(posx + (w / (float)bufferLength)*i, posy + h - ofMap(0.0, min, max, 0.0, h), 0.0);
    }
    else {
      glVertex3d(posx + (w / (float)bufferLength)*i, posy + h - ofMap(dataBeta[i + offset], min, max, 0.0, h), 0.0);
    }
  }
  glEnd();

  ofPopStyle();
}

void PhaseDiffGraphViewer::saveDelayCapture()
{
  if (saveCapFlg) {
    ScreenCapture::getInstance().saveScreenCapture(Const::getInstance().DST_PATH, ofSplitString(timer[0], "__")[1] + "/delay.png");
    timer.erase(timer.begin());
    saveCapFlg = false;
  }
}

void PhaseDiffGraphViewer::setSize(float _width, float _height)
{
  width = _width;
  height = _height;
}

void PhaseDiffGraphViewer::setRange(float _min, float _max)
{
  min = _min;
  max = _max;
}

void PhaseDiffGraphViewer::setColor(int _lineColor1, int _lineColor2)
{
  lineColor1 = _lineColor1;
  lineColor2 = _lineColor2;
}

void PhaseDiffGraphViewer::culcDiff(int _lifetime)
{
  bool isError = true;
  string content = "";
  string label = "";
  string logAlpha = "";
  string logBeta = "";

  dispersion = (float)Const::getInstance().thresholdDispersion;
  float minVAlpha = dataAlpha[0];
  float maxVAlpha = dataAlpha[0];
  float minVBeta = dataBeta[0];
  float maxVBeta = dataBeta[0];

  int len = std::min(_lifetime * 20, bufferLength);
  float avgAlpha = 0;
  float avgBeta = 0;
  for (int i = len; i > 0; i--) {
    avgAlpha += dataAlpha[i - 1];
    minVAlpha = std::min(minVAlpha, dataAlpha[i - 1]);
    maxVAlpha = std::max(maxVAlpha, dataAlpha[i - 1]);

    avgBeta += dataBeta[i - 1];
    minVBeta = std::min(minVBeta, dataBeta[i - 1]);
    maxVBeta = std::max(maxVBeta, dataBeta[i - 1]);


    if (i > 1) {
      logAlpha += ofToString(dataAlpha[i - 1]) + ",";
      logBeta += ofToString(dataBeta[i - 1]) + ",";
    }
    else {
      logAlpha += ofToString(dataAlpha[i - 1]);
      logBeta += ofToString(dataBeta[i - 1]);
    }
  }
  avgAlpha /= len;
  avgBeta /= len;

  // 標準偏差
  //float standardDeviation
  float totalAlpha = 0;
  float totalBeta = 0;
  for (int i = len; i > 0; i--) {
    totalAlpha += (dataAlpha[i - 1] - avgAlpha) * (dataAlpha[i - 1] - avgAlpha);
    totalBeta += (dataBeta[i - 1] - avgBeta) * (dataBeta[i - 1] - avgBeta);
  }
  float standardDeviationAlpha = sqrt(totalAlpha / len);
  float standardDeviationBeta = sqrt(totalBeta / len);
  //std::cerr << LogUtil::getInstance().getIndentStr() + "標準偏差Alpha: " << standardDeviationAlpha << std::endl;
  //std::cerr << LogUtil::getInstance().getIndentStr() + "標準偏差Beta: " << standardDeviationBeta << std::endl;
  string time = DateUtil::getInstance().getDateString("") + "_" + DateUtil::getInstance().getClockString("");

  label += "Date,StandardDeviationAlpha,StandardDeviationBeta,";
  content += time + "," + ofToString(standardDeviationAlpha) + "," + ofToString(standardDeviationBeta) + ",";

  /*if (minVAlpha > avgAlpha - dispersion && maxVAlpha < avgAlpha + dispersion  && minVBeta > avgBeta - dispersion && maxVBeta < avgBeta + dispersion) {
    std::cerr << "◆流星検知    duration: " << (float)len * 0.025f << "s: " << "  len: " << len << "  avgAlpha:" << avgAlpha << std::endl;
    ScreenCapture::getInstance().saveScreenCapture("./tmp/", time2);
  }*/

  //minVAlpha = maxVAlpha = avgAlpha = 138.7; // theta1=50.1, theta2=21.5, 方位角=50.1, 仰角=30.8
  //minVBeta = maxVBeta = avgBeta = 66.1;

  //minVAlpha = maxVAlpha = avgAlpha = -96.3; // theta1=-32.3, theta2=4.78, 方位角=171.3, 仰角=57.3
  //minVBeta = maxVBeta = avgBeta = 14.9;


  // 最頻値の計算
  int maxA = 0, cntA = 0, indexA = 0;

  for (int j = 0; j<len; j++, cntA = 0)
  {
      for (int i = j; i<len; i++)
      {
          if (abs(dataAlpha[j] - dataAlpha[i]) <= dispersion)
              cntA++;
      }
      if (maxA < cntA) {
          maxA = cntA;
          indexA = j;
      }
  }

  int maxB = 0, cntB = 0, indexB = 0;

  for (int k = 0; k<len; k++, cntB = 0)
  {
      for (int l = k; l<len; l++)
      {
          if (abs(dataBeta[k] - dataBeta[l]) <= dispersion)
              cntB++;
      }
      if (maxB < cntB) {
          maxB = cntB;
          indexB = k;
      }
  }



  //if (minVAlpha > avgAlpha - dispersion && maxVAlpha < avgAlpha + dispersion) {
  if ((standardDeviationAlpha <= dispersion && standardDeviationBeta <= dispersion) || Const::getInstance().enableDebug) {
    isError = false;
    std::cerr << LogUtil::getInstance().getIndentStr() + "◆流星検知　duration: " << (float)len * 0.025f << "s: " << "  len: " << len << "  avgAlpha:" << avgAlpha << "  avgBeta:" << avgBeta << std::endl;
    std::cerr << LogUtil::getInstance().getIndentStr() + "最頻値Alpha: " << dataAlpha[indexA] << std::endl;
    std::cerr << LogUtil::getInstance().getIndentStr() + "最頻値Beta: " << dataBeta[indexB] << std::endl;

    float theta1 = asin(dataAlpha[indexA] / 180) * 180 / M_PI;
    float theta2 = asin(dataBeta[indexB] / 180) * 180 / M_PI;

	// 5chの処理
	float _theta1 = asin((data5chAlpha[indexA] / 5) / 180) * 180 / M_PI;
	float _theta2 = asin((data5chBeta[indexB] / 5) / 180) * 180 / M_PI;

	float __theta1 = asin((data5chAlpha[indexA] / 5) / 180) * 180 / M_PI;
	float __theta2 = asin((data5chBeta[indexB] / 5) / 180) * 180 / M_PI;

	int _n = 0;
	for (int n = 0; n < 5; n++)
	{
		__theta1 = asin((data5chAlpha[indexA] / 5 - (float)n / 2.5) / 180) * 180 / M_PI;
		/*if (abs(__theta1 - theta1) <= abs(_theta1 - theta1)) {
			
			_n = n;
		}*/
		__theta2 = asin((data5chBeta[indexB] / 5 - (float)n / 2.5) / 180) * 180 / M_PI;
		if (abs(__theta2 - theta2) <= abs(_theta2 - theta2)) {
			_theta1 = __theta1;
			_theta2 = __theta2;
			_n = n;
		}
	}

    std::cerr << "　　　　　　　　　theta1: " << _theta1 << "  theta2: " << theta2 << std::endl;
	std::cerr << "　　　　　　　　　5ch theta1: " << _theta1 << "  5ch theta2: " << _theta2 << "  n=" << _n << std::endl;

	theta1 = _theta1;
	theta2 = _theta2;


    //float azimuthAngle = 180 - atan(cos((90 - theta2) / 180 * M_PI) / cos((90 - theta1) / 180 * M_PI)) * 180 / M_PI;
    float azimuthAngle = atan(cos((90 - theta2) / 180 * M_PI) / cos((90 - theta1) / 180 * M_PI)) * 180 / M_PI + 180;
    if (azimuthAngle > 180) {
      azimuthAngle = 360 - azimuthAngle;
    }

    if (azimuthAngle < -180) {
      azimuthAngle = -360 - azimuthAngle;
    }

    float elevationAngle = acos(sqrt(pow(cos((90 - theta1) / 180 * M_PI), 2) + pow(cos((90 - theta2) / 180 * M_PI), 2))) * 180 / M_PI;

    std::cerr << LogUtil::getInstance().getIndentStr() + LogUtil::getInstance().getTabStr() + "方位角: " << azimuthAngle << "  仰角: " << elevationAngle << std::endl;

    string cmd = "curl -X GET \"https://e49lvsoi62.execute-api.ap-northeast-1.amazonaws.com/production/city?azimuth=" + ofToString(azimuthAngle) + "\&elevation=" + ofToString(elevationAngle) + "\"";
    system(cmd.c_str());

    label += "Duration(s),DataLength,AvgAlpha,AvgBeta,Theta1,Theta2,AzimuthAngle,ElevationAngle";
    content += ofToString((float)len * 0.025f) + "," + ofToString(len) + "," + ofToString(avgAlpha) + "," + ofToString(avgBeta) + "," + ofToString(theta1) + "," + ofToString(theta2) + "," + ofToString(azimuthAngle) + "," + ofToString(elevationAngle);
  }
  else {
    isError = true;
    std::cerr << LogUtil::getInstance().getIndentStr() + LogUtil::getInstance().getTabStr() + "ばらつき多い" << "  avgAlpha:" << avgAlpha << "  min:" << minVAlpha << "  max:" << maxVAlpha << std::endl;
  }
  //std::cerr << std::endl;

  if (Const::getInstance().enableDebug || !isError) {
    // キャプチャ画像残す
    if (Const::getInstance().enableCapture) {
      ScreenCapture::getInstance().saveScreenCapture(Const::getInstance().DST_PATH, time + "/end.png");
      timer.push_back("0__" + time);
    }

    // CSV残す
    if (Const::getInstance().enableLogCsv) LogUtil::getInstance().saveCsv(Const::getInstance().DST_PATH, time, label, content, logAlpha, logBeta);
    
    // DailyのCSV
    string dateStr = ofSplitString(time, "_")[0];
    string timeStr = ofSplitString(time, "_")[1];
    string contentDaily = dateStr + "," + timeStr + "," + ofSplitString(content, ",")[1] + "," + ofSplitString(content, ",")[2] + "," + ofSplitString(content, ",")[3] + "," + ofSplitString(content, ",")[9] + "," + ofSplitString(content, ",")[10];

    if (Const::getInstance().enableDailyLogCsv) LogUtil::getInstance().saveDailyCsv(Const::getInstance().DST_PATH, dateStr, contentDaily);

    // ADI残す
    if (Const::getInstance().enableSaveAdi) {
      FileUtil::getInstance().copyFile(Const::getInstance().CACHE_PATH, Const::getInstance().DST_PATH + time + "/DaqLog.bak");
      FileUtil::getInstance().copyFile(Const::getInstance().SRC_PATH, Const::getInstance().DST_PATH + time + "/DaqLog_next.bak");
    }

  }
}