#pragma once

typedef struct {
	unsigned char R, G, B;
}Color;

//typedef struct

unsigned char inline Blend(const unsigned char& a, const unsigned char& b, const float& w) {
	return  a + (b - a) * w;
}

Color inline BlendColor(const Color& a, const Color& b,const float& w){
	return Color{ Blend(a.R,b.R,w), Blend(a.G,b.G,w), Blend(a.B,b.B,w) };
}


