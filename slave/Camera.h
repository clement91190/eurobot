#ifndef CAMERA_H
#define CAMERA_H


#include <Servo.h>
class Camera
{
	private:
		Servo camera;
		
	public:
		Camera();
		void inclinaison_frontale();
		void inclinaison_mediane();
		void inclinaison_par_terre();
		int inclinaison();
};

#endif

