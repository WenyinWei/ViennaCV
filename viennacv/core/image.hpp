#pragma once
/* =========================================================================
   Copyright (c) 2010-2016, Department of Engineering Physics,
                            Tsinghua University, Beijing, China.

   Portions of this software are copyright by UChicago Argonne, LLC and ViennaCL team.

                            -----------------
                  ViennaCV - The Vienna Computer Vision Library
                            -----------------

   Project Head:    Wenyin Wei                   weiwy16@mails.tsinghua.edu.cn

   (A list of authors and contributors can be found in the manual)

   License:         MIT (X11), see file LICENSE in the base directory
============================================================================= */

/** @file viennacv/core/image.hpp
    @brief Implementation of the image matrix class in ViennaCV library
*/

#include "viennacl/forwards.h"
// #include "viennacl/detail/matrix_def.hpp"
#include "viennacl/scalar.hpp"
#include "viennacv/core/image_enum.hpp"
// #include "viennacl/linalg/matrix_operations.hpp"
// #include "viennacl/linalg/sparse_matrix_operations.hpp"
// #include "viennacl/tools/tools.hpp"
// #include "viennacl/tools/matrix_size_deducer.hpp"
// #include "viennacl/meta/result_of.hpp"
// #include "viennacl/meta/enable_if.hpp"
// #include "viennacl/traits/handle.hpp"
// #include "viennacl/traits/row_major.hpp"
// Conversion from stl multi-dimension vector class to viennacv standard image class.





// SECTION 01a Predeclare the image class
namespace viennacv
{
template <typename NumericT>
class image_colpre;

} //namespace viennacv








// SECTION 02 COPY interface with other classes
// It depends on the image_colpre, so we need a predefinition of image_colpre class.
// TODO: interface with opencv and magick++ are needed.
namespace viennacl
{
// SECTION 02_001 COPY interface from <- other classes
template <typename NumericT>
/** @brief Conversion: std::vector -> image_colpre
 * 
 * @param  {std::vector<std::vector<std::vector<NumericT>>>} i_std_image : 
 * @param  {viennacv::image_colpre<NumericT>} o_image_colpre             : 
 */
void copy(  const std::vector<std::vector<std::vector<NumericT>>> & i_std_image, 
            viennacv::image_colpre<NumericT> *o_image_colpre)
{
    size_t l_color_num = i_std_image.size();
    for (size_t color = 0; color < l_color_num; color++)
        viennacl::copy(i_std_image[color], o_image_colpre->data_[color]);
}

// SECTION 02_002 COPY interface to -> other classes
template <typename NumericT>
/** @brief Conversion: image_colpre -> std::vector
 * 
 * @param  {viennacv::image_colpre<NumericT>} i_image_colpre              : 
 * @param  {std::vector<std::vector<std::vector<NumericT>>>} i_std_image : 
 */
void copy(  const viennacv::image_colpre<NumericT> & i_image_colpre, 
            std::vector<std::vector<std::vector<NumericT>>> *o_std_image)
{
    for (size_t color = 0; color < i_image_colpre.get_color_num(); color++)
        viennacl::copy(i_image_colpre.data_[color], o_std_image->at(color));
}
} // namespace viennacl




// SECTION 01 Declare the image class
namespace viennacv
{

template <typename NumericT>
class image_colpre
{
public:
    std::vector<viennacl::matrix<NumericT>> data_;    /** @brief The image data_ organized by a STL vector */
    image_format image_format_;
public:
    inline size_t get_color_num()  const { return data_.size();};
    inline size_t get_row_num()    const { return data_[0].size1();};
    inline size_t get_column_num() const { return data_[0].size2();};

public:
    // SECTION 01_001 Constructor & Destructor
    /** @brief Constuctor for the image_colpre class by an existing 3D std::vector class
     * @param  {size_t} l_color_num   : 
     * @param  {ssize_t} l_row_num            : 
     * @param  {ssize_t} l_column_num : 
     */
    explicit image_colpre(size_t l_color_num, ssize_t l_row_num, ssize_t l_column_num);
    /** @brief image_colpre constructor 
     * @param  {std::vector<std::vector<std::vector<NumericT>>>} i_std_image : 
     */
    explicit image_colpre(const std::vector<std::vector<std::vector<NumericT>>> & i_std_image);
    explicit image_colpre(const image_colpre<NumericT> & i_image);
    // TODO: Move assignment not yet implemented. 
    // image_colpre(image_colpre<NumericT> && i_image);
    /** @brief ~image Destructor*/
    // ~image_colpre();

