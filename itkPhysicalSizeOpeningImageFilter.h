#ifndef __itkPhysicalSizeOpeningImageFilter_h
#define __itkPhysicalSizeOpeningImageFilter_h

#include "itkAttributeMorphologyBaseImageFilter.h"

namespace itk
{
template <class TInputImage, class TOutputImage>
class ITK_EXPORT PhysicalSizeOpeningImageFilter :
    public AttributeMorphologyBaseImageFilter<TInputImage, TOutputImage, typename TInputImage::SpacingType::ValueType, std::greater<typename TInputImage::PixelType> >

{
public:
  typedef PhysicalSizeOpeningImageFilter Self;
  typedef AttributeMorphologyBaseImageFilter<TInputImage, TOutputImage, typename TInputImage::SpacingType::ValueType, std::greater<typename TInputImage::PixelType> > Superclass;

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

  /** Runtime information support. */
  itkTypeMacro(PhysicalSizeOpeningImageFilter, 
               AttributeMorphologyBaseImageFilter);

protected:
  PhysicalSizeOpeningImageFilter(){}
  virtual ~PhysicalSizeOpeningImageFilter() {}

  void GenerateData()
    {
    // compute pixel size
    this->m_AttributeValuePerPixel = 1;
    for( unsigned int i=0; i<ImageDimension; i++)
      {
      this->m_AttributeValuePerPixel *= this->GetInput()->GetSpacing()[i];
      }
    // std::cout << "m_AttributeValuePerPixel: " << this->m_AttributeValuePerPixel << std::endl;
    // and call superclass implementation of GenerateData()
    Superclass::GenerateData();
    }


private:

  PhysicalSizeOpeningImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented


};

} // namespace itk
#endif
