#ifndef __itkPhysicalSizeClosingImageFilter_h
#define __itkPhysicalSizeClosingImageFilter_h

#include "itkAttributeMorphologyBaseImageFilter.h"

namespace itk
{
template <class TInputImage, class TOutputImage>
class ITK_EXPORT PhysicalSizeClosingImageFilter :
    public AttributeMorphologyBaseImageFilter<TInputImage, TOutputImage, typename TInputImage::SpacingType::ValueType, std::less<typename TInputImage::PixelType> >

{
public:
  typedef PhysicalSizeClosingImageFilter Self;
  typedef AttributeMorphologyBaseImageFilter<TInputImage, TOutputImage, typename TInputImage::SpacingType::ValueType, std::less<typename TInputImage::PixelType> > Superclass;

  typedef SmartPointer<Self>   Pointer;
  typedef SmartPointer<const Self>  ConstPointer;

  /**
   * Extract some information from the image types.  Dimensionality
   * of the two images is assumed to be the same.
   */
  typedef typename TOutputImage::PixelType OutputPixelType;
  typedef typename TOutputImage::InternalPixelType OutputInternalPixelType;
  typedef typename TInputImage::PixelType InputPixelType;
  typedef typename TInputImage::InternalPixelType InputInternalPixelType;
  typedef typename TInputImage::IndexType IndexType;
  typedef typename TInputImage::OffsetType OffsetType;
  typedef typename TInputImage::SizeType SizeType;

  itkStaticConstMacro(ImageDimension, unsigned int,
                      TOutputImage::ImageDimension);
  
  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  virtual ~PhysicalSizeClosingImageFilter() {}
protected:
  PhysicalSizeClosingImageFilter(){}
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
