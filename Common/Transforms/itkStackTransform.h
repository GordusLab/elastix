/*=========================================================================
 *
 *  Copyright UMC Utrecht and contributors
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *        http://www.apache.org/licenses/LICENSE-2.0.txt
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *=========================================================================*/
#ifndef itkStackTransform_h
#define itkStackTransform_h

#include "itkAdvancedTransform.h"
#include "itkIndex.h"

namespace itk
{

/** \class StackTransform
 * \brief Implements stack of transforms: one for every last dimension index.
 *
 * A list of transforms with dimension of Dimension - 1 is maintained:
 * one for every last dimension index. This transform selects the right
 * transform based on the last dimension index of the input point.
 *
 * \ingroup Transforms
 *
 */
template <class TScalarType, unsigned int NInputDimensions, unsigned int NOutputDimensions>
class ITK_TEMPLATE_EXPORT StackTransform : public AdvancedTransform<TScalarType, NInputDimensions, NOutputDimensions>
{
public:
  /** Standard class typedefs. */
  typedef StackTransform                                                      Self;
  typedef AdvancedTransform<TScalarType, NInputDimensions, NOutputDimensions> Superclass;
  typedef SmartPointer<Self>                                                  Pointer;
  typedef SmartPointer<const Self>                                            ConstPointer;

  /** New method for creating an object using a factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(StackTransform, AdvancedTransform);

  /** (Reduced) dimension of the domain space. */
  itkStaticConstMacro(InputSpaceDimension, unsigned int, NInputDimensions);
  itkStaticConstMacro(OutputSpaceDimension, unsigned int, NOutputDimensions);
  itkStaticConstMacro(ReducedInputSpaceDimension, unsigned int, NInputDimensions - 1);
  itkStaticConstMacro(ReducedOutputSpaceDimension, unsigned int, NOutputDimensions - 1);

  /** Typedefs from the Superclass. */
  using typename Superclass::ScalarType;
  using typename Superclass::ParametersType;
  using typename Superclass::NumberOfParametersType;
  using typename Superclass::ParametersValueType;
  using typename Superclass::JacobianType;
  using typename Superclass::SpatialJacobianType;
  using typename Superclass::JacobianOfSpatialJacobianType;
  using typename Superclass::SpatialHessianType;
  using typename Superclass::JacobianOfSpatialHessianType;
  using typename Superclass::NonZeroJacobianIndicesType;
  using typename Superclass::InputPointType;
  using typename Superclass::InputVectorType;
  using typename Superclass::OutputVectorType;
  using typename Superclass::InputVnlVectorType;
  using typename Superclass::OutputVnlVectorType;
  using typename Superclass::OutputCovariantVectorType;
  using typename Superclass::InputCovariantVectorType;
  using typename Superclass::OutputPointType;
  using typename Superclass::OutputVectorPixelType;
  using typename Superclass::InputVectorPixelType;

  /** Sub transform types, having a reduced dimension. */
  typedef AdvancedTransform<TScalarType, Self::ReducedInputSpaceDimension, Self::ReducedOutputSpaceDimension>
                                                  SubTransformType;
  typedef typename SubTransformType::Pointer      SubTransformPointer;
  typedef typename SubTransformType::JacobianType SubTransformJacobianType;

  /** Dimension - 1 point types. */
  typedef typename SubTransformType::InputPointType  SubTransformInputPointType;
  typedef typename SubTransformType::OutputPointType SubTransformOutputPointType;

  /** Array type for parameter vector instantiation. */
  typedef typename ParametersType::ArrayType ParametersArrayType;

  /**  Method to transform a point. */
  OutputPointType
  TransformPoint(const InputPointType & ipp) const override;

  /** This returns a sparse version of the Jacobian of the transformation.
   * In this class however, the Jacobian is not sparse.
   * However, it is a useful function, since the Jacobian is passed
   * by reference, which makes it threadsafe, unlike the normal
   * GetJacobian function. */
  void
  GetJacobian(const InputPointType & ipp, JacobianType & jac, NonZeroJacobianIndicesType & nzji) const override;

  /** Set the parameters. Checks if the number of parameters
   * is correct and sets parameters of sub transforms. */
  void
  SetParameters(const ParametersType & param) override;

  /** Get the parameters. Concatenates the parameters of the
   * sub transforms. */
  const ParametersType &
  GetParameters(void) const override;

  /** Set the fixed parameters. */
  void
  SetFixedParameters(const ParametersType &) override
  {
    // \todo: to be implemented by Coert
  }


  /** Get the Fixed Parameters. */
  const ParametersType &
  GetFixedParameters(void) const override
  {
    // \todo: to be implemented by Coert: check this:
    return this->m_FixedParameters;
  }


