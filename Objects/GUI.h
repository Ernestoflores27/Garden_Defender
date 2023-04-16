/**
@file GUI.h
@brief Header file containing the declaration of references needed in the GUI class.
*/
#include <opencv4/opencv2/opencv.hpp>
#include <opencv4/opencv2/highgui.hpp>
#include <thread>

/**
@class GUI
@brief Class that handles the GUI and the turret mode.
*/
class GUI
{
public:
	/*
	@brief Constructor for GUI.
	@param *detector_model_ Pointer to the detector object.
	@param turret Pointer to the Turret object.
	*/
	GUI(Detector *detector_model_, Turret *turret_);
	/**
	@brief Method for showing the frame with detected objects and the information to command the turret in manual or automatic mode. It also command the turrent when it is in Manual mode.
	*/
	void start();

	/**
	@brief Callback interface which needs to be implemented by the user.
	**/
	struct Callback_GUI
	{
		virtual void callback_func(int k) = 0;
	};

	/**
	 * Register the callback interface here to receive data.
	 **/
	void registerCallback(Callback_GUI *ca)
	{
		callback = ca;
	}

private:
	/**
	 * @brief Variable to terminate thread.
	 */
	bool running = false;
	/**
	 * @brief Pointer for the thread.
	 */
	std::thread *worker = nullptr;
	/**
	 * @brief Input width of the YOLOv5 model.
	 */
	const int inpWidth = 352;
	/**
	 * @brief Input height of the YOLOv5 model.
	 */
	const int inpHeight = 352;
	/**
	 * @brief Draws a crosshair in the center of a given frame for tracking.
	 */
	void drawCrossair(cv::Mat &frame);
	/**
	 * @brief Draws a green line between the center of the detected object and the center of the video frame for tracking.
	 */
	void lineClosest(cv::Mat &frame);
	Detector *detector_model;
	Callback_GUI *callback = nullptr;
	Turret *turret;
	/**
	@brief Method for showing the frame with detected objects and the crosshair on the turret.
	*/
	void showShooting();
	/**
	@brief OpenCV Mat object containing the current frame.
	*/
	cv::Mat frame;
};
