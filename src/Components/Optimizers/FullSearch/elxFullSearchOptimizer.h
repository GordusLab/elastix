	
#ifndef __elxFullSearchOptimizer_h
#define __elxFullSearchOptimizer_h

#include "itkFullSearchOptimizer.h"
#include "elxIncludes.h"
#include <map>

#include "itkNDImageBase.h"


namespace elastix
{
using namespace itk;

	/**
	 * \class FullSearch
	 * \brief An optimizer based on full search.
	 *
	 * This optimizer ...
	 *
	 * \ingroup Optimizers
	 */

	template <class TElastix>
		class FullSearch :
		public
			FullSearchOptimizer,
		public
			OptimizerBase<TElastix>
	{
	public:

		/** Standard ITK.*/
		typedef FullSearch													Self;
		typedef FullSearchOptimizer									Superclass1;
		typedef OptimizerBase<TElastix>							Superclass2;
		typedef SmartPointer<Self>									Pointer;
		typedef SmartPointer<const Self>						ConstPointer;
		
		/** Method for creation through the object factory. */
		itkNewMacro( Self );
		
		/** Run-time type information (and related methods). */
		itkTypeMacro( FullSearch, FullSearchOptimizer );
		
		/** Name of this class.*/
		elxClassNameMacro( "FullSearch" );

		/** Typedef's inherited from Superclass1.*/
	  typedef Superclass1::CostFunctionType								CostFunctionType;
		typedef Superclass1::CostFunctionPointer						CostFunctionPointer;
		typedef Superclass1::ParametersType									ParametersType;
		typedef Superclass1::MeasureType										MeasureType;
		typedef Superclass1::ParameterValueType							ParameterValueType; 
		typedef Superclass1::RangeValueType									RangeValueType;
		typedef Superclass1::RangeType											RangeType;
		typedef Superclass1::SearchSpaceType								SearchSpaceType;
		typedef Superclass1::SearchSpacePointer							SearchSpacePointer;
		typedef Superclass1::SearchSpaceIteratorType				SearchSpaceIteratorType;
		typedef Superclass1::SearchSpacePointType						SearchSpacePointType;
		typedef Superclass1::SearchSpaceIndexType						SearchSpaceIndexType;
		typedef Superclass1::SearchSpaceSizeType						SearchSpaceSizeType;
		
		/** Typedef's inherited from Elastix.*/
		typedef typename Superclass2::ElastixType						ElastixType;
		typedef typename Superclass2::ElastixPointer				ElastixPointer;
		typedef typename Superclass2::ConfigurationType			ConfigurationType;
		typedef typename Superclass2::ConfigurationPointer	ConfigurationPointer;
		typedef typename Superclass2::RegistrationType			RegistrationType;
		typedef typename Superclass2::RegistrationPointer		RegistrationPointer;
		typedef typename Superclass2::ITKBaseType						ITKBaseType;
		
		/** To store the results of the full search */
		typedef NDImageBase<float>													NDImageType;
		typedef typename NDImageType::Pointer								NDImagePointer;

		/** To store the names of the search space dimensions */
		typedef std::map<unsigned int, std::string> DimensionNameMapType;
		typedef typename DimensionNameMapType::const_iterator NameIteratorType;
				

		/** Methods that have to be present everywhere.*/
		virtual void BeforeRegistration(void);
		virtual void BeforeEachResolution(void);
		virtual void AfterEachResolution(void);
		virtual void AfterEachIteration(void);
		virtual void AfterRegistration(void);		
		/** \todo BeforeAll, checking parameters. */

		/** Get a pointer to the image containing the optimization surface. */
		itkGetObjectMacro(OptimizationSurface, NDImageType);

	protected:

	  FullSearch();
		virtual ~FullSearch() {};

		NDImagePointer m_OptimizationSurface;

		DimensionNameMapType m_SearchSpaceDimensionNames;

		/** Checks if an error generated while reading the search space
		 * ranges from the parameter file is a real error. Prints some
		 * error message if so.
		 */
		virtual int CheckSearchSpaceRangeDefinition(const std::string & fullFieldName, int errorcode, unsigned int entry_nr);
			
	private:

	  FullSearch( const Self& );	// purposely not implemented
		void operator=( const Self& );							// purposely not implemented


		
	}; // end class FullSearch
	

} // end namespace elastix

#ifndef ITK_MANUAL_INSTANTIATION
#include "elxFullSearchOptimizer.hxx"
#endif

#endif // end #ifndef __elxFullSearchOptimizer_h
