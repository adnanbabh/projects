//------------------------------------------------------------------------------
// ass3.c
//
// Program for EP Assignment 3 WS18 - QR Code generator
//
// Group: Group 20065, study assistant 
//
// Authors: Adnan Babahmetovic 01634163
// Daniel Livingston 
//------------------------------------------------------------------------------
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <inttypes.h>
#include "qrc_ecc.h"
#define BUFFER 100
#define MASK_ID 1
#define MASK 5
#define BIT_MASK 3
#define NON_BIT_MASK 4

#define WRONG_ARGUMENTS "Usage: ./ass3 [-b FILENAME]\n"
#define OUT_OF_MEMORY "[ERR] Out of memory.\n"
#define ERROR_TOO_LONG "[ERR] Text to encode is too long, max. "\
                       "106 bytes can be encoded.\n"
#define WRONG_PARAMETERS "[ERR] Function from errorcorrection library "\
                         "called with wrong parameters.\n"
#define WRITE_ERROR "[ERR] Could not write file [output-filename].\n"

//------------------------------------------------------------------------------
///
/// Struct containing the QR info
//
struct _QRParameters_
{
  int version_;
  int error_correction_level_;
  size_t number_of_error_correction_code_words_;
  int character_capacity_;
  size_t total_length_;
};

//------------------------------------------------------------------------------
///
/// Enum containing all the error return values.
//
enum _Errors_
{
  ERROR_TOO_MANY_ARGUMENTS = 1,
  ERROR_OUT_OF_MEMORY = 2,
  ERROR_TEXT_TOO_LONG = 3,
  ERROR_WRONG_PARAMETERS = 4,
  ERROR_FILE = 5
};

//------------------------------------------------------------------------------
///
/// Function for printing the error messages if one is encountered. Returns
/// an int value of error.
///
/// @param error Return value of the encountered error
//
int handleError(int error)
{
  switch (error)
  {
   case 1:
     printf(WRONG_ARGUMENTS);
     break;
   case 2:
     printf(OUT_OF_MEMORY);
     break;
   case 3:
     printf(ERROR_TOO_LONG);
     break;
   case 4:
     printf(WRONG_PARAMETERS);
     break;
   case 5:
     printf(WRITE_ERROR);
     break;
  }
  return error;
}

//------------------------------------------------------------------------------
///
/// Compares the return from the qrc_ecc.h library and setes the error to the 
/// according value. 
///
/// @param error Return value
//
int checkErrorCorrectionLibraryError(int error)
{
  if (error == ERROR_CORRECTION_ERROR_OUT_OF_MEMORY)
    error = ERROR_OUT_OF_MEMORY;
  if (error == ERROR_CORRECTION_ERROR_INVALID_PARAMETER)
    error = ERROR_WRONG_PARAMETERS;

  return error;
}

//------------------------------------------------------------------------------
///
/// Function that is used for the input of characters from stdin. Implemented 
/// with the fgetc() library function. Uses a temporary array (input_string)
/// and reallocates enough memory when needed (step of 100). After '\n' or EOF
/// is reached it stops reading, prints the message and its length and 
/// accordingly (it's length is NOT >106 bytes) saves the string into an
/// array which was previously allocated with enough memory. 
/// 
/// @param buffer Adress of a pointer which will be allocated with enough memory
///         and contain the final string after all the checks are done
/// 
/// @return zero when successful, else error
//
int inputString(char **buffer)
{
  char *input_string;
  unsigned int max_length = 100, size = 100;
  int character = EOF;
  int position = 0;
  int length;

  *buffer = malloc(1);
  if (buffer == NULL)
  {
    free(buffer);
    return ERROR_OUT_OF_MEMORY;
  }
  

  printf("--- QR-Code Encoder ---\n\nPlease enter a text:\n");
  input_string = malloc (sizeof(char)*BUFFER);

  if (input_string != NULL)
  {
    while((character = fgetc(stdin)) != '\n' && character != EOF && !feof(stdin))
    {
      input_string[position++] = character;

      if (position == size)
      {
        size = position + max_length;
        void *temporary = (char*)realloc(input_string, size);
        if (temporary == NULL)
        {
          free(input_string);
          return ERROR_OUT_OF_MEMORY;
        }
        input_string = temporary;
      }
    }
    input_string[position] = '\0';
  }
  else
  {
    return ERROR_OUT_OF_MEMORY;
  }

  length = strlen(input_string);

  printf("\nMessage: %s\nLength: %d\n\n", input_string, length);

  if (length > 106)
  {
    free(input_string);
    return ERROR_TEXT_TOO_LONG;
  }


  *buffer = realloc(*buffer, size);
  if (buffer == NULL)
  {
    free(buffer);
    return ERROR_OUT_OF_MEMORY;
  }

  strcpy(*buffer, input_string);

  free(input_string);
  input_string = NULL;

  return 0;
}

