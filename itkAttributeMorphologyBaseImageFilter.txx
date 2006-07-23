#ifndef _itkAttributeMorphologyBaseImageFilter_txx
#define _itkAttributeMorphologyBaseImageFilter_txx

#include "itkAttributeMorphologyBaseImageFilter.h"
#include "itkImageRegionIterator.h"
#include "itkImageRegionConstIteratorWithIndex.h"
#include "itkNumericTraits.h"
#include "itkProgressReporter.h"
#include "itkConnectedComponentAlgorithm.h"
#include "itkConstShapedNeighborhoodIterator.h"
#include "itkNeighborhoodAlgorithm.h"



namespace itk
{
template< class TInputImage, class TOutputImage, class TFunction>
void
AttributeMorphologyBaseImageFilter< TInputImage, TOutputImage, TFunction >
::GenerateInputRequestedRegion()
{
  // call the superclass' implementation of this method
  Superclass::GenerateInputRequestedRegion();
  
  // We need all the input.
  InputImagePointer input = const_cast<InputImageType *>(this->GetInput());
  
  input->SetRequestedRegion( input->GetLargestPossibleRegion() );
}



template <class TInputImage, class TOutputImage, class TFunction>
void 
AttributeMorphologyBaseImageFilter<TInputImage, TOutputImage, TFunction >
::EnlargeOutputRequestedRegion(DataObject *)
{
  this->GetOutput()
    ->SetRequestedRegion( this->GetOutput()->GetLargestPossibleRegion() );
}


template< class TInputImage, class TOutputImage, class TFunction >
void
AttributeMorphologyBaseImageFilter< TInputImage, TOutputImage, TFunction >
::GenerateData()
{
  typename TOutputImage::Pointer output = this->GetOutput();
  typename TInputImage::ConstPointer input = this->GetInput();
  // Allocate the output
  this->AllocateOutputs();

  unsigned long buffsize = output->GetRequestedRegion().GetNumberOfPixels();

  SizeType kernelRadius;
  kernelRadius.Fill(1);
  typedef itk::NeighborhoodAlgorithm::ImageBoundaryFacesCalculator<TInputImage>
FaceCalculatorType;
  FaceCalculatorType faceCalculator;
  typename FaceCalculatorType::FaceListType faceList;
  faceList = faceCalculator(input, output->GetRequestedRegion(), kernelRadius);
  typename FaceCalculatorType::FaceListType::iterator fit;

  fit = faceList.begin();

  m_SortPixels = new GreyAndPos[buffsize];
  m_Parent = new long[buffsize];
#ifndef PAMI
  m_Processed = new bool[buffsize];
#endif
  // This is a bit ugly, but I can't see an easy way around
  m_Raw = new InputPixelType[buffsize];
  m_AuxData = new long[buffsize];

  // copy the pixels to the sort buffer
  typedef ImageRegionConstIteratorWithIndex<TInputImage> CRegionIteratorType;
  CRegionIteratorType RegIt(input, output->GetRequestedRegion());
  RegIt.GoToBegin();
  //IndexType Origin = RegIt.GetIndex();
  long int pos = 0;

  for (;!RegIt.IsAtEnd();++RegIt, ++pos)
    {
    GreyAndPos P;
    P.Val = RegIt.Get();
    P.Where = RegIt.GetIndex();
    P.Pos = pos;
    m_SortPixels[pos] = P;
    m_Raw[pos] = P.Val;
#ifndef PAMI
    m_Processed[pos]=false;
#endif
    m_Parent[pos]=m_INACTIVE;
    m_AuxData[pos] = -1;  // invalid value;
    }

  std::sort(&(m_SortPixels[0]), &(m_SortPixels[buffsize - 1]), ComparePixStruct());

  // set up the offset vector
  OffsetVecType TheseOffsets;
  OffsetDirectVecType TheseDirectOffsets;
  SetupOffsetVec(TheseDirectOffsets, TheseOffsets);
  // the core algorithm
  // process first pixel
#ifdef PAMI
  MakeSet(m_SortPixels[0].Pos);
  //m_Processed[0] = true;
  for (long k = 1; k < buffsize;k++)
    {
    long ThisPos = m_SortPixels[k].Pos;
    IndexType ThisWhere = m_SortPixels[k].Where;
    InputPixelType ThisPix = m_SortPixels[k].Val;
    MakeSet(ThisPos);
    // Some optimization of bounds check
    if (fit->IsInside(ThisWhere))
      {
      // no need for bounds check on neighbours
      for (unsigned i = 0; i<TheseDirectOffsets.size();i++)
	{
	long NeighInd = ThisPos + TheseDirectOffsets[i];
	InputPixelType NeighPix = m_Raw[NeighInd];
	if ((ThisPix < NeighPix) || ((ThisPix == NeighPix) && (NeighInd < ThisPos)))
	  {
	  Union(NeighInd, ThisPos);
	  }
	}
      }
    else
      {
      // need a bounds check for each neighbour
      for (unsigned i = 0; i<TheseOffsets.size();i++)
	{
	if (fit->IsInside(ThisWhere + TheseOffsets[i]))
	  {
	  long NeighInd = ThisPos + TheseDirectOffsets[i];
	  InputPixelType NeighPix = m_Raw[NeighInd];
	  if ((ThisPix < NeighPix) || ((ThisPix == NeighPix) && (NeighInd < ThisPos)))
	    {
	    Union(NeighInd, ThisPos);
	    }
	  }
	}
      }
    }

#else
  MakeSet(m_SortPixels[0].Pos);
  m_Processed[0] = true;
  for (long k = 1; k < buffsize;k++)
    {
    long ThisPos, PrevPos;
    InputPixelType ThisPix, PrevPix;
    ThisPos = m_SortPixels[k].Pos;
    PrevPos = m_SortPixels[k-1].Pos;
    ThisPix = m_Raw[ThisPos];
    PrevPix = m_Raw[PrevPos];
    IndexType ThisWhere = m_SortPixels[k].Where;
    if (ThisPix != PrevPix)
      {
      for (long QPos = k-1; QPos >= 0; --QPos)
	{
	long QLoc = m_SortPixels[QPos].Pos;
	if (m_Raw[QLoc] != PrevPix)
	  {
	  break;
	  }
	if ((m_Parent[QLoc] == m_ACTIVE) && 
	    (m_AuxData[QLoc] >= m_Lambda))
	  {
	  m_Parent[QLoc] = m_INACTIVE;
	  m_AuxData[QLoc]=-1;
	  // dispose auxdata[QLoc]
	  }
	}
      }
    MakeSet(ThisPos);
    if (fit->IsInside(ThisWhere))
      {
      // no need for neighbor bounds check
      for (unsigned i = 0; i<TheseDirectOffsets.size();i++)
	{
	long NeighInd = ThisPos + TheseDirectOffsets[i];
	if (m_Processed[NeighInd])
	  {
	  Union(NeighInd, ThisPos);
	  }
	}
      }
    else
      {
      for (unsigned i = 0; i<TheseOffsets.size();i++)
	{
	if (fit->IsInside(ThisWhere + TheseOffsets[i]))
	  {
	  long NeighInd = ThisPos + TheseDirectOffsets[i];
	  if (m_Processed[NeighInd])
	    {
	    Union(NeighInd, ThisPos);
	    }
	  }
	}
      }
    m_Processed[ThisPos] = true;
    }
#endif


  // copy pixels back 
  typedef ImageRegionIterator<TOutputImage> RegionIteratorType;
  RegionIteratorType ORegIt(output, output->GetRequestedRegion());
  ORegIt.GoToBegin();
  
  // fill Raw - worry about iteration details later.
  // We aren't filling m_Parent, as suggested in the paper, because it
  // is an integer array. We want this to work with float types
#ifdef PAMI
  // write the new image to Raw - note that we aren't putting the
  // result in parent
  for (pos = buffsize - 1;pos >= 0; --pos)
    {
    long RPos = m_SortPixels[pos].Pos;
    if (m_Parent[RPos] >= 0)
      {
      m_Raw[RPos] = m_Raw[m_Parent[RPos]];
      }
#if 0
    else
      {
      // Original value already in raw
      }
#endif
    }
  for (pos = 0;pos < buffsize; ++pos, ++ORegIt)
    {
    ORegIt.Set(static_cast<OutputPixelType>(m_Raw[pos]));
    }

#else
  // the version from the paper
  for (pos = buffsize;pos >= 0; --pos)
    {
    long RPos = m_SortPixels[pos].Pos;
    if (m_Parent[RPos] < 0)
      {
      m_Parent[RPos] = (long)m_Raw[RPos];
      }
    else
      {
      m_Parent[RPos] = m_Parent[m_Parent[RPos]];
      }
    }
  for (pos = 0;pos < buffsize; ++pos, ++ORegIt)
    {
    ORegIt.Set(static_cast<OutputPixelType>(m_Parent[pos]));
    }
#endif
  delete [] m_Raw;
  delete [] m_SortPixels;
  delete [] m_Parent;
#ifndef PAMI
  delete [] m_Processed;
#endif
  delete [] m_AuxData;
}

template< class TInputImage, class TOutputImage, class TFunction >
void
AttributeMorphologyBaseImageFilter< TInputImage, TOutputImage, TFunction >
::SetupOffsetVec(OffsetDirectVecType &PosOffsets, OffsetVecType &Offsets)
{
  typedef ConstShapedNeighborhoodIterator<TOutputImage> NeighType;
  SizeType KernRad;
  KernRad.Fill(1);
  NeighType It(KernRad,
	       this->GetOutput(), this->GetOutput()->GetRequestedRegion());
  setConnectivity(&It, m_FullyConnected);
  typename NeighType::IndexListType OffsetList;
  typename NeighType::IndexListType::const_iterator LIt;

  OffsetList = It.GetActiveIndexList();
  SizeType Size = this->GetOutput()->GetRequestedRegion().GetSize();
  SizeType SizeBuf;
  SizeBuf[0] = 1;

  for (unsigned i = 0;i<TInputImage::ImageDimension - 1; i++)
    {
    SizeBuf[i+1] = Size[i];
    }
  for (LIt = OffsetList.begin(); LIt != OffsetList.end(); LIt++)
    {
    OffsetType O = It.GetOffset(*LIt);
    long oo = 0;
    for (unsigned p = 0; p<TInputImage::ImageDimension; p++)
      {
      oo += O[p] * SizeBuf[p];
      }
    PosOffsets.push_back(oo);
    Offsets.push_back(O);
    }
}

template< class TInputImage, class TOutputImage, class TFunction >
void
AttributeMorphologyBaseImageFilter< TInputImage, TOutputImage, TFunction >
::PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf(os,indent);

  os << indent << "FullyConnected: "  << m_FullyConnected << std::endl;
}

} // end namespace itk

#endif