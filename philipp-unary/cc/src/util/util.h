

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

class QString;
class LabelImage;
class ColorImage;
template<class T >
class QVector;

enum DataType{
	TRAIN=1,
	VALID=2,
	TRAINVALID=3,
	TEST=4,
	ALL=7
};

// void loadMSRC( QVector< ColorImage >& images, QVector< LabelImage >& annotations, QVector< QString > & names, int type );
// void loadVOC2010( QVector< ColorImage >& images, QVector< LabelImage >& annotations, QVector< QString > & names, int type );
void loadImages( QVector< ColorImage >& images, QVector< LabelImage >& annotations, QVector< QString > & names, int type );
void loadImages( QVector< ColorImage >& images, QVector< LabelImage >& annotations, QVector< QString > & names, int type );
QVector< QString > listVOC2010( int type );
void loadVOC2010byNames(QVector< ColorImage >& images, QVector< LabelImage >& annotations, QVector< QString > & names,QVector< QString >& filenames);

void loadImages(QVector< ColorImage >& images, QVector< LabelImage >& annotations, QVector< QString > & names,  QString list_path,QString img_folder,QString png_folder);
QVector< QString > listImagesPath( QString path , QString base_dir);
void loadImagesbyNames(QVector< ColorImage >& images, QVector< LabelImage >& annotations, QVector< QString > & names,QVector< QString >& filenames, QString img_folder);

