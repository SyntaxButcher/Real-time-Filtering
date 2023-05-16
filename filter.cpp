#include "filter.h"

// function to convert to grayscale, all 3 channels set to same value w.r.t 0.33/0.33/0.33 RGB ratio roughly
int greyscale(Mat frame, Mat& edittedFrame) {
	for (int row = 0; row < frame.rows; row++) {
		// using row pointers for speed
		Vec3b* rowptr = frame.ptr<Vec3b>(row);
		Vec3b* rowptrf = edittedFrame.ptr<Vec3b>(row);
		for (int col = 0; col < frame.cols; col++) {
			int grayscaleVal = (rowptr[col][0] + rowptr[col][1] + rowptr[col][2]) / 3;
			for (int channel = 0; channel <= 2; channel++) {
				rowptrf[col][channel] = grayscaleVal;
			}
		}
	}
	return 0;
}

// function to apply seperable gaussian filter
int gaussBlur(Mat frame, Mat& edittedFrame) {
	// filters are [1 2 4 2 1] and [1 2 4 2 1] transposed & normalized
	// intermediate dummy array so we don't convolve with editted values
	Mat dummy;
	frame.copyTo(dummy);

	for (int row = 2; row < frame.rows - 2; row++) {
		// row ptrs for all the rows(up 2, down 2)
		Vec3b* rowptr = frame.ptr<Vec3b>(row);
		Vec3b* rowptru1 = frame.ptr<Vec3b>(row - 1);
		Vec3b* rowptru2 = frame.ptr<Vec3b>(row - 2);
		Vec3b* rowptrd1 = frame.ptr<Vec3b>(row + 1);
		Vec3b* rowptrd2 = frame.ptr<Vec3b>(row + 2);
		// dummy ptr to update
		Vec3b* rowptrf = dummy.ptr<Vec3b>(row);
		for (int col = 0; col < frame.cols; col++) {
			for (int channel = 0; channel <= 2; channel++) {
				rowptrf[col][channel] = (1 * rowptru2[col][channel] + 2 * rowptru1[col][channel] + 4 * rowptr[col][channel] + 2 * rowptrd1[col][channel] + 1 * rowptrd2[col][channel]) / 10;
			}
		}
	}

	for (int row = 0; row < frame.rows; row++) {
		// row ptr to apply on final frame from dummy frame
		Vec3b* rowptr = dummy.ptr<Vec3b>(row);
		Vec3b* rowptrf = edittedFrame.ptr<Vec3b>(row);
		for (int col = 2; col < frame.cols - 2; col++) {
			for (int channel = 0; channel <= 2; channel++) {
				rowptrf[col][channel] = (1 * rowptr[col - 2][channel] + 2 * rowptr[col - 1][channel] + 4 * rowptr[col][channel] + 2 * rowptr[col + 1][channel] + 1 * rowptr[col + 2][channel]) / 10;
			}
		}
	}
	return 0;
}

// function to apply seperable sobel x filter(seperable filters are significantly faster)
int sobelx(Mat frame, Mat3s& edittedFrame) {
	// filters are [-1 0 1] and [1 2 1] transposed & normalized
	// intermediate dummy array so we don't convolve with editted values
	Mat dummy;
	frame.copyTo(dummy);

	for (int row = 1; row < frame.rows - 1; row++) {
		// row ptrs for all the rows(up 1, down 1)
		Vec3b* rowptr = frame.ptr<Vec3b>(row);
		Vec3b* rowptru1 = frame.ptr<Vec3b>(row - 1);
		Vec3b* rowptrd1 = frame.ptr<Vec3b>(row + 1);
		// dummy ptr to update
		Vec3b* rowptrf = dummy.ptr<Vec3b>(row);
		for (int col = 0; col < frame.cols; col++) {
			for (int channel = 0; channel <= 2; channel++) {
				rowptrf[col][channel] = (0.25 * rowptru1[col][channel] + 0.5 * rowptr[col][channel] + 0.25 * rowptrd1[col][channel]);
			}
		}
	}

	for (int row = 0; row < frame.rows; row++) {
		// row ptr to apply on final frame from dummy frame
		Vec3b* rowptr = dummy.ptr<Vec3b>(row);
		Vec3s* rowptrf = edittedFrame.ptr<Vec3s>(row);
		for (int col = 1; col < frame.cols - 1; col++) {
			for (int channel = 0; channel <= 2; channel++) {
				rowptrf[col][channel] = (-1 * rowptr[col - 1][channel] + 0 * rowptr[col][channel] + 1 * rowptr[col + 1][channel]);
			}
		}
	}
	return 0;
}