//------------------------------------------------------------------------------
///
/// Reads the length of the input and returns the QR version accordingly. First
/// sets the range for the version, then the range for error correction level
/// and sets all the parameters of the struct to the values of that version. 
///
/// @param input Input characters
///
/// @return the QR parameters
//
struct _QRParameters_ Check(char *input)
{
  struct  _QRParameters_ QRParameters;
  int length;
  size_t version_number, error_correction, error_data, capacity;

  length = strlen(input);

  if (length <= 17)
  {
    version_number = 1;
    if (length <= 7)
      error_correction = 3, error_data = 17, capacity = 7;
    if (length > 7 && length <= 11)
      error_correction = 2, error_data = 13, capacity = 11;
    if (length > 11 && length <= 14)
      error_correction = 1, error_data = 10, capacity = 14;
    if (length > 14)
      error_correction = 0, error_data = 7, capacity = 17;
  }

  if (length > 17 && length <= 32)
  {
    version_number = 2;
    if (length > 17 && length <= 20)
      error_correction = 2, error_data = 22, capacity = 20;
    if (length > 20 && length <= 26)
      error_correction = 1, error_data = 16, capacity = 26;
    if (length > 26)
      error_correction = 0, error_data = 10, capacity = 32;
  }

  if (length > 32 && length <= 53)
  {
    version_number = 3;
    if (length > 32 && length <= 42)
      error_correction = 1, error_data = 26, capacity = 42;
    if (length > 42)
      error_correction = 0, error_data = 15, capacity = 53;
  }

  if (length > 53 && length <= 78)
    version_number = 4, error_correction = 0, error_data = 20, capacity = 78;
  if (length > 78)
    version_number = 5, error_correction = 0, error_data = 26, capacity = 106;

  QRParameters.version_ = version_number;
  QRParameters.error_correction_level_ = error_correction;
  QRParameters.number_of_error_correction_code_words_ = error_data;
  QRParameters.character_capacity_ = capacity;
  QRParameters.total_length_ = capacity + 2;

  return QRParameters;
}

//-----------------------------------------------------------------------------
///
/// Simple function for printing the QR info.
///
/// @param version The version of the QR
/// @param error_correction_level Error correction level 
//
void printQRInfo(int version, int error_correction_level)
{
  char characters[4] = {'L', 'M', 'Q', 'H'};

  printf("QR-Code: %d-%c\n\n", version, characters[error_correction_level]);

}

