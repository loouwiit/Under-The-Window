//#define __STDC_CONSTANT_MACROS
#include "libavcodec/avcodec.h "

/*
此文件用于播放视频
于主版本1.0.4.0加入
*/

int main()
{
	printf("%s", avcodec_configuration());
	return 0;
}