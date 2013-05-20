//=================================================================================================
/*!
//  \file blaze/math/views/DenseRow.h
//  \brief Header file for the DenseRow class template
//
//  Copyright (C) 2011 Klaus Iglberger - All Rights Reserved
//
//  This file is part of the Blaze library. This library is free software; you can redistribute
//  it and/or modify it under the terms of the GNU General Public License as published by the
//  Free Software Foundation; either version 3, or (at your option) any later version.
//
//  This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
//  without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//  See the GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License along with a special
//  exception for linking and compiling against the Blaze library, the so-called "runtime
//  exception"; see the file COPYING. If not, see http://www.gnu.org/licenses/.
*/
//=================================================================================================

#ifndef _BLAZE_MATH_VIEWS_DENSEROW_H_
#define _BLAZE_MATH_VIEWS_DENSEROW_H_


//*************************************************************************************************
// Includes
//*************************************************************************************************

#include <iterator>
#include <stdexcept>
#include <blaze/math/constraints/DenseMatrix.h>
#include <blaze/math/constraints/Expression.h>
#include <blaze/math/constraints/RequiresEvaluation.h>
#include <blaze/math/constraints/StorageOrder.h>
#include <blaze/math/constraints/TransposeFlag.h>
#include <blaze/math/expressions/DenseVector.h>
#include <blaze/math/expressions/Expression.h>
#include <blaze/math/Forward.h>
#include <blaze/math/Intrinsics.h>
#include <blaze/math/shims/Reset.h>
#include <blaze/math/traits/AddTrait.h>
#include <blaze/math/traits/DivTrait.h>
#include <blaze/math/traits/MultTrait.h>
#include <blaze/math/traits/RowTrait.h>
#include <blaze/math/traits/SubTrait.h>
#include <blaze/math/typetraits/IsRowMajorMatrix.h>
#include <blaze/math/typetraits/IsSparseVector.h>
#include <blaze/util/Assert.h>
#include <blaze/util/constraints/Vectorizable.h>
#include <blaze/util/DisableIf.h>
#include <blaze/util/EnableIf.h>
#include <blaze/util/logging/FunctionTrace.h>
#include <blaze/util/Template.h>
#include <blaze/util/Types.h>
#include <blaze/util/typetraits/IsConst.h>
#include <blaze/util/typetraits/IsNumeric.h>
#include <blaze/util/typetraits/IsSame.h>
#include <blaze/util/typetraits/RemoveReference.h>


