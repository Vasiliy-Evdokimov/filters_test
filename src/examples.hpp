/*
 * examples.hpp
 *
 *  Created on: 4 окт. 2023 г.
 *      Author: vevdokimov
 */

#include "opencv2/core/types.hpp"

#ifndef EXAMPLES_HPP_
#define EXAMPLES_HPP_

namespace blur_example { int run(cv::Mat aSrc); }
namespace erosion_dilation_example { int run(cv::Mat aSrc); }
namespace moprphology_example {	int run(cv::Mat aSrc); }
namespace hitmiss_example {	int run(cv::Mat aSrc); }
namespace threshold_example { int run(cv::Mat aSrc); }

#endif /* EXAMPLES_HPP_ */
