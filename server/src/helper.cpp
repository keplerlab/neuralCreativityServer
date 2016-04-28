


#include "helper.h"

unsigned char npMax(cv::Mat& input1) 
{
	unsigned char* input1Ptr = input1.ptr<unsigned char>() ; 
	int size = input1.rows * input1.step  ;
	unsigned char maxVal = 0 ; 
	for(int i=0 ; i< size ;i++ )
	{
		if(input1Ptr[i]> maxVal)
			maxVal = input1Ptr[i] ;
		 
	}
	return maxVal ;
}


void matrixMul(cv::Mat& input1,cv::Mat& input2,cv::Mat& output)
{

	input1.copyTo(output) ; 
	unsigned char* input1Ptr = input1.ptr<unsigned char>() ; 
	unsigned char* input2Ptr = input2.ptr<unsigned char>() ; 
	unsigned char* outputPtr = output.ptr<unsigned char>() ; 

	int size = input1.rows * input1.step  ;
	for(int i=0 ; i< size ;i++ )
	{
		outputPtr[i] = input1Ptr[i]*input2Ptr[i] ; 
	}


}

void appendMatrixHorz(cv::Mat& srcMatrix , cv::Mat& matToBeAppended ) 
{
	if( (srcMatrix.rows != matToBeAppended.rows) || (srcMatrix.type() != matToBeAppended.type() ) ) 
	{
		std::cerr << "Error In Matrix Append" << std::endl ; 
		return ; 	
	}

	cv::Mat destMat(srcMatrix.rows,srcMatrix.cols+matToBeAppended.cols,srcMatrix.type() ) ;
	//destMat = destMat * 0 ; 


	int i=0; 
	for(i=0;i< srcMatrix.cols;i++)
	{
		srcMatrix.col(i).copyTo(destMat.col(i) ) ; 
		// destMat.col(i) = srcMatrix.col(i) ; 
	}
	for(int j=0;j< matToBeAppended.cols ; j++)
	{
		matToBeAppended.col(j).copyTo(destMat.col(i+j) ) ;  
	}

	srcMatrix = destMat ; 
	return ;
}


void appendMatrixVert(cv::Mat& srcMatrix, cv::Mat& matToBeAppended)
{

	if( (srcMatrix.cols != matToBeAppended.cols) || (srcMatrix.type() != matToBeAppended.type() ) ) 
	{
		std::cerr << "Error In Matrix Append" << std::endl ; 
		return ; 	
	}

	cv::Mat destMat(srcMatrix.rows+matToBeAppended.rows,srcMatrix.cols,srcMatrix.type() ) ;
	//destMat = destMat * 0 ; 


	int i=0; 
	for(i=0;i< srcMatrix.rows;i++)
	{
		srcMatrix.row(i).copyTo(destMat.row(i) ) ; 
		// destMat.col(i) = srcMatrix.col(i) ; 
	}
	for(int j=0;j< matToBeAppended.rows ; j++)
	{
		matToBeAppended.row(j).copyTo(destMat.row(i+j) ) ;  
	}

	srcMatrix = destMat ; 
	return ;


} 


void putTextMat(cv::Mat& img ) // ,std::string& text)
{
	std::string text = "USER CONCENTRATION MODEL";
	int fontFace = cv::FONT_HERSHEY_COMPLEX_SMALL ;
	double fontScale = 1;
	int thickness = 1;

	int baseline=0;
	cv::Size textSize = cv::getTextSize(text, fontFace,
		fontScale, thickness, &baseline);
	baseline += thickness;
	// center the text
	cv::Point textOrg((img.cols - textSize.width)/2,
		(img.rows + textSize.height)/15);
	cv::putText(img,text, textOrg,fontFace,fontScale,cvScalar(50,50,50),thickness,8) ;
}


void putTextMat(cv::Mat& img ,int x , int y) // ,std::string& text)
{
	std::string text = "User Concentration model";
	int fontFace = cv::FONT_HERSHEY_COMPLEX_SMALL ;
	double fontScale = 1;
	int thickness = 1;

	int baseline=0;
	cv::Size textSize = cv::getTextSize(text, fontFace,
		fontScale, thickness, &baseline);
	baseline += thickness;
	// center the text
	cv::Point textOrg((img.cols - textSize.width)/2,
		(img.rows + textSize.height)/2);
	cv::putText(img,text, textOrg,fontFace,fontScale,cvScalar(200,200,250),thickness,8) ;

}



