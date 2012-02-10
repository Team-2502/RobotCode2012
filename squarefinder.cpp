#include <WPILib.h>
#include <vector>
#include <cmath>
#include "squarefinder.h"

SquareFinder::SquareFinder()
{
	lumPlane = imaqCreateImage(IMAQ_IMAGE_U8, 7);
}

SquareFinder::~SquareFinder()
{
	imaqDispose(lumPlane);
}

TargetReport SquareFinder::getBestTarget(HSLImage *img)
{
	TargetReport ret;
	ret.x = 1337.0; //error code
	ret.y = 0;
	if(!img)
		return ret;
	ret.x = 1338.0;

	int height = img->GetHeight();
	int width = img->GetWidth();
	Image *image = img->GetImaqImage();
	ParticleFilterCriteria2 particleCriteria;
	ParticleFilterOptions particleFilterOptions;
	int numParticles;

	imaqExtractColorPlanes(image, IMAQ_HSL, NULL, NULL, lumPlane);
	image = lumPlane;
	imaqLocalThreshold(image, image, 32, 32, IMAQ_BACKGROUND_CORRECTION, 0.2, IMAQ_DARK_OBJECTS, 1);

	particleCriteria.parameter = IMAQ_MT_AREA;
	particleCriteria.lower = 500;
	particleCriteria.upper = 2000;
	particleCriteria.calibrated = 0;
	particleCriteria.exclude = 0;

	particleFilterOptions.rejectMatches = FALSE;
	particleFilterOptions.rejectBorder = 0;
	particleFilterOptions.connectivity8 = FALSE;

	imaqParticleFilter3(image, image, &particleCriteria, 1, &particleFilterOptions, NULL, &numParticles);

	//imaqConvexHull(image,image,TRUE); //For convex hull analysis.

	//Use the largest one only.
	if(imaqCountParticles(image,TRUE,&numParticles)) {
		if(numParticles >= 1) {
			double smallestDiff = 10000.0;
			for(int i = 0; i < numParticles; i++) {
				double h,w;
				imaqMeasureParticle(image,i,FALSE,IMAQ_MT_BOUNDING_RECT_HEIGHT,&h);
				imaqMeasureParticle(image,i,FALSE,IMAQ_MT_BOUNDING_RECT_WIDTH,&w);
				if(fabs(w/h-28.0/22.0) < smallestDiff && i != 0) {
					smallestDiff = fabs(w/h-28.0/22.0);
					ret.height = h;
					ret.width  = w;
					imaqMeasureParticle(image,i,FALSE,IMAQ_MT_BOUNDING_RECT_WIDTH,&ret.width);
					imaqMeasureParticle(image,i,FALSE,IMAQ_MT_CENTER_OF_MASS_X,&ret.normalized_x);
					imaqMeasureParticle(image,i,FALSE,IMAQ_MT_CENTER_OF_MASS_Y,&ret.normalized_y);
					imaqMeasureParticle(image,i,FALSE,IMAQ_MT_AREA,&ret.size);
					imaqMeasureParticle(image,i,FALSE,IMAQ_MT_BOUNDING_RECT_LEFT,&ret.x);
					imaqMeasureParticle(image,i,FALSE,IMAQ_MT_BOUNDING_RECT_TOP,&ret.y);
				}
			}
			ret.normalized_x = (-1.0+2.0*((ret.normalized_x)/width)); //Map to [-1.0,1.0]
			ret.normalized_y = (-1.0+2.0*((ret.normalized_y)/height));
		}
	}
	return ret;
}
