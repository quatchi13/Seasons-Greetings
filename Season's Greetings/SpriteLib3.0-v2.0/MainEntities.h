#ifndef __MAINENTITIES_H__
#define __MAINENTITIES_H__

class MainEntities
{
public:
	//Getters
	static int MainCamera();
	static int MainPlayer();
	static int CameraFocus();

	//Setters
	static void MainCamera(int main);
	static void MainPlayer(int main);
	static void CameraFocus(int main);

private:
	static int m_mainCamera;
	static int m_mainPlayer;
	static int m_cameraFocus;
};

#endif // !__MAINENTITIES_H__
