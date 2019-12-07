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

/** @file viennacv/core/image_enum.hpp
    @brief Offer constants' and enumeration definitions in ViennaCV library
*/






namespace viennacv
{

enum OptimizeLevel {
    First = 0,
    Second = -1,
    Third = -2,
    Fourth = -3,
};


enum Direction {X, Y, Upright};

enum image_format
{
    RGB,
    Gray
};

enum ConvolutionType
{
    INNER,
    OUTER,
    EQUIV
};


} //namespace viennacv


