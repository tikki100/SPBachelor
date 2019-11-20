#pragma once


namespace Eng
{

	/**
     * A structure to represent pixels
     */
	typedef struct Pixel{
		unsigned int x; ///The x-coordinate
		unsigned int y; ///The y-coordinate
		
		bool operator ==(const Pixel& rhs) const
		{
			return (this->x == rhs.x && this->y == rhs.y);
		};

		bool operator !=(const Pixel& rhs) const
		{
			return (this->x != rhs.x || this->y != rhs.y);
		};

		bool operator <(const Pixel& rhs) const
		{
			return std::tie(this->x, this->y) < std::tie(rhs.x, rhs.y);
		}

		std::ostream& operator<<(std::ostream& os)
		{
		    os << "{" << this->x << "," << this->y << "}";
		    return os;
		}
	} Pixel;
	/**
     * A structure to represent weighted pixels
     */
	typedef struct WeightedPixel{
		unsigned int x; ///The x-coordinate
		unsigned int y; ///The y-coordinate
		float w; ///The weight
		bool operator ==(const WeightedPixel& rhs) const
		{
			return (this->x == rhs.x && this->y == rhs.y && this->w == rhs.w);
		};
		bool operator !=(const WeightedPixel& rhs) const
		{
			return (this->x != rhs.x || this->y != rhs.y || this->w != rhs.w);
		};

		bool operator <(const WeightedPixel& rhs) const
		{
			return rhs.w < this->w;
		}
		std::ostream& operator<<(std::ostream& os)
		{
		    os << "{" << this->x << "," << this->y << "," << this->w << "}";
		    return os;
		}
	} WeightedPixel;


	/**
     * A structure to represent colors
     */
	typedef struct RGB{
		unsigned char R;
		unsigned char G;
		unsigned char B;
		bool operator ==(const RGB& rhs) const
		{
			return (this->R == rhs.R && this->G == rhs.G && this->B == rhs.B);
		};

		std::ostream& operator<<(std::ostream& os)
		{
		    os << "R:" << this->R << " G:" << this->G << " B:" << this->B;
		    return os;
		}
	} RGB;
}//End of namespace Eng
