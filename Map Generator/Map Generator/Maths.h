#pragma once
#include "SFML/System/Vector2.hpp"
#include <iostream>
#include <algorithm>

namespace Utils
{
	[[nodiscard]]
	static inline float Lerp(float in_from, float in_to, float in_time) noexcept
	{
		return in_from + in_time * (in_to - in_from);
	}

	[[nodiscard]]
	static inline float MapToRange(float in_value, float start1, float end1, float start2, float end2) noexcept
	{
		return ((in_value - start1) / (end1 - start1)) * (end2 - start2) + start2;
	}

	[[nodiscard]]
	static inline float Distance(sf::Vector2f in_from, sf::Vector2f in_to)noexcept
	{
		float a = in_to.x - in_from.x;
		float b = in_to.y - in_from.y;

		float output;

		output = sqrt(powf(a, 2) + powf(b, 2));

		return output;
	}

	[[nodiscard]]
	static inline float ManhattenDistance(sf::Vector2f in_from, sf::Vector2f in_to) noexcept
	{
		float a = in_to.x - in_from.x;
		float b = in_to.y - in_from.y;

		return fabs(a) + fabs(b);
	}

	[[nodiscard]]
	static inline float Dot(sf::Vector2f one, sf::Vector2f two)noexcept
	{
		float product = 0;
		product = product + one.x * two.x;
		product = product + one.y * two.y;
		return product;
	}

	[[nodiscard]]
	// Source:
	// https://stackoverflow.com/a/1501725
	static float DistanceToLineSegment(sf::Vector2f pointOne, sf::Vector2f pointTwo, sf::Vector2f in_point)
	{
		// Return minimum distance between line segment vw and point p
		const float minDistance = powf(Distance(pointOne, pointTwo), 2);  // i.e. |w-v|^2 -  avoid a sqrt
		if (minDistance == 0.0)
		{
			return Distance(in_point, pointOne);   // v == w case
		}
		// Consider the line extending the segment, parameterized as v + t (w - v).
		// We find projection of point p onto the line.
		// It falls where t = [(p-v) . (w-v)] / |w-v|^2
		// We clamp t from [0,1] to handle points outside the segment vw.
		const float t = std::max<float>(0, std::min<float>(1, Dot(in_point - pointOne, pointTwo - pointOne) / minDistance));
		const sf::Vector2f projection = pointOne + t * (pointTwo - pointOne);  // Projection falls on the segment
		return Distance(in_point, projection);
	}

	[[nodiscard]]
	// Source:
	//https://github.com/SFML/SFML/wiki/Source%3A-cubic-bezier-curve
	static std::vector<sf::Vector2f> CalcCubicBezier(
		const sf::Vector2f& start,
		const sf::Vector2f& end,
		const sf::Vector2f& startControl,
		const sf::Vector2f& endControl,
		const size_t numSegments)
	{
		std::vector<sf::Vector2f> ret;
		if (!numSegments) // Any points at all?
			return ret;

		ret.push_back(start); // First point is fixed
		float p = 1.f / numSegments;
		float q = p;
		for (size_t i = 1; i < numSegments; i++, p += q) // Generate all between
			ret.push_back(p * p * p * (end + 3.f * (startControl - endControl) - start) +
				3.f * p * p * (start - 2.f * startControl + endControl) +
				3.f * p * (startControl - start) + start);
		ret.push_back(end); // Last point is fixed
		return ret;
	}

	[[nodiscard]]
	static void WriteTOBMP(std::string filename, int width, int height, std::vector<int> Cells)
	{
		FILE* file;
		unsigned char* img = NULL;
		int filesize = 54 + 3 * width * height;  //w is your image width, h is image height, both int

		img = (unsigned char*)malloc(3 * width * height);
		memset(img, 0, 3 * width * height);

		for (int i = 0; i < width; i++)
		{
			for (int j = 0; j < height; j++)
			{
				int x = i;
				int y = (height - 1) - j;
				int r, g, b;

				switch (Cells[(x + y * width)])
				{
				case 0: // NONE
					r = 0;
					g = 255;
					b = 0;
					break;
				case 1: // WALL
					r = 0;
					g = 0;
					b = 0;
					break;
				case 2: // WALKABLE
					r = 0;
					g = 255;
					b = 255;
					break;
				case 3: // BUILDING
					r = 255;
					g = 0;
					b = 255;
					break;
				default:
					break;
				}
				img[(x + y * width) * 3 + 2] = (unsigned char)(r);
				img[(x + y * width) * 3 + 1] = (unsigned char)(g);
				img[(x + y * width) * 3 + 0] = (unsigned char)(b);
			}
		}

		unsigned char bmpfileheader[14] = { 'B','M', 0,0,0,0, 0,0, 0,0, 54,0,0,0 };
		unsigned char bmpinfoheader[40] = { 40,0,0,0, 0,0,0,0, 0,0,0,0, 1,0, 24,0 };
		unsigned char bmppad[3] = { 0,0,0 };

		bmpfileheader[2] = (unsigned char)(filesize);
		bmpfileheader[3] = (unsigned char)(filesize >> 8);
		bmpfileheader[4] = (unsigned char)(filesize >> 16);
		bmpfileheader[5] = (unsigned char)(filesize >> 24);

		bmpinfoheader[4] = (unsigned char)(width);
		bmpinfoheader[5] = (unsigned char)(width >> 8);
		bmpinfoheader[6] = (unsigned char)(width >> 16);
		bmpinfoheader[7] = (unsigned char)(width >> 24);
		bmpinfoheader[8] = (unsigned char)(height);
		bmpinfoheader[9] = (unsigned char)(height >> 8);
		bmpinfoheader[10] = (unsigned char)(height >> 16);
		bmpinfoheader[11] = (unsigned char)(height >> 24);

		fopen_s(&file, filename.c_str(), "wb");
		fwrite(bmpfileheader, 1, 14, file);
		fwrite(bmpinfoheader, 1, 40, file);
		for (int i = 0; i < height; i++)
		{
			fwrite(img + (width * (height - i - 1) * 3), 3, width, file);
			fwrite(bmppad, 1, (4 - (width * 3) % 4) % 4, file);
		}

		free(img);
		fclose(file);
	}
}