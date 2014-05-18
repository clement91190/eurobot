#include "Camera.h" 
   
   Camera::Camera()
   {
	   #ifdef PMI
			//camera.attach(PIN_SERVO_CAM);
			//inclinaison_par_terre();
	   #else
			//placer la camera du gros si ca marche
			//camera.attach(PIN_SERVO_CAM);
	   #endif
   }
   
   void Camera::inclinaison_frontale()
   {
	   #ifdef PMI
			camera.writeMicroseconds(1950);
	   #else
	   
	   #endif
   }
   void Camera::inclinaison_mediane()
   {
	   #ifdef PMI
			camera.writeMicroseconds(1575);
	   #else
	   
	   #endif
   }
    void Camera::inclinaison_par_terre()
   {
	   #ifdef PMI
			camera.writeMicroseconds(1200);
	   #else
	   
	   #endif
   }


