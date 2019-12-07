#pragma once
/* =========================================================================
   Copyright (c) 2016-2019, Department of Engineering Physics,
                            Tsinghua University, Beijing, China.

   Portions of this software are copyright by UChicago Argonne, LLC and ViennaCL team.

                            -----------------
                  ViennaCV - The Vienna Computer Vision Library
                            -----------------

   Project Head:    Wenyin Wei                   weiwy16@mails.tsinghua.edu.cn

   (A list of authors and contributors can be found in the manual)

   License:         MIT (X11), see file LICENSE in the base directory
============================================================================= */

/** @file viennacv/core/image_format.hpp
    @brief Implementation of image format transformation for image class
*/

#include "./image.hpp"


// SECTION 01b Declare the image class
namespace viennacv
{

template <typename NumericT>
void format_transform(
    viennacv::image_colpre<NumericT> & i_image,
    viennacv::image_colpre<NumericT> & o_image,
    image_format o_image_format)
{
    // TODO Here make the image initialization RGB
    if ( (o_image_format == Gray))
    {
        // 0.299 * R + 0.587 * G + 0.114 * B
        // TODO: Make the variable private
        o_image.data_.resize(1);
        o_image.data_[0].resize(i_image.get_row_num(), i_image.get_column_num());
        o_image.data_[0].clear();
        o_image.data_[0] += 0.2989 * i_image.data_[0] + 
                            0.5870 * i_image.data_[1] + 
                            0.1140 * i_image.data_[2];
        o_image.image_format_ = viennacv::Gray;
    }
}


} //namespace viennacv


