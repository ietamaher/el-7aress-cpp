#include "videocapture_cuda.h"
#include <iostream>
CaptureVideoCuda::CaptureVideoCuda(QObject *parent)
    : QThread(parent), input(nullptr), output(nullptr), net(nullptr), running(false), enableDetection(false), enableTracking(false) {
    // Constructor body can be empty for now, or include initial setup not related to Qt objects
}


CaptureVideoCuda::~CaptureVideoCuda() {
    //Cleanup();
}

void CaptureVideoCuda::run() {
    running = true;

    if (!InitializeVideoSource() ||  !InitializeDetectNet()) {
        emit ErrorOccurred("Initialization failed");
        return;
    }

    //SetupSignalHandler();

    while (running) {
        uchar3* frame = nullptr;
        int status = 0;

        if (!input->Capture(&frame, &status)) {
            if (status == videoSource::TIMEOUT) continue;
            break; // EOS or error
        }
	// Desired dimensions and aspect ratio
	/*int desired_width = 800;
	int desired_height = 600;
	float desired_aspect = float(desired_width) / desired_height;

	// Original dimensions
	int orig_width = frame.width();
	int orig_height = frame.height();
	
	// Calculate crop dimensions to maintain aspect ratio
	int crop_width, crop_height, x_offset, y_offset;
	if (float(orig_width) / orig_height > desired_aspect) {
	    // Original is too wide
	    crop_width = int(orig_height * desired_aspect);
	    crop_height = orig_height;
	    x_offset = int((orig_width - crop_width) / 2);
	    y_offset = 0;
	} else {
	    // Original is too tall
	    crop_width = orig_width;
	    crop_height = int(orig_width / desired_aspect);
	    x_offset = 0;
	    y_offset = int((orig_height - crop_height) / 2);
	}
	// Assuming 'frame' is a valid cudaImage
	uchar3*  cropped_frame =nullptr;
	cropped_frame.Allocate(crop_width, crop_height, frame.format);*/

	// Compute the ROI as (left, top, right, bottom)
	/*int4 crop_roi = make_int4(x_offset, y_offset, x_offset + crop_width, y_offset + crop_height);

	// Crop the image to the ROI
	cudaCrop(frame, cropped_frame, crop_roi);

	cudaImage resized_frame = {};
	resized_frame.Allocate(desired_width, desired_height, frame.format);

	// Resize the cropped image to the desired dimensions
	cudaResize(cropped_frame, resized_frame);*/	
        // Object detection and other processing on 'image'
        DetectAndRender(frame);

        // Example of emitting a signal with a frame (assuming conversion to QImage is done in DetectAndRender)
        //QImage frame; // Placeholder for actual conversion
        
        //emit FrameReady(frame);
        QThread::msleep(20); // Slow down the loop to a reasonable rate
    }

    //Cleanup();
}

bool CaptureVideoCuda::InitializeVideoSource() {
    input = videoSource::Create("/dev/video1"); // Example usage, adjust according to your API  "pedestrians_tracking.mp4"
    if (!input) {
        emit ErrorOccurred("Failed to create video source");
        return false;
    }
    return true;
}

bool CaptureVideoCuda::InitializeDetectNet() {
    net = detectNet::Create("ssd-mobilenet-v2"); // Example usage, adjust according to your API  ssd-mobilenet-v2
    net->SetTracker(objectTrackerIOU::Create(3, 15, 0.5f));
    if (!net) {
        emit ErrorOccurred("Failed to load detection network");
        return false;
    }
    return true;
}

void CaptureVideoCuda::DetectAndRender(uchar3* image) {
    if (!image) return;

    // Object detection logic
    if (this->enableDetection){
    	detectNet::Detection* detections = nullptr;
	//int numDetections = net.Detect(image, desired_width, desired_height, &detections, detectNet::OVERLAY_NONE);
       const int numDetections = net->Detect(image, input->GetWidth(), input->GetHeight(), &detections, detectNet::OVERLAY_NONE);
       if (numDetections > 0){
		for (int i = 0; i < numDetections; ++i) {
			float center_x, center_y;
			detections[i].Center(&center_x, &center_y); // Assuming Center is a method
			float width = detections[i].Width();        // Assuming Width is a method
			float height = detections[i].Height();      // Assuming Height is a method

			// Calculate bounding box coordinates
			float x1 = center_x - width / 2;
			float x2 = center_x + width / 2;
			float y1 = center_y - height / 2;
			float y2 = center_y + height / 2;

			// Draw lines

			CUDA(cudaDrawLine(image, 1280, 720, x1, y1 , x2, y1, make_float4(230,0,0,200), 1));	
			CUDA(cudaDrawLine(image, 1280, 720, x1, y1 , x1, y2, make_float4(230,0,0,200), 1));			
			CUDA(cudaDrawLine(image, 1280, 720, x1, y2 , x2, y2, make_float4(230,0,0,200), 1));			
			CUDA(cudaDrawLine(image, 1280, 720, x2, y1 , x2, y2, make_float4(230,0,0,200), 1));
			//CUDA(cudaDrawRect(image, 1280, 720, x1, y1, x2, y2, make_float4(0,127,0,200)));
			CUDA(cudaDrawCircle(image, 1280, 720, 1200, 80, 50, make_float4(230,0,0,200)));
			
		}       
       
       }
  
		// Optionally, render detections on the image here
    }
    // Convert the processed image to QImage for GUI display
    QImage qImage = ConvertToQImage(image, input->GetWidth(), input->GetHeight());
    emit frameReady(qImage);
}


QImage CaptureVideoCuda::ConvertToQImage(uchar3* image, int width, int height) {
    // Ensure 'image' is in host memory and in an appropriate format for conversion
    QImage qImage(reinterpret_cast<uchar*>(image), width, height, QImage::Format_RGB888);
    return qImage.copy(); // The copy ensures the QImage owns its data
}

void CaptureVideoCuda::toggleDetection(bool enabled) {
    // Logic to enable/disable detection
    this->enableDetection = enabled;
    std::cout << "Detection toggled to: " << (enabled ? "ON" : "OFF");
}

void CaptureVideoCuda::toggleTracking(bool enabled) {
    this->enableTracking = enabled;
    // Logic to enable/disable tracking
    std::cout << "Tracking toggled to: " << (enabled ? "ON" : "OFF");
} 

void CaptureVideoCuda::stop() {
    running = false;
    //wait(); // Wait for the run loop to finish
}
