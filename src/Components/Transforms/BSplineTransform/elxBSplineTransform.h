#ifndef __elxBSplineTransform_h
#define __elxBSplineTransform_h

/* For easy changing the BSplineOrder: */
#define __VSplineOrder 3

#include "itkBSplineDeformableTransform.h"
#include "itkBSplineCombinationTransform.h"

#include "elxIncludes.h"


namespace elastix
{
using namespace itk;

	/**
	 * \class BSplineTransform
	 * \brief A transform based on the itkBSplineTransform.
	 *
	 * This transform is a B-spline transformation, commonly used for nonrigid registration.
	 *
	 * The parameters used in this class are:
	 * \parameter Transform: Select this transform as follows:\n
	 *		<tt>(%Transform "BSplineTransform")</tt>
	 * \parameter FinalGridSpacing: the grid spacing of the B-spline transform for each dimension. \n
	 *		example: <tt>(FinalGridSpacing 8.0 8.0 8.0)</tt> \n
	 *    If only one argument is given, that factor is used for each dimension. The spacing
	 *    is not in millimeters, but in "voxel size units".
	 *		The default is 8.0 in every dimension.
	 * \parameter UpsampleGridOption: a flag to determine if the B-spline grid should
	 *		be upsampled from one resolution level to another. Choose from {true, false}. \n
	 *		example: <tt>(UpsampleGridOption "true")</tt> \n
	 *		example: <tt>(UpsampleGridOption "true" "false" "true")</tt>
	 *		The default is "true" inbetween all resolutions.
	 * 
	 * The transform parameters necessary for transformix, additionally defined by this class, are:
	 * \transformparameter GridSize: stores the size of the B-spline grid. \n
	 *		example: <tt>(GridSize 16 16 16)</tt>
	 * \transformparameter GridIndex: stores the index of the B-spline grid. \n
	 *		example: <tt>(GridIndex 0 0 0)</tt>
	 * \transformparameter GridSpacing: stores the spacing of the B-spline grid. \n
	 *		example: <tt>(GridSpacing 16.0 16.0 16.0)</tt>
	 * \transformparameter GridOrigin: stores the origin of the B-spline grid. \n
	 *		example: <tt>(GridOrigin 0.0 0.0 0.0)</tt>
	 *
	 * \todo It is unsure what happens when one of the image dimensions has length 1. 
	 * 
	 * \ingroup Transforms
	 */