//------------------------------------------------------------------------------
///
/// Takes the input string and shifts the whole array by 4 bits to the right;
/// sets the needed bits accordingly; 0100 is set at the beginning; number of 
/// characters comes next in a 8bit form; the message codewords are next and 
/// four 0 bits at the end. Rest of the array is filled accordingly with the 
/// padding. In the end the array is copied into the message array.
///
/// @param input Inputed string of chars
/// @param character_capacity Used to determine the final length of the shifter
///                           array
/// @param message Adress of the array where the shifter array is to be copied
///
/// @return zero
//
int messageGenerate(char *input, int character_capacity, uint8_t **message)
{
  int new_length = character_capacity + 2;
  uint8_t *buffer = (uint8_t*)malloc(new_length);

  if (buffer == NULL)
  {
    free(buffer);
    free(message);
    return ERROR_OUT_OF_MEMORY;
  }

  *message = malloc(1);
  if (message == NULL)
  {
    free(buffer);
    free(message);
    return ERROR_OUT_OF_MEMORY;
  }

  for (int counter = 0; counter < new_length; counter++)
  	buffer[counter] = 0;

  buffer[0] = strlen(input);

  for (int  counter = 1; counter <= strlen(input); counter++)
    buffer[counter] = input[counter - 1];

  uint8_t *shift = buffer + (new_length - 1);
  
  while (shift > buffer)
  {
    *(shift) = (*(shift) & 0xF0) >> 4 | (*(shift - 1) & 0x0F) << 4;
    shift--;
  }
  *buffer = (*buffer & 0xF0) >> 4;

  buffer[0] |= 1 << 6;

  for (int counter = strlen(input) + 2; counter < new_length; counter++)
  {
    if (buffer[counter - 1] == 236)
      buffer[counter] = 17;
    else
      buffer[counter] = 236;
  }

  *message = realloc(*message, new_length);

  if (message == NULL)
  {
    free(message);
    free(buffer);
    return ERROR_OUT_OF_MEMORY;
  }

  memmove(*message, buffer, new_length);

  free(buffer);

  return 0;
}

//------------------------------------------------------------------------------
///
/// Prints the array in a form of hexes.
///
/// @param array Array to be printed
/// @param length Length of the array
/// @param counter Basic iterator
//
void printArray(uint8_t *array, int length)
{
  for (int counter = 0; counter < length - 1; counter++)
  printf("0x%02X, ", array[counter]);

  printf("0x%02X", array[length - 1]);
  printf("\n");
}

//------------------------------------------------------------------------------
///
/// A function that creates a final array from the input array and the error
/// corrected words (i.e. merges the two arrays into one).
///
/// @param message Array containing the message
/// @param corrected_words Array containing the corrected codewords
/// @param final_array Final array in which the two will be merged
/// @param new_length Final length of the new array
/// @param QRParameters Struct with the QR info
//
void mergeArrays(uint8_t *message, uint8_t *corrected_words,
                 uint8_t *final_array, int new_length, 
                 struct _QRParameters_ QRParameters)
{
  for (int counter = 0; counter < QRParameters.total_length_; counter++)
    final_array[counter] = message[counter];

  for (int counter = 0; 
       counter < QRParameters.number_of_error_correction_code_words_; counter++)
    final_array[counter + QRParameters.total_length_] = 
    	corrected_words[counter];

  printf("Data codewords:\n");
  printArray(final_array, new_length);
}

//------------------------------------------------------------------------------
///
/// This is a function that prints the matrix as a combination of "#" and " "
/// Bit 1 (in this case 1s and 3s since the 3s represent the bit 1) is printed 
/// as "# "/"#" and bit 0 (0s and 4s) as "  "/" ".
///
/// @param size
/// @param matrix[size][size]
/// @param counter_v/counter_h Vertical and horizontal iterators
//
void printMatrix(int size, int matrix[size][size])
{
  for (int counter_v = 0; counter_v < size; counter_v++)
  {
    for (int counter_h = 0; counter_h < size; counter_h++)
    {
      if (counter_h < size - 1)
      {
        if (matrix[counter_v][counter_h] == 1 || 
        		  matrix[counter_v][counter_h] == 3)
        {
          printf("# ");
        }
        else printf("  ");
      }

      if (counter_h == size - 1)
      {
       if (matrix[counter_v][counter_h] == 1 || 
       		 	 matrix[counter_v][counter_h] == 3)
          {
            printf("#");
          }
       else
         printf(" ");

      }
    }
    printf("\n");
  }
}

//------------------------------------------------------------------------------
///
/// Function that tests the bits in the message array. Works on a bitwise/
/// bitshift  principle where a bit on the particular position is "compared" 
/// with a mask and then returned.
/// 
/// @param array Array in which the bit is to be tested
/// @param bit_position Index of the bit in the array
/// @param length Length of the message array
/// @param limit Complete number of bits in the array, used to terminate the 
///              testing after it is reached and continues to return 0 bits 
///
//
int testBit(uint8_t *array, int bit_position, int length)
{
  int bit;
  int limit = 8 * length;

  if (bit_position < limit)
  {
    if ( (array[bit_position / 8] & (0x80 >> (bit_position % 8))) !=0 )
      bit = 1;
    else
      bit = 0;
  }
  else
    bit = 0;

  return bit;
}

