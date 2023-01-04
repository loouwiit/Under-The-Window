#include <SFML/Graphics.hpp>

extern "C"
{
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libswscale/swscale.h>

#include <libavcodec/dxva2.h>

#include <libavutil/avutil.h>
#include <libavutil/imgutils.h>
#include <libavutil/frame.h>
#include <libavutil/hwcontext.h>
}

//https://zhuanlan.zhihu.com/p/242448731

sf::RenderWindow window;
sf::View view;
sf::Texture texture;
sf::Sprite sprite;
sf::Uint8* pixels = nullptr;

void ffmpeg(const char path[]);
void updateWindow();

int main()
{
	window.create(sf::VideoMode(1240, 720), L"test");
	window.setView(view);
	//texture.create(1920, 1080);
	texture.setSmooth(true);

	ffmpeg(".\\1_4K.mp4");
	/*


	srand((unsigned)time(nullptr));
	//memset(pixels, 0x00, 1240 * 720 * 4 * sizeof(sf::Uint8));
	for (unsigned i = 0; i < 1240 * 720 * 4; i++) pixels[i] = rand();


	window.clear(sf::Color(0x000000FF));
	window.draw(sprite);
	window.display();
	window.display();
	window.display();

	sf::Event event;
	while (window.pollEvent(event))
	{

	}
	*/

	return 0;
}

void ffmpeg(const char path[])
{
	AVFormatContext* format = avformat_alloc_context(); //��Ƶ�ļ�
	int video_Index = -1; //��Ƶ������
	AVCodecHWConfig* HWContext = nullptr; //Ӳ��������������
	AVCodecParameters* codecParameters = nullptr; //����������
	AVCodecContext* codecContext= nullptr; //������������
	const AVCodec* codec = nullptr; //������
	SwsContext* swsContext = nullptr; //����
	unsigned bufferSize = 0; //���������ݳ���
	unsigned char* buffer = nullptr; //�����õĻ�����
	AVFrame* frame = av_frame_alloc(); //ԭʼ֡
	AVFrame* frameRGB = av_frame_alloc(); //ת��֡
	AVPacket* packet = av_packet_alloc(); //ԭʼ����
	int ret = 0; //��������ֵ(��ʱ)

	if (avformat_open_input(&format, path, NULL, NULL))
	{
		printf_s("avformat��ʧ��\n");
		return;
	}

	if (avformat_find_stream_info(format, NULL) < 0)
	{
		printf_s("stream��Ϣʧ��\n");
		return;
	}

	printf_s("������:%d\n", format->nb_streams);

	for (unsigned index = 0; index < format->nb_streams; index++)
	{
		codecParameters = format->streams[index]->codecpar;
		if (codecParameters->codec_type == AVMEDIA_TYPE_VIDEO)
		{
			printf_s("��Ƶ�����:%d\n", index);
			video_Index = index;
			break;
		}
	}

	if (video_Index == -1)
	{
		printf_s("δ�ҵ���Ƶ��\n");
		goto END;
	}

	codec = avcodec_find_decoder_by_name("dxva2");
	if (codec == nullptr)
	{
		printf_s("dxva2Ѱ��ʧ��\n");

		codec = avcodec_find_decoder(codecParameters->codec_id);
		if (codec == nullptr)
		{
			printf_s("codecѰ��ʧ��\n");
			goto END;
		}
	}

	codecContext = avcodec_alloc_context3(codec);
	if (codecContext == nullptr)
	{
		printf_s("codec�����Ĵ���ʧ��\n");
		goto END;
	}
	else
	{
		avcodec_parameters_to_context(codecContext, codecParameters);
	}

	if (avcodec_open2(codecContext,codec,NULL))
	{
		printf_s("codec��ʧ��\n");
		goto END;
	}

	printf_s("�ֱ���:%dx%d\n", codecParameters->width, codecParameters->height);
	texture.create(codecParameters->width, codecParameters->height); //SFML
	view.setSize((float)codecParameters->width, (float)codecParameters->height); //SFML
	view.setCenter((float)codecParameters->width / 2, (float)codecParameters->height / 2); //SFML
	window.setView(view); //SFML

	swsContext = sws_getContext(codecParameters->width, codecParameters->height,
		(AVPixelFormat)codecParameters->format, codecParameters->width, codecParameters->height,
		AV_PIX_FMT_RGBA, SWS_FAST_BILINEAR, 0, 0, 0);

	bufferSize = av_image_get_buffer_size(AV_PIX_FMT_RGBA, codecParameters->width, codecParameters->height, 1);
	buffer = (unsigned char*)av_malloc(bufferSize);
	if (buffer == nullptr)
	{
		printf_s("�����ڴ�ʧ��\n");
		goto END;
	}
	printf_s("����%u���ڴ�\n", bufferSize);
	av_image_fill_arrays(frameRGB->data, frameRGB->linesize, buffer, AV_PIX_FMT_RGBA, codecParameters->width, codecParameters->height, 1);
	pixels = buffer; //SFML
	
	while (av_read_frame(format, packet) >= 0) //��֡
	{
		if (packet->stream_index != video_Index) continue;
		if (avcodec_send_packet(codecContext, packet))
		{
			printf_s("����packetʧ��\n");
			goto END;
		}
		ret = avcodec_receive_frame(codecContext, frame);
		
		if (ret == -EAGAIN)
		{
			avcodec_send_packet(codecContext, packet);
			ret = avcodec_receive_frame(codecContext, frame);
		}

		if (ret != 0)
		{
			printf_s("����֡ʧ��:%d\n", ret);
			break;
		}

		sws_scale(swsContext, frame->data, frame->linesize, 0, codecParameters->height, frameRGB->data, frameRGB->linesize);

		updateWindow();
		sf::sleep(sf::milliseconds(1000/60));
	}

;END:
	if (format != nullptr)
	{
		avformat_free_context(format);
		format = nullptr;
	}
	if (buffer != nullptr)
	{
		av_free(buffer);
		buffer = nullptr;
	}
	if (swsContext != nullptr)
	{
		sws_freeContext(swsContext);
		swsContext = nullptr;
	}
	if (frameRGB != nullptr)
	{
		av_frame_free(&frameRGB);
		//frameRGB = nullptr;
	}
	if (frame != nullptr)
	{
		av_frame_free(&frame);
		//frame = nullptr;
	}
	if (codecContext != nullptr)
	{
		avcodec_close(codecContext);
		codecContext = nullptr;
	}
	if (packet != nullptr)
	{
		av_packet_free(&packet);
		//packet = nullptr;
	}
}

void updateWindow()
{
	texture.update(pixels);
	sprite.setTexture(texture);

	window.clear();
	window.draw(sprite);
	window.display();
}
