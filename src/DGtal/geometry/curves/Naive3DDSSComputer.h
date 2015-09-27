/**
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as
 *  published by the Free Software Foundation, either version 3 of the
 *  License, or  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 **/

#pragma once

/**
 * @file Naive3DDSSComputer.h
 * @author Kacper Pluta (\c kacper.pluta@esiee.fr )
 * Laboratoire d'Informatique Gaspard-Monge - LIGM, A3SI, France
 *
 * @date 2014/10/07
 *
 * Header file for module Naive3DDSSComputer.cpp
 *
 * This file is part of the DGtal library.
 */

#if defined(Naive3DDSSComputer_RECURSES)
#error Recursive header files inclusion detected in Naive3DDSSComputer.h
#else // defined(Naive3DDSSComputer_RECURSES)
/** Prevents recursive inclusion of headers. */
#define Naive3DDSSComputer_RECURSES

#if !defined Naive3DDSSComputer_h
/** Prevents repeated inclusion of headers. */
#define Naive3DDSSComputer_h

//////////////////////////////////////////////////////////////////////////////
// Inclusions
#include <iostream>
#include <list>
#include "DGtal/base/Exceptions.h"
#include "DGtal/base/Common.h"
#include "DGtal/kernel/PointVector.h"
#include "DGtal/kernel/CInteger.h"
#include "DGtal/geometry/curves/ArithmeticalDSSComputer.h"
#include "DGtal/base/ConstIteratorAdapter.h"
#include "DGtal/kernel/BasicPointFunctors.h"
//////////////////////////////////////////////////////////////////////////////

namespace DGtal
{

/////////////////////////////////////////////////////////////////////////////
// class Naive3DDSSComputer
/**
 * Description of class 'Naive3DDSSComputer' model of CForwardSegmentComputer <p>
 * \brief Aim:
 * Dynamic recognition of a 3d-digital straight segment (DSS)

 */

/////////////////////////////////////////////////////////////////////////////
// class ArithmeticalDSSComputer
/**
 * \brief Aim: This class is a wrapper around ArithmeticalDSSComputer that is devoted 
 * to the dynamic recognition of digital straight segments (DSS) along any 
 * sequence of digital points.
 *
 * In basis algorithm project 3d curve onto three orthogonal planes. Then each projection
 * is segmented by using ArithmeticalDSSComputer as long as at least two segmentation
 * for 2d planes are valid. By valid we understand there is no at least two 3d points which
 * have same projection onto one of orthogonal 2d planes.
 *
 * @tparam TIterator type of iterator on 3d digital points,
 * readable and forward. 
 * @tparam TInteger type of integers used for the computation of remainders,
 * which is a model of CInteger.
 * @tparam adjacency an unsigned integer equal to 4 for standard 
 * (simply 4-connected) DSS or 8 for naive (simply 8-connected) DSS (default).
 * Corresponds to adjacency of 2d projections (see above). Notice that choosing 8-adjacency for
 * each 2d projection onto orthogonal planes means that 3d curve is 26-connected. While 4-adjacency
 * means that 3d curve is 6-connected.
 *
 * This class is a model of CForwardSegmentComputer. 
 * It is also default constructible, copy constructible, assignable and equality comparable.
 *
 * @see exampleNaive3DDSSComputer.cpp
 */

template <typename TIterator, typename TInteger, int connectivity = 8>
class Naive3DDSSComputer
{
    // ----------------------- Types ------------------------------
  public:
    //entier
    BOOST_CONCEPT_ASSERT(( concepts::CInteger<TInteger> ) );
    typedef TInteger Integer;


    //requiered types
    typedef TIterator ConstIterator;
    typedef Naive3DDSSComputer<ConstIterator,TInteger,connectivity> Self;
    typedef Naive3DDSSComputer<ReverseIterator<ConstIterator>,TInteger,connectivity> Reverse;


    //points and vectors
    typedef typename IteratorCirculatorTraits<ConstIterator>::Value Point3d;
    typedef typename IteratorCirculatorTraits<ConstIterator>::Value Vector3d;
    typedef typename Point3d::Coordinate Coordinate;

    typedef DGtal::PointVector<2,Coordinate> Point2d;
    typedef DGtal::PointVector<2,Coordinate> Vector2d;

    typedef DGtal::PointVector<2,double> PointD2d;
    typedef DGtal::PointVector<3,double> PointD3d;
    typedef DGtal::PointVector<3,double> VectorD3d;

    // adapters for iterator
    typedef functors::Projector<SpaceND<2,Coordinate> > Projector2d;

    typedef ConstIteratorAdapter<ConstIterator,Projector2d,Point2d> IteratorAdapter;


    //2d-arithmeticalDSS recognition algorithm
    typedef DGtal::ArithmeticalDSSComputer<IteratorAdapter,TInteger,connectivity> ArithmeticalDSSComputer2d;

    // ----------------------- Standard services ------------------------------
  public:


    /**
     * Default constructor.
     * not valid
     */
    Naive3DDSSComputer();

    /**
     * Constructor with initialisation
     * @param it an iterator
     * @see init
     */
    Naive3DDSSComputer ( const ConstIterator& it );

    /**
     * Initialisation.
     * @param it an iterator
     */
    void init ( const ConstIterator& it );


    /**
     * Copy constructor.
     * @param other the object to clone.
     */
    Naive3DDSSComputer ( const Naive3DDSSComputer & other );