    // SECTION 02_001 COPY interface from other classes
    friend void viennacl::copy<NumericT>(const std::vector<std::vector<std::vector<NumericT>>> & i_std_image, viennacv::image_colpre<NumericT> *o_image_colpre);
    friend void viennacl::copy<NumericT>(  const viennacv::image_colpre<NumericT> & i_image_colpre, 
            std::vector<std::vector<std::vector<NumericT>>> *o_std_image);
    
    
};

// SECTION 01_001 Constructor & Destructor

// SECTION 01_001a Null Constructor
template <typename NumericT>
image_colpre<NumericT>::image_colpre(size_t l_color_num, ssize_t l_row_num, ssize_t l_column_num)
{
    this->data_.resize(l_color_num);
    for (size_t color = 0; color < l_color_num; color++)
        this->data_[color].resize(l_row_num, l_column_num);
}

// SECTION 01_001b Constructor <- std::vector<std::vector<std::vector<NumericT>>>
template <typename NumericT>
image_colpre<NumericT>::image_colpre(const std::vector<std::vector<std::vector<NumericT>>> & i_std_image)
{
    this->data_.resize(i_std_image.size());
    for (size_t color = 0; color < i_std_image.size(); color++)
    {
        this->data_[color].resize(i_std_image[0].size(), i_std_image[0][0].size());
        viennacl::copy(i_std_image[color], this->data_[color]);
    }
}

// SECTION 01_001c Copy Constructor <- viennacv::image_colpre
template <typename NumericT>
image_colpre<NumericT>::image_colpre(const image_colpre<NumericT> & i_image)
{
    this->data_.resize(i_image.get_color_num());
    for (size_t color = 0; color < i_image.get_color_num(); color++)
    {
        this->data_[color].resize(i_image.get_row_num(), i_image.get_column_num());
        this->data_[color] = i_image.data_[color];
    }
}


// template <typename NumericT>
// image_colpre<NumericT>::~image_colpre()
// {   
//     // delete this->data_;
// }


// SECTION 03_002a Image Convolution, utilizing 01_002a
/** @brief Convolve the image data by the 2D matrix kernel, which would be the base of image filter
 * 
 * @param  {viennacl::matrix<NumericT>} i_kernel    : 
 * @param  {std::vector<std::pair<size_t} undefined : 
 * @param  {size_t>>} ROIxy_vec                     : 
 * 
 * @example
 * vcl_MatrixT kernel(5, 5);
 * kernel.clear();
 * kernel(2, 1) = -1.0/2; kernel(2, 2) = 0; kernel(2, 3) = 1.0/2;
 * std::vector<std::pair<size_t, size_t>> vec;
 * vec.push_back(std::make_pair<int,int>(2, 3));
 * vcl_image_colpre.convolve(kernel, vec);
 */
template <  typename NumericT, 
            viennacv::ConvolutionType ConvolType = EQUIV,
            bool KerElementIdentity = false, 
            OptimizeLevel optimize_level = OptimizeLevel::First>
void convolve(
    const viennacl::matrix<NumericT> & i_matrix,
    const viennacl::matrix<NumericT> & i_kernel,
    viennacl::matrix<NumericT> & o_matrix,
    std::vector<std::pair<size_t, size_t>> ROIrc_vec = std::vector<std::pair<size_t, size_t>>() ) // REVIEW: ROIrc_vec may be combined with KerElementIdentity to make the same element compute together
{
    // STUB 01 
    size_t l_kernel_size1 = (i_kernel.size1()-1)/2;
    size_t l_kernel_size2 = (i_kernel.size2()-1)/2;

    // NOTE Argument ROIrc_vec default empty case, all entries are filled in it here.
    if (ROIrc_vec.empty())
    {
        for (size_t i = 0; i < i_kernel.size1(); i++)
        for (size_t j = 0; j < i_kernel.size2(); j++)
            ROIrc_vec.push_back(std::make_pair<int, int>(i, j));
    }
    
    // STUB 02 Multiply the scalar and contribute to the final image.

    o_matrix.clear(); // REVIEW This may be the efficiency bottleneck which is safe but slow 
    for(auto & iter: ROIrc_vec)
    {
        size_t  l_row = iter.first, 
                l_column = iter.second;
        int bias1 = l_row-l_kernel_size1; //TODO, here is int, make sure it will not leak
        int bias2 = l_column-l_kernel_size2;
        // REVIEW The following command is helpful to avoid unexpected too big kernel, however, I am not satisfactory with the if statement which may be a bottleneck in this loop.
        if ((std::abs(bias1)>=l_kernel_size1) || (std::abs(bias2)>=l_kernel_size2)) continue;
        if constexpr (ConvolType == ConvolutionType::EQUIV)
        {
            viennacl::range submat_row_range_from(std::max(bias1, 0), 
                                                    i_matrix.size1() + std::min(bias1, 0)),
                            submat_column_range_from(std::max(bias2, 0), 
                                                    i_matrix.size2() + std::min(bias2, 0)),
                            submat_row_range_to(std::max(-bias1, 0), 
                                                    i_matrix.size1() + std::min(-bias1, 0)),
                            submat_column_range_to(std::max(-bias2, 0), 
                                                    i_matrix.size2() + std::min(-bias2, 0));
            viennacl::matrix_range<viennacl::matrix<NumericT>>  
                submatrix_to_add(i_matrix, submat_row_range_from, submat_column_range_from),
                submatrix_tobe_add(o_matrix, submat_row_range_to, submat_column_range_to); 

            submatrix_tobe_add += i_kernel(l_row, l_column) * submatrix_to_add;
        }
        else // TODO: Not yet implemented for other convolution type
        {
            std::cerr << "Not yet implemented for other convolution type than EQUIV." << std::endl;
        }
        
    }    
} //function void viennacv::convolve

// TODO totally change the comment & plut the output initialization & temp image removal
// SECTION 03_002a Image Convolution, utilizing 01_002a
/** @brief Convolve the image data by the 2D matrix kernel, which would be the base of image shift and filter
 * 
 * @param  {viennacl::matrix<NumericT>} i_kernel    : 
 * @param  {std::vector<std::pair<size_t} undefined : 
 * @param  {size_t>>} ROIxy_vec                     : 
 * 
 * @example
 * vcl_MatrixT kernel(5, 5);
 * kernel.clear();
 * kernel(2, 1) = -1.0/2; kernel(2, 2) = 0; kernel(2, 3) = 1.0/2;
 * std::vector<std::pair<size_t, size_t>> vec;
 * vec.push_back(std::make_pair<int,int>(2, 3));
 * vcl_image_colpre.convolve(kernel, vec);
 */
template <  typename NumericT, 
            viennacv::ConvolutionType ConvolType = EQUIV,
            bool KerElementIdentity = false, 
            OptimizeLevel optimize_level = OptimizeLevel::First>
void convolve(
    const viennacv::image_colpre<NumericT> & i_image,
    const viennacl::matrix<NumericT> & i_kernel,
    viennacv::image_colpre<NumericT> & o_image,
    std::vector<std::pair<size_t, size_t>> ROIrc_vec = std::vector<std::pair<size_t, size_t>>() )
{
    o_image.data_.resize(i_image.get_color_num()); // REVIEW This may be the efficiency bottleneck which is safe but slow 
    // STUB 02 Multiply the scalar and contribute to the final image.

    // NOTE Argument ROIrc_vec default empty case, all entries are filled in it here.
    if (ROIrc_vec.empty())
    {
        for (size_t i = 0; i < i_kernel.size1(); i++)
        for (size_t j = 0; j < i_kernel.size2(); j++)
            ROIrc_vec.push_back(std::make_pair<int, int>(i, j));
    }
    for (size_t color=0; color< i_image.get_color_num(); color++) 
        viennacv::convolve<NumericT, ConvolType, KerElementIdentity, optimize_level> 
            (i_image.data_[color], i_kernel, o_image.data_[color], ROIrc_vec);
} //function void viennacv::convolve



// SECTION 01_002b Image Convolution
/** @brief the same as the convolve funciton 01_002a, just the default ROI becomes the whole matrix.
 * @param  {viennacl::matrix<NumericT>} i_kernel : 
 * 
 * @example
 * vcl_MatrixT kernel(5, 5);
 * kernel.clear();
 * kernel(2, 1) = -1.0/2; kernel(2, 2) = 0; kernel(2, 3) = 1.0/2;
 * vcl_image_colpre.convolve(kernel);
 */
template <  typename NumericT, 
            viennacv::ConvolutionType ConvolType = EQUIV,
            bool KerElementIdentity = false, 
            OptimizeLevel optimize_level = OptimizeLevel::First>
void convolve(
    viennacv::image_colpre<NumericT> & i_image,
    const viennacl::matrix<NumericT> & i_kernel,
    std::vector<std::pair<size_t, size_t>> ROIrc_vec = std::vector<std::pair<size_t, size_t>>() )
{
    // FIXME A strange bug here that if you use make_pair<size_t, size_t>, the compiler fails.
    viennacv::image_colpre<NumericT> t_image(i_image);
    viennacv::convolve<NumericT, ConvolType, KerElementIdentity, optimize_level>(t_image, i_kernel, i_image, ROIrc_vec);
} //function image_colpre::convolve




} //namespace viennacv


