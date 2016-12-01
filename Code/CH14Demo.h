#ifndef _H_CH14_DEMO_
#define _H_CH14_DEMO_

#include "DemoBase.h"

class CH14Demo : public DemoBase {
protected:
	OBB obb;
	Ray ray;
	bool leftWasDown;
public:
	void Initialize(int width, int height);
	void Render();
	void Update(float dt);
};

#endif 