//------------------------------------------------------------------------------
///
/// Works on the same principle as the testBit function; the difference is 
/// limit is always 15 since the number of bits is always the same in the 
/// format string (15 bits indexed from 0 to 14).
/// 
/// @param array Array in which the bit is to be tested
/// @param bit_position Index of the bit in the array
///
//
int testBitFormat(uint32_t *array, int bit_position)
{
  int bit;
  int limit = 15;

  if (bit_position < limit)
  {
    if ( (array[bit_position / 32] & (0x00000001 << (bit_position % 32))) !=0 )
      bit = 1;
    else
      bit = 0;
  }
  else
    bit = 0;

  return bit;
}

//------------------------------------------------------------------------------
///
/// Function that creates the position module matrix. 0s are encoded as 4s 
/// (NON_BIT_MASK) and 1s are encoded as 3s (BIT_MASK); this is to make the
/// XOR masking of the final matrix easier.
///
/// @param position_module 2D Matrix where the corresponding bits are filled
//
void positionModule(int position_module[7][7])
{
  for (int counter = 0; counter < 7; counter++)
  {
    position_module[0][counter] = BIT_MASK;
    position_module[counter][0] = BIT_MASK;
    position_module[6][counter] = BIT_MASK;
    position_module[counter][6] = BIT_MASK;
  }

  for (int counter = 1; counter < 6; counter++)
  {
    position_module[1][counter] = NON_BIT_MASK;
    position_module[counter][1] = NON_BIT_MASK;
    position_module[5][counter] = NON_BIT_MASK;
    position_module[counter][5] = NON_BIT_MASK;
  }

  for (int counter_v = 2; counter_v < 5; counter_v++)
  {
    for (int counter_h = 2; counter_h < 5; counter_h++)
    {
      position_module[counter_v][counter_h] = BIT_MASK;
    }
  }
}

//------------------------------------------------------------------------------
///
/// Function that creates the direction module matrix. 0s are encoded as 4s 
/// (NON_BIT_MASK) and 1s are encoded as 3s (BIT_MASK); this is to make the
/// XOR masking of the final matrix easier.
///
/// @param direction_module 2D Matrix where the corresponding bits are filled
//
void directionModule(int direction_module[5][5])
{
  for (int counter = 0; counter < 5; counter++)
  {
    direction_module[0][counter] = BIT_MASK;
    direction_module[counter][0] = BIT_MASK;
    direction_module[4][counter] = BIT_MASK;
    direction_module[counter][4] = BIT_MASK;
  }

  for (int counter = 1; counter < 4; counter++)
  {
    direction_module[1][counter] = NON_BIT_MASK;
    direction_module[counter][1] = NON_BIT_MASK;
    direction_module[3][counter] = NON_BIT_MASK;
    direction_module[counter][3] = NON_BIT_MASK;
  }

  direction_module[2][2] = BIT_MASK;
}

//------------------------------------------------------------------------------
///
/// Puts the position module into the QR matrix. The position of the module is
/// determined by the final size of the matrix, i.e. version of the qr code.
///
/// @param x_axis, y_axis - Coordinates of the position module in the matrix
/// @param size - Size of the matrix
/// @param matrix 
/// @param position_module
/// @param counter_v/counter_h  Vertical and horizontal iterators 
//
void putPositionModule(int x_axis, int y_axis, int size, int matrix[size][size],
                       int position_module[7][7])
{

  for (int counter_v = 0; counter_v < 7; counter_v++)
  {
    for (int counter_h = 0; counter_h < 7; counter_h++)
    {
      matrix[counter_v + x_axis][counter_h + y_axis] =
        position_module[counter_v][counter_h];
    }
  }
}

