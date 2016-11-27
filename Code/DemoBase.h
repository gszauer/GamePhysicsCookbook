#ifndef _H_DEMO_BASE_
#define _H_DEMO_BASE_

class DemoBase {
private: // Disable
	DemoBase(const DemoBase&);
	DemoBase& operator=(const DemoBase);
protected:
	bool show_help = false; // TODO: Include scene navigation instructions
	// TODO: We should add the camera and shit here!
	// TODO: Common Variables
public:
	inline DemoBase() { }
	inline virtual ~DemoBase() { }

	inline virtual void Initialize() { }
	inline virtual void Render() { }
	inline virtual void Update(float dt) { }
	inline virtual void ImGUI() { }
	inline virtual void Shutdown() { }
};

#endif 