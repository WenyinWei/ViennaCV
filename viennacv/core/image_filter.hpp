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

/** @file viennacv/core/image_filter.hpp
    @brief Implementation of filter convolution for image class
*/
#include "viennacl/linalg/matrix_operations.hpp"
#include "./image.hpp"
#include "./image_enum.hpp"

// SECTION 01b Declare the image class
namespace viennacv
{
namespace filter
{

// SECTION 01 Sobel operator filter
/** @brief Sobel operator for viennacv::image_colpre type
 * @tparam {viennacv::Direction}                      : Direct
 * @tparam NumericT                                   : Numeric Type
 * @tparam {viennacv::OptimizeLevel} OptimizeL        : Optimization Level
 * @param  {viennacv::image_colpre<NumericT>} i_image : Input image
 * @param  {viennacv::image_colpre<NumericT>} o_image : Output image that has been operated by Sobel operator
 * @param  {viennacv::ConvolutionType} type           : Convolution range type, Expand, Equiv or Shrink.
 */
template <  Direction Direct, typename NumericT, 
            viennacv::OptimizeLevel OptimizeL = OptimizeLevel::First>
void sobel(
    const viennacv::image_colpre<NumericT> & i_image,
    viennacv::image_colpre<NumericT> & o_image,
    const viennacv::ConvolutionType & type = EQUIV)
{
    if constexpr (OptimizeL==OptimizeLevel::First)
    {
        viennacl::matrix<NumericT> sobel_kernel(3, 3);
        std::vector< std::vector<NumericT> > t_sobel_kernel;
        std::vector<std::pair<size_t, size_t>> t_ROIrc_vec;
        if constexpr (Direct==viennacv::filter::Direction::X)
        {
            t_sobel_kernel.assign({
                {-1.0, 0.0, 1.0}, 
                {-2.0, 0.0, 2.0}, 
                {-1.0, 0.0, 1.0}});
            t_ROIrc_vec.assign({
                std::make_pair(0, 0),   std::make_pair(0, 2),
                std::make_pair(1, 0),   std::make_pair(1, 2),
                std::make_pair(2, 0),   std::make_pair(2, 2)
            });
        }
        else if constexpr (Direct == viennacv::filter::Direction::Y)
        {
            t_sobel_kernel.assign({
                { 1.0, 2.0, 1.0}, 
                { 0.0, 0.0, 0.0}, 
                {-1.0,-2.0,-1.0}});
            t_ROIrc_vec.assign({
                std::make_pair(0, 0), std::make_pair(0, 1),   std::make_pair(0, 2),
                std::make_pair(2, 0), std::make_pair(2, 1),   std::make_pair(2, 2)
            });
        }
        viennacl::copy(t_sobel_kernel, sobel_kernel);
        
        viennacv::convolve<NumericT, viennacv::ConvolutionType::EQUIV, false>(i_image, sobel_kernel, o_image,  t_ROIrc_vec);
    }
    else
    {
        std::cerr << "No other optimization level code of sobel x operator is not yet finished.";
    }
}


// SECTION 02 Gaussian kernel convolution filter
// SECTION 02_001 Gaussian 2D kernel generator
/** @brief Gaussian kernel generator (more accurately, transformer). Input a viennacl::matrix and let it be distributed by gaussian 2D distribution coefficients. The origin is set to be the center of the matrix which requires this matrix to have odd row number and odd column number! 
 * 
 * @param  {viennacl::matrix<NumericT>} kernel : Kernel to be distributed by gaussian distribution coefficients
 * @param  {NumericT} sigma                    : Gaussian distribution sigma
 * @return {null}                              : This function works directly on the input matrix 
 */
template <typename NumericT>
inline void matrix_to_gaussian_kernel(viennacl::matrix<NumericT> & kernel, NumericT sigma)
{
    // REVIEW: This is a GPU version convolution kernel construtor, you may wanna know if CPU alone would be faster
    size_t  ker_size1 = kernel.size1(), //i_image.get_row_num() * 2 + 1
            ker_size1_half = ker_size1 / 2;
    size_t  ker_size2 = kernel.size2(), //i_image.get_column_num() * 2 + 1
            ker_size2_half = ker_size2 / 2;
    viennacl::matrix<NumericT>  t_vcl_x(ker_size1, ker_size2),
                                t_vcl_y(ker_size1, ker_size2);
    std::vector< std::vector<NumericT> >    t_std_x(ker_size1), 
                                            t_std_y(ker_size1);
    for (size_t i = 0; i < ker_size1; i++)
    {
        t_std_y[i].resize(ker_size2);
        t_std_x[i].resize(ker_size2);
        for (size_t j = 0; j < ker_size2; j++)
        {   // The coordinate is changed to another one with center of the kernel being the origin and the y axis direction is inversed.
            t_std_y[i][j] = (NumericT) ker_size1_half - (NumericT) i;
            t_std_x[i][j] = (NumericT) j - (NumericT) ker_size2_half;
        }
        
    }
    viennacl::copy(t_std_x, t_vcl_x); viennacl::copy(t_std_y, t_vcl_y);
    const NumericT pi = 3.1415926535897;
    using viennacl::linalg::element_prod, viennacl::linalg::element_div, viennacl::linalg::element_exp;
    // NOTE the gausiian kernel calculation formqula is e^(-(x^2+y^2)/(2 sigma^2))  / (2pi sigma^2)
    kernel = element_exp( (element_prod(t_vcl_x, t_vcl_x)+element_prod(t_vcl_y, t_vcl_y))  
                        /   (-2.0 * std::pow(sigma,2))) ;
    kernel /= 2.0 * pi * (NumericT)std::pow(sigma, 2);
}

// SECTION 02_002 Gaussian convolution for viennacl::matrix 
/** @brief Convolve input matrix with a gaussian 2D distribution kernel and then output to the o_matrix parameter.
 * 
 * @tparam {NumericT}                            : Numeric type for viennacl::matrix and so on.
 * @param  {viennacl::matrix<NumericT>} i_matrix : Input matrix to be convolve with gaussian kernel
 * @param  {NumericT} sigma                      : Gaussian kernel 2D distribution sigma
 * @param  {viennacl::matrix<NumericT>} o_matrix : Output matrix which has been convolve with gaussian kernel
 * @param  {viennacv::ConvolutionType} type      : Convolution type, Expand, Equiv or Shrink.
 * @return {null}                                : The function output to the function parameter o_matrix
 */
template <  typename NumericT, 
            viennacv::OptimizeLevel OptimizeL = OptimizeLevel::First>
void gaussian(
    const viennacl::matrix<NumericT> & i_matrix,
    const NumericT sigma,
    viennacl::matrix<NumericT> & o_matrix,
    const viennacv::ConvolutionType & type = EQUIV)
{
    if constexpr (OptimizeL==OptimizeLevel::First)
    {
        // REVIEW  It has been experimentally tested that for the gaussian kernel, there is no necessity to convolve the kernel all over the image.  
        size_t  ker_size1 = std::min(i_matrix.size1() * 2 + 1, (size_t)51), //i_image.get_row_num() * 2 + 1         
                ker_size2 = std::min(i_matrix.size2() * 2 + 1, (size_t)51); //i_image.get_column_num() * 2 + 1
        viennacl::matrix<NumericT>  gaussian_kernel(ker_size1, ker_size2);
        viennacv::filter::matrix_to_gaussian_kernel<NumericT> (gaussian_kernel, sigma);
        viennacv::convolve<NumericT, viennacv::ConvolutionType::EQUIV, false>
                            (i_matrix, gaussian_kernel, o_matrix);
    }
    else
    {
        std::cerr << "No other optimization level code of gaussian kernel is not yet finished.";
    }
    

}

// SECTION 02_003 Gaussian convolution for viennacv::image_colpre 
/** @brief Convolve the viennacv::image_colpre class objects with a gaussian 2D distribution kernel.
 * 
 * @param  {viennacv::image_colpre<NumericT>} i_image : Input image to be convolved with gaussian kernel
 * @param  {NumericT} sigma                           : Gaussian 2D distribution 
 * @param  {viennacv::image_colpre<NumericT>} o_image : Output image which has been convolved with gaussian kernel
 * @param  {viennacv::ConvolutionType} type           : Convolution type, Expand, Equiv or Shrink.
 * @return {null}                                     : The function directly outputs to the o_image parameter reference. 
 */
template <  typename NumericT, 
            viennacv::OptimizeLevel OptimizeL = OptimizeLevel::First>
void gaussian(
    const viennacv::image_colpre<NumericT> & i_image,
    const NumericT sigma,
    viennacv::image_colpre<NumericT> & o_image,
    const viennacv::ConvolutionType & type = EQUIV)
{
    for (size_t color = 0; color < i_image.get_color_num(); color++)
        gaussian<NumericT, OptimizeL>(i_image.data_[color], sigma, o_image.data_[color], type);
}

} //namespace viennacv::filter


} //namespace viennacv


