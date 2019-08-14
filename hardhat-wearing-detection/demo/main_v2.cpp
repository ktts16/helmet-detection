#define _snprintf_s(a,b,c,...) snprintf(a,b,__VA_ARGS__)

// #define WINDOWS  /* uncomment this line to use it for windows.*/ 
#ifdef WINDOWS
#include <direct.h>
#define GetCurrentDir _getcwd
#else
#include <unistd.h>
#define GetCurrentDir getcwd
#endif
#include <iostream>

#include <vector>
#include <algorithm>
#include <stdio.h>
#include <caffe/caffe.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <dirent.h>
#include <sys/types.h>
#include <bits/stdc++.h>

using namespace cv;
using namespace std;
using namespace caffe;

struct BBox {
	float x1, y1, x2, y2, score;
	int label;
};
void preprocess(cv::Mat& frame, cv::Mat& preprocessed, cv::Size caffeInputSize, cv::Scalar mean, float inputscale);
void resizeImage(cv::Mat& frame, cv::Mat& dst, cv::Size dims);

typedef struct NetParameters {
	cv::Size caffeInputSize;
	cv::Scalar mean;
	float inputscale;
} NetParameters;

/** my methods **/

void object_detection(std::string filepath, std::string filename, std::string caffemodel, Net *net, NetParameters *netParams, float kScoreThreshold);

std::string GetCurrentWorkingDir( void ) {
	char buff[FILENAME_MAX];
	GetCurrentDir( buff, FILENAME_MAX );
	std::string current_working_dir(buff);
	return current_working_dir;
}

std::string GetLastPart( std::string s, std::string delimiter ) {
	size_t pos = 0;
	std::string token;
	while ((pos = s.find(delimiter)) != std::string::npos) {
		token = s.substr(0, pos);
		s.erase(0, pos + delimiter.length());
		// std::cout << token << std::endl;
	}
	// std::cout << s << std::endl;
	return s;
}

std::string ReplaceString( std::string s, std::string oldstr, std::string newstr ) {
	// replace oldstr with newstr
	size_t pos = 0;
	pos = s.find(oldstr);
	s.replace(pos, oldstr.length(), newstr);
	return s;
}

void list_dir(const char *path, vector <string> * filenames) {
	std::string s;
	struct dirent *entry;
	DIR *dir = opendir(path);

	if (dir == NULL) {
		return;
	}
	while ((entry = readdir(dir)) != NULL) {
		if(strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0)	//exclude . and ..
			filenames->push_back(entry->d_name);
	}
	closedir(dir);
}