namespace blaze {

//=================================================================================================
//
//  CLASS DEFINITION
//
//=================================================================================================

//*************************************************************************************************
/*!\defgroup dense_row DenseRow
// \ingroup views
*/
/*!\brief Reference to a specific row of a dense matrix.
// \ingroup dense_row
//
// The DenseRow template represents a reference to a specific row of a dense matrix primitive.
// The type of the dense matrix is specified via the first template parameter:

   \code
   template< typename MT, bool SO >
   class DenseRow;
   \endcode

//  - MT: specifies the type of the dense matrix primitive. DenseRow can be used with any dense
//        matrix primitive, but does not work with any matrix expression type.
//  - SO: specifies the storage order (blaze::rowMajor, blaze::columnMajor) of the dense matrix.
//        This template parameter doesn't have to be explicitly defined, but is automatically
//        derived from the first template parameter.
//
// A reference to a dense row can conveniently be created via the row() function. The row can
// be either used as an alias to grant write access to a specific row of a matrix primitive on
// the left-hand side of an assignment or to grant read-access to a specific row of a matrix
// primitive or expression on the right-hand side of an assignment:

   \code
   blaze::DynamicVector<double,rowVector> x;
   blaze::DynamicMatrix<double,rowMajor> A, B;
   // ... Resizing and initialization

   // Setting the 2nd row of matrix A to x
   row( A, 2UL ) = x;

   // Setting x to the 3rd row of the result of the matrix multiplication
   x = row( A * B, 3UL );
   \endcode

// A dense row can be used like any other row vector. The elements of the dense row can be
// directly accessed with the subscript operator. The numbering of the row elements is

                             \f[\left(\begin{array}{*{5}{c}}
                             0 & 1 & 2 & \cdots & N-1 \\
                             \end{array}\right),\f]

// where N is the number of columns of the referenced matrix. The following example gives an
// impression of the use of DenseRow. All operations (addition, subtraction, multiplication,
// scaling, ...) can be performed on all possible combinations of dense and sparse vectors
// with fitting element types:

   \code
   using blaze::DynamicVector;
   using blaze::CompressedVector;
   using blaze::DynamicMatrix;
   using blaze::DenseRow;

   DynamicVector<double,rowVector> a( 2UL, 2.0 ), b;
   CompressedVector<double,rowVector> c( 2UL );
   c[1] = 3.0;

   typedef DynamicMatrix<double,rowMajor>  DenseMatrix;
   DenseMatrix A( 4UL, 2UL );  // Non-initialized 4x2 matrix

   typedef DenseRow<DenseMatrix>  RowType;
   RowType row0( row( A, 0UL ) );  // Reference to the 0th row of A

   row0[0] = 0UL;        // Manual initialization of the 0th row of A
   row0[1] = 0UL;
   row( A, 1UL ) = 1.0;  // Homogeneous initialization of the 1st row of A
   row( A, 2UL ) = a;    // Dense vector initialization of the 2nd row of A
   row( A, 3UL ) = c;    // Sparse vector initialization of the 3rd row of A

   b = row0 + a;              // Dense vector/dense vector addition
   b = c + row( A, 1UL );     // Sparse vector/dense vector addition
   b = row0 * row( A, 2UL );  // Component-wise vector multiplication

   row( A, 1UL ) *= 2.0;     // In-place scaling of the 1st row
   b = row( A, 1UL ) * 2.0;  // Scaling of the 1st row
   b = 2.0 * row( A, 1UL );  // Scaling of the 1st row

   row( A, 2UL ) += a;              // Addition assignment
   row( A, 2UL ) -= c;              // Subtraction assignment
   row( A, 2UL ) *= row( A, 0UL );  // Multiplication assignment

   double scalar = row( A, 1UL ) * trans( c );  // Scalar/dot/inner product between two vectors

   A = trans( c ) * row( A, 1UL );  // Outer product between two vectors
   \endcode

// It is possible to create a row view on both row-major and column-major matrices. However,
// please note that creating a row view on a matrix stored in column-major fashion can result
// in a considerable performance decrease in comparison to a row view on a row-major matrix
// due to the non-contiguous storage of the matrix elements. Therefore care has to be taken
// in the choice of the most suitable storage order:

   \code
   // Setup of two column-major matrices
   DynamicMatrix<double,columnMajor> A( 128UL, 128UL );
   DynamicMatrix<double,columnMajor> B( 128UL, 128UL );
   // ... Resizing and initialization

   // The computation of the 15th row of the multiplication between A and B ...
   DynamicVector<double,rowVector> x = row( A * B, 15UL );

   // ... is essentially the same as the following computation, which multiplies
   // the 15th row of the column-major matrix A with B.
   DynamicVector<double,rowVector> x = row( A, 15UL ) * B;
   \endcode

// Although Blaze performs the resulting vector/matrix multiplication as efficiently as possible
// using a row-major storage order for matrix A would result in a more efficient evaluation.
*/
template< typename MT                              // Type of the dense matrix
        , bool SO = IsRowMajorMatrix<MT>::value >  // Storage order
class DenseRow : public DenseVector< DenseRow<MT,SO>, true >
               , private Expression
{
 private:
   //**Type definitions****************************************************************************
   typedef IntrinsicTrait<typename MT::ElementType>  IT;  //!< Intrinsic trait for the row element type.
   //**********************************************************************************************

   //**********************************************************************************************
   //! Compilation switch for the non-const reference and iterator types.
   /*! The \a useConst compile time constant expression represents a compilation switch for
       the non-const reference and iterator types. In case the given dense matrix of type
       \a MT is const qualified, \a useConst will be set to 1 and the dense row will return
       references and iterators to const. Otherwise \a useConst will be set to 0 and the
       dense row will offer write access to the dense matrix elements both via the subscript
       operator and iterators. */
   enum { useConst = IsConst<MT>::value };
   //**********************************************************************************************

 public:
   //**Type definitions****************************************************************************
   typedef DenseRow<MT,SO>                     This;           //!< Type of this DenseRow instance.
   typedef typename RowTrait<MT>::Type         ResultType;     //!< Result type for expression template evaluations.
   typedef typename ResultType::TransposeType  TransposeType;  //!< Transpose type for expression template evaluations.
   typedef typename MT::ElementType            ElementType;    //!< Type of the row elements.
   typedef typename IT::Type                   IntrinsicType;  //!< Intrinsic type of the row elements.
   typedef typename MT::ReturnType             ReturnType;     //!< Return type for expression template evaluations
   typedef const DenseRow&                     CompositeType;  //!< Data type for composite expression templates.

   //! Reference to a constant row value.
   typedef typename MT::ConstReference  ConstReference;

   //! Reference to a non-constant row value.
   typedef typename SelectType< useConst, ConstReference, typename MT::Reference >::Type  Reference;

   //! Pointer to a constant row value.
   typedef const ElementType*  ConstPointer;

   //! Pointer to a constant row value.
   typedef typename SelectType< useConst, ConstPointer, ElementType* >::Type  Pointer;

   //! Iterator over constant elements.
   typedef typename MT::ConstIterator  ConstIterator;

   //! Iterator over non-constant elements.
   typedef typename SelectType< useConst, ConstIterator, typename MT::Iterator >::Type  Iterator;
   //**********************************************************************************************

   //**Compilation flags***************************************************************************
   //! Compilation switch for the expression template evaluation strategy.
   enum { vectorizable = MT::vectorizable };
   //**********************************************************************************************

   //**Constructors********************************************************************************
   /*!\name Constructors */
   //@{
   explicit inline DenseRow( MT& matrix, size_t index );
   // No explicitly declared copy constructor.
   //@}
   //**********************************************************************************************

   //**Destructor**********************************************************************************
   // No explicitly declared destructor.
   //**********************************************************************************************

   //**Data access functions***********************************************************************
   /*!\name Data access functions */
   //@{
   inline Reference      operator[]( size_t index );
   inline ConstReference operator[]( size_t index ) const;
   inline Pointer        data  ();
   inline ConstPointer   data  () const;
   inline Iterator       begin ();
   inline ConstIterator  begin () const;
   inline ConstIterator  cbegin() const;
   inline Iterator       end   ();
   inline ConstIterator  end   () const;
   inline ConstIterator  cend  () const;
   //@}
   //**********************************************************************************************

   //**Assignment operators************************************************************************
   /*!\name Assignment operators */
   //@{
                           inline DenseRow& operator= ( const ElementType& rhs );
                           inline DenseRow& operator= ( const DenseRow& rhs );
   template< typename VT > inline DenseRow& operator= ( const Vector<VT,true>& rhs );
   template< typename VT > inline DenseRow& operator+=( const Vector<VT,true>& rhs );
   template< typename VT > inline DenseRow& operator-=( const Vector<VT,true>& rhs );
   template< typename VT > inline DenseRow& operator*=( const Vector<VT,true>& rhs );

   template< typename Other >
   inline typename EnableIf< IsNumeric<Other>, DenseRow >::Type&
      operator*=( Other rhs );

   template< typename Other >
   inline typename EnableIf< IsNumeric<Other>, DenseRow >::Type&
      operator/=( Other rhs );
   //@}
   //**********************************************************************************************

   //**Utility functions***************************************************************************
   /*!\name Utility functions */
   //@{
                              inline size_t    size() const;
                              inline size_t    capacity() const;
                              inline size_t    nonZeros() const;
                              inline void      reset();
   template< typename Other > inline DenseRow& scale( const Other& scalar );
   //@}
   //**********************************************************************************************

 private:
   //**********************************************************************************************
   /*! \cond BLAZE_INTERNAL */
   //! Helper structure for the explicit application of the SFINAE principle.
   template< typename VT >
   struct VectorizedAssign {
      enum { value = vectorizable && VT::vectorizable &&
                     IsSame<ElementType,typename VT::ElementType>::value };
   };
   /*! \endcond */
   //**********************************************************************************************

   //**********************************************************************************************
   /*! \cond BLAZE_INTERNAL */
   //! Helper structure for the explicit application of the SFINAE principle.
   template< typename VT >
   struct VectorizedAddAssign {
      enum { value = vectorizable && VT::vectorizable &&
                     IsSame<ElementType,typename VT::ElementType>::value &&
                     IntrinsicTrait<ElementType>::addition };
   };
   /*! \endcond */
   //**********************************************************************************************

   //**********************************************************************************************
   /*! \cond BLAZE_INTERNAL */
   //! Helper structure for the explicit application of the SFINAE principle.
   template< typename VT >
   struct VectorizedSubAssign {
      enum { value = vectorizable && VT::vectorizable &&
                     IsSame<ElementType,typename VT::ElementType>::value &&
                     IntrinsicTrait<ElementType>::subtraction };
   };
   /*! \endcond */
   //**********************************************************************************************

   //**********************************************************************************************
   /*! \cond BLAZE_INTERNAL */
   //! Helper structure for the explicit application of the SFINAE principle.
   template< typename VT >
   struct VectorizedMultAssign {
      enum { value = vectorizable && VT::vectorizable &&
                     IsSame<ElementType,typename VT::ElementType>::value &&
                     IntrinsicTrait<ElementType>::multiplication };
   };
   /*! \endcond */
   //**********************************************************************************************

 public:
   //**Expression template evaluation functions****************************************************
   /*!\name Expression template evaluation functions */
   //@{
   template< typename Other > inline bool          canAlias ( const Other* alias ) const;
   template< typename Other > inline bool          isAliased( const Other* alias ) const;
                              inline IntrinsicType get      ( size_t index ) const;

   template< typename VT >
   inline typename DisableIf< VectorizedAssign<VT> >::Type
      assign( const DenseVector<VT,true>& rhs );

   template< typename VT >
   inline typename EnableIf< VectorizedAssign<VT> >::Type
      assign( const DenseVector<VT,true>& rhs );

   template< typename VT > inline void assign( const SparseVector<VT,true>& rhs );

   template< typename VT >
   inline typename DisableIf< VectorizedAddAssign<VT> >::Type
      addAssign( const DenseVector<VT,true>& rhs );

   template< typename VT >
   inline typename EnableIf< VectorizedAddAssign<VT> >::Type
      addAssign( const DenseVector<VT,true>& rhs );

   template< typename VT > inline void addAssign( const SparseVector<VT,true>& rhs );

   template< typename VT >
   inline typename DisableIf< VectorizedSubAssign<VT> >::Type
      subAssign( const DenseVector<VT,true>& rhs );

   template< typename VT >
   inline typename EnableIf< VectorizedSubAssign<VT> >::Type
      subAssign( const DenseVector<VT,true>& rhs );

   template< typename VT > inline void subAssign( const SparseVector<VT,true>& rhs );

   template< typename VT >
   inline typename DisableIf< VectorizedMultAssign<VT> >::Type
      multAssign( const DenseVector<VT,true>& rhs );

   template< typename VT >
   inline typename EnableIf< VectorizedMultAssign<VT> >::Type
      multAssign( const DenseVector<VT,true>& rhs );

   template< typename VT > inline void multAssign( const SparseVector<VT,true>& rhs );
   //@}
   //**********************************************************************************************

 private:
   //**Member variables****************************************************************************
   /*!\name Member variables */
   //@{
   MT&          matrix_;  //!< The dense matrix containing the row.
   const size_t row_;     //!< The index of the row in the matrix.
   //@}
   //**********************************************************************************************

   //**Compile time checks*************************************************************************
   /*! \cond BLAZE_INTERNAL */
   BLAZE_CONSTRAINT_MUST_BE_DENSE_MATRIX_TYPE    ( MT );
   BLAZE_CONSTRAINT_MUST_BE_ROW_MAJOR_MATRIX_TYPE( MT );
   BLAZE_CONSTRAINT_MUST_NOT_BE_EXPRESSION_TYPE  ( MT );
   /*! \endcond */
   //**********************************************************************************************
};
//*************************************************************************************************




//=================================================================================================
//
//  CONSTRUCTOR
//
//=================================================================================================

//*************************************************************************************************
/*!\brief The constructor for DenseRow.
//
// \param matrix The matrix containing the row.
// \param index The index of the row.
// \exception std::invalid_argument Invalid row access index.
*/
template< typename MT  // Type of the dense matrix
        , bool SO >    // Storage order
inline DenseRow<MT,SO>::DenseRow( MT& matrix, size_t index )
   : matrix_( matrix )  // The dense matrix containing the row
   , row_   ( index  )  // The index of the row in the matrix
{
   if( matrix_.rows() <= index )
      throw std::invalid_argument( "Invalid row access index" );
}
//*************************************************************************************************




//=================================================================================================
//
//  DATA ACCESS FUNCTIONS
//
//=================================================================================================

//*************************************************************************************************
/*!\brief Subscript operator for the direct access to the row elements.
//
// \param index Access index. The index must be smaller than the number of matrix columns.
// \return Reference to the accessed value.
*/
template< typename MT  // Type of the dense matrix
        , bool SO >    // Storage order
inline typename DenseRow<MT,SO>::Reference DenseRow<MT,SO>::operator[]( size_t index )
{
   BLAZE_USER_ASSERT( index < size(), "Invalid row access index" );
   return matrix_(row_,index);
}
//*************************************************************************************************


//*************************************************************************************************
/*!\brief Subscript operator for the direct access to the row elements.
//
// \param index Access index. The index must be smaller than the number of matrix columns.
// \return Reference to the accessed value.
*/
template< typename MT  // Type of the dense matrix
        , bool SO >    // Storage order
inline typename DenseRow<MT,SO>::ConstReference DenseRow<MT,SO>::operator[]( size_t index ) const
{
   BLAZE_USER_ASSERT( index < size(), "Invalid row access index" );
   return matrix_(row_,index);
}
//*************************************************************************************************


//*************************************************************************************************
/*!\brief Low-level data access to the row elements.
//
// \return Pointer to the internal element storage.
*/
template< typename MT  // Type of the dense matrix
        , bool SO >    // Storage order
inline typename DenseRow<MT,SO>::Pointer DenseRow<MT,SO>::data()
{
   return matrix_.data( row_ );
}
//*************************************************************************************************


//*************************************************************************************************
/*!\brief Low-level data access to the row elements.
//
// \return Pointer to the internal element storage.
*/
template< typename MT  // Type of the dense matrix
        , bool SO >    // Storage order
inline typename DenseRow<MT,SO>::ConstPointer DenseRow<MT,SO>::data() const
{
   return matrix_.data( row_ );
}
//*************************************************************************************************


//*************************************************************************************************
/*!\brief Returns an iterator to the first element of the row.
//
// \return Iterator to the first element of the row.
//
// This function returns an iterator to the first element of the row.
*/
template< typename MT  // Type of the dense matrix
        , bool SO >    // Storage order
inline typename DenseRow<MT,SO>::Iterator DenseRow<MT,SO>::begin()
{
   return matrix_.begin( row_ );
}
//*************************************************************************************************


//*************************************************************************************************
/*!\brief Returns an iterator to the first element of the row.
//
// \return Iterator to the first element of the row.
//
// This function returns an iterator to the first element of the row.
*/
template< typename MT  // Type of the dense matrix
        , bool SO >    // Storage order
inline typename DenseRow<MT,SO>::ConstIterator DenseRow<MT,SO>::begin() const
{
   return matrix_.begin( row_ );
}
//*************************************************************************************************


//*************************************************************************************************
/*!\brief Returns an iterator to the first element of the row.
//
// \return Iterator to the first element of the row.
//
// This function returns an iterator to the first element of the row.
*/
template< typename MT  // Type of the dense matrix
        , bool SO >    // Storage order
inline typename DenseRow<MT,SO>::ConstIterator DenseRow<MT,SO>::cbegin() const
{
   return matrix_.begin( row_ );
}
//*************************************************************************************************


//*************************************************************************************************
/*!\brief Returns an iterator just past the last element of the row.
//
// \return Iterator just past the last element of the row.
//
// This function returns an iterator just past the last element of the row.
*/
template< typename MT  // Type of the dense matrix
        , bool SO >    // Storage order
inline typename DenseRow<MT,SO>::Iterator DenseRow<MT,SO>::end()
{
   return matrix_.end( row_ );
}
//*************************************************************************************************


//*************************************************************************************************
/*!\brief Returns an iterator just past the last element of the row.
//
// \return Iterator just past the last element of the row.
//
// This function returns an iterator just past the last element of the row.
*/
template< typename MT  // Type of the dense matrix
        , bool SO >    // Storage order
inline typename DenseRow<MT,SO>::ConstIterator DenseRow<MT,SO>::end() const
{
   return matrix_.end( row_ );
}
//*************************************************************************************************


//*************************************************************************************************
/*!\brief Returns an iterator just past the last element of the row.
//
// \return Iterator just past the last element of the row.
//
// This function returns an iterator just past the last element of the row.
*/
template< typename MT  // Type of the dense matrix
        , bool SO >    // Storage order
inline typename DenseRow<MT,SO>::ConstIterator DenseRow<MT,SO>::cend() const
{
   return matrix_.end( row_ );
}
//*************************************************************************************************




//=================================================================================================
//
//  ASSIGNMENT OPERATORS
//
//=================================================================================================

//*************************************************************************************************
/*!\brief Homogenous assignment to all row elements.
//
// \param rhs Scalar value to be assigned to all row elements.
// \return Reference to the assigned row.
*/
template< typename MT  // Type of the dense matrix
        , bool SO >    // Storage order
inline DenseRow<MT,SO>& DenseRow<MT,SO>::operator=( const ElementType& rhs )
{
   const size_t columns( size() );

   for( size_t j=0UL; j<columns; ++j )
      matrix_(row_,j) = rhs;

   return *this;
}
//*************************************************************************************************


//*************************************************************************************************
/*!\brief Copy assignment operator for DenseRow.
//
// \param rhs Dense row to be copied.
// \return Reference to the assigned row.
// \exception std::invalid_argument Row sizes do not match.
//
// In case the current sizes of the two rows don't match, a \a std::invalid_argument
// exception is thrown.
*/
template< typename MT  // Type of the dense matrix
        , bool SO >    // Storage order
inline DenseRow<MT,SO>& DenseRow<MT,SO>::operator=( const DenseRow& rhs )
{
   if( &rhs == this ) return *this;

   if( size() != rhs.size() )
      throw std::invalid_argument( "Row sizes do not match" );

   const size_t columns( size() );

   for( size_t j=0UL; j<columns; ++j )
      matrix_(row_,j) = rhs[j];

   return *this;
}
//*************************************************************************************************


//*************************************************************************************************
/*!\brief Assignment operator for different vectors.
//
// \param rhs Vector to be assigned.
// \return Reference to the assigned row.
// \exception std::invalid_argument Vector sizes do not match.
//
// In case the current sizes of the two vectors don't match, a \a std::invalid_argument
// exception is thrown.
*/
template< typename MT    // Type of the dense matrix
        , bool SO >      // Storage order
template< typename VT >  // Type of the right-hand side vector
inline DenseRow<MT,SO>& DenseRow<MT,SO>::operator=( const Vector<VT,true>& rhs )
{
   using blaze::assign;

   BLAZE_CONSTRAINT_MUST_BE_TRANSPOSE_VECTOR_TYPE( typename VT::ResultType );
   BLAZE_CONSTRAINT_MUST_NOT_REQUIRE_EVALUATION  ( typename VT::ResultType );

   if( size() != (~rhs).size() )
      throw std::invalid_argument( "Vector sizes do not match" );

   if( (~rhs).canAlias( &matrix_ ) ) {
      const typename VT::ResultType tmp( ~rhs );
      assign( *this, tmp );
   }
   else {
      if( IsSparseVector<VT>::value )
         reset();
      assign( *this, ~rhs );
   }

   return *this;
}
//*************************************************************************************************


//*************************************************************************************************
/*!\brief Addition assignment operator for the addition of a vector (\f$ \vec{a}+=\vec{b} \f$).
//
// \param rhs The right-hand side vector to be added to the dense row.
// \return Reference to the assigned row.
// \exception std::invalid_argument Vector sizes do not match.
//
// In case the current sizes of the two vectors don't match, a \a std::invalid_argument exception
// is thrown.
*/
template< typename MT    // Type of the dense matrix
        , bool SO >      // Storage order
template< typename VT >  // Type of the right-hand side vector
inline DenseRow<MT,SO>& DenseRow<MT,SO>::operator+=( const Vector<VT,true>& rhs )
{
   using blaze::addAssign;

   BLAZE_CONSTRAINT_MUST_BE_TRANSPOSE_VECTOR_TYPE( typename VT::ResultType );
   BLAZE_CONSTRAINT_MUST_NOT_REQUIRE_EVALUATION  ( typename VT::ResultType );

   if( size() != (~rhs).size() )
      throw std::invalid_argument( "Vector sizes do not match" );

   if( (~rhs).canAlias( &matrix_ ) ) {
      const typename VT::ResultType tmp( ~rhs );
      addAssign( *this, tmp );
   }
   else {
      addAssign( *this, ~rhs );
   }

   return *this;
}
//*************************************************************************************************


//*************************************************************************************************
/*!\brief Subtraction assignment operator for the subtraction of a vector (\f$ \vec{a}-=\vec{b} \f$).
//
// \param rhs The right-hand side vector to be subtracted from the dense row.
// \return Reference to the assigned row.
// \exception std::invalid_argument Vector sizes do not match.
//
// In case the current sizes of the two vectors don't match, a \a std::invalid_argument exception
// is thrown.
*/
template< typename MT    // Type of the dense matrix
        , bool SO >      // Storage order
template< typename VT >  // Type of the right-hand side vector
inline DenseRow<MT,SO>& DenseRow<MT,SO>::operator-=( const Vector<VT,true>& rhs )
{
   using blaze::subAssign;

   BLAZE_CONSTRAINT_MUST_BE_TRANSPOSE_VECTOR_TYPE( typename VT::ResultType );
   BLAZE_CONSTRAINT_MUST_NOT_REQUIRE_EVALUATION  ( typename VT::ResultType );

   if( size() != (~rhs).size() )
      throw std::invalid_argument( "Vector sizes do not match" );

   if( (~rhs).canAlias( &matrix_ ) ) {
      const typename VT::ResultType tmp( ~rhs );
      subAssign( *this, tmp );
   }
   else {
      subAssign( *this, ~rhs );
   }

   return *this;
}
//*************************************************************************************************


//*************************************************************************************************
/*!\brief Multiplication assignment operator for the multiplication of a vector
//        (\f$ \vec{a}*=\vec{b} \f$).
//
// \param rhs The right-hand side vector to be multiplied with the dense row.
// \return Reference to the assigned row.
// \exception std::invalid_argument Vector sizes do not match.
//
// In case the current sizes of the two vectors don't match, a \a std::invalid_argument exception
// is thrown.
*/
template< typename MT    // Type of the dense matrix
        , bool SO >      // Storage order
template< typename VT >  // Type of the right-hand side vector
inline DenseRow<MT,SO>& DenseRow<MT,SO>::operator*=( const Vector<VT,true>& rhs )
{
   using blaze::multAssign;

   BLAZE_CONSTRAINT_MUST_BE_TRANSPOSE_VECTOR_TYPE( typename VT::ResultType );
   BLAZE_CONSTRAINT_MUST_NOT_REQUIRE_EVALUATION  ( typename VT::ResultType );

   if( size() != (~rhs).size() )
      throw std::invalid_argument( "Vector sizes do not match" );

   if( (~rhs).canAlias( &matrix_ ) || IsSparseVector<VT>::value ) {
      const typename VT::ResultType tmp( ~rhs );
      multAssign( *this, tmp );
   }
   else {
      multAssign( *this, ~rhs );
   }

   return *this;
}
//*************************************************************************************************


//*************************************************************************************************
/*!\brief Multiplication assignment operator for the multiplication between a vector and
//        a scalar value (\f$ \vec{a}*=s \f$).
//
// \param rhs The right-hand side scalar value for the multiplication.
// \return Reference to the vector.
*/
template< typename MT       // Type of the dense matrix
        , bool SO >         // Storage order
template< typename Other >  // Data type of the right-hand side scalar
inline typename EnableIf< IsNumeric<Other>, DenseRow<MT,SO> >::Type&
   DenseRow<MT,SO>::operator*=( Other rhs )
{
   return operator=( (*this) * rhs );
}
//*************************************************************************************************


//*************************************************************************************************
/*!\brief Division assignment operator for the division of a vector by a scalar value
//        (\f$ \vec{a}/=s \f$).
//
// \param rhs The right-hand side scalar value for the division.
// \return Reference to the vector.
//
// \b Note: A division by zero is only checked by an user assert.
*/
template< typename MT       // Type of the dense matrix
        , bool SO >         // Storage order
template< typename Other >  // Data type of the right-hand side scalar
inline typename EnableIf< IsNumeric<Other>, DenseRow<MT,SO> >::Type&
   DenseRow<MT,SO>::operator/=( Other rhs )
{
   BLAZE_USER_ASSERT( rhs != Other(0), "Division by zero detected" );

   return operator=( (*this) / rhs );
}
//*************************************************************************************************




//=================================================================================================
//
//  UTILITY FUNCTIONS
//
//=================================================================================================

//*************************************************************************************************
/*!\brief Returns the current size/dimension of the row.
//
// \return The size of the row.
*/
template< typename MT  // Type of the dense matrix
        , bool SO >    // Storage order
inline size_t DenseRow<MT,SO>::size() const
{
   return matrix_.columns();
}
//*************************************************************************************************


//*************************************************************************************************
/*!\brief Returns the maximum capacity of the dense row.
//
// \return The capacity of the dense row.
*/
template< typename MT  // Type of the dense matrix
        , bool SO >    // Storage order
inline size_t DenseRow<MT,SO>::capacity() const
{
   return matrix_.capacity( row_ );
}
//*************************************************************************************************


//*************************************************************************************************
/*!\brief Returns the number of non-zero elements in the row.
//
// \return The number of non-zero elements in the row.
//
// Note that the number of non-zero elements is always less than or equal to the current number
// of columns of the matrix containing the row.
*/
template< typename MT  // Type of the dense matrix
        , bool SO >    // Storage order
inline size_t DenseRow<MT,SO>::nonZeros() const
{
   return matrix_.nonZeros( row_ );
}
//*************************************************************************************************


//*************************************************************************************************
/*!\brief Reset to the default initial values.
//
// \return void
*/
template< typename MT  // Type of the dense matrix
        , bool SO >    // Storage order
inline void DenseRow<MT,SO>::reset()
{
   matrix_.reset( row_ );
}
//*************************************************************************************************


//*************************************************************************************************
/*!\brief Scaling of the row by the scalar value \a scalar (\f$ \vec{a}=\vec{b}*s \f$).
//
// \param scalar The scalar value for the row scaling.
// \return Reference to the dense row.
*/
template< typename MT       // Type of the dense matrix
        , bool SO >         // Storage order
template< typename Other >  // Data type of the scalar value
inline DenseRow<MT,SO>& DenseRow<MT,SO>::scale( const Other& scalar )
{
   for( size_t j=0UL; j<size(); ++j ) {
      matrix_(row_,j) *= scalar;
   }
   return *this;
}
//*************************************************************************************************




//=================================================================================================
//
//  EXPRESSION TEMPLATE EVALUATION FUNCTIONS
//
//=================================================================================================

//*************************************************************************************************
/*!\brief Returns whether the dense row can alias with the given address \a alias.
//
// \param alias The alias to be checked.
// \return \a true in case the alias corresponds to this dense row, \a false if not.
//
// This function returns whether the given address can alias with the dense row. In contrast
// to the isAliased() function this function is allowed to use compile time expressions to
// optimize the evaluation.
*/
template< typename MT       // Type of the dense matrix
        , bool SO >         // Storage order
template< typename Other >  // Data type of the foreign expression
inline bool DenseRow<MT,SO>::canAlias( const Other* alias ) const
{
   return static_cast<const void*>( &matrix_ ) == static_cast<const void*>( alias );
}
//*************************************************************************************************


//*************************************************************************************************
/*!\brief Returns whether the dense row is aliased with the given address \a alias.
//
// \param alias The alias to be checked.
// \return \a true in case the alias corresponds to this dense row, \a false if not.
//
// This function returns whether the given address is aliased with the dense row. In contrast
// to the canAlias() function this function is not allowed to use compile time expressions to
// optimize the evaluation.
*/
template< typename MT       // Type of the dense matrix
        , bool SO >         // Storage order
template< typename Other >  // Data type of the foreign expression
inline bool DenseRow<MT,SO>::isAliased( const Other* alias ) const
{
   return static_cast<const void*>( &matrix_ ) == static_cast<const void*>( alias );
}
//*************************************************************************************************


//*************************************************************************************************
/*!\brief Access to the intrinsic elements of the dense row.
//
// \param index Access index. The index must be smaller than the number of matrix columns.
// \return Reference to the accessed values.
//
// This function offers a direct access to the intrinsic elements of the row. It must \b NOT
// be called explicitly! It is used internally for the performance optimized evaluation of
// expression templates. Calling this function explicitly might result in erroneous results
// and/or in compilation errors.
*/
template< typename MT  // Type of the dense matrix
        , bool SO >    // Storage order
inline typename DenseRow<MT,SO>::IntrinsicType DenseRow<MT,SO>::get( size_t index ) const
{
   return matrix_.get( row_, index );
}
//*************************************************************************************************


//*************************************************************************************************
/*!\brief Default implementation of the assignment of a dense vector.
//
// \param rhs The right-hand side dense vector to be assigned.
// \return void
//
// This function must \b NOT be called explicitly! It is used internally for the performance
// optimized evaluation of expression templates. Calling this function explicitly might result
// in erroneous results and/or in compilation errors. Instead of using this function use the
// assignment operator.
*/
template< typename MT    // Type of the dense matrix
        , bool SO >      // Storage order
template< typename VT >  // Type of the right-hand side dense vector
inline typename DisableIf< typename DenseRow<MT,SO>::BLAZE_TEMPLATE VectorizedAssign<VT> >::Type
   DenseRow<MT,SO>::assign( const DenseVector<VT,true>& rhs )
{
   BLAZE_INTERNAL_ASSERT( size() == (~rhs).size(), "Invalid vector sizes" );

   const size_t jend( (~rhs).size() & size_t(-2) );
   for( size_t j=0UL; j<jend; j+=2UL ) {
      matrix_(row_,j    ) = (~rhs)[j    ];
      matrix_(row_,j+1UL) = (~rhs)[j+1UL];
   }
   if( jend < (~rhs).size() )
      matrix_(row_,jend) = (~rhs)[jend];
}
//*************************************************************************************************


//*************************************************************************************************
/*!\brief Intrinsic optimized implementation of the assignment of a dense vector.
//
// \param rhs The right-hand side dense vector to be assigned.
// \return void
//
// This function must \b NOT be called explicitly! It is used internally for the performance
// optimized evaluation of expression templates. Calling this function explicitly might result
// in erroneous results and/or in compilation errors. Instead of using this function use the
// assignment operator.
*/
template< typename MT    // Type of the dense matrix
        , bool SO >      // Storage order
template< typename VT >  // Type of the right-hand side dense vector
inline typename EnableIf< typename DenseRow<MT,SO>::BLAZE_TEMPLATE VectorizedAssign<VT> >::Type
   DenseRow<MT,SO>::assign( const DenseVector<VT,true>& rhs )
{
   BLAZE_INTERNAL_ASSERT( size() == (~rhs).size(), "Invalid vector sizes" );

   BLAZE_CONSTRAINT_MUST_BE_VECTORIZABLE_TYPE( ElementType );

   const size_t columns( size() );

   if( columns > ( cacheSize/( sizeof(ElementType) * 3UL ) ) && !(~rhs).isAliased( &matrix_ ) )
   {
      for( size_t j=0UL; j<columns; j+=IT::size ) {
         stream( &matrix_(row_,j), (~rhs).get(j) );
      }
   }
   else
   {
      BLAZE_INTERNAL_ASSERT( ( columns - ( columns % (IT::size*4UL) ) ) == ( columns & size_t(-IT::size*4) ), "Invalid end calculation" );
      const size_t jend( columns & size_t(-IT::size*4) );

      for( size_t j=0UL; j<jend; j+=IT::size*4UL ) {
         store( &matrix_(row_,j             ), (~rhs).get(j             ) );
         store( &matrix_(row_,j+IT::size    ), (~rhs).get(j+IT::size    ) );
         store( &matrix_(row_,j+IT::size*2UL), (~rhs).get(j+IT::size*2UL) );
         store( &matrix_(row_,j+IT::size*3UL), (~rhs).get(j+IT::size*3UL) );
      }
      for( size_t j=jend; j<columns; j+=IT::size ) {
         store( &matrix_(row_,j), (~rhs).get(j) );
      }
   }
}
//*************************************************************************************************


//*************************************************************************************************
/*!\brief Default implementation of the assignment of a sparse vector.
//
// \param rhs The right-hand side sparse vector to be assigned.
// \return void
//
// This function must \b NOT be called explicitly! It is used internally for the performance
// optimized evaluation of expression templates. Calling this function explicitly might result
// in erroneous results and/or in compilation errors. Instead of using this function use the
// assignment operator.
*/
template< typename MT    // Type of the dense matrix
        , bool SO >      // Storage order
template< typename VT >  // Type of the right-hand side sparse vector
inline void DenseRow<MT,SO>::assign( const SparseVector<VT,true>& rhs )
{
   BLAZE_INTERNAL_ASSERT( size() == (~rhs).size(), "Invalid vector sizes" );

   for( typename VT::ConstIterator element=(~rhs).begin(); element!=(~rhs).end(); ++element )
      matrix_(row_,element->index()) = element->value();
}
//*************************************************************************************************


//*************************************************************************************************
/*!\brief Default implementation of the addition assignment of a dense vector.
//
// \param rhs The right-hand side dense vector to be added.
// \return void
//
// This function must \b NOT be called explicitly! It is used internally for the performance
// optimized evaluation of expression templates. Calling this function explicitly might result
// in erroneous results and/or in compilation errors. Instead of using this function use the
// assignment operator.
*/
template< typename MT    // Type of the dense matrix
        , bool SO >      // Storage order
template< typename VT >  // Type of the right-hand side dense vector
inline typename DisableIf< typename DenseRow<MT,SO>::BLAZE_TEMPLATE VectorizedAddAssign<VT> >::Type
   DenseRow<MT,SO>::addAssign( const DenseVector<VT,true>& rhs )
{
   BLAZE_INTERNAL_ASSERT( size() == (~rhs).size(), "Invalid vector sizes" );

   const size_t jend( (~rhs).size() & size_t(-2) );
   for( size_t j=0UL; j<jend; j+=2UL ) {
      matrix_(row_,j    ) += (~rhs)[j    ];
      matrix_(row_,j+1UL) += (~rhs)[j+1UL];
   }
   if( jend < (~rhs).size() )
      matrix_(row_,jend) += (~rhs)[jend];
}
//*************************************************************************************************


//*************************************************************************************************
/*!\brief Intrinsic optimized implementation of the addition assignment of a dense vector.
//
// \param rhs The right-hand side dense vector to be added.
// \return void
//
// This function must \b NOT be called explicitly! It is used internally for the performance
// optimized evaluation of expression templates. Calling this function explicitly might result
// in erroneous results and/or in compilation errors. Instead of using this function use the
// assignment operator.
*/
template< typename MT    // Type of the dense matrix
        , bool SO >      // Storage order
template< typename VT >  // Type of the right-hand side dense vector
inline typename EnableIf< typename DenseRow<MT,SO>::BLAZE_TEMPLATE VectorizedAddAssign<VT> >::Type
   DenseRow<MT,SO>::addAssign( const DenseVector<VT,true>& rhs )
{
   BLAZE_INTERNAL_ASSERT( size() == (~rhs).size(), "Invalid vector sizes" );

   BLAZE_CONSTRAINT_MUST_BE_VECTORIZABLE_TYPE( ElementType );

   const size_t columns( size() );

   BLAZE_INTERNAL_ASSERT( ( columns - ( columns % (IT::size*4UL) ) ) == ( columns & size_t(-IT::size*4) ), "Invalid end calculation" );
   const size_t jend( columns & size_t(-IT::size*4) );

   for( size_t j=0UL; j<jend; j+=IT::size*4UL ) {
      store( &matrix_(row_,j             ), load( &matrix_(row_,j             ) ) + (~rhs).get(j             ) );
      store( &matrix_(row_,j+IT::size    ), load( &matrix_(row_,j+IT::size    ) ) + (~rhs).get(j+IT::size    ) );
      store( &matrix_(row_,j+IT::size*2UL), load( &matrix_(row_,j+IT::size*2UL) ) + (~rhs).get(j+IT::size*2UL) );
      store( &matrix_(row_,j+IT::size*3UL), load( &matrix_(row_,j+IT::size*3UL) ) + (~rhs).get(j+IT::size*3UL) );
   }
   for( size_t j=jend; j<columns; j+=IT::size ) {
      store( &matrix_(row_,j), load( &matrix_(row_,j) ) + (~rhs).get(j) );
   }
}
//*************************************************************************************************


//*************************************************************************************************
/*!\brief Default implementation of the addition assignment of a sparse vector.
//
// \param rhs The right-hand side sparse vector to be added.
// \return void
//
// This function must \b NOT be called explicitly! It is used internally for the performance
// optimized evaluation of expression templates. Calling this function explicitly might result
// in erroneous results and/or in compilation errors. Instead of using this function use the
// assignment operator.
*/
template< typename MT    // Type of the dense matrix
        , bool SO >      // Storage order
template< typename VT >  // Type of the right-hand side sparse vector
inline void DenseRow<MT,SO>::addAssign( const SparseVector<VT,true>& rhs )
{
   BLAZE_INTERNAL_ASSERT( size() == (~rhs).size(), "Invalid vector sizes" );

   for( typename VT::ConstIterator element=(~rhs).begin(); element!=(~rhs).end(); ++element )
      matrix_(row_,element->index()) += element->value();
}
//*************************************************************************************************


//*************************************************************************************************
/*!\brief Default implementation of the subtraction assignment of a dense vector.
//
// \param rhs The right-hand side dense vector to be subtracted.
// \return void
//
// This function must \b NOT be called explicitly! It is used internally for the performance
// optimized evaluation of expression templates. Calling this function explicitly might result
// in erroneous results and/or in compilation errors. Instead of using this function use the
// assignment operator.
*/
template< typename MT    // Type of the dense matrix
        , bool SO >      // Storage order
template< typename VT >  // Type of the right-hand side dense vector
inline typename DisableIf< typename DenseRow<MT,SO>::BLAZE_TEMPLATE VectorizedSubAssign<VT> >::Type
   DenseRow<MT,SO>::subAssign( const DenseVector<VT,true>& rhs )
{
   BLAZE_INTERNAL_ASSERT( size() == (~rhs).size(), "Invalid vector sizes" );

   const size_t jend( (~rhs).size() & size_t(-2) );
   for( size_t j=0UL; j<jend; j+=2UL ) {
      matrix_(row_,j    ) -= (~rhs)[j    ];
      matrix_(row_,j+1UL) -= (~rhs)[j+1UL];
   }
   if( jend < (~rhs).size() )
      matrix_(row_,jend) -= (~rhs)[jend];
}
//*************************************************************************************************


//*************************************************************************************************
/*!\brief Intrinsic optimized implementation of the subtraction assignment of a dense vector.
//
// \param rhs The right-hand side dense vector to be subtracted.
// \return void
//
// This function must \b NOT be called explicitly! It is used internally for the performance
// optimized evaluation of expression templates. Calling this function explicitly might result
// in erroneous results and/or in compilation errors. Instead of using this function use the
// assignment operator.
*/
template< typename MT    // Type of the dense matrix
        , bool SO >      // Storage order
template< typename VT >  // Type of the right-hand side dense vector
inline typename EnableIf< typename DenseRow<MT,SO>::BLAZE_TEMPLATE VectorizedSubAssign<VT> >::Type
   DenseRow<MT,SO>::subAssign( const DenseVector<VT,true>& rhs )
{
   BLAZE_INTERNAL_ASSERT( size() == (~rhs).size(), "Invalid vector sizes" );

   BLAZE_CONSTRAINT_MUST_BE_VECTORIZABLE_TYPE( ElementType );

   const size_t columns( size() );

   BLAZE_INTERNAL_ASSERT( ( columns - ( columns % (IT::size*4UL) ) ) == ( columns & size_t(-IT::size*4) ), "Invalid end calculation" );
   const size_t jend( columns & size_t(-IT::size*4) );

   for( size_t j=0UL; j<jend; j+=IT::size*4UL ) {
      store( &matrix_(row_,j             ), load( &matrix_(row_,j             ) ) - (~rhs).get(j             ) );
      store( &matrix_(row_,j+IT::size    ), load( &matrix_(row_,j+IT::size    ) ) - (~rhs).get(j+IT::size    ) );
      store( &matrix_(row_,j+IT::size*2UL), load( &matrix_(row_,j+IT::size*2UL) ) - (~rhs).get(j+IT::size*2UL) );
      store( &matrix_(row_,j+IT::size*3UL), load( &matrix_(row_,j+IT::size*3UL) ) - (~rhs).get(j+IT::size*3UL) );
   }
   for( size_t j=jend; j<columns; j+=IT::size ) {
      store( &matrix_(row_,j), load( &matrix_(row_,j) ) - (~rhs).get(j) );
   }
}
//*************************************************************************************************


//*************************************************************************************************
/*!\brief Default implementation of the subtraction assignment of a sparse vector.
//
// \param rhs The right-hand side sparse vector to be subtracted.
// \return void
//
// This function must \b NOT be called explicitly! It is used internally for the performance
// optimized evaluation of expression templates. Calling this function explicitly might result
// in erroneous results and/or in compilation errors. Instead of using this function use the
// assignment operator.
*/
template< typename MT    // Type of the dense matrix
        , bool SO >      // Storage order
template< typename VT >  // Type of the right-hand side sparse vector
inline void DenseRow<MT,SO>::subAssign( const SparseVector<VT,true>& rhs )
{
   BLAZE_INTERNAL_ASSERT( size() == (~rhs).size(), "Invalid vector sizes" );

   for( typename VT::ConstIterator element=(~rhs).begin(); element!=(~rhs).end(); ++element )
      matrix_(row_,element->index()) -= element->value();
}
//*************************************************************************************************


//*************************************************************************************************
/*!\brief Default implementation of the multiplication assignment of a dense vector.
//
// \param rhs The right-hand side dense vector to be multiplied.
// \return void
//
// This function must \b NOT be called explicitly! It is used internally for the performance
// optimized evaluation of expression templates. Calling this function explicitly might result
// in erroneous results and/or in compilation errors. Instead of using this function use the
// assignment operator.
*/
template< typename MT    // Type of the dense matrix
        , bool SO >      // Storage order
template< typename VT >  // Type of the right-hand side dense vector
inline typename DisableIf< typename DenseRow<MT,SO>::BLAZE_TEMPLATE VectorizedMultAssign<VT> >::Type
   DenseRow<MT,SO>::multAssign( const DenseVector<VT,true>& rhs )
{
   BLAZE_INTERNAL_ASSERT( size() == (~rhs).size(), "Invalid vector sizes" );

   const size_t jend( (~rhs).size() & size_t(-2) );
   for( size_t j=0UL; j<jend; j+=2UL ) {
      matrix_(row_,j    ) *= (~rhs)[j    ];
      matrix_(row_,j+1UL) *= (~rhs)[j+1UL];
   }
   if( jend < (~rhs).size() )
      matrix_(row_,jend) *= (~rhs)[jend];
}
//*************************************************************************************************


//*************************************************************************************************
/*!\brief Intrinsic optimized implementation of the multiplication assignment of a dense vector.
//
// \param rhs The right-hand side dense vector to be multiplied.
// \return void
//
// This function must \b NOT be called explicitly! It is used internally for the performance
// optimized evaluation of expression templates. Calling this function explicitly might result
// in erroneous results and/or in compilation errors. Instead of using this function use the
// assignment operator.
*/
template< typename MT    // Type of the dense matrix
        , bool SO >      // Storage order
template< typename VT >  // Type of the right-hand side dense vector
inline typename EnableIf< typename DenseRow<MT,SO>::BLAZE_TEMPLATE VectorizedMultAssign<VT> >::Type
   DenseRow<MT,SO>::multAssign( const DenseVector<VT,true>& rhs )
{
   BLAZE_INTERNAL_ASSERT( size() == (~rhs).size(), "Invalid vector sizes" );

   BLAZE_CONSTRAINT_MUST_BE_VECTORIZABLE_TYPE( ElementType );

   const size_t columns( size() );

   BLAZE_INTERNAL_ASSERT( ( columns - ( columns % (IT::size*4UL) ) ) == ( columns & size_t(-IT::size*4) ), "Invalid end calculation" );
   const size_t jend( columns & size_t(-IT::size*4) );

   for( size_t j=0UL; j<jend; j+=IT::size*4UL ) {
      store( &matrix_(row_,j             ), load( &matrix_(row_,j             ) ) * (~rhs).get(j             ) );
      store( &matrix_(row_,j+IT::size    ), load( &matrix_(row_,j+IT::size    ) ) * (~rhs).get(j+IT::size    ) );
      store( &matrix_(row_,j+IT::size*2UL), load( &matrix_(row_,j+IT::size*2UL) ) * (~rhs).get(j+IT::size*2UL) );
      store( &matrix_(row_,j+IT::size*3UL), load( &matrix_(row_,j+IT::size*3UL) ) * (~rhs).get(j+IT::size*3UL) );
   }
   for( size_t j=jend; j<columns; j+=IT::size ) {
      store( &matrix_(row_,j), load( &matrix_(row_,j) ) * (~rhs).get(j) );
   }
}
//*************************************************************************************************


//*************************************************************************************************
/*!\brief Default implementation of the multiplication assignment of a sparse vector.
//
// \param rhs The right-hand side sparse vector to be multiplied.
// \return void
//
// This function must \b NOT be called explicitly! It is used internally for the performance
// optimized evaluation of expression templates. Calling this function explicitly might result
// in erroneous results and/or in compilation errors. Instead of using this function use the
// assignment operator.
*/
template< typename MT    // Type of the dense matrix
        , bool SO >      // Storage order
template< typename VT >  // Type of the right-hand side sparse vector
inline void DenseRow<MT,SO>::multAssign( const SparseVector<VT,true>& rhs )
{
   BLAZE_INTERNAL_ASSERT( size() == (~rhs).size(), "Invalid vector sizes" );

   const ResultType tmp( *this );

   reset();

   for( typename VT::ConstIterator element=(~rhs).begin(); element!=(~rhs).end(); ++element )
      matrix_(row_,element->index()) = tmp[element->index()] * element->value();
}
//*************************************************************************************************








//=================================================================================================
//
//  CLASS TEMPLATE SPECIALIZATION FOR COLUMN-MAJOR MATRICES
//
//=================================================================================================

//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Specialization of DenseRow for column-major matrices.
// \ingroup dense_row
//
// This specialization of DenseRow adapts the class template to the requirements of
// column-major matrices.
*/
template< typename MT >  // Type of the dense matrix
class DenseRow<MT,false> : public DenseVector< DenseRow<MT,false>, true >
                         , private Expression
{
 private:
   //**********************************************************************************************
   //! Compilation switch for the non-const reference and iterator types.
   /*! The \a useConst compile time constant expression represents a compilation switch for
       the non-const reference and iterator types. In case the given dense matrix of type
       \a MT is const qualified, \a useConst will be set to 1 and the dense row will return
       references and iterators to const. Otherwise \a useConst will be set to 0 and the
       dense row will offer write access to the dense matrix elements both via the subscript
       operator and iterators. */
   enum { useConst = IsConst<MT>::value };
   //**********************************************************************************************

 public:
   //**Type definitions****************************************************************************
   typedef DenseRow<MT,false>                  This;           //!< Type of this DenseRow instance.
   typedef typename RowTrait<MT>::Type         ResultType;     //!< Result type for expression template evaluations.
   typedef typename ResultType::TransposeType  TransposeType;  //!< Transpose type for expression template evaluations.
   typedef typename MT::ElementType            ElementType;    //!< Type of the row elements.
   typedef typename MT::ReturnType             ReturnType;     //!< Return type for expression template evaluations
   typedef const ResultType                    CompositeType;  //!< Data type for composite expression templates.

   //! Reference to a constant row value.
   typedef typename MT::ConstReference  ConstReference;

   //! Reference to a non-constant row value.
   typedef typename SelectType< useConst, ConstReference, typename MT::Reference >::Type  Reference;
   //**********************************************************************************************

   //**RowIterator class definition****************************************************************
   /*!\brief Iterator over the elements of the dense row.
   */
   template< typename MatrixType >  // Type of the dense matrix
   class RowIterator
   {
    private:
      //*******************************************************************************************
      //! Compilation switch for the return type of the value member function.
      /*! The \a returnConst compile time constant expression represents a compilation switch for
          the return type of the member access operators. In case the given matrix type \a MatrixType
          is const qualified, \a returnConst will be set to 1 and the member access operators will
          return a reference to const. Otherwise \a returnConst will be set to 0 and the member
          access operators will offer write access to the dense matrix elements. */
      enum { returnConst = IsConst<MatrixType>::value };
      //*******************************************************************************************

    public:
      //**Type definitions*************************************************************************
      //! Return type for the access to the value of a dense element.
      typedef typename SelectType< returnConst, typename MT::ConstReference, typename MT::Reference >::Type  Reference;

      typedef std::forward_iterator_tag    IteratorCategory;  //!< The iterator category.
      typedef RemoveReference<Reference>   ValueType;         //!< Type of the underlying elements.
      typedef ValueType*                   PointerType;       //!< Pointer return type.
      typedef Reference                    ReferenceType;     //!< Reference return type.
      typedef ptrdiff_t                    DifferenceType;    //!< Difference between two iterators.

      // STL iterator requirements
      typedef IteratorCategory  iterator_category;  //!< The iterator category.
      typedef ValueType         value_type;         //!< Type of the underlying elements.
      typedef PointerType       pointer;            //!< Pointer return type.
      typedef ReferenceType     reference;          //!< Reference return type.
      typedef DifferenceType    difference_type;    //!< Difference between two iterators.
      //*******************************************************************************************

      //**Constructor******************************************************************************
      /*!\brief Constructor for the RowIterator class.
      //
      // \param matrix The matrix containing the row.
      // \param row The row index.
      // \param column The column index.
      */
      inline RowIterator( MatrixType& matrix, size_t row, size_t column )
         : matrix_( matrix )  // The dense matrix containing the row.
         , row_   ( row    )  // The current row index.
         , column_( column )  // The current column index.
      {}
      //*******************************************************************************************

      //**Constructor******************************************************************************
      /*!\brief Conversion constructor from different RowIterator instances.
      //
      // \param it The row iterator to be copied.
      */
      template< typename MatrixType2 >
      inline RowIterator( const RowIterator<MatrixType2>& it )
         : matrix_( it.matrix_ )  // The dense matrix containing the row.
         , row_   ( it.row_    )  // The current row index.
         , column_( it.column_ )  // The current column index.
      {}
      //*******************************************************************************************

      //**Prefix increment operator****************************************************************
      /*!\brief Pre-increment operator.
      //
      // \return Reference to the incremented iterator.
      */
      inline RowIterator& operator++() {
         ++column_;
         return *this;
      }
      //*******************************************************************************************

      //**Postfix increment operator***************************************************************
      /*!\brief Post-increment operator.
      //
      // \return The previous position of the iterator.
      */
      inline const RowIterator operator++( int ) {
         const RowIterator tmp( *this );
         ++(*this);
         return tmp;
      }
      //*******************************************************************************************

      //**Element access operator******************************************************************
      /*!\brief Direct access to the dense vector element at the current iterator position.
      //
      // \return The current value of the dense element.
      */
      inline ReferenceType operator*() const {
         return matrix_(row_,column_);
      }
      //*******************************************************************************************

      //**Element access operator******************************************************************
      /*!\brief Direct access to the dense vector element at the current iterator position.
      //
      // \return Reference to the dense vector element at the current iterator position.
      */
      inline PointerType operator->() const {
         return &matrix_(row_,column_);
      }
      //*******************************************************************************************

      //**Equality operator************************************************************************
      /*!\brief Equality comparison between two RowIterator objects.
      //
      // \param rhs The right-hand side expression iterator.
      // \return \a true if the iterators refer to the same element, \a false if not.
      */
      template< typename MatrixType2 >
      inline bool operator==( const RowIterator<MatrixType2>& rhs ) const {
         return ( &matrix_ == &rhs.matrix_ ) && ( row_ == rhs.row_ ) && ( column_ == rhs.column_ );
      }
      //*******************************************************************************************

      //**Inequality operator**********************************************************************
      /*!\brief Inequality comparison between two RowIterator objects.
      //
      // \param rhs The right-hand side expression iterator.
      // \return \a true if the iterators don't refer to the same element, \a false if they do.
      */
      template< typename MatrixType2 >
      inline bool operator!=( const RowIterator<MatrixType2>& rhs ) const {
         return !( *this == rhs );
      }
      //*******************************************************************************************

      //**Subtraction operator*********************************************************************
      /*!\brief Calculating the number of elements between two expression iterators.
      //
      // \param rhs The right-hand side expression iterator.
      // \return The number of elements between the two expression iterators.
      */
      inline DifferenceType operator-( const RowIterator& rhs ) const {
         return column_ - rhs.column_;
      }
      //*******************************************************************************************

    private:
      //**Member variables*************************************************************************
      MatrixType&  matrix_;  //!< The dense matrix containing the row.
      size_t       row_;     //!< The current row index.
      size_t       column_;  //!< The current column index.
      //*******************************************************************************************

      //**Friend declarations**********************************************************************
      /*! \cond BLAZE_INTERNAL */
      template< typename MatrixType2 > friend class RowIterator;
      /*! \endcond */
      //*******************************************************************************************
   };
   //**********************************************************************************************

   //**Type definitions****************************************************************************
   //! Iterator over constant elements.
   typedef RowIterator<const MT>  ConstIterator;

   //! Iterator over non-constant elements.
   typedef typename SelectType< useConst, ConstIterator, RowIterator<MT> >::Type  Iterator;
   //**********************************************************************************************

   //**Compilation flags***************************************************************************
   //! Compilation switch for the expression template evaluation strategy.
   enum { vectorizable = 0 };
   //**********************************************************************************************

   //**Constructors********************************************************************************
   /*!\name Constructors */
   //@{
   explicit inline DenseRow( MT& matrix, size_t index );
   // No explicitly declared copy constructor.
   //@}
   //**********************************************************************************************

   //**Destructor**********************************************************************************
   // No explicitly declared destructor.
   //**********************************************************************************************

   //**Data access functions***********************************************************************
   /*!\name Data access functions */
   //@{
   inline Reference      operator[]( size_t index );
   inline ConstReference operator[]( size_t index ) const;
   inline Iterator       begin ();
   inline ConstIterator  begin () const;
   inline ConstIterator  cbegin() const;
   inline Iterator       end   ();
   inline ConstIterator  end   () const;
   inline ConstIterator  cend  () const;
   //@}
   //**********************************************************************************************

   //**Assignment operators************************************************************************
   /*!\name Assignment operators */
   //@{
                           inline DenseRow& operator= ( const ElementType& rhs );
                           inline DenseRow& operator= ( const DenseRow& rhs );
   template< typename VT > inline DenseRow& operator= ( const Vector<VT,true>& rhs );
   template< typename VT > inline DenseRow& operator+=( const Vector<VT,true>& rhs );
   template< typename VT > inline DenseRow& operator-=( const Vector<VT,true>& rhs );
   template< typename VT > inline DenseRow& operator*=( const Vector<VT,true>& rhs );

   template< typename Other >
   inline typename EnableIf< IsNumeric<Other>, DenseRow >::Type&
      operator*=( Other rhs );

   template< typename Other >
   inline typename EnableIf< IsNumeric<Other>, DenseRow >::Type&
      operator/=( Other rhs );
   //@}
   //**********************************************************************************************

   //**Utility functions***************************************************************************
   /*!\name Utility functions */
   //@{
                              inline size_t    size() const;
                              inline size_t    capacity() const;
                              inline size_t    nonZeros() const;
                              inline void      reset();
   template< typename Other > inline DenseRow& scale( const Other& scalar );
   //@}
   //**********************************************************************************************

 public:
   //**Expression template evaluation functions****************************************************
   /*!\name Expression template evaluation functions */
   //@{
   template< typename Other > inline bool canAlias  ( const Other* alias ) const;
   template< typename Other > inline bool isAliased ( const Other* alias ) const;
   template< typename VT >    inline void assign    ( const DenseVector <VT,true>& rhs );
   template< typename VT >    inline void assign    ( const SparseVector<VT,true>& rhs );
   template< typename VT >    inline void addAssign ( const DenseVector <VT,true>& rhs );
   template< typename VT >    inline void addAssign ( const SparseVector<VT,true>& rhs );
   template< typename VT >    inline void subAssign ( const DenseVector <VT,true>& rhs );
   template< typename VT >    inline void subAssign ( const SparseVector<VT,true>& rhs );
   template< typename VT >    inline void multAssign( const DenseVector <VT,true>& rhs );
   template< typename VT >    inline void multAssign( const SparseVector<VT,true>& rhs );
   //@}
   //**********************************************************************************************

 private:
   //**Member variables****************************************************************************
   /*!\name Member variables */
   //@{
   MT&          matrix_;  //!< The dense matrix containing the row.
   const size_t row_;     //!< The index of the row in the matrix.
   //@}
   //**********************************************************************************************

   //**Compile time checks*************************************************************************
   /*! \cond BLAZE_INTERNAL */
   BLAZE_CONSTRAINT_MUST_BE_DENSE_MATRIX_TYPE       ( MT );
   BLAZE_CONSTRAINT_MUST_BE_COLUMN_MAJOR_MATRIX_TYPE( MT );
   BLAZE_CONSTRAINT_MUST_NOT_BE_EXPRESSION_TYPE     ( MT );
   /*! \endcond */
   //**********************************************************************************************
};
/*! \endcond */
//*************************************************************************************************




//=================================================================================================
//
//  CONSTRUCTOR
//
//=================================================================================================

//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief The constructor for DenseRow.
//
// \param matrix The matrix containing the row.
// \param index The index of the row.
// \exception std::invalid_argument Invalid row access index.
*/
template< typename MT >  // Type of the dense matrix
inline DenseRow<MT,false>::DenseRow( MT& matrix, size_t index )
   : matrix_( matrix )  // The dense matrix containing the row
   , row_   ( index  )  // The index of the row in the matrix
{
   if( matrix_.rows() <= index )
      throw std::invalid_argument( "Invalid row access index" );
}
/*! \endcond */
//*************************************************************************************************




//=================================================================================================
//
//  DATA ACCESS FUNCTIONS
//
//=================================================================================================

//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Subscript operator for the direct access to the row elements.
//
// \param index Access index. The index must be smaller than the number of matrix columns.
// \return Reference to the accessed value.
*/
template< typename MT >  // Type of the dense matrix
inline typename DenseRow<MT,false>::Reference DenseRow<MT,false>::operator[]( size_t index )
{
   BLAZE_USER_ASSERT( index < size(), "Invalid row access index" );
   return matrix_(row_,index);
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Subscript operator for the direct access to the row elements.
//
// \param index Access index. The index must be smaller than the number of matrix columns.
// \return Reference to the accessed value.
*/
template< typename MT >  // Type of the dense matrix
inline typename DenseRow<MT,false>::ConstReference DenseRow<MT,false>::operator[]( size_t index ) const
{
   BLAZE_USER_ASSERT( index < size(), "Invalid row access index" );
   return matrix_(row_,index);
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Returns an iterator to the first element of the row.
//
// \return Iterator to the first element of the row.
//
// This function returns an iterator to the first element of the row.
*/
template< typename MT >  // Type of the dense matrix
inline typename DenseRow<MT,false>::Iterator DenseRow<MT,false>::begin()
{
   return Iterator( matrix_, row_, 0UL );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Returns an iterator to the first element of the row.
//
// \return Iterator to the first element of the row.
//
// This function returns an iterator to the first element of the row.
*/
template< typename MT >  // Type of the dense matrix
inline typename DenseRow<MT,false>::ConstIterator DenseRow<MT,false>::begin() const
{
   return ConstIterator( matrix_, row_, 0UL );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Returns an iterator to the first element of the row.
//
// \return Iterator to the first element of the row.
//
// This function returns an iterator to the first element of the row.
*/
template< typename MT >  // Type of the dense matrix
inline typename DenseRow<MT,false>::ConstIterator DenseRow<MT,false>::cbegin() const
{
   return ConstIterator( matrix_, row_, 0UL );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Returns an iterator just past the last element of the row.
//
// \return Iterator just past the last element of the row.
//
// This function returns an iterator just past the last element of the row.
*/
template< typename MT >  // Type of the dense matrix
inline typename DenseRow<MT,false>::Iterator DenseRow<MT,false>::end()
{
   return Iterator( matrix_, row_, size() );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Returns an iterator just past the last element of the row.
//
// \return Iterator just past the last element of the row.
//
// This function returns an iterator just past the last element of the row.
*/
template< typename MT >  // Type of the dense matrix
inline typename DenseRow<MT,false>::ConstIterator DenseRow<MT,false>::end() const
{
   return ConstIterator( matrix_, row_, size() );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Returns an iterator just past the last element of the row.
//
// \return Iterator just past the last element of the row.
//
// This function returns an iterator just past the last element of the row.
*/
template< typename MT >  // Type of the dense matrix
inline typename DenseRow<MT,false>::ConstIterator DenseRow<MT,false>::cend() const
{
   return ConstIterator( matrix_, row_, size() );
}
/*! \endcond */
//*************************************************************************************************




//=================================================================================================
//
//  ASSIGNMENT OPERATORS
//
//=================================================================================================

//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Homogenous assignment to all row elements.
//
// \param rhs Scalar value to be assigned to all row elements.
// \return Reference to the assigned row.
*/
template< typename MT >  // Type of the dense matrix
inline DenseRow<MT,false>& DenseRow<MT,false>::operator=( const ElementType& rhs )
{
   const size_t columns( size() );

   for( size_t j=0UL; j<columns; ++j )
      matrix_(row_,j) = rhs;

   return *this;
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Copy assignment operator for DenseRow.
//
// \param rhs Dense row to be copied.
// \return Reference to the assigned row.
// \exception std::invalid_argument Row sizes do not match.
//
// In case the current sizes of the two rows don't match, a \a std::invalid_argument
// exception is thrown.
*/
template< typename MT >  // Type of the dense matrix
inline DenseRow<MT,false>& DenseRow<MT,false>::operator=( const DenseRow& rhs )
{
   if( &rhs == this ) return *this;

   if( size() != rhs.size() )
      throw std::invalid_argument( "Row sizes do not match" );

   const size_t columns( size() );

   for( size_t j=0UL; j<columns; ++j )
      matrix_(row_,j) = rhs[j];

   return *this;
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Assignment operator for different vectors.
//
// \param rhs Vector to be assigned.
// \return Reference to the assigned row.
// \exception std::invalid_argument Vector sizes do not match.
//
// In case the current sizes of the two vectors don't match, a \a std::invalid_argument
// exception is thrown.
*/
template< typename MT >  // Type of the dense matrix
template< typename VT >  // Type of the right-hand side vector
inline DenseRow<MT,false>& DenseRow<MT,false>::operator=( const Vector<VT,true>& rhs )
{
   using blaze::assign;

   if( size() != (~rhs).size() )
      throw std::invalid_argument( "Vector sizes do not match" );

   if( (~rhs).canAlias( &matrix_ ) ) {
      const ResultType tmp( ~rhs );
      assign( *this, tmp );
   }
   else {
      if( IsSparseVector<VT>::value )
         reset();
      assign( *this, ~rhs );
   }

   return *this;
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Addition assignment operator for the addition of a vector (\f$ \vec{a}+=\vec{b} \f$).
//
// \param rhs The right-hand side vector to be added to the dense row.
// \return Reference to the assigned row.
// \exception std::invalid_argument Vector sizes do not match.
//
// In case the current sizes of the two vectors don't match, a \a std::invalid_argument exception
// is thrown.
*/
template< typename MT >  // Type of the dense matrix
template< typename VT >  // Type of the right-hand side vector
inline DenseRow<MT,false>& DenseRow<MT,false>::operator+=( const Vector<VT,true>& rhs )
{
   using blaze::addAssign;

   BLAZE_CONSTRAINT_MUST_BE_TRANSPOSE_VECTOR_TYPE( typename VT::ResultType );
   BLAZE_CONSTRAINT_MUST_NOT_REQUIRE_EVALUATION  ( typename VT::ResultType );

   if( size() != (~rhs).size() )
      throw std::invalid_argument( "Vector sizes do not match" );

   if( (~rhs).canAlias( &matrix_ ) ) {
      const typename VT::ResultType tmp( ~rhs );
      addAssign( *this, tmp );
   }
   else {
      addAssign( *this, ~rhs );
   }

   return *this;
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Subtraction assignment operator for the subtraction of a vector (\f$ \vec{a}-=\vec{b} \f$).
//
// \param rhs The right-hand side vector to be subtracted from the dense row.
// \return Reference to the assigned row.
// \exception std::invalid_argument Vector sizes do not match.
//
// In case the current sizes of the two vectors don't match, a \a std::invalid_argument exception
// is thrown.
*/
template< typename MT >  // Type of the dense matrix
template< typename VT >  // Type of the right-hand side vector
inline DenseRow<MT,false>& DenseRow<MT,false>::operator-=( const Vector<VT,true>& rhs )
{
   using blaze::subAssign;

   BLAZE_CONSTRAINT_MUST_BE_TRANSPOSE_VECTOR_TYPE( typename VT::ResultType );
   BLAZE_CONSTRAINT_MUST_NOT_REQUIRE_EVALUATION  ( typename VT::ResultType );

   if( size() != (~rhs).size() )
      throw std::invalid_argument( "Vector sizes do not match" );

   if( (~rhs).canAlias( &matrix_ ) ) {
      const typename VT::ResultType tmp( ~rhs );
      subAssign( *this, tmp );
   }
   else {
      subAssign( *this, ~rhs );
   }

   return *this;
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Multiplication assignment operator for the multiplication of a vector
//        (\f$ \vec{a}*=\vec{b} \f$).
//
// \param rhs The right-hand side vector to be multiplied with the dense row.
// \return Reference to the assigned row.
// \exception std::invalid_argument Vector sizes do not match.
//
// In case the current sizes of the two vectors don't match, a \a std::invalid_argument exception
// is thrown.
*/
template< typename MT >  // Type of the dense matrix
template< typename VT >  // Type of the right-hand side vector
inline DenseRow<MT,false>& DenseRow<MT,false>::operator*=( const Vector<VT,true>& rhs )
{
   using blaze::multAssign;

   BLAZE_CONSTRAINT_MUST_BE_TRANSPOSE_VECTOR_TYPE( typename VT::ResultType );
   BLAZE_CONSTRAINT_MUST_NOT_REQUIRE_EVALUATION  ( typename VT::ResultType );

   if( size() != (~rhs).size() )
      throw std::invalid_argument( "Vector sizes do not match" );

   if( (~rhs).canAlias( &matrix_ ) || IsSparseVector<VT>::value ) {
      const typename VT::ResultType tmp( ~rhs );
      multAssign( *this, tmp );
   }
   else {
      multAssign( *this, ~rhs );
   }

   return *this;
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Multiplication assignment operator for the multiplication between a vector and
//        a scalar value (\f$ \vec{a}*=s \f$).
//
// \param rhs The right-hand side scalar value for the multiplication.
// \return Reference to the vector.
*/
template< typename MT >     // Type of the dense matrix
template< typename Other >  // Data type of the right-hand side scalar
inline typename EnableIf< IsNumeric<Other>, DenseRow<MT,false> >::Type&
   DenseRow<MT,false>::operator*=( Other rhs )
{
   return operator=( (*this) * rhs );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Division assignment operator for the division of a vector by a scalar value
//        (\f$ \vec{a}/=s \f$).
//
// \param rhs The right-hand side scalar value for the division.
// \return Reference to the vector.
//
// \b Note: A division by zero is only checked by an user assert.
*/
template< typename MT >     // Type of the dense matrix
template< typename Other >  // Data type of the right-hand side scalar
inline typename EnableIf< IsNumeric<Other>, DenseRow<MT,false> >::Type&
   DenseRow<MT,false>::operator/=( Other rhs )
{
   BLAZE_USER_ASSERT( rhs != Other(0), "Division by zero detected" );

   return operator=( (*this) / rhs );
}
/*! \endcond */
//*************************************************************************************************




//=================================================================================================
//
//  UTILITY FUNCTIONS
//
//=================================================================================================

//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Returns the current size/dimension of the row.
//
// \return The size of the row.
*/
template< typename MT >  // Type of the dense matrix
inline size_t DenseRow<MT,false>::size() const
{
   return matrix_.columns();
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Returns the maximum capacity of the dense row.
//
// \return The capacity of the dense row.
*/
template< typename MT >  // Type of the dense matrix
inline size_t DenseRow<MT,false>::capacity() const
{
   return matrix_.columns();
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Returns the number of non-zero elements in the row.
//
// \return The number of non-zero elements in the row.
//
// Note that the number of non-zero elements is always less than or equal to the current number
// of columns of the matrix containing the row.
*/
template< typename MT >  // Type of the dense matrix
inline size_t DenseRow<MT,false>::nonZeros() const
{
   const size_t columns( size() );
   size_t nonzeros( 0UL );

   for( size_t j=0UL; j<columns; ++j )
      if( !isDefault( matrix_(row_,j) ) )
         ++nonzeros;

   return nonzeros;
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Reset to the default initial values.
//
// \return void
*/
template< typename MT >  // Type of the dense matrix
inline void DenseRow<MT,false>::reset()
{
   using blaze::reset;
   const size_t columns( size() );
   for( size_t j=0UL; j<columns; ++j )
      reset( matrix_(row_,j) );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Scaling of the row by the scalar value \a scalar (\f$ \vec{a}=\vec{b}*s \f$).
//
// \param scalar The scalar value for the row scaling.
// \return Reference to the dense row.
*/
template< typename MT >     // Type of the dense matrix
template< typename Other >  // Data type of the scalar value
inline DenseRow<MT,false>& DenseRow<MT,false>::scale( const Other& scalar )
{
   for( size_t j=0UL; j<size(); ++j ) {
      matrix_(row_,j) *= scalar;
   }
   return *this;
}
/*! \endcond */
//*************************************************************************************************




//=================================================================================================
//
//  EXPRESSION TEMPLATE EVALUATION FUNCTIONS
//
//=================================================================================================

//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Returns whether the dense row can alias with the given address \a alias.
//
// \param alias The alias to be checked.
// \return \a true in case the alias corresponds to this dense row, \a false if not.
//
// This function returns whether the given address can alias with the dense row. In contrast
// to the isAliased() function this function is allowed to use compile time expressions to
// optimize the evaluation.
*/
template< typename MT >     // Type of the dense matrix
template< typename Other >  // Data type of the foreign expression
inline bool DenseRow<MT,false>::canAlias( const Other* alias ) const
{
   return static_cast<const void*>( &matrix_ ) == static_cast<const void*>( alias );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Returns whether the dense row is aliased with the given address \a alias.
//
// \param alias The alias to be checked.
// \return \a true in case the alias corresponds to this dense row, \a false if not.
//
// This function returns whether the given address is aliased with the dense row. In contrast
// to the canAlias() function this function is not allowed to use compile time expressions to
// optimize the evaluation.
*/
template< typename MT >     // Type of the dense matrix
template< typename Other >  // Data type of the foreign expression
inline bool DenseRow<MT,false>::isAliased( const Other* alias ) const
{
   return static_cast<const void*>( &matrix_ ) == static_cast<const void*>( alias );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Default implementation of the assignment of a dense vector.
//
// \param rhs The right-hand side dense vector to be assigned.
// \return void
//
// This function must \b NOT be called explicitly! It is used internally for the performance
// optimized evaluation of expression templates. Calling this function explicitly might result
// in erroneous results and/or in compilation errors. Instead of using this function use the
// assignment operator.
*/
template< typename MT >  // Type of the dense matrix
template< typename VT >  // Type of the right-hand side dense vector
inline void DenseRow<MT,false>::assign( const DenseVector<VT,true>& rhs )
{
   BLAZE_INTERNAL_ASSERT( size() == (~rhs).size(), "Invalid vector sizes" );

   const size_t jend( (~rhs).size() & size_t(-2) );
   for( size_t j=0UL; j<jend; j+=2UL ) {
      matrix_(row_,j    ) = (~rhs)[j    ];
      matrix_(row_,j+1UL) = (~rhs)[j+1UL];
   }
   if( jend < (~rhs).size() )
      matrix_(row_,jend) = (~rhs)[jend];
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Default implementation of the assignment of a sparse vector.
//
// \param rhs The right-hand side sparse vector to be assigned.
// \return void
//
// This function must \b NOT be called explicitly! It is used internally for the performance
// optimized evaluation of expression templates. Calling this function explicitly might result
// in erroneous results and/or in compilation errors. Instead of using this function use the
// assignment operator.
*/
template< typename MT >  // Type of the dense matrix
template< typename VT >  // Type of the right-hand side sparse vector
inline void DenseRow<MT,false>::assign( const SparseVector<VT,true>& rhs )
{
   BLAZE_INTERNAL_ASSERT( size() == (~rhs).size(), "Invalid vector sizes" );

   for( typename VT::ConstIterator element=(~rhs).begin(); element!=(~rhs).end(); ++element )
      matrix_(row_,element->index()) = element->value();
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Default implementation of the addition assignment of a dense vector.
//
// \param rhs The right-hand side dense vector to be added.
// \return void
//
// This function must \b NOT be called explicitly! It is used internally for the performance
// optimized evaluation of expression templates. Calling this function explicitly might result
// in erroneous results and/or in compilation errors. Instead of using this function use the
// assignment operator.
*/
template< typename MT >  // Type of the dense matrix
template< typename VT >  // Type of the right-hand side dense vector
inline void DenseRow<MT,false>::addAssign( const DenseVector<VT,true>& rhs )
{
   BLAZE_INTERNAL_ASSERT( size() == (~rhs).size(), "Invalid vector sizes" );

   const size_t jend( (~rhs).size() & size_t(-2) );
   for( size_t j=0UL; j<jend; j+=2UL ) {
      matrix_(row_,j    ) += (~rhs)[j    ];
      matrix_(row_,j+1UL) += (~rhs)[j+1UL];
   }
   if( jend < (~rhs).size() )
      matrix_(row_,jend) += (~rhs)[jend];
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Default implementation of the addition assignment of a sparse vector.
//
// \param rhs The right-hand side sparse vector to be added.
// \return void
//
// This function must \b NOT be called explicitly! It is used internally for the performance
// optimized evaluation of expression templates. Calling this function explicitly might result
// in erroneous results and/or in compilation errors. Instead of using this function use the
// assignment operator.
*/
template< typename MT >  // Type of the dense matrix
template< typename VT >  // Type of the right-hand side sparse vector
inline void DenseRow<MT,false>::addAssign( const SparseVector<VT,true>& rhs )
{
   BLAZE_INTERNAL_ASSERT( size() == (~rhs).size(), "Invalid vector sizes" );

   for( typename VT::ConstIterator element=(~rhs).begin(); element!=(~rhs).end(); ++element )
      matrix_(row_,element->index()) += element->value();
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Default implementation of the subtraction assignment of a dense vector.
//
// \param rhs The right-hand side dense vector to be subtracted.
// \return void
//
// This function must \b NOT be called explicitly! It is used internally for the performance
// optimized evaluation of expression templates. Calling this function explicitly might result
// in erroneous results and/or in compilation errors. Instead of using this function use the
// assignment operator.
*/
template< typename MT >  // Type of the dense matrix
template< typename VT >  // Type of the right-hand side dense vector
inline void DenseRow<MT,false>::subAssign( const DenseVector<VT,true>& rhs )
{
   BLAZE_INTERNAL_ASSERT( size() == (~rhs).size(), "Invalid vector sizes" );

   const size_t jend( (~rhs).size() & size_t(-2) );
   for( size_t j=0UL; j<jend; j+=2UL ) {
      matrix_(row_,j    ) -= (~rhs)[j    ];
      matrix_(row_,j+1UL) -= (~rhs)[j+1UL];
   }
   if( jend < (~rhs).size() )
      matrix_(row_,jend) -= (~rhs)[jend];
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Default implementation of the subtraction assignment of a sparse vector.
//
// \param rhs The right-hand side sparse vector to be subtracted.
// \return void
//
// This function must \b NOT be called explicitly! It is used internally for the performance
// optimized evaluation of expression templates. Calling this function explicitly might result
// in erroneous results and/or in compilation errors. Instead of using this function use the
// assignment operator.
*/
template< typename MT >  // Type of the dense matrix
template< typename VT >  // Type of the right-hand side sparse vector
inline void DenseRow<MT,false>::subAssign( const SparseVector<VT,true>& rhs )
{
   BLAZE_INTERNAL_ASSERT( size() == (~rhs).size(), "Invalid vector sizes" );

   for( typename VT::ConstIterator element=(~rhs).begin(); element!=(~rhs).end(); ++element )
      matrix_(row_,element->index()) -= element->value();
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Default implementation of the multiplication assignment of a dense vector.
//
// \param rhs The right-hand side dense vector to be multiplied.
// \return void
//
// This function must \b NOT be called explicitly! It is used internally for the performance
// optimized evaluation of expression templates. Calling this function explicitly might result
// in erroneous results and/or in compilation errors. Instead of using this function use the
// assignment operator.
*/
template< typename MT >  // Type of the dense matrix
template< typename VT >  // Type of the right-hand side dense vector
inline void DenseRow<MT,false>::multAssign( const DenseVector<VT,true>& rhs )
{
   BLAZE_INTERNAL_ASSERT( size() == (~rhs).size(), "Invalid vector sizes" );

   const size_t jend( (~rhs).size() & size_t(-2) );
   for( size_t j=0UL; j<jend; j+=2UL ) {
      matrix_(row_,j    ) *= (~rhs)[j    ];
      matrix_(row_,j+1UL) *= (~rhs)[j+1UL];
   }
   if( jend < (~rhs).size() )
      matrix_(row_,jend) *= (~rhs)[jend];
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Default implementation of the multiplication assignment of a sparse vector.
//
// \param rhs The right-hand side sparse vector to be multiplied.
// \return void
//
// This function must \b NOT be called explicitly! It is used internally for the performance
// optimized evaluation of expression templates. Calling this function explicitly might result
// in erroneous results and/or in compilation errors. Instead of using this function use the
// assignment operator.
*/
template< typename MT >  // Type of the dense matrix
template< typename VT >  // Type of the right-hand side sparse vector
inline void DenseRow<MT,false>::multAssign( const SparseVector<VT,true>& rhs )
{
   BLAZE_INTERNAL_ASSERT( size() == (~rhs).size(), "Invalid vector sizes" );

   const ResultType tmp( *this );

   reset();

   for( typename VT::ConstIterator element=(~rhs).begin(); element!=(~rhs).end(); ++element )
      matrix_(row_,element->index()) = tmp[element->index()] * element->value();
}
/*! \endcond */
//*************************************************************************************************








//=================================================================================================
//
//  DENSEROW OPERATORS
//
//=================================================================================================

//*************************************************************************************************
/*!\name DenseRow operators */
//@{
template< typename MT, bool SO >
inline void reset( DenseRow<MT,SO>& row );

template< typename MT, bool SO >
inline void clear( DenseRow<MT,SO>& row );

template< typename MT, bool SO >
inline bool isnan( const DenseRow<MT,SO>& row );

template< typename MT, bool SO >
inline bool isDefault( const DenseRow<MT,SO>& row );
//@}
//*************************************************************************************************


//*************************************************************************************************
/*!\brief Resetting the given dense row.
// \ingroup dense_row
//
// \param row The dense row to be resetted.
// \return void
*/
template< typename MT  // Type of the dense matrix
        , bool SO >    // Storage order
inline void reset( DenseRow<MT,SO>& row )
{
   row.reset();
}
//*************************************************************************************************


//*************************************************************************************************
/*!\brief Clearing the given dense row.
// \ingroup dense_row
//
// \param row The dense row to be cleared.
// \return void
*/
template< typename MT  // Type of the dense matrix
        , bool SO >    // Storage order
inline void clear( DenseRow<MT,SO>& row )
{
   row.reset();
}
//*************************************************************************************************


//*************************************************************************************************
/*!\brief Checks the given dense row for not-a-number elements.
// \ingroup dense_row
//
// \param row The dense row to be checked for not-a-number elements.
// \return \a true if at least one element of the row is not-a-number, \a false otherwise.
//
// This function checks the dense row for not-a-number (NaN) elements. If at least one element
// of the row is not-a-number, the function returns \a true, otherwise it returns \a false.

   \code
   blaze::DynamicMatrix<int,rowMajor> A;
   // ... Resizing and initialization
   if( isnan( row( A, 0UL ) ) ) { ... }
   \endcode
*/
template< typename MT  // Type of the dense matrix
        , bool SO >    // Storage order
inline bool isnan( const DenseRow<MT,SO>& row )
{
   for( size_t i=0UL; i<row.size(); ++i ) {
      if( isnan( row[i] ) ) return true;
   }
   return false;
}
//*************************************************************************************************


//*************************************************************************************************
/*!\brief Returns whether the given dense row is in default state.
// \ingroup dense_row
//
// \param row The dense row to be tested for its default state.
// \return \a true in case the given dense row is component-wise zero, \a false otherwise.
//
// This function checks whether the dense row is in default state. For instance, in case the
// row is instantiated for a built-in integral or floating point data type, the function
// returns \a true in case all row elements are 0 and \a false in case any row element
// is not 0. The following example demonstrates the use of the \a isDefault function:

   \code
   blaze::DynamicMatrix<int,rowMajor> A;
   // ... Resizing and initialization
   if( isDefault( row( A, 0UL ) ) ) { ... }
   \endcode
*/
template< typename MT  // Type of the dense matrix
        , bool SO >    // Storage order
inline bool isDefault( const DenseRow<MT,SO>& row )
{
   for( size_t i=0UL; i<row.size(); ++i )
      if( !isDefault( row[i] ) ) return false;
   return true;
}
//*************************************************************************************************




//=================================================================================================
//
//  GLOBAL OPERATORS
//
//=================================================================================================

//*************************************************************************************************
/*!\brief Creating a view on a specific row of the given dense matrix.
// \ingroup views
//
// \param dm The dense matrix containing the row.
// \param index The index of the row.
// \return View on the specified row of the matrix.
// \exception std::invalid_argument Invalid row access index.
//
// This function returns an expression representing the specified row of the given dense matrix.

   \code
   typedef blaze::DynamicMatrix<double,rowMajor>  Matrix;

   Matrix A;
   // ... Resizing and initialization
   blaze::DenseRow<Matrix> = row( A, 3UL );
   \endcode
*/
template< typename MT  // Type of the dense matrix
        , bool SO >    // Storage order
inline DenseRow<MT> row( DenseMatrix<MT,SO>& dm, size_t index )
{
   BLAZE_FUNCTION_TRACE;

   return DenseRow<MT>( ~dm, index );
}
//*************************************************************************************************


//*************************************************************************************************
/*!\brief Creating a view on a specific row of the given constant dense matrix.
// \ingroup views
//
// \param dm The constant dense matrix containing the row.
// \param index The index of the row.
// \return View on the specified row of the matrix.
// \exception std::invalid_argument Invalid row access index.
//
// This function returns an expression representing the specified row of the given dense matrix.

   \code
   typedef blaze::DynamicMatrix<double,rowMajor>  Matrix;

   const Matrix A;
   // ... Resizing and initialization
   blaze::DenseRow<const Matrix> = row( A, 3UL );
   \endcode
*/
template< typename MT  // Type of the dense matrix
        , bool SO >    // Storage order
inline DenseRow<const MT> row( const DenseMatrix<MT,SO>& dm, size_t index )
{
   BLAZE_FUNCTION_TRACE;

   return DenseRow<const MT>( ~dm, index );
}
//*************************************************************************************************




//=================================================================================================
//
//  ADDTRAIT SPECIALIZATIONS
//
//=================================================================================================

//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
template< typename T1, bool SO, typename T2, size_t N >
struct AddTrait< DenseRow<T1,SO>, StaticVector<T2,N,true> >
{
   typedef typename AddTrait< typename DenseRow<T1,SO>::ResultType,
                              StaticVector<T2,N,true> >::Type  Type;
};

template< typename T1, size_t N, typename T2, bool SO >
struct AddTrait< StaticVector<T1,N,true>, DenseRow<T2,SO> >
{
   typedef typename AddTrait< StaticVector<T1,N,true>,
                              typename DenseRow<T2,SO>::ResultType >::Type  Type;
};

template< typename T1, bool SO, typename T2 >
struct AddTrait< DenseRow<T1,SO>, DynamicVector<T2,true> >
{
   typedef typename AddTrait< typename DenseRow<T1,SO>::ResultType,
                              DynamicVector<T2,true> >::Type  Type;
};

template< typename T1, typename T2, bool SO >
struct AddTrait< DynamicVector<T1,true>, DenseRow<T2,SO> >
{
   typedef typename AddTrait< DynamicVector<T1,true>,
                              typename DenseRow<T2,SO>::ResultType >::Type  Type;
};

template< typename T1, bool SO, typename T2 >
struct AddTrait< DenseRow<T1,SO>, CompressedVector<T2,true> >
{
   typedef typename AddTrait< typename DenseRow<T1,SO>::ResultType,
                              CompressedVector<T2,true> >::Type  Type;
};

template< typename T1, typename T2, bool SO >
struct AddTrait< CompressedVector<T1,true>, DenseRow<T2,SO> >
{
   typedef typename AddTrait< CompressedVector<T1,true>,
                              typename DenseRow<T2,SO>::ResultType >::Type  Type;
};

template< typename T1, bool SO1, typename T2, bool SO2 >
struct AddTrait< DenseRow<T1,SO1>, DenseRow<T2,SO2> >
{
   typedef typename AddTrait< typename DenseRow<T1,SO1>::ResultType,
                              typename DenseRow<T2,SO2>::ResultType >::Type  Type;
};
/*! \endcond */
//*************************************************************************************************




//=================================================================================================
//
//  SUBTRAIT SPECIALIZATIONS
//
//=================================================================================================

//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
template< typename T1, bool SO, typename T2, size_t N >
struct SubTrait< DenseRow<T1,SO>, StaticVector<T2,N,true> >
{
   typedef typename SubTrait< typename DenseRow<T1,SO>::ResultType,
                              StaticVector<T2,N,true> >::Type  Type;
};

template< typename T1, size_t N, typename T2, bool SO >
struct SubTrait< StaticVector<T1,N,true>, DenseRow<T2,SO> >
{
   typedef typename SubTrait< StaticVector<T1,N,true>,
                              typename DenseRow<T2,SO>::ResultType >::Type  Type;
};

template< typename T1, bool SO, typename T2 >
struct SubTrait< DenseRow<T1,SO>, DynamicVector<T2,true> >
{
   typedef typename SubTrait< typename DenseRow<T1,SO>::ResultType,
                              DynamicVector<T2,true> >::Type  Type;
};

template< typename T1, typename T2, bool SO >
struct SubTrait< DynamicVector<T1,true>, DenseRow<T2,SO> >
{
   typedef typename SubTrait< DynamicVector<T1,true>,
                              typename DenseRow<T2,SO>::ResultType >::Type  Type;
};

template< typename T1, bool SO, typename T2 >
struct SubTrait< DenseRow<T1,SO>, CompressedVector<T2,true> >
{
   typedef typename SubTrait< typename DenseRow<T1,SO>::ResultType,
                              CompressedVector<T2,true> >::Type  Type;
};

template< typename T1, typename T2, bool SO >
struct SubTrait< CompressedVector<T1,true>, DenseRow<T2,SO> >
{
   typedef typename SubTrait< CompressedVector<T1,true>,
                              typename DenseRow<T2,SO>::ResultType >::Type  Type;
};

template< typename T1, bool SO1, typename T2, bool SO2 >
struct SubTrait< DenseRow<T1,SO1>, DenseRow<T2,SO2> >
{
   typedef typename SubTrait< typename DenseRow<T1,SO1>::ResultType,
                              typename DenseRow<T2,SO2>::ResultType >::Type  Type;
};
/*! \endcond */
//*************************************************************************************************




//=================================================================================================
//
//  MULTTRAIT SPECIALIZATIONS
//
//=================================================================================================

//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
template< typename T1, bool SO, typename T2 >
struct MultTrait< DenseRow<T1,SO>, T2 >
{
   typedef typename MultTrait< typename DenseRow<T1,SO>::ResultType, T2 >::Type  Type;
   BLAZE_CONSTRAINT_MUST_BE_NUMERIC_TYPE( T2 );
};

template< typename T1, typename T2, bool SO >
struct MultTrait< T1, DenseRow<T2,SO> >
{
   typedef typename MultTrait< T1, typename DenseRow<T2,SO>::ResultType >::Type  Type;
   BLAZE_CONSTRAINT_MUST_BE_NUMERIC_TYPE( T1 );
};

template< typename T1, bool SO, typename T2, size_t N, bool TF >
struct MultTrait< DenseRow<T1,SO>, StaticVector<T2,N,TF> >
{
   typedef typename MultTrait< typename DenseRow<T1,SO>::ResultType,
                               StaticVector<T2,N,TF> >::Type  Type;
};

template< typename T1, size_t N, bool TF, typename T2, bool SO >
struct MultTrait< StaticVector<T1,N,TF>, DenseRow<T2,SO> >
{
   typedef typename MultTrait< StaticVector<T1,N,TF>,
                               typename DenseRow<T2,SO>::ResultType >::Type  Type;
};

template< typename T1, bool SO, typename T2, bool TF >
struct MultTrait< DenseRow<T1,SO>, DynamicVector<T2,TF> >
{
   typedef typename MultTrait< typename DenseRow<T1,SO>::ResultType,
                               DynamicVector<T2,TF> >::Type  Type;
};

template< typename T1, bool TF, typename T2, bool SO >
struct MultTrait< DynamicVector<T1,TF>, DenseRow<T2,SO> >
{
   typedef typename MultTrait< DynamicVector<T1,TF>,
                               typename DenseRow<T2,SO>::ResultType >::Type  Type;
};

template< typename T1, bool SO, typename T2, bool TF >
struct MultTrait< DenseRow<T1,SO>, CompressedVector<T2,TF> >
{
   typedef typename MultTrait< typename DenseRow<T1,SO>::ResultType,
                               CompressedVector<T2,TF> >::Type  Type;
};

template< typename T1, bool TF, typename T2, bool SO >
struct MultTrait< CompressedVector<T1,TF>, DenseRow<T2,SO> >
{
   typedef typename MultTrait< CompressedVector<T1,TF>,
                               typename DenseRow<T2,SO>::ResultType >::Type  Type;
};

template< typename T1, bool SO1, typename T2, bool SO2 >
struct MultTrait< DenseRow<T1,SO1>, DenseRow<T2,SO2> >
{
   typedef typename MultTrait< typename DenseRow<T1,SO1>::ResultType,
                               typename DenseRow<T2,SO2>::ResultType >::Type  Type;
};

template< typename T1, bool SO1, typename T2, size_t M, size_t N, bool SO2 >
struct MultTrait< DenseRow<T1,SO1>, StaticMatrix<T2,M,N,SO2> >
{
   typedef typename MultTrait< typename DenseRow<T1,SO1>::ResultType,
                               StaticMatrix<T2,M,N,SO2> >::Type  Type;
};

template< typename T1, bool SO1, typename T2, bool SO2 >
struct MultTrait< DenseRow<T1,SO1>, DynamicMatrix<T2,SO2> >
{
   typedef typename MultTrait< typename DenseRow<T1,SO1>::ResultType,
                               DynamicMatrix<T2,SO2> >::Type  Type;
};

template< typename T1, bool SO1, typename T2, bool SO2 >
struct MultTrait< DenseRow<T1,SO1>, CompressedMatrix<T2,SO2> >
{
   typedef typename MultTrait< typename DenseRow<T1,SO1>::ResultType,
                               CompressedMatrix<T2,SO2> >::Type  Type;
};
/*! \endcond */
//*************************************************************************************************




//=================================================================================================
//
//  DIVTRAIT SPECIALIZATIONS
//
//=================================================================================================

//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
template< typename T1, bool SO, typename T2 >
struct DivTrait< DenseRow<T1,SO>, T2 >
{
   typedef typename DivTrait< typename DenseRow<T1,SO>::ResultType, T2 >::Type  Type;
   BLAZE_CONSTRAINT_MUST_BE_NUMERIC_TYPE( T2 );
};
/*! \endcond */
//*************************************************************************************************

} // namespace blaze

#endif