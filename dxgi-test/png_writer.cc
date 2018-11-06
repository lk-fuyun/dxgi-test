#include "stdafx.h"
#include "png_writer.h"
#include "logger.h"

#define BIT_DEPTH 8
#define BYTES_PER_PIXEL 4
namespace png
{
	bool WritePng(png_bytep buffer, int width, int height, const char* file_name)
	{
		FILE* file;
		/*����Ҫд����ļ�*/
		fopen_s(&file, file_name, "wb");
		if (file == NULL)
		{
			logger::error("�ļ������ڣ�%s", file_name);
			return false;
		}

		/* ��������ʼ�� png_struct ��������Ĵ������������������ʹ��Ĭ�ϵ� stderr �� longjump() ������
		* ����Խ��������������ΪNULL����ʹ�ö�̬���ӿ������£�����Ҳ���⺯����汾�Ƿ��ڱ���ʱʹ�õİ汾�Ƿ����
		*/
		png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
		if (png_ptr == NULL)
		{
			fclose(file);
			return false;
		}
		// �����ڴ沢��ʼ��ͼ����Ϣ����
		png_infop png_info_ptr = png_create_info_struct(png_ptr);
		if (png_info_ptr == NULL)
		{
			fclose(file);
			png_destroy_write_struct(&png_ptr, NULL);
			return false;
		}
		// ���ô�������������ڵ��� png_create_write_struct() ʱû�����ô�������������ô��δ����Ǳ����
		if (setjmp(png_jmpbuf(png_ptr)))
		{
			// ��������ܵ������ˣ���ôд���ļ�ʱ����������
			fclose(file);
			png_destroy_write_struct(&png_ptr, &png_info_ptr);
			return false;
		}
		// �����������
		png_init_io(png_ptr, file);
		// ����IHDR
		png_set_IHDR(png_ptr, png_info_ptr, width, height, BIT_DEPTH, PNG_COLOR_TYPE_RGBA, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
		// д���ļ�ͷ����Ϣ
		png_write_info(png_ptr, png_info_ptr);
		png_bytep change_buffer = new png_byte[width * height * BYTES_PER_PIXEL];
		for (int i = 0; i < width * height; i++)
		{
			png_bytep src = buffer + i * BYTES_PER_PIXEL;
			png_bytep dest = change_buffer + i * BYTES_PER_PIXEL;
			dest[0] = src[2];
			dest[1] = src[1];
			dest[2] = src[0];
			dest[3] = src[3];
		}
		png_bytep* row_pointers = new png_bytep[height];
		for (int k = 0; k < height; k++)
		{
			row_pointers[k] = change_buffer + k * width * BYTES_PER_PIXEL;
		}
		// д����������
		png_write_image(png_ptr, row_pointers);
		delete[] row_pointers;
		delete[] change_buffer;
		// д���ļ�β
		png_write_end(png_ptr, png_info_ptr);
		// �ͷ��ڴ�
		png_destroy_write_struct(&png_ptr, &png_info_ptr);
		fclose(file);
		return true;
	}
}