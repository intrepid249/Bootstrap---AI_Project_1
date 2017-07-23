#pragma once


/** Created by Jack McCall*/
struct Colour {
public:
	Colour() {
		colour = 0xffffffff;
	}

	Colour(unsigned int col) {
		colour = col;
	}

	Colour operator = (const unsigned int &rhs) {
		colour = rhs;
	}

	operator unsigned int() {
		return colour;
	}

	/** Darken the colour by the specified amount*/
	Colour darken(double amount = 0.4) const {
		unsigned int r = (((colour & 0xFF000000) * amount) < 0x0) ? 0x0 : (unsigned int)((colour & 0xFF000000) * amount);
		unsigned int g = (((colour & 0x00FF0000) * amount) < 0x0) ? 0x0 : (unsigned int)((colour & 0x00FF0000) * amount);
		unsigned int b = (((colour & 0x0000FF00) * amount) < 0x0) ? 0x0 : (unsigned int)((colour & 0x0000FF00) * amount);
		unsigned int a = (unsigned int)(colour & 0x000000FF);
		return r | g | b | a;
	}

	/** Lighten the colour by the specified amount*/
	Colour lighten(double amount = 0.6) const {
		unsigned int r = (((colour & 0xFF000000) + (0xFF * amount)) > 0xFF) ? 0xFF : (unsigned int)((colour & 0xFF000000) + (0xFF * amount));
		unsigned int g = (((colour & 0x00FF0000) + (0xFF * amount)) > 0xFF) ? 0xFF : (unsigned int)((colour & 0x00FF0000) + (0xFF * amount));
		unsigned int b = (((colour & 0x0000FF00) + (0xFF * amount)) > 0xFF) ? 0xFF : (unsigned int)((colour & 0x0000FF00) + (0xFF * amount));
		unsigned int a = (unsigned int)(colour & 0x000000FF);
		return r | g | b | a;
	}

private:
	/** An rgba value in the format 0xRRGGBBAA*/
	unsigned int colour;
};