	template < class TElastix >
		class BSplineTransform:
	public
		BSplineCombinationTransform< 
			ITK_TYPENAME elx::TransformBase<TElastix>::CoordRepType,
			elx::TransformBase<TElastix>::FixedImageDimension,
			__VSplineOrder > ,
	public
		TransformBase<TElastix>
	{
	public:

		/** Standard ITK-stuff. */
		typedef BSplineTransform 										Self;
		
		typedef BSplineCombinationTransform<
			typename elx::TransformBase<TElastix>::CoordRepType,
			elx::TransformBase<TElastix>::FixedImageDimension,
			__VSplineOrder >													Superclass1;

		typedef elx::TransformBase<TElastix>				Superclass2;		
				
		/** The ITK-class that provides most of the functionality, and
		 * that is set as the "CurrentTransform" in the CombinationTransform */
		typedef typename 
			Superclass1::BSplineTransformType					BSplineTransformType;

		typedef SmartPointer<Self>									Pointer;
		typedef SmartPointer<const Self>						ConstPointer;
		
		/** Method for creation through the object factory. */
		itkNewMacro( Self );
		
		/** Run-time type information (and related methods). */
		itkTypeMacro( BSplineTransform, BSplineCombinationTransform );

		/** Name of this class.
		 * Use this name in the parameter file to select this specific transform. \n
		 * example: <tt>(Transform "BSplineTransform")</tt>\n
		 */
		elxClassNameMacro( "BSplineTransform" );
		
		/** Dimension of the fixed image. */
		itkStaticConstMacro( SpaceDimension, unsigned int, Superclass2::FixedImageDimension );
		
		/** The BSpline order. */
		itkStaticConstMacro( SplineOrder, unsigned int, __VSplineOrder );
		
		/** Typedefs inherited from the superclass. */
		typedef typename Superclass1::ScalarType 								ScalarType;
		typedef typename Superclass1::ParametersType 						ParametersType;
		typedef typename Superclass1::JacobianType 							JacobianType;
		typedef typename Superclass1::InputVectorType						InputVectorType;
		typedef typename Superclass1::OutputVectorType 					OutputVectorType;
		typedef typename Superclass1::InputCovariantVectorType 	InputCovariantVectorType;
		typedef typename Superclass1::OutputCovariantVectorType	OutputCovariantVectorType;
		typedef typename Superclass1::InputVnlVectorType 				InputVnlVectorType;
		typedef typename Superclass1::OutputVnlVectorType				OutputVnlVectorType;
		typedef typename Superclass1::InputPointType 						InputPointType;
		typedef typename Superclass1::OutputPointType						OutputPointType;
		
		/** Typedef's specific for the BSplineTransform. */
		typedef typename BSplineTransformType::PixelType				PixelType;
		typedef typename BSplineTransformType::ImageType				ImageType;
		typedef typename BSplineTransformType::ImagePointer			ImagePointer;
		typedef typename BSplineTransformType::RegionType				RegionType;
		typedef typename BSplineTransformType::IndexType				IndexType;
		typedef typename BSplineTransformType::SizeType					SizeType;
		typedef typename BSplineTransformType::SpacingType			SpacingType;
		typedef typename BSplineTransformType::OriginType				OriginType;
		typedef typename 
			BSplineTransformType::BulkTransformType								BulkTransformType;
		typedef typename 
			BSplineTransformType::BulkTransformPointer						BulkTransformPointer;
		typedef typename 
			BSplineTransformType::WeightsFunctionType							WeightsFunctionType;
		typedef typename 
			BSplineTransformType::WeightsType											WeightsType;
		typedef typename 
			BSplineTransformType::ContinuousIndexType							ContinuousIndexType;
		typedef typename 
			BSplineTransformType::ParameterIndexArrayType					ParameterIndexArrayType;

		/** Typedef's from TransformBase. */
		typedef typename Superclass2::ElastixType								ElastixType;
		typedef typename Superclass2::ElastixPointer						ElastixPointer;
		typedef typename Superclass2::ConfigurationType					ConfigurationType;
		typedef typename Superclass2::ConfigurationPointer			ConfigurationPointer;
		typedef typename Superclass2::RegistrationType					RegistrationType;
		typedef typename Superclass2::RegistrationPointer				RegistrationPointer;
		typedef typename Superclass2::CoordRepType							CoordRepType;
		typedef typename Superclass2::FixedImageType						FixedImageType;
		typedef typename Superclass2::MovingImageType						MovingImageType;
		typedef typename Superclass2::ITKBaseType								ITKBaseType;
		typedef typename Superclass2::CombinationTransformType	CombinationTransformType;

		/** Extra typedef */
		typedef typename BSplineTransformType::Pointer					BSplineTransformPointer;

		/** Execute stuff before the actual registration:
		 * \li Create an initial B-spline grid.
		 * \li Create initial registration parameters.
		 * Initially, the transform is set to use a 1x1x1 grid, with deformation (0,0,0).
		 * In the method BeforeEachResolution() this will be replaced by the right grid size.
		 * This seems not logical, but it is required, since the registration
		 * class checks if the number of parameters in the transform is equal to
		 * the number of parameters in the registration class. This check is done
		 * before calling the BeforeEachResolution() methods.
		 */
		virtual void BeforeRegistration(void);

		/** Execute stuff before each new pyramid resolution:
		 * \li In the first resolution call InitializeTransform().
		 * \li In next resolutions upsample the B-spline grid if necessary (so, call IncreaseScale())
		 */
		virtual void BeforeEachResolution(void);
		
		/** Method to set the initial BSpline grid and initialize the parameters (to 0).
		 * \li ComputeInitialGridSpacingFactor
		 * \li Define the initial grid region, origin and spacing, using the just computed grid spacing factor
		 * \li Set the initial parameters to zero and set then as InitialParametersOfNextLevel in the registration object.
		 * Called by BeforeEachResolution().
		 */
		virtual void InitializeTransform(void);

		/** Method to increase the density of the BSpline grid.
		 * \li Half the m_GridSpacingFactor
		 * \li Define the region, origin and spacing of the new grid
		 * \li Determine the new B-spline coefficients that describe the current deformation field.
		 * \li Set these coefficients as InitialParametersOfNextLevel in the registration object.
		 * Called by BeforeEachResolution().
		 */
		virtual void IncreaseScale(void);
		
		/** Method to compute m_GridSpacingFactor in the first resolution.
		 * The initial grid spacing factor depends on:
		 * \li The FinalGridSpacing parameter, read from the parameter file.
		 * \li The UpsampleGridOption parameter, read from the parameter file.
		 * \li The NumberOfResolutions, read from the parameter file
		 * Called by SetInitialGrid() 
		 */
		virtual void ComputeInitialGridSpacingFactor(void);

		/** Define a bspline grid, based on:
		 * \li The current m_GridSpacingFactor,
		 * \li FixedImage size/spacing/origin, and
		 * \li InitialTransform; only used in case UseComposition is true.
		 *     in this case the BSpline grid is located at the position of
		 *     the fixed image after undergoing the initial transform.
		 * Returns gridregion, gridorigin and gridspacing.
		 * Called by InitializeTransform() and IncreaseScale().
		 */
		virtual void DefineGrid(RegionType & gridregion,
			OriginType & gridorigin, SpacingType & gridspacing  ) const;

		/** Function to read transform-parameters from a file. */
		virtual void ReadFromFile(void);
		/** Function to write transform-parameters to a file. */
		virtual void WriteToFile( const ParametersType & param );
		
	protected:

		/** The constructor. */
		BSplineTransform();
		/** The destructor. */
		virtual ~BSplineTransform() {}
		
		/** Member variables. */
		SpacingType						m_GridSpacingFactor;
		std::vector< bool >		m_UpsampleBSplineGridOption;

	private:

		/** The private constructor. */
		BSplineTransform( const Self& );	// purposely not implemented
		/** The private copy constructor. */
		void operator=( const Self& );		// purposely not implemented

		BSplineTransformPointer	m_BSplineTransform;
		
	}; // end class BSplineTransform
	

} // end namespace elastix

#ifndef ITK_MANUAL_INSTANTIATION
#include "elxBSplineTransform.hxx"
#endif

#endif // end #ifndef __elxBSplineTransform_h

