/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkConvolutionImageFilter.h,v $
  Language:  C++
  Date:      $Date: 2009-02-17 20:58:48 $
  Version:   $Revision: 1.4 $

  Copyright ( c ) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __itkPhysicalSizeClosingImageFilter_h
#define __itkPhysicalSizeClosingImageFilter_h

#include "itkAttributeMorphologyBaseImageFilter.h"

namespace itk
{

/**
 * \class PhysicalSizeClosingImageFilter
 * \brief Morphological closing by attributes
 *
 * This is the base class for morphology attribute
 * operations. Attribute openings remove blobs according to criteria
 * such as area. When applied to grayscale images it has the effect of
 * trimming peaks based on area while leaving the rest of the image
 * unchanged. It is possible to use attributes besides area, but no
 * others are implemented yet. This filter uses some dodgy coding
 * practices - most notably copying the image data to a linear buffer
 * to allow direct implementation of the published algorithm. It
 * should therefore be quite a good candidate to carry out tests of
 * itk iterator performance with randomish access patterns. 
 *
 * This filter is implemented using the method of Wilkinson, "A
 * comparison of algorithms for Connected set openings and Closings",
 * A. Meijster and M. H. Wilkinson, PAMI, vol 24, no. 4, April 2002.
 * Attempts at implementing the method from ISMM 2000 are also
 * included, but operation appears incorrect. Check the ifdefs if you
 * are interested.
 */
template <class TInputImage, class TOutputImage>
class ITK_EXPORT PhysicalSizeClosingImageFilter :
    public AttributeMorphologyBaseImageFilter<TInputImage, TOutputImage, typename TInputImage::SpacingType::ValueType, std::less<typename TInputImage::PixelType> >

{
public:
  typedef PhysicalSizeClosingImageFilter Self;
  typedef AttributeMorphologyBaseImageFilter<TInputImage, TOutputImage, typename TInputImage::SpacingType::ValueType, std::less<typename TInputImage::PixelType> >
                                         Superclass;

  typedef SmartPointer<Self>        Pointer;
  typedef SmartPointer<const Self>  ConstPointer;

  /**
   * Extract some information from the image types.  Dimensionality
   * of the two images is assumed to be the same.
   */
  typedef typename TOutputImage::PixelType         OutputPixelType;
  typedef typename TOutputImage::InternalPixelType OutputInternalPixelType;
  typedef typename TInputImage::PixelType          InputPixelType;
  typedef typename TInputImage::InternalPixelType  InputInternalPixelType;
  typedef typename TInputImage::IndexType          IndexType;
  typedef typename TInputImage::OffsetType         OffsetType;
  typedef typename TInputImage::SizeType           SizeType;

  itkStaticConstMacro(ImageDimension, unsigned int,
                      TOutputImage::ImageDimension);
  
  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Runtime information support. */
  itkTypeMacro(PhysicalSizeClosingImageFilter, 
               AttributeMorphologyBaseImageFilter);

protected:
  PhysicalSizeClosingImageFilter(){}
  virtual ~PhysicalSizeClosingImageFilter() {}

  void GenerateData()
    {
    // compute pixel size
    this->m_AttributeValuePerPixel = 1;
    for( unsigned i=0; i<ImageDimension; i++)
      {
      this->m_AttributeValuePerPixel *= this->GetInput()->GetSpacing()[i];
      }
    // std::cout << "m_AttributeValuePerPixel: " << this->m_AttributeValuePerPixel << std::endl;
    // and call superclass implementation of GenerateData()
    Superclass::GenerateData();
    }


private:

  PhysicalSizeClosingImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented


};

} // namespace itk
#endif
