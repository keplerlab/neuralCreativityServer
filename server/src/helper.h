#ifndef _HELPER_H_
#define _HELPER_H_


// Include OpenCV headers
#include "opencv2/core/core.hpp"
#include "opencv2/video/tracking.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/objdetect/objdetect.hpp"

//Include standard C++ headers
#include <iostream>
#include <ctype.h>
#include <iterator>
#include <stdio.h>
#include <fstream>
#include <vector>

void matrixMul(cv::Mat& input1,cv::Mat& input2,cv::Mat& output);

unsigned char npMax(cv::Mat& input) ;

void appendMatrixHorz(cv::Mat& sourceMatrix , cv::Mat& matToBeAppended) ; // Append matrix horizontaaly 

void appendMatrixVert(cv::Mat& sourceMatrix, cv::Mat& matToBeAppended) ;  // Append matrix vertically 

void putTextMat(cv::Mat& img ) ; // put text as heading in image color white

void putTextMat(cv::Mat& img ,int x , int y) ; // put text with white color at x , y position 

void putTextMat(cv::Mat& img ,std::string& text , int x , int y)  ; 

void drawBarHist(cv::Mat& img , float number) ; // Draw a bar with relative value based on float number (number should be between 0 and 1 )

void drawStockLine(cv::Mat& img, std::vector<float>& numbers) ; 

void rotateImage(cv::Mat& image,cv::Mat& result, cv::Point &image_center,double angle) ;

//cv::Rect getAveragePosition( std::vector< cv::Rect >& rectangles   ) ; 



static inline bool is_base64(unsigned char c) {
    return (isalnum(c) || (c == '+') || (c == '/'));
}

std::string base64_encode(unsigned char const* bytes_to_encode, unsigned int in_len) ;

std::string base64_decode(std::string const& encoded_string) ;



#endif 