//------------------------------------------------------------------------------
///
/// Puts the seperation module into the QR matrix. Again; 0s are represented
/// with 4s (NON_BIT_MASK) and 1s with 3s (BIT_MASK).
///
/// @param size - Size of the matrix
/// @param matrix 
/// @param counter - Basic iterator
//
void putSeperationModule(int size, int matrix[size][size])
{
  for (int counter = 0; counter < 7; counter++)
  {
    matrix[size-8][counter] = NON_BIT_MASK;
    matrix[7][counter] = NON_BIT_MASK;
    matrix[7][counter + size - 7] = NON_BIT_MASK;
  }

  for (int counter = 0; counter < 8; counter++)
  {
    matrix[counter][size - 8] = NON_BIT_MASK;
    matrix[counter][7] = NON_BIT_MASK;
    matrix[counter + size - 8][7] = NON_BIT_MASK;
  }
}

//------------------------------------------------------------------------------
///
/// Puts the direction module into the QR matrix. Again; 0s are represented
/// with 4s (NON_BIT_MASK) and 1s with 3s (BIT_MASK).
///
/// @param size - Size of the matrix
/// @param matrix 
/// @param counter_v/counter_h  Vertical and horizontal matrix iterators 
/// @param start_coordinate Coordinate of the direction module according
///                         to the version
//
void putDirectionModule(int size, int version, int matrix[size][size],
                        int direction_module[5][5])
{
  int start_coordinate = 16 + 4 * (version - 2);

  for (int counter_v = 0; counter_v < 5; counter_v++)
  {
    for (int counter_h = 0; counter_h < 5; counter_h++)
    {
      matrix[start_coordinate + counter_v][start_coordinate + counter_h] =
      	direction_module[counter_v][counter_h];
    }
  }
}

//------------------------------------------------------------------------------
///
/// Puts the synchronisation module into the QR matrix. Again; 0s are 
/// represented with 4s (NON_BIT_MASK) and 1s with 3s (BIT_MASK).
///
/// @param size - Size of the matrix
/// @param matrix 
/// @param counter_v/counter_h  Vertical and horizontal matrix iterators 
//
void putSynchroModule(int size, int matrix[size][size])
{
  for (int counter_h = 8; counter_h < size - 8; counter_h++)
  {
    if (counter_h % 2 == 0)
      matrix[6][counter_h] = BIT_MASK;
    else
      matrix[6][counter_h] = NON_BIT_MASK;
  }

  for (int counter_v = 8; counter_v < size - 8; counter_v++)
  {
    if (counter_v % 2 == 0)
      matrix[counter_v][6] = BIT_MASK;
    else
      matrix[counter_v][6] = NON_BIT_MASK;
  }
}

//------------------------------------------------------------------------------
///
/// Reserves the space for the format module string which will later be  
/// accordingly encoded.
///
/// @param size - Size of the matrix
/// @param matrix 
/// @param counter - Basic iterator
//
void reserveFormatModule(int size, int matrix[size][size])
{
  for (int counter = 0; counter < 9; counter++)
  {
    if (matrix[8][counter] != BIT_MASK)
    {
      matrix[8][counter] = NON_BIT_MASK;
    }
    if (counter < 8)
      matrix[8][counter + size - 8] = NON_BIT_MASK;
  }

  for (int counter = 0; counter < 8; counter++)
  {
    if (matrix[counter][8] != BIT_MASK)
      matrix[counter][8] = NON_BIT_MASK;
    if (counter < 7)
      matrix[counter + 1 + size - 8][8] = NON_BIT_MASK;
  }
}

