//=================================================================================================
/*!
//  \file src/mathtest/smatsmatmult/UCaUCa.cpp
//  \brief Source file for the UCaUCa sparse matrix/sparse matrix multiplication math test
//
//  Copyright (C) 2013 Klaus Iglberger - All Rights Reserved
//
//  This file is part of the Blaze library. You can redistribute it and/or modify it under
//  the terms of the New (Revised) BSD License. Redistribution and use in source and binary
//  forms, with or without modification, are permitted provided that the following conditions
//  are met:
//
//  1. Redistributions of source code must retain the above copyright notice, this list of
//     conditions and the following disclaimer.
//  2. Redistributions in binary form must reproduce the above copyright notice, this list
//     of conditions and the following disclaimer in the documentation and/or other materials
//     provided with the distribution.
//  3. Neither the names of the Blaze development group nor the names of its contributors
//     may be used to endorse or promote products derived from this software without specific
//     prior written permission.
//
//  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY
//  EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
//  OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
//  SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
//  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
//  TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
//  BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
//  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
//  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
//  DAMAGE.
*/
//=================================================================================================


//*************************************************************************************************
// Includes
//*************************************************************************************************

#include <cstdlib>
#include <iostream>
#include <blaze/math/CompressedMatrix.h>
#include <blaze/math/UpperMatrix.h>
#include <blazetest/mathtest/Creator.h>
#include <blazetest/mathtest/smatsmatmult/OperationTest.h>
#include <blazetest/system/MathTest.h>


//=================================================================================================
//
//  MAIN FUNCTION
//
//=================================================================================================

//*************************************************************************************************
int main()
{
   std::cout << "   Running 'UCaUCa'..." << std::endl;

   using blazetest::mathtest::TypeA;

   try
   {
      // Matrix type definitions
      typedef blaze::UpperMatrix< blaze::CompressedMatrix<TypeA> >  UCa;

      // Creator type definitions
      typedef blazetest::Creator<UCa>  CUCa;

      // Running tests with small matrices
      for( size_t i=0UL; i<=6UL; ++i ) {
         RUN_SMATSMATMULT_OPERATION_TEST( CUCa( i,     0UL ), CUCa( i,     0UL ) );
         RUN_SMATSMATMULT_OPERATION_TEST( CUCa( i,     0UL ), CUCa( i, 0.2*i*i ) );
         RUN_SMATSMATMULT_OPERATION_TEST( CUCa( i,     0UL ), CUCa( i, 0.5*i*i ) );
         RUN_SMATSMATMULT_OPERATION_TEST( CUCa( i, 0.2*i*i ), CUCa( i,     0UL ) );
         RUN_SMATSMATMULT_OPERATION_TEST( CUCa( i, 0.2*i*i ), CUCa( i, 0.2*i*i ) );
         RUN_SMATSMATMULT_OPERATION_TEST( CUCa( i, 0.2*i*i ), CUCa( i, 0.5*i*i ) );
         RUN_SMATSMATMULT_OPERATION_TEST( CUCa( i, 0.5*i*i ), CUCa( i,     0UL ) );
         RUN_SMATSMATMULT_OPERATION_TEST( CUCa( i, 0.5*i*i ), CUCa( i, 0.2*i*i ) );
         RUN_SMATSMATMULT_OPERATION_TEST( CUCa( i, 0.5*i*i ), CUCa( i, 0.5*i*i ) );
      }

      // Running tests with large matrices
      RUN_SMATSMATMULT_OPERATION_TEST( CUCa( 15UL,  7UL ), CUCa( 15UL,  7UL ) );
      RUN_SMATSMATMULT_OPERATION_TEST( CUCa( 37UL,  7UL ), CUCa( 37UL,  7UL ) );
      RUN_SMATSMATMULT_OPERATION_TEST( CUCa( 63UL,  7UL ), CUCa( 63UL,  7UL ) );
      RUN_SMATSMATMULT_OPERATION_TEST( CUCa( 16UL,  8UL ), CUCa( 16UL,  8UL ) );
      RUN_SMATSMATMULT_OPERATION_TEST( CUCa( 32UL,  8UL ), CUCa( 32UL,  8UL ) );
      RUN_SMATSMATMULT_OPERATION_TEST( CUCa( 64UL, 16UL ), CUCa( 64UL, 16UL ) );
   }
   catch( std::exception& ex ) {
      std::cerr << "\n\n ERROR DETECTED during sparse matrix/sparse matrix multiplication:\n"
                << ex.what() << "\n";
      return EXIT_FAILURE;
   }

   return EXIT_SUCCESS;
}
//*************************************************************************************************