int main(int argc, char* argv[]) {
	if (caffe::GPUAvailable()) {
		caffe::SetMode(caffe::GPU, 0);
	}

	// char path_c[] = "/home/krittametht/data/SPID2016V2/set00_sel/images";
	// char path_c[] = "/home/krittametht/data/SPID2016V2/set00_sel02";
	char path_c[] = "/home/krittametht/data/SPID2016V2/set00_sel03";
	std::string path(path_c);

	vector <string> filenames; 
	list_dir(path_c, &filenames);

	std::string caffemodel;
	std::string prototxt;
	NetParameters netParams;
	float kScoreThreshold;

	/** input size, mean, input scale from: 
	https://github.com/wujixiu/helmet-detection 
	**/
	// > Pelee RPA
	// prototxt = "../../models/Pelee_RPA.prototxt";
	// caffemodel = "../../models/Pelee-RPA_Hardhat_iter_120000.caffemodel";
	// netParams.caffeInputSize = cv::Size(304, 304);
	// netParams.mean = cv::Scalar(103.94, 116.78, 123.68);
	// netParams.inputscale = 0.017f;
	//// cv::Size caffeInputSize(304, 304);
	//// cv::Scalar mean(103.94, 116.78, 123.68);
	//// float inputscale = 0.017f;

	// > SSD RPA 512
	prototxt = "../../models/SSD_RPA512.prototxt";
	caffemodel = "../../models/VGG_RPA512_Hardhat_iter_148000.caffemodel";
	netParams.caffeInputSize = cv::Size(512, 512);
	netParams.mean = cv::Scalar(104, 117, 123);
	netParams.inputscale = 1.0f;
	//// cv::Size caffeInputSize(512, 512);
	//// cv::Scalar mean(104, 117, 123);
	//// float inputscale = 1.0f;

	// > my trained model
	// // prototxt = "../../models/SSD_RPA300.prototxt";	//???
	// prototxt = "../../SSD-RPA/examples/Hardhat/SSD300/deploy.prototxt";
	// caffemodel = "../../SSD-RPA/examples/Hardhat/SSD300/models/VGG_Hat_SSD_300x300_iter_6000.caffemodel";
	// netParams.caffeInputSize = cv::Size(300, 300);
	// netParams.mean = cv::Scalar(104, 117, 123);
	// netParams.inputscale = 1.0f;

	Net net(prototxt);
	net.CopyTrainedLayersFrom(caffemodel);

	// std::cout << GetCurrentWorkingDir() << std::endl;	//DEBUG

	// kScoreThreshold = 0.1f;
	kScoreThreshold = 0.2f;
	// kScoreThreshold = 0.3f;
	// kScoreThreshold = 0.4f;

	// object_detection(path, filenames[0]);
	for (int i=0; i<filenames.size(); i++) {
		cout << filenames[i] << "\n";
		object_detection(path, filenames[i], caffemodel, &net, &netParams, kScoreThreshold);
	}

	/*
	// file = "../../imgs/demo.jpg";
	// file = "../../imgs/bikerider_test1.jpg";
	// file = "../../imgs/bike_helmet_detection__video__1_mp4__1.png";
	// file = "../../imgs/bike_helmet_detection__video__1_mp4__2.png";
	// file = "../../imgs/bike_helmet_detection__video__1_mp4__3.png";
	// file = "../../imgs/bike_helmet_detection__video__2_mp4__1.png";
	// file = "../../imgs/bike_helmet_detection__video__2_mp4__2.png";
	// file = "../../imgs/bike_helmet_detection__video__2_mp4__3.png";
	// file = "/home/krittametht/data/SPID2016V2/set00_sel/CommunityEntrance-001_00002.jpg";
	// file = "/home/krittametht/data/SPID2016V2/set00_sel/CommunityEntrance-001_00004.jpg";
	// file = "/home/krittametht/data/SPID2016V2/set00_sel/SeieeDept-051_00111.jpg";
	// file = "/home/krittametht/data/SPID2016V2/set00_sel/SeieeDept-051_00207.jpg";
	file = "/home/krittametht/data/SPID2016V2/set00_sel/SeieeDept-051_00925.jpg";
	// file = "/home/krittametht/data/SPID2016V2/set00_sel/SeieeDept-051_00973.jpg";
	// file = "/home/krittametht/data/SPID2016V2/set00_sel/SeieeDept-051_00456.jpg";
	// file = "/home/krittametht/data/SPID2016V2/set00_sel/SeieeDept-051_00781.jpg";
	*/
	//// std::string delimiter = "/";
	//// std::string filename = GetLastPart(file, delimiter);
	////// std::cout << filename << std::endl;

	return 0;
}