//------------------------------------------------------------------------------
///
/// Function that encodes the message codewords into the matrix. It works by
/// going double-column by double-column from right to left - from bottom to top
/// and from top to bottom. It checks if the bit is masked with a 5 (free
/// space) and accordingly sets the bit by calling the testBit function.
///
/// @param size - Size of the matrix
/// @param matrix
/// @param array - Array with the codewords/bits to be encoded
/// @param shifter - Shifts the printing by one column to the left when the
///                  seperation module is encountered.
/// @param bit_position - Bit counter which is needed for calling the testBit
/// @param counter - Counter the goes from right to left of the matrix
///                      by a step of 4 columns.
/// @param holder_one/holder_two - Iterators that go from down to top, and from
///                              top to bottom of the matrix, respectively
//
void putMatrixBits(int size, int matrix[size][size], uint8_t *array, int length)
{
  int holder_one = size;
  int shifter = 0;
  int bit_position = 0;
  int bit;


  for (int counter = 0; counter < size - 4; counter += 4)
  {
    for (holder_one = size - 1; holder_one >= 0; holder_one--)
    {
      if (matrix[holder_one][size - counter - shifter -  1] == 5)
      {
        bit = testBit(array, bit_position, length);
        matrix[holder_one][size - counter - shifter -  1] = bit;
        bit_position++;
      }
      if (matrix[holder_one][size - counter - shifter -  2] == 5)
      {
      	bit = testBit(array, bit_position, length);
      	matrix[holder_one][size - counter - shifter -  2] = bit;
        bit_position++;
      }
    }

    if (counter == (size - 9))
      shifter = 1;

    for (int holder_two = 0; holder_two < size; holder_two++)
    {
      if (matrix[holder_two][size - counter - shifter - 3] == 5)
      {
        bit = testBit(array, bit_position, length);
        matrix[holder_two][size - counter - shifter - 3] = bit;
        bit_position++;
      }
      if (matrix[holder_two][size - counter - shifter - 4] == 5)
      {
        bit = testBit(array, bit_position, length);
        matrix[holder_two][size- counter - shifter - 4] = bit;
        bit_position++;
      }
    }
  }
}

//------------------------------------------------------------------------------
///
/// Creates the mask matrix for the XOR masking of the final matrix. In our
/// case the mask ID is = 1 (20065%2 = 1).
///
/// @param size - Size of the matrix
/// @param matrix - Mask matrix
/// @param counter_v/counter_h - Vertical and horizontal iterators
//
void maskMatrix(int size, int matrix[size][size])
{
  int mask_matrix[size][size];

  for ( int counter_v = 0; counter_v < size; counter_v++)
  {
    for (int counter_h = 0; counter_h < size; counter_h++)
    {
      if ( counter_v % 2 == 0 )
        mask_matrix[counter_v][counter_h] = 1;
      else
        mask_matrix[counter_v][counter_h] = 0;
    }
  }

  for ( int counter_v = 0; counter_v < size; counter_v++)
  {
    for (int counter_h = 0; counter_h < size; counter_h++)
    {
      if (matrix[counter_v][counter_h] == 0 ||
          	matrix[counter_v][counter_h] == 1)
      {
        matrix[counter_v][counter_h] =
        	matrix[counter_v][counter_h] ^ mask_matrix[counter_v][counter_h];
      }

    }
  }
}

//------------------------------------------------------------------------------
///
/// Encodes the format string into the reserved space for the format module.
/// To test the bit in the format string the function testBitFormat is called
/// with the right parameters.
///
/// @param size Size of the matrix
/// @param matrix
/// @param format_string Array containing the format string
/// @param counter  Basic iterator
/// @param bit_position Bit "index" that is given to the function testFormatBit
//
void putFormatString(int size, int matrix[size][size], uint32_t *format_string)
{
  int bit_position = 0;

  for (int counter = 0; counter < 16; counter++)
  {
    if ((matrix[counter][8] != BIT_MASK) && (counter<9))
    {
      matrix[counter][8] = testBitFormat(format_string, bit_position);
      bit_position++;

    }
    if ((counter > 8) && (matrix[counter][8] != BIT_MASK))
    {
      matrix[counter + size - 16][8] =
      	testBitFormat(format_string, bit_position);
      bit_position++;
    }
  }

  bit_position = 0;

  for (int counter = 1; counter < 17; counter++)
  {
    if ((counter < 9))
    {
      matrix[8][size - counter] = testBitFormat(format_string, bit_position);
      bit_position++;

    }
    if ((counter > 8) && (matrix[8][size - counter - 9] != BIT_MASK))
    {
      matrix[8][size - counter - 9 ] =
      	testBitFormat(format_string, bit_position);
      bit_position++;
    }
  }
}

