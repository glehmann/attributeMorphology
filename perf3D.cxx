#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"

#include "itkAreaOpeningImageFilter.h"
#include "itkAreaClosingImageFilter.h"
#include "itkPhysicalSizeOpeningImageFilter.h"
#include "itkPhysicalSizeClosingImageFilter.h"

#include "itkTimeProbe.h"
#include <vector>
#include "itkMultiThreader.h"
#include <iomanip>

int main(int, char * argv[])
{
  itk::MultiThreader::SetGlobalMaximumNumberOfThreads(1);

  const int dim = 3;
  typedef unsigned char PType;
  typedef itk::Image< PType, dim >    IType;
  
  // read the input image
  typedef itk::ImageFileReader< IType > ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName( argv[1] );

  typedef itk::AreaOpeningImageFilter< IType, IType > AreaOpeningType;
  AreaOpeningType::Pointer opening = AreaOpeningType::New();
  opening->SetInput( reader->GetOutput() );
  
  typedef itk::AreaClosingImageFilter< IType, IType > AreaClosingType;
  AreaClosingType::Pointer closing = AreaClosingType::New();
  closing->SetInput( reader->GetOutput() );
  
  typedef itk::PhysicalSizeOpeningImageFilter< IType, IType > PhysicalSizeOpeningType;
  PhysicalSizeOpeningType::Pointer psopening = PhysicalSizeOpeningType::New();
  psopening->SetInput( reader->GetOutput() );
  
  typedef itk::PhysicalSizeClosingImageFilter< IType, IType > PhysicalSizeClosingType;
  PhysicalSizeClosingType::Pointer psclosing = PhysicalSizeClosingType::New();
  psclosing->SetInput( reader->GetOutput() );
  


  reader->Update();
  
  
  std::cout << "#F" << "\t" 
            << "lambda" << "\t" 
            << "opening" << "\t" 
            << "closing" << "\t" 
            << "psopen" << "\t" 
            << "psclose" << "\t" 
            << std::endl;

  for(int F=0; F<=1; F++ )
    {
    for(int M=1; M<=1000; M*=5 )
      {
      itk::TimeProbe otime;
      itk::TimeProbe ctime;
      itk::TimeProbe psotime;
      itk::TimeProbe psctime;
  
      opening->SetFullyConnected( F );
      closing->SetFullyConnected( F );
      psopening->SetFullyConnected( F );
      psclosing->SetFullyConnected( F );
  
      opening->SetLambda( M );
      closing->SetLambda( M );
      psopening->SetLambda( M );
      psclosing->SetLambda( M );
  
      for( int i=0; i<5; i++ )
        {
        otime.Start();
        opening->Update();
        otime.Stop();

        ctime.Start();
        closing->Update();
        ctime.Stop();

        psotime.Start();
        psopening->Update();
        psotime.Stop();

        psctime.Start();
        psclosing->Update();
        psctime.Stop();

        opening->Modified();
        closing->Modified();
        psopening->Modified();
        psclosing->Modified();
        }
        
      std::cout << std::setprecision(3)
                << F << "\t" 
                << M << "\t" 
                << otime.GetMeanTime() << "\t" 
                << ctime.GetMeanTime() << "\t" 
                << psotime.GetMeanTime() << "\t" 
                << psctime.GetMeanTime() << "\t" 
                << std::endl;
      }
    }
  
  return 0;
}