    /**
     * Assignment.
     * @param other the object to copy.
     * @return a reference on 'this'.
     */
    Naive3DDSSComputer & operator= ( const Naive3DDSSComputer & other );

    /**
     * @return a default-constructed instance of Self.
     */
    Self getSelf() const;

    /**
     * @return a default-constructed instance of Reverse.
     */
    Reverse getReverse() const;

    /**
     * Checks whether a point belongs to the DSS or not
     * @param aPoint the point to be checked
     * @return 'true' if yes, 'false' otherwise
     */
    bool isInDSS ( const Point3d& aPoint ) const;

    /**
     * Checks whether a point belongs to the DSS or not
     * @param it an iterator on the point to be checked
     * @return 'true' if yes, 'false' otherwise
     */
    bool isInDSS ( const ConstIterator & it ) const;

    /**
     * Equality operator.
     * @param other the object to compare with.
     * @return 'true' either if the leaning points perfectly match
     * or if the first leaning points match to the last ones
     * (same DSS scanned in the reverse way)
     * and 'false' otherwise
     */
    bool operator== ( const Naive3DDSSComputer & other ) const;

    /**
     * Difference operator.
     * @param other the object to compare with.
     * @return 'false' if equal
     * 'true' otherwise
     */
    bool operator!= ( const Naive3DDSSComputer & other ) const;

    /**
     * Destructor.
     */
    ~Naive3DDSSComputer(){}

    // ----------------------- Interface --------------------------------------
  public:


    /**
     * Tests whether the current DSS can be extended at the front.
     * Computes the parameters of the extended DSS if yes
     * and adds the point to the current DSS in this case.
     * @return 'true' if yes, 'false' otherwise.
     */
    bool extendFront();


    /**
     * Tests whether the 3d DSS can be extended at the front.
     *
     * @return 'true' if yes, 'false' otherwise
     */
    bool isExtendableFront();

    // ------------------------- Accessors ------------------------------

    /**
     * Computes the parameters
     * (direction, intercept, thickness)
     * of the DSS
     * @param direction direction
     * @param intercept intercept
     * @param thickness thickness
     */
    void getParameters ( Vector3d& direction, PointD3d& intercept, PointD3d& thickness ) const;

    /**
     * Checks the validity/consistency of the object.
     * @return 'true' if the object is valid, 'false' otherwise.
     */
    bool isValid() const;

    /**
     *
     * @return begin iterator of the 3d DSS range.
     */
    ConstIterator begin() const;
    /**
     * @return end iterator of the 3d DSS range.
     */
    ConstIterator end() const;

    /**
       @return a const-reference on the arithmetical DSS recognition
       algorithm along the XY plane.
    */
    const ArithmeticalDSSComputer2d & arithmeticalDSS2dXY () const;

    /**
       @return a const-reference on the arithmetical DSS recognition
       algorithm along the XZ plane.
    */
    const ArithmeticalDSSComputer2d & arithmeticalDSS2dXZ () const;

    /**
       @return a const-reference on the arithmetical DSS recognition
       algorithm along the YZ plane.
    */
    const ArithmeticalDSSComputer2d & arithmeticalDSS2dYZ () const;

    /**
       @param i the axis orthogonal to the plane
       @return a const-reference on the arithmetical DSS recognition
       algorithm along the plane orthogonal to the \a i-th axis.
    */
    const ArithmeticalDSSComputer2d & arithmeticalDSS2d( Dimension i ) const;
    
    /**
       @ param i the axis orthogonal to the plane
       @return true if given 2D DSS along orthogonal axis is valid
     */
    bool validArithmeticalDSS2d ( Dimension i ) const;


    // ------------------ Display ------------------------------------------
  public:

    /**
     * @return the style name used for drawing this object.
     */
    std::string className() const;

    /**
     * Writes/Displays the object on an output stream.
     * @param out the output stream where the object is written.
     */
    void selfDisplay ( std::ostream & out ) ;

    // ------------------------- Hidden services ------------------------------
  private:
    /**
     * Tests whether the current 2d-DSS can be extended at the front.
     * Computes the parameters of the extended 2d-DSS if yes
     * and adds the point to the current 2d-DSS in this case.
     * Used internally to simplify extendFront().
     * @param DSS2D reference to 2d-DSSComputer
     * @param blocked reference to status of DSS2D
     * updated if DSS2D cannot be extended at the front.
     * @return 'true' if yes, 'false' otherwise.
     */
    bool extendFront ( ArithmeticalDSSComputer2d & DSS2D, bool & blocked );

    // ------------------------- Protected Datas ------------------------------
  protected:

    /// projectors
    Projector2d myProjXY, myProjXZ, myProjYZ;

    /// 2d-arithmeticalDSS recognition algorithms
    ArithmeticalDSSComputer2d myXYalgo, myXZalgo,myYZalgo;
    
    /**
     * Used internally to store information which 2d-arithemticalDSS
     * should not be any more extended. This happened when two successive 3D points
     * have same projections onto respective 2d plane.
     */
    bool blockXY, blockXZ, blockYZ;
    
    /// begin and end iterators
    ConstIterator myBegin, myEnd;
}; // end of class Naive3DDSSComputer

} // namespace DGtal


///////////////////////////////////////////////////////////////////////////////
// Includes inline functions.
#if !defined(BUILD_INLINE)
#include "DGtal/geometry/curves/Naive3DDSSComputer.ih"
#endif


//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#endif // !defined Naive3DDSSComputer_h

#undef Naive3DDSSComputer_RECURSES
#endif // else defined(Naive3DDSSComputer_RECURSES)
