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
*/

#pragma once

#include "util/image.h"
#include <ANN/ANN.h>
#include <QVector>
#include <QMutex>

class KMeans
{
protected:
	QVector< float > center_;
	int feature_size_;
	int n_center_;
protected:
	mutable QMutex mutex_;
	mutable ANNkd_tree * kd_tree_;
	mutable ANNpointArray ann_points_;
	void initAnn() const;
	void evaluateMany( const float* features, int feature_size, int N, short int* res) const;
public:
	KMeans();
	KMeans(const KMeans& o);
	~KMeans();
	
	// Training and evaluation
	void train( const float * features, int feature_size, int n_features, int N=512 );
	Image<short> evaluate( const Image< float >& fim) const;
	short evaluate( const float * feature, int feature_size ) const;
	
	// Loading and saving
	void load(const QString& s);
	void save(const QString& s) const;
};
