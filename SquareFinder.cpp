#include <WPILib.h>
#include <algorithm>
#include <vector>
#include <cmath>
#include "SquareFinder.h"

#include "Display.h"
#include "Singleton.h"

SquareFinder::SquareFinder()
{
	lumPlane = imaqCreateImage(IMAQ_IMAGE_U8, 7);
}

SquareFinder::~SquareFinder()
{
	imaqDispose(lumPlane);
}

void SquareFinder::GetBestTargets(HSLImage *img, TargetReport* targets, int& count)
{
	count = 0;
	TargetReport& ret2 = targets[0];
	ret2.x = 1337.0; //error code
	ret2.y = 0;
	if(!img)
		return;
	ret2.x = 1338.0;

	int height = img->GetHeight();
	int width = img->GetWidth();
	Image *image = img->GetImaqImage();
	//Parameter, Lower, Upper, Calibrated?, Exclude?
	ParticleFilterCriteria2 particleCriteria_initial[1] = { {IMAQ_MT_AREA_BY_IMAGE_AREA,25,100,0,1} };
	ParticleFilterCriteria2 particleCriteria[1] = { {IMAQ_MT_RATIO_OF_EQUIVALENT_RECT_SIDES,1,2,0,0} };
	ParticleFilterOptions particleFilterOptions[1] = { {FALSE,0,FALSE} };
	ParticleFilterOptions particleFilterOptions_conn8[1] = { {FALSE,0,TRUE} };
	int numParticles;

	imaqExtractColorPlanes(image, IMAQ_HSL, NULL, NULL, lumPlane);
	image = lumPlane;
	//imaqLocalThreshold(image, image, 32, 32, IMAQ_BACKGROUND_CORRECTION, 0.2, IMAQ_DARK_OBJECTS, 1);
	imaqInverse(image,image,NULL);
	imaqAutoThreshold2(image, image, 2, IMAQ_THRESH_INTERCLASS, NULL);
	imaqParticleFilter3(image, image, particleCriteria_initial, 1, particleFilterOptions, NULL, &numParticles);
	imaqFillHoles(image, image, TRUE);

	int pKernel[9] = {1,1,1,1,1,1,1,1,1};
	StructuringElement structElem[1] = { { 3, 3, FALSE, pKernel } };
	imaqSizeFilter(image, image, TRUE, 2, IMAQ_KEEP_LARGE, structElem);

	imaqParticleFilter3(image, image, particleCriteria, 1, particleFilterOptions_conn8, NULL, &numParticles);
	//imaqWriteJPEGFile(image,"/step7.jpg",1000,NULL);

	vector<TargetReport> reports;

	//Use the most proportional.
	if(imaqCountParticles(image,TRUE,&numParticles)) {
		TargetReport ret;
		if(numParticles >= 1) {
			for(int i = 0; i < numParticles; i++) {
				double h,w,area;
				imaqMeasureParticle(image,i,FALSE,IMAQ_MT_BOUNDING_RECT_HEIGHT,&h);
				imaqMeasureParticle(image,i,FALSE,IMAQ_MT_BOUNDING_RECT_WIDTH,&w);
				imaqMeasureParticle(image,i,FALSE,IMAQ_MT_AREA,&area);
				if(area/(w*h) > 0.8) {
					ret.height = h;
					ret.width  = w;
					ret.size = area;
					imaqMeasureParticle(image,i,FALSE,IMAQ_MT_CENTER_OF_MASS_X,&ret.normalized_x);
					imaqMeasureParticle(image,i,FALSE,IMAQ_MT_CENTER_OF_MASS_Y,&ret.normalized_y);
					imaqMeasureParticle(image,i,FALSE,IMAQ_MT_BOUNDING_RECT_LEFT,&ret.x);
					imaqMeasureParticle(image,i,FALSE,IMAQ_MT_BOUNDING_RECT_TOP,&ret.y);
					ret.normalized_x = (-1.0+2.0*((ret.normalized_x)/width)); //Map to [-1.0,1.0]
					ret.normalized_y = (-1.0+2.0*((ret.normalized_y)/height));
					ret.normalizedWidth = (w / width);
					ret.normalizedHeight = (h / height);
					//ret.distance = 705.178571429 / h; //In feet.
					ret.distance = 752.746418773 / h; //In feet.
					reports.push_back(ret);
				}
			}
		}
	}
	
	sort(reports.begin(),reports.end());

	if(reports.size()) {
		count = reports.size() >= 4 ? 4 : reports.size();
		unsigned i = 0;
		for(vector<TargetReport>::iterator it = reports.begin(); it != reports.end(); ++it) {
			targets[i++] = (*it);
		}
	} else {
		targets[0].distance = 0;
	}
}