  /** Return the number of sub transforms that have been set. */
  NumberOfParametersType
  GetNumberOfParameters(void) const override
  {
    if (this->m_SubTransformContainer.empty())
    {
      return 0;
    }
    else
    {
      return this->m_SubTransformContainer.size() * m_SubTransformContainer[0]->GetNumberOfParameters();
    }
  }


  /** Set/get number of transforms needed. */
  void
  SetNumberOfSubTransforms(const unsigned int num)
  {
    if (this->m_SubTransformContainer.size() != num)
    {
      this->m_SubTransformContainer.clear();
      this->m_SubTransformContainer.resize(num);
      this->Modified();
    }
  }


  auto
  GetNumberOfSubTransforms() const
  {
    return static_cast<unsigned>(m_SubTransformContainer.size());
  }


  /** Set/get stack transform parameters. */
  itkSetMacro(StackSpacing, TScalarType);
  itkGetConstMacro(StackSpacing, TScalarType);
  itkSetMacro(StackOrigin, TScalarType);
  itkGetConstMacro(StackOrigin, TScalarType);

  /** Set the initial transform for sub transform i. */
  void
  SetSubTransform(unsigned int i, SubTransformType * transform)
  {
    this->m_SubTransformContainer[i] = transform;
    this->Modified();
  }


  /** Set all sub transforms to transform. */
  void
  SetAllSubTransforms(const SubTransformType & transform)
  {
    for (auto & subTransform : m_SubTransformContainer)
    {
      // Copy transform
      SubTransformPointer transformcopy = dynamic_cast<SubTransformType *>(transform.CreateAnother().GetPointer());
      transformcopy->SetFixedParameters(transform.GetFixedParameters());
      transformcopy->SetParameters(transform.GetParameters());
      // Set sub transform
      subTransform = transformcopy;
    }
  }


  /** Get a sub transform. */
  SubTransformPointer
  GetSubTransform(unsigned int i)
  {
    return this->m_SubTransformContainer[i];
  }


  /** Get number of nonzero Jacobian indices. */
  NumberOfParametersType
  GetNumberOfNonZeroJacobianIndices(void) const override;

protected:
  StackTransform();
  ~StackTransform() override = default;

private:
  StackTransform(const Self &) = delete;
  void
  operator=(const Self &) = delete;


  static constexpr const char * unimplementedOverrideMessage = "Not implemented for StackTransform";

  /** These vector transforms are not implemented for this transform. */
  OutputVectorType
  TransformVector(const InputVectorType &) const override
  {
    itkExceptionMacro(<< unimplementedOverrideMessage);
  }

  OutputVnlVectorType
  TransformVector(const InputVnlVectorType &) const override
  {
    itkExceptionMacro(<< unimplementedOverrideMessage);
  }

  OutputCovariantVectorType
  TransformCovariantVector(const InputCovariantVectorType &) const override
  {
    itkExceptionMacro(<< unimplementedOverrideMessage);
  }


  /** Must be provided. */
  void
  GetSpatialJacobian(const InputPointType &, SpatialJacobianType &) const override
  {
    itkExceptionMacro(<< unimplementedOverrideMessage);
  }

  void
  GetSpatialHessian(const InputPointType &, SpatialHessianType &) const override
  {
    itkExceptionMacro(<< unimplementedOverrideMessage);
  }

  void
  GetJacobianOfSpatialJacobian(const InputPointType &,
                               JacobianOfSpatialJacobianType &,
                               NonZeroJacobianIndicesType &) const override
  {
    itkExceptionMacro(<< unimplementedOverrideMessage);
  }

  void
  GetJacobianOfSpatialJacobian(const InputPointType &,
                               SpatialJacobianType &,
                               JacobianOfSpatialJacobianType &,
                               NonZeroJacobianIndicesType &) const override
  {
    itkExceptionMacro(<< unimplementedOverrideMessage);
  }

  void
  GetJacobianOfSpatialHessian(const InputPointType &,
                              JacobianOfSpatialHessianType &,
                              NonZeroJacobianIndicesType &) const override
  {
    itkExceptionMacro(<< unimplementedOverrideMessage);
  }


  void
  GetJacobianOfSpatialHessian(const InputPointType &,
                              SpatialHessianType &,
                              JacobianOfSpatialHessianType &,
                              NonZeroJacobianIndicesType &) const override
  {
    itkExceptionMacro(<< unimplementedOverrideMessage);
  }


  // Transform container
  std::vector<SubTransformPointer> m_SubTransformContainer;

  // Stack spacing and origin of last dimension
  TScalarType m_StackSpacing{ 1.0 };
  TScalarType m_StackOrigin{ 0.0 };
};

} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#  include "itkStackTransform.hxx"
#endif

#endif