void object_detection(std::string filepath, std::string filename, std::string caffemodel, Net *net, NetParameters *netParams, float kScoreThreshold) {
	vector<cv::Scalar> showColor = { cv::Scalar(0), cv::Scalar(0, 255, 255), cv::Scalar(0, 0, 255),
		cv::Scalar(255, 0, 0), cv::Scalar::all(255), cv::Scalar(0, 255, 0)};

	const char* kClassNames[] = { "__background__", "yellow", "red", "blue", "white", "none" };

	// std::cout << GetCurrentWorkingDir() << std::endl;	//DEBUG

	// Net net("../models/Pelee_RPA.prototxt");
	// net.CopyTrainedLayersFrom("../models/Pelee_RPA.caffemodel");
	// Mat img = imread("../imgs/demo.jpg");

	std::string file;
	file = filepath + "/" + filename;

	Mat img = imread(file);	

	std::string delimiter = "/";

	std::string filename_2 = ReplaceString(filename, ".", "__");
	// std::cout << filename_2 << std::endl;

	std::string caffemodel_2 = ReplaceString(GetLastPart(caffemodel, delimiter), ".caffemodel", "__cfm");
	// std::cout << caffemodel_2 << std::endl;

	std::ostringstream ss;
	ss << kScoreThreshold;
	std::string kScoreThresholdStr(ss.str());
	kScoreThresholdStr = ReplaceString(kScoreThresholdStr, ".", "__");
	// std::cout << kScoreThresholdStr << std::endl;	//DEBUG

	Profiler* profiler = Profiler::Get();
	profiler->TurnON();
	uint64_t tic = profiler->Now();

	// preprocess
	cv::Mat preprocessed;
	int height = img.rows;
	int width = img.cols;

	preprocess(img, preprocessed, netParams->caffeInputSize, netParams->mean, netParams->inputscale);

	vector<Mat> bgr;
	cv::split(preprocessed, bgr);

	// fill network input
	shared_ptr<Blob> data = net->blob_by_name("data");
	data->Reshape(1, 3, preprocessed.rows, preprocessed.cols);
	const int bias = data->offset(0, 1, 0, 0);
	const int bytes = bias * sizeof(float);
	memcpy(data->mutable_cpu_data() + 0 * bias, bgr[0].data, bytes);
	memcpy(data->mutable_cpu_data() + 1 * bias, bgr[1].data, bytes);
	memcpy(data->mutable_cpu_data() + 2 * bias, bgr[2].data, bytes);

	// forward
	net->Forward();

	// get output, shape is N x 7
	shared_ptr<Blob> result = net->blob_by_name("detection_out");
	const float* result_data = result->cpu_data();
	const int num_det = result->num();
	vector<BBox> detections;
	for (int k = 0; k < num_det; ++k) {
		if (result_data[0] != -1 && result_data[2] > kScoreThreshold) {
			// [image_id, label, score, xmin, ymin, xmax, ymax]
			BBox bbox;
			bbox.x1 = result_data[3] * width;
			bbox.y1 = result_data[4] * height;
			bbox.x2 = result_data[5] * width;
			bbox.y2 = result_data[6] * height;
			bbox.score = result_data[2];
			bbox.label = static_cast<int>(result_data[1]);
			detections.push_back(bbox);
		}
		result_data += 7;
	}

	// draw
	for (auto& bbox : detections) {
		cv::Rect rect(bbox.x1, bbox.y1, bbox.x2 - bbox.x1 + 1, bbox.y2 - bbox.y1 + 1);
		cv::rectangle(img, rect, showColor[bbox.label], 2);
		char buff[300];
		_snprintf_s(buff, sizeof(buff), "%s: %.2f", kClassNames[bbox.label], bbox.score);
		cv::putText(img, buff, cv::Point(bbox.x1, bbox.y1), FONT_HERSHEY_PLAIN, 1, showColor[bbox.label]);
	}
	uint64_t toc = profiler->Now();
	profiler->TurnOFF();
	profiler->DumpProfile("./ssd-profile.json");

	LOG(INFO) << "Costs " << (toc - tic) / 1000.f << " ms";
	cv::imwrite("./ssd-result/" + caffemodel_2 + "___" + "st_" + kScoreThresholdStr + "___" + filename_2 + ".jpg", img);
	// cv::imshow("result", img);
	// cv::waitKey(0);

}

void preprocess(cv::Mat& frame, cv::Mat& preprocessed, cv::Size caffeInputSize, cv::Scalar mean, float inputscale)
{

	frame.convertTo(preprocessed, CV_32F, 1.0 / 255.0, 0.0);
	resizeImage(preprocessed, preprocessed, caffeInputSize);
	preprocessed.convertTo(preprocessed, CV_32F, 255.0, 0);
	cv::subtract(preprocessed, mean, preprocessed);
	preprocessed = preprocessed * inputscale;
}
void resizeImage(cv::Mat& frame, cv::Mat& dst, cv::Size dims)
{
	double max, min;
	int idx_min[2] = { 255, 255 }, idx_max[2] = { 255, 255 };
	cv::Mat tempImage;
	frame.copyTo(tempImage);
	tempImage = tempImage.reshape(1);
	cv::minMaxIdx(tempImage, &min, &max, idx_min, idx_max);

	cv::Mat imageStd;
	cv::Mat resizedStd;
	cv::subtract(frame, cv::Scalar::all(min), imageStd);

	imageStd = imageStd / (max - min);
	cv::resize(imageStd, resizedStd, dims);
	resizedStd = resizedStd * (max - min);
	cv::add(resizedStd, cv::Scalar::all(min), dst);
}