void putTextMat(cv::Mat& img ,std::string& text , int x , int y) 
{
	//std::string text = "User  concentration model";
	
	
	 //int fontFace = cv::FONT_HERSHEY_PLAIN ;
	 //double fontScale = 0.8;
	 //int thickness = 1;


	int fontFace = cv::FONT_HERSHEY_DUPLEX;
	double fontScale = 0.55;
	int thickness = 1;

	int baseline=0;
	cv::Size textSize = cv::getTextSize(text, fontFace,
		fontScale, thickness, &baseline);
	baseline += thickness;
	// center the text
	cv::Point textOrg( x,y );
	cv::putText(img,text, textOrg,fontFace,fontScale,cvScalar(0,0,255),thickness,8) ;

}


void drawBarHist(cv::Mat& img , float number)
{
	assert(number >= 0.0 || number <= 1.0 ) ;

	if(number > 1.0  || number < 0.0 )
	{
		std::cerr << "Number Out of bounds" << std::endl ;
		return ;
	}


	number = ( number > 0.99f ) ? 0.99f : number ; 
	number = ( number < 0.01f ) ? 0.03f : number ;

	unsigned char* ptr = img.ptr<unsigned char>() ; 
	
	int size = img.rows * img.step  ;
	for(int i=0 ; i< size ;i++ )
	{
		ptr[i] = 230 ; 
	}

	
	int nonWhiteRowsStart = (int)( img.rows * (1.0 -number) ) ;


	unsigned char grayValueB = 0 ; 
	unsigned char grayValueG = (unsigned char) (100.0 * number) ; 
	unsigned char grayValueR =(unsigned char) ( 140.0 * (1.0-number) ) ;
	unsigned char grayValue  = 0 ; 
	
	unsigned char grayValueThr = 80 ; 
	
	
	for(int i=nonWhiteRowsStart ; i< img.rows ;i++ )
	{

		for(int j=0 ; j< img.cols/2; j++)
		{
			ptr[i*img.step+j*3] = grayValueB + grayValue ; 
			ptr[i*img.step+j*3+1] = grayValueG + grayValue ; 
			ptr[i*img.step+j*3+2] = grayValueR + grayValue ; 
			grayValue+=2 ;

		}

		for(int j=img.cols/2 ; j< img.cols; j++)
		{
			ptr[i*img.step+j*3] =  grayValueB + grayValue ; 
			ptr[i*img.step+j*3+1] = grayValueG + grayValue ; 
			ptr[i*img.step+j*3+2] = grayValueR + grayValue ; 
			grayValue-=2 ;			

		}


		/*
		for(int j=0 ; j< img.step/2 ; j+=3)
		{
		ptr[i*img.step+j] = grayValue ; 
		ptr[i*img.step+j+1] = grayValue ; 
		ptr[i*img.step+j+2] = grayValue ; 
		grayValue+=2 ;
		}
		for(int j=img.step/2 ; j< img.step ; j+=3)
		{
		ptr[i*img.step+j] = grayValue ; 
		ptr[i*img.step+j+1] = grayValue ; 
		ptr[i*img.step+j+2] = grayValue ; 
		grayValue-=2 ;
		}

		*/
		grayValue += 1 ; 
		grayValue = (grayValue<grayValueThr) ? grayValue : grayValueThr ; 

	}


	return ; 


}




#define LINE_WIDTH 2
#define LINE_COLOR (CV_RGB(0, 116, 232))
#define LINE_COLOR2 (CV_RGB(255, 116, 0))