// function to apply seperable sobel y filter(seperable filters are significantly faster)
int sobely(Mat frame, Mat3s& edittedFrame) {
	// filters are [1 0 -1] and [1 2 1] transposed & normalized(flipped to previous function)
	// intermediate dummy array so we don't convolve with editted values
	Mat3s dummy;
	frame.copyTo(dummy);

	for (int row = 1; row < frame.rows - 1; row++) {
		// row ptrs for all the rows(up 1, down 1)
		Vec3b* rowptr = frame.ptr<Vec3b>(row);
		Vec3b* rowptru1 = frame.ptr<Vec3b>(row - 1);
		Vec3b* rowptrd1 = frame.ptr<Vec3b>(row + 1);
		// dummy ptr to update
		Vec3s* rowptrf = dummy.ptr<Vec3s>(row);
		for (int col = 0; col < frame.cols; col++) {
			for (int channel = 0; channel <= 2; channel++) {
				rowptrf[col][channel] = (1 * rowptru1[col][channel] + 0 * rowptr[col][channel] + -1 * rowptrd1[col][channel]);
			}
		}
	}

	for (int row = 0; row < frame.rows; row++) {
		// row ptr to apply on final frame from dummy frame
		Vec3s* rowptr = dummy.ptr<Vec3s>(row);
		Vec3s* rowptrf = edittedFrame.ptr<Vec3s>(row);
		for (int col = 1; col < frame.cols - 1; col++) {
			for (int channel = 0; channel <= 2; channel++) {
				rowptrf[col][channel] = (0.25 * rowptr[col - 1][channel] + 0.5 * rowptr[col][channel] + 0.25 * rowptr[col + 1][channel]);
			}
		}
	}
	return 0;
}

// function to find magnitude, sum of sobelx and y squared.
int magnitude(Mat sobelx, Mat sobely, Mat& edittedFrame) {
	// we add the sum of sqrt of the x and y edges
	for (int row = 0; row < sobelx.rows; row++) {
		// row ptrs
		Vec3b* rowptrx = sobelx.ptr<Vec3b>(row);
		Vec3b* rowptry = sobely.ptr<Vec3b>(row);
		Vec3b* rowptrf = edittedFrame.ptr<Vec3b>(row);
		for (int col = 0; col < sobelx.cols; col++) {
			for (int channel = 0; channel <= 2; channel++) {
				rowptrf[col][channel] = sqrt(rowptrx[col][channel] * rowptrx[col][channel] + rowptry[col][channel] * rowptry[col][channel]);
			}
		}
	}

	return 0;
}

// function to blur and image and quantize colors into N levels
int blurQuantize(Mat frame, Mat& edittedFrame, int levels) {

	Mat dummy;
	int bucket = 255 / levels;
	dummy = frame.clone();
	gaussBlur(frame, dummy);
	// variables for bucket calculations
	short xt, xf;
	for (int row = 0; row < dummy.rows; row++) {
		// row ptrs
		Vec3b* rowptr = dummy.ptr<Vec3b>(row);
		Vec3b* rowptrf = edittedFrame.ptr<Vec3b>(row);
		for (int col = 0; col < dummy.cols; col++) {
			for (int channel = 0; channel <= 2; channel++) {
				xt = (rowptr[col][channel] / bucket);
				xf = xt * bucket;
				rowptrf[col][channel] = xf;
			}
		}
	}

	return 0;
}

// function to apply a custom cartoon filter
int cartoon(cv::Mat& src, cv::Mat& dst, int levels, int magThreshold) {

	cv::Mat3s temp_sx, temp_sy;
	cv::Mat result, sx, sy;

	src.copyTo(result);
	src.copyTo(temp_sx);
	src.copyTo(temp_sy);

	sobelx(src, temp_sx);
	convertScaleAbs(temp_sx, sx);

	sobely(src, temp_sy);
	convertScaleAbs(temp_sy, sy);

	magnitude(sx, sy, result);

	blurQuantize(src, dst, levels);

	for (int row = 0; row < result.rows; row++) {
		// row ptrs
		Vec3b* rowptr = result.ptr<Vec3b>(row);
		Vec3b* rowptrf = dst.ptr<Vec3b>(row);
		for (int col = 0; col < result.cols; col++) {
			for (int channel = 0; channel <= 2; channel++) {
				rowptrf[col][channel] = rowptr[col][channel] > magThreshold ? 0 : rowptrf[col][channel];
			}
		}
	}
	return 0;

}