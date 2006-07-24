#ifndef __itkAreaOpeningImageFilter_h
#define __itkAreaOpeningImageFilter_h

#include "itkAttributeMorphologyBaseImageFilter.h"

namespace itk
{
template <class TInputImage, class TOutputImage>
class ITK_EXPORT AreaOpeningImageFilter :
    public AttributeMorphologyBaseImageFilter<TInputImage, TOutputImage, unsigned long, std::greater<typename TInputImage::PixelType> >

{
public:
  typedef AreaOpeningImageFilter Self;
  typedef AttributeMorphologyBaseImageFilter<TInputImage, TOutputImage, unsigned long, std::greater<typename TInputImage::PixelType> > Superclass;

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

  virtual ~AreaOpeningImageFilter() {}
protected:
  AreaOpeningImageFilter(){}
  void PrintSelf(std::ostream& os, Indent indent) const
  {
    os << indent << "Area opening: " << std::endl;
  }

private:

  AreaOpeningImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented


};

} // namespace itk
#endif
