/*
 *  AstRawDark
 *
 *  Copyright (C) 2015 - Frédéric CORNU
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef SRC_DATA_DTO_IMAGESTACK_H_
#define SRC_DATA_DTO_IMAGESTACK_H_m

#include "data/dto/imageInfo.h"

#include <QList>

/**
 * @brief Stores a set of images for a given temperature
 */
class ImageStack {

public:

	/**
	 * Constructor
	 *
	 * @param temperature the temperature of all images in this stack
	 */
	ImageStack(int temperature);

	/**
	 * Desctructor
	 */
	virtual ~ImageStack();

	/**
	 * Get the temperature of all images in this stack
	 *
	 * @return the temperature
	 */
	int		getTemperature() const { return _temperature; }

	/**
	 * Get the number of images in this stack
	 *
	 * @return image count
	 */
	int		getSize() const { return _images.size(); }

	/**
	 * Add an image to this stack
	 *
	 * @param info The image to add
	 */
	void	add(ImageInfo info) { _images.append(info); }

	/**
	 * Get all images
	 *
	 * @return a list of ImageInfo
	 */
	QList<ImageInfo> getImages() const { return _images; }

private:
	int					_temperature;
	QList<ImageInfo>	_images;

};

#endif /* SRC_DATA_DTO_IMAGESTACK_H_ */
