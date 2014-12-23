/*
    Copyright (c) 2011, Philipp Krähenbühl
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:
        * Redistributions of source code must retain the above copyright
        notice, this list of conditions and the following disclaimer.
        * Redistributions in binary form must reproduce the above copyright
        notice, this list of conditions and the following disclaimer in the
        documentation and/or other materials provided with the distribution.
        * Neither the name of the Stanford University nor the
        names of its contributors may be used to endorse or promote products
        derived from this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY Philipp Krähenbühl ''AS IS'' AND ANY
    EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
    WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL Philipp Krähenbühl BE LIABLE FOR ANY
    DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
    (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
    LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
    ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
    SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

    Modification:
    @tttor
    @rizkivmaster
*/

#pragma once
const int CACHE_SIZE = 100;// The number of loaded images to the memory when testing for process parallelization

// Dataset parameters
//#define USE_MSRC
static const char MSRC_DIRECTORY  [] = "/declared_but_not_used/path_to_non_existence_dir";
static const char MSRC_TRAIN_FILE [] = "/declared_but_not_used/path_to_non_existence_dir";
static const char MSRC_VALID_FILE [] = "/declared_but_not_used/path_to_non_existence_dir";
static const char MSRC_TEST_FILE  [] = "/declared_but_not_used/path_to_non_existence_dir";

// Dataset parameters
static const char VOC2010_DIRECTORY  [] = "/declared_but_not_used/path_to_non_existence_dir";
static const char VOC2010_TRAIN_FILE [] = "/declared_but_not_used/path_to_non_existence_dir";
static const char VOC2010_VALID_FILE [] = "/declared_but_not_used/path_to_non_existence_dir";
static const char VOC2010_TEST_FILE  [] = "/declared_but_not_used/path_to_non_existence_dir";

// VOC Cache
static const char VOC2010_BBOX_DIRECTORY [] = "/declared_but_not_used/path_to_non_existence_dir";

static const int N_TEXTONS = 400;//: 50, see Shotton, 2007: TextonBoost for Image Understanding: ...

static const float FILTER_BANK_SIZE = 1.0;

// Boosting parameters
static const int N_BOOSTING_ROUNDS  = 3000; // Number of boosting rounds: 10000, 3000
static const int N_CLASSIFIERS      = 200; // Number of random classifiers to test [per round]: 750, 200
static const int N_THRESHOLDS       = 100; // Number of thresholds to test [per round]: 100, 150

#ifdef USE_MSRC
static const int BOOSTING_SUBSAMPLE = 5  ; // Subsampling factor (tradeoff between memory/computation and accuracy)
#else
static const int BOOSTING_SUBSAMPLE = 7  ; // Subsampling factor (tradeoff between memory/computation and accuracy)
#endif

static const int MIN_RECT_SIZE      = BOOSTING_SUBSAMPLE; // Minimum size of texton rectangle
static const int MAX_RECT_SIZE      = 200; // Maximum size of texton rectangle

// Other parameters
// #define AREA_SAMPLING   // Sample the rect size proportional to the area of the rectangle (uniform in w*h instead of uniform in w and h)
#define GAUSSIAN_OFFSET // Use Sample the offset from a gaussian centered at 0

// Shall we return the raw boosting results H or P = 1/Z * exp(-H)
//#define RAW_BOOSTING_OUTPUT
