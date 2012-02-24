#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

struct TargetReport {
	double width;
	double height;
	double x;
	double y;
	double size;
	double normalized_x;
	double normalized_y;
	double normalizedWidth;
	double normalizedHeight;
	double distance;
};

bool *compareTargets(TargetReport *targets, int numTargets);
int main() {
	TargetReport bask1; bask1.x = 400; bask1.y = 150; bask1.width = 300; bask1.height = 200;
	TargetReport bask2; bask2.x = 200; bask2.y = 300; bask2.width = 300; bask2.height = 200;
	TargetReport bask3; bask3.x = 600; bask3.y = 310; bask3.width = 300; bask3.height = 200;
	TargetReport bask4; bask4.x = 400; bask4.y = 425; bask4.width = 300; bask4.height = 200;
	TargetReport *targets = new TargetReport[4];
	//targets[0] = bask2;
	targets[0] = bask1;
	targets[1] = bask4;
	targets[2] = bask3;
	
	compareTargets(targets, 3);
}

bool compareTargetReportY(TargetReport a, TargetReport b) { return (a.y < b.y); }
bool compareTargetReportX(TargetReport a, TargetReport b) { return (a.x < b.x); }

bool *compareTargets(TargetReport *targets, int numTargets) {
	vector <TargetReport> ordered;
	for (int i = 0; i < numTargets; i++) ordered.push_back(targets[i]);
	
	sort(ordered.begin(), ordered.end(), compareTargetReportY);
	bool *ret = new bool[4];
	ret[0] = 0; ret[1] = 0; ret[2] = 0; ret[3] = 0;
	int last_dif = 0;
	int middle_y = 0;
	int middle_x = 0;
	for (int i = 1; i < ordered.size(); i++) {
		last_dif = ordered[i].y - ordered[i-1].y;
		if (last_dif <= ordered[i].height * .2 && last_dif >= 0-ordered[i].height * .2) {
			ret[1] = 1;
			ret[2] = 1;
			if (ordered[i].x < ordered[i-1].x) {
				targets[1] = ordered[i];
				targets[2] = ordered[i-1];
			} else {
				targets[2] = ordered[i];
				targets[1] = ordered[i-1];
			}
			middle_y = ordered[i].y;
		}
	}
	sort(ordered.begin(), ordered.end(), compareTargetReportX);
	for (int i = 1; i < ordered.size(); i++) {
		last_dif = ordered[i].x - ordered[i-1].x;
		if (last_dif <= ordered[i].width * .1 && last_dif >= 0-ordered[i].width * .1) {
			ret[0] = 1;
			ret[3] = 1;
			if (ordered[i].y < ordered[i-1].y) {
				targets[0] = ordered[i];
				targets[3] = ordered[i-1];
			} else {
				targets[3] = ordered[i];
				targets[0] = ordered[i-1];
			}
			middle_x = ordered[i].x;
		}
	}
	
	for (int i = 0; i < ordered.size(); i++) {
		if (middle_y != 0) {
			if (ordered[i].y < middle_y
			 && ordered[i].x >= (targets[1].x + targets[2].x) / 2 - ordered[i].width * .1
			 && ordered[i].x <= (targets[1].x + targets[2].x) / 2 + ordered[i].width * .1){
				ret[0] = 1;
				targets[0] = ordered[i];
			}
			if (ordered[i].y > middle_y
			 && ordered[i].x >= (targets[1].x + targets[2].x) / 2 - ordered[i].width * .1
			 && ordered[i].x <= (targets[1].x + targets[2].x) / 2 + ordered[i].width * .1){
				ret[3] = 1;
				targets[3] = ordered[i];
			}
		}
	}
	
	for (int i = 0; i < ordered.size(); i++) {
		if (middle_x != 0) {
			if (ordered[i].x > middle_x 
			 && ordered[i].y >= (targets[0].y + targets[3].y) / 2 - ordered[i].height * .2
			 && ordered[i].y <= (targets[0].y + targets[3].y) / 2 + ordered[i].height * .2 ){
				ret[2] = 1;
				targets[2] = ordered[i];
			}
			if (ordered[i].x < middle_x 
			 && ordered[i].y >= (targets[0].y + targets[3].y) / 2 - ordered[i].height * .2
			 && ordered[i].y <= (targets[0].y + targets[3].y) / 2 + ordered[i].height * .2 ){
				ret[1] = 1;
				targets[1] = ordered[i];
			}
		}
	}
	
	
	for (int i = 0; i < 4; i++) {
		cout << (i+1) << ": " << ret[i] << "\n";
		cout << "\t" << (i+1) << ": " << targets[i].x << ", " << targets[i].y << "\n";
	}
	return ret;
}

