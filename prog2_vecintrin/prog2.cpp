#include "prog2_intrin.h"

// computes the absolute value of all elements in the input array
// values, stores result in output
void absSerial(float* values, float* output, int N) {
  for (int i = 0; i < N; i++) {
    float x = values[i];
    if (x < 0) {
      output[i] = -x;
    } else {
      output[i] = x;
    }
  }
}

// implementation of absSerial() above, but it is vectorized using the fake
// intrinsics
void absVector(float* values, float* output, int N) {
  __prog2_vec_float x;
  __prog2_vec_float result;
  __prog2_vec_float zero = _prog2_vset_float(0.f);
  __prog2_mask maskAll, maskIsNegative, maskIsNotNegative;

  //  Note: Take a careful look at this loop indexing.  This example
  //  code is not guaranteed to work when (N % VECTOR_WIDTH) != 0.
  //  Why is that the case?
  for (int i = 0; i < N; i += VECTOR_WIDTH) {
    // All ones
    maskAll = _prog2_init_ones();

    // All zeros
    maskIsNegative = _prog2_init_ones(0);

    // Load vector of values from contiguous memory addresses
    _prog2_vload_float(x, values + i, maskAll);  // x = values[i];

    // Set mask according to predicate
    _prog2_vlt_float(maskIsNegative, x, zero, maskAll);  // if (x < 0) {

    // Execute instruction using mask ("if" clause)
    _prog2_vsub_float(result, zero, x, maskIsNegative);  //   output[i] = -x;

    // Inverse maskIsNegative to generate "else" mask
    maskIsNotNegative = _prog2_mask_not(maskIsNegative);  // } else {

    // Execute instruction ("else" clause)
    _prog2_vload_float(result, values + i,
                       maskIsNotNegative);  //   output[i] = x; }

    // Write results back to memory
    _prog2_vstore_float(output + i, result, maskAll);
  }
}

// accepts an array of values and an array of exponents
//
// For each element, compute values[i]^exponents[i] and clamp value to
// 9.999.  Store result in output.
void clampedExpSerial(float* values, int* exponents, float* output, int N) {
  for (int i = 0; i < N; i++) {
    float x = values[i];
    int y = exponents[i];
    if (y == 0) {
      output[i] = 1.f;
    } else {
      float result = x;
      int count = y - 1;
      while (count > 0) {
        result *= x;
        count--;
      }
      if (result > 9.999999f) {
        result = 9.999999f;
      }
      output[i] = result;
    }
  }
}

void clampedExpVector(float* values, int* exponents, float* output, int N) {
  //
  // ECE476 STUDENTS TODO: Implement your vectorized version of
  // clampedExpSerial() here.
  //
  // Your solution should work for any value of
  // N and VECTOR_WIDTH, not just when VECTOR_WIDTH divides N
  //
  //
  __prog2_vec_float value_vec,  init_value_vec;
  __prog2_vec_int exponent_vec;
  __prog2_vec_int zero_int = _prog2_vset_int(0);
  __prog2_vec_int one_int = _prog2_vset_int(1);
  __prog2_vec_float one_float = _prog2_vset_float(1.f);
  __prog2_vec_float max_vel_vec = _prog2_vset_float(9.999999f);
  float max_val = 9.999999f;
  __prog2_mask maskAll, maskExponentZero, maskTemp, maskNotSaved;

  //  Note: Take a careful look at this loop indexing.  This example
  //  code is not guaranteed to work when (N % VECTOR_WIDTH) != 0.
  //  Why is that the case?
  for (int i = 0; i < N; i += VECTOR_WIDTH) {
    // All ones
    maskAll = _prog2_init_ones();

    if(i + VECTOR_WIDTH > N){
      int numExtra = N % VECTOR_WIDTH;
      i -= VECTOR_WIDTH - numExtra;
      maskAll = _prog2_init_ones(numExtra);
      maskAll = _prog2_mask_not(maskAll);
    }

    // All zeros
    maskExponentZero = _prog2_init_ones(0);

    // Load vector of values from contiguous memory addresses
    _prog2_vload_float(value_vec, values + i, maskAll);  // value_vec = values[i];
    _prog2_vmove_float(init_value_vec, value_vec, maskAll);

    // Load vector of values from contiguous memory addresses
    _prog2_vload_int(exponent_vec, exponents + i, maskAll);  // exponent_vec = values[i];

    // set mask according to if exponent is initially zero // if (exponent_vec == 0) {
    _prog2_veq_int(maskExponentZero, exponent_vec, zero_int, maskAll);

    // Execute instruction using mask ("if" clause)
    _prog2_vstore_float(output + i, one_float, maskExponentZero);

    // Inverse maskExponentZero to generate "else" mask
    maskTemp = _prog2_mask_not(maskExponentZero);
    maskTemp = _prog2_mask_and(maskTemp, maskAll);
    maskNotSaved = maskTemp;  // } else {

    // decrement exponents
    _prog2_vsub_int(exponent_vec, exponent_vec, one_int, maskTemp); // exponent_vec -= 1;

    // recompute remaining mask
    _prog2_vlt_int(maskTemp, zero_int, exponent_vec, maskNotSaved);

    while(_prog2_cntbits(maskTemp)>0){ // while (some exponent remaining){
      // multiply values by themselves
      _prog2_vmult_float(value_vec, value_vec, init_value_vec, maskTemp); // value_vec *= value_vec;

      // decrement exponents counter
      _prog2_vsub_int(exponent_vec, exponent_vec, one_int, maskTemp); // exponent_vec -= 1;

      // recompute remaining mask
      _prog2_vlt_int(maskTemp, zero_int, exponent_vec, maskTemp);
    } // }


    maskTemp = maskNotSaved;
    _prog2_vgt_float(maskTemp, value_vec, max_vel_vec, maskNotSaved); // if(value_vec[i] > max_vel_vec){

    _prog2_vset_float(value_vec, max_val, maskTemp); // value_vec[i] = max_vel_vec; }

    // Write results back to memory
    _prog2_vstore_float(output + i, value_vec, maskNotSaved);
  }


}

// returns the sum of all elements in values
float arraySumSerial(float* values, int N) {
  float sum = 0;
  for (int i = 0; i < N; i++) {
    sum += values[i];
  }

  return sum;
}

// returns the sum of all elements in values
// You can assume N is a multiple of VECTOR_WIDTH
// You can assume VECTOR_WIDTH is a power of 2
float arraySumVector(float* values, int N) {
  //
  // ECE476 STUDENTS TODO: Implement your vectorized version of arraySumSerial
  // here
  //
  // This is extra credit.
  __prog2_mask maskAll;
  __prog2_vec_float x;
  __prog2_vec_float running_sum = _prog2_vset_float(0.f);
  
  for (int i = 0; i < N; i += VECTOR_WIDTH) {
    // All ones
    maskAll = _prog2_init_ones();

    // Load vector of values from contiguous memory addresses
    _prog2_vload_float(x, values + i, maskAll);  // x = values[i];

    // add from vec to running sum
    _prog2_vadd_float(running_sum, running_sum, x, maskAll);
  }

  for(int width = VECTOR_WIDTH; width > 1; width /= 2){
    _prog2_hadd_float(running_sum, running_sum);
    _prog2_interleave_float(running_sum, running_sum);
  }

  float sum = 0;
  __prog2_mask maskOne = _prog2_init_ones(1);

  _prog2_vstore_float(&sum, running_sum, maskOne);

  return sum;
}