//------------------------------------------------------------------------------
///
/// Function that encodes all the data in matrices and prints the data matrix
/// and the final matrix, respectively. First the base matrix is masked (in our
/// case with 5s), then encoded with all the necessary modules (again masked,
/// where 0s are represented with 4s and 1s with 3s - this will make the XOR
/// masking of the final matrix easier). Data is encoded, the matrix is masked
/// and the final matrix is printed.
///
/// @param version Used QR-Code version
/// @param array The array which contains all the data which is to be encoded
/// @param format_string Array with the corresponding format string
/// @param length Number of bytes in the final message array
//
int generateMatrices(int version, uint8_t *array, uint32_t *format_string, 
										 	 int length)
{
  int size = (21 + 4 * (version - 1));
  int data_matrix[size][size];
  int position_module[7][7];
  int direction_mdule[5][5];

  positionModule(position_module);
  directionModule(direction_mdule);

  for (int counter_v = 0; counter_v < size; counter_v++)
  {
    for (int counter_h = 0; counter_h < size; counter_h++)
    {
      data_matrix[counter_v][counter_h] = MASK;
    }
  }

  putPositionModule(0, 0, size, data_matrix, position_module);
  putPositionModule(0, size - 7, size, data_matrix, position_module);
  putPositionModule(size - 7, 0, size, data_matrix, position_module);
  putSeperationModule(size, data_matrix);

  if (version != 1)
    putDirectionModule(size, version, data_matrix, direction_mdule);

  putSynchroModule(size, data_matrix);


  data_matrix[(4 * version) + 9][8] = BIT_MASK;

  reserveFormatModule(size, data_matrix);

  putMatrixBits(size, data_matrix, array, length);


  printf("Data matrix:\n");
  printMatrix(size, data_matrix);
  printf("\n");

  maskMatrix(size, data_matrix);
  putFormatString(size, data_matrix, format_string);

  printf("Mask id: %d\n", MASK_ID),
  printf("Format string: 0x%06" PRIX32 "\n\n", format_string[0]);

  printf("Final matrix:\n");
  printMatrix(size, data_matrix);

  return 0;
}

//------------------------------------------------------------------------------
///
/// Handles the whole process; returns 0 if successful and error value if not.
///
/// @param  argc:  the number of arguments at the program start (must be smaller
///                or equal to 3)
//
int main(int argc, char const *argv[])
{
  char *input = NULL;
  uint8_t *message = NULL;
  uint32_t format_string[1];
  struct _QRParameters_ QRInfo;
  int error = 0;

  if(argc > 3)
  {
     handleError(ERROR_TOO_MANY_ARGUMENTS);
     free(input);
     free(message);
     return ERROR_TOO_MANY_ARGUMENTS;
  }

  error = inputString(&input);
  if (error != 0)
  {
    handleError(error);
    free(input);
    free(message);
    return error;
  }

  QRInfo = Check(input);

  printQRInfo(QRInfo.version_, QRInfo.error_correction_level_);

  error = messageGenerate(input, QRInfo.character_capacity_, &message);
  if (error != 0)
  {
    handleError(error);
    free(input);
    free(message);
    return error;
  }

  free(input);

  printf("Message data codewords:\n");

  printArray(message, QRInfo.total_length_);

  uint8_t error_correction_code_words[
            QRInfo.number_of_error_correction_code_words_];

  error = generateErrorCorrectionCodewords(error_correction_code_words,
                         QRInfo.number_of_error_correction_code_words_, 
                                         message, QRInfo.total_length_);

  if (error!=0)
  {
    error = checkErrorCorrectionLibraryError(error);
    handleError(error);
    free(message);
    return error;
  }

  error = generateFormatString(format_string, QRInfo.version_,
                      QRInfo.error_correction_level_, MASK_ID);
  if (error!=0)
  {
    error = checkErrorCorrectionLibraryError(error);
    handleError(error);
    free(message);
    return error;
  }

  generateFormatString(format_string, QRInfo.version_, 
  										 QRInfo.error_correction_level_, MASK_ID);

  printf("Error correction codewords:\n");

  printArray(error_correction_code_words,
             QRInfo.number_of_error_correction_code_words_);

  int new_length = QRInfo.number_of_error_correction_code_words_ +
                   QRInfo.total_length_;

  uint8_t new_array[new_length];

  mergeArrays(message, error_correction_code_words, new_array, new_length, 
  						QRInfo);
  printf("\n");

  free(message);

  generateMatrices(QRInfo.version_, new_array, format_string, new_length);

  return 0;
}