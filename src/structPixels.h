#pragma once

#include <iostream>
#include <map>
#include <string>

namespace Eng
{

	/**
     * A structure to represent pixels
     */
	struct Pixel{
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
	};

	inline std::ostream& operator<<(std::ostream& os, const Pixel& rhs)
	{
	    os << "{" << rhs.x << "," << rhs.y << "}";
	    return os;
	}
	/**
     * A structure to represent weighted pixels
     */
	struct WeightedPixel{
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
	};

	inline std::ostream& operator<<(std::ostream& os, const WeightedPixel& rhs)
	{
	    os << "{" << rhs.x << "," << rhs.y << "," << rhs.w << "}";
	    return os;
	}


	/**
     * A structure to represent colors
     */
	struct RGB{
		unsigned char R;
		unsigned char G;
		unsigned char B;

		bool operator ==(const RGB& rhs) const
		{
			return (this->R == rhs.R && this->G == rhs.G && this->B == rhs.B);
		};
	};

	inline std::ostream& operator<<(std::ostream& os, const RGB& rhs)
	{
	    os << "R:" << rhs.R << "B:" << rhs.B << "G:" << rhs.G;
	    return os;
	}

}//End of namespace Eng

namespace std {
    template <>
    struct hash<Eng::Pixel>
    {
        size_t operator()( const Eng::Pixel& k ) const
        {
            // Compute individual hash values for first and second
            // http://stackoverflow.com/a/1646913/126995
            size_t res = 17;
            res = res * 31 + hash<unsigned int>()( k.x );
            res = res * 31 + hash<unsigned int>()( k.y );
            return res;
        }
    };

}//End of namespace std



