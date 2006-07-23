#ifndef __itkAreaOpeningImageFilter_h
#define __itkAreaOpeningImageFilter_h

#include "itkAttributeMorphologyBaseImageFilter.h"

namespace itk
{
template <class TInputImage, class TOutputImage>
class ITK_EXPORT AreaOpeningImageFilter :
    public AttributeMorphologyBaseImageFilter<TInputImage, TOutputImage, std::greater<typename TInputImage::PixelType> >

{
public:
  typedef AreaOpeningImageFilter Self;
  typedef AttributeMorphologyBaseImageFilter<TInputImage, TOutputImage, std::greater<typename TInputImage::PixelType> > Superclass;

  typedef SmartPointer<Self>   Pointer;
  typedef SmartPointer<const Self>  ConstPointer;

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