void drawStockLine(cv::Mat& img, std::vector<float>& numbers) 
{
	cv::Scalar color = LINE_COLOR;

	unsigned char* ptr = img.ptr<unsigned char>() ; 
	
	//(0, 116, 232)
	int size = img.rows * img.step  ;
	unsigned char grayValueB =  (unsigned char) (232) ; 
	unsigned char grayValueG = (unsigned char) (116) ; 
	unsigned char grayValueR =(unsigned char) ( 0 ) ;
	unsigned char grayValue  = 0 ; 


	for(int i=0 ; i< size ;i++ )
	{
		ptr[i] = (unsigned char) 0 ; 
	}

	int numbersStart = (  numbers.size() < img.cols  ) ? ( 0 ) : ( numbers.size() - img.cols ) ;
	
	for( int i=0  , currentNum = numbersStart ; ( i < numbers.size() ) && ( i < img.cols) ; i+=2 , currentNum+=2 ) 
	{
		cv::Point pt1 , pt2 ; 
		pt1.x = i ; 
		pt1.y = img.rows ;
		pt2.x = i ; 
		pt2.y = img.rows * ( 1.0 - numbers.at(currentNum) * 0.9 ) ; 
		// 	cvCircle(img, pt1, LINE_WIDTH, color, -1, CV_AA, 0);
		cv::line( img, pt1, pt2, color ) ;
	
	
	}


}

void rotateImage(cv::Mat& image,cv::Mat& result, cv::Point &image_center,double angle)
{
	cv::Mat rot = cv::getRotationMatrix2D(image_center,angle,1.0) ;
	cv::warpAffine(image,result, rot, image.size()) ;

}



static const std::string base64_chars =
"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
"abcdefghijklmnopqrstuvwxyz"
"0123456789+/";


std::string base64_encode(unsigned char const* bytes_to_encode, unsigned int in_len) {
    std::string ret;
    int i = 0;
    int j = 0;
    unsigned char char_array_3[3];
    unsigned char char_array_4[4];
    
    while (in_len--) {
        char_array_3[i++] = *(bytes_to_encode++);
        if (i == 3) {
            char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
            char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
            char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
            char_array_4[3] = char_array_3[2] & 0x3f;
            
            for (i = 0; (i < 4); i++)
                ret += base64_chars[char_array_4[i]];
            i = 0;
        }
    }
    
    if (i)
    {
        for (j = i; j < 3; j++)
            char_array_3[j] = '\0';
        
        char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
        char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
        char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
        char_array_4[3] = char_array_3[2] & 0x3f;
        
        for (j = 0; (j < i + 1); j++)
            ret += base64_chars[char_array_4[j]];
        
        while ((i++ < 3))
            ret += '=';
        
    }
    
    return ret;
    
}

std::string base64_decode(std::string const& encoded_string) {
    int in_len = encoded_string.size();
    int i = 0;
    int j = 0;
    int in_ = 0;
    unsigned char char_array_4[4], char_array_3[3];
    std::string ret;
    
    while (in_len-- && (encoded_string[in_] != '=') && is_base64(encoded_string[in_])) {
        char_array_4[i++] = encoded_string[in_]; in_++;
        if (i == 4) {
            for (i = 0; i < 4; i++)
                char_array_4[i] = base64_chars.find(char_array_4[i]);
            
            char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
            char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
            char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];
            
            for (i = 0; (i < 3); i++)
                ret += char_array_3[i];
            i = 0;
        }
    }
    
    if (i) {
        for (j = i; j < 4; j++)
            char_array_4[j] = 0;
        
        for (j = 0; j < 4; j++)
            char_array_4[j] = base64_chars.find(char_array_4[j]);
        
        char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
        char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
        char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];
        
        for (j = 0; (j < i - 1); j++) ret += char_array_3[j];
    }
    
    return ret;
}




//
//cv::Rect getAveragePosition( std::vector< cv::Rect >& rectangles   ) 
//{
//
//	std::vector< float > midVecX ; 
//	std::vector< float > midVecY ; 
//
//	for(int i=0;i<rectangles.size() ; i++) 
//	{
//		float midX = ( rectangles.at(i).x + rectangles.at(i).width ) /2.0 ; 
//		float midY =  ( rectangles.at(i).y  + rectangles.at(i).height) / 2.0 ;
//		midVecX.push_back( midX) ; 
//		midVecY.push_back( midY ) ;
//
//	
//	}
//
//}

