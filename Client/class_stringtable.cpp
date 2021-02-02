#include "class_stringtable.hpp"

#pragma warning(disable:4996)
#pragma warning(disable:4018)

char cStringTable::nullStr[5] = "null";
char cStringTable::errorMsg[256] = "";


// ==================================================== //

cStringTable::cStringTable()
{
	
	loaded = false;
	stringCount = 0;
	usedMemory = sizeof(*this);
	
	memset(stringTable, 0, sizeof(stringTable));
		
}

// ==================================================== //
//	                                                    //
//	                                                    //
//	                                                    //
// ==================================================== //

cStringTable::~cStringTable()
{

	empty();

}

// ==================================================== //
//	                                                    //
//	                                                    //
//	                                                    //
// ==================================================== //

// Empties the string table and then loads
// data from the specified file.
// Any errors are reported in errorMsg.

void cStringTable::loadBin(const char *file)
{
	
	// Empty the string table first
	empty();

	
	// Open & check for errors
	std::ifstream infile(file, std::ios::binary);
	if (!infile) {
		
		sprintf(errorMsg, "Error opening \"%s\"", file);
		return;
	}
	
	
	char buffer[ST_MAX_STRING_SIZE];
	int index;
	
	while (1) {
		
		// Read the string and index
		if ( !readString(infile, buffer, index) )
			break;
			
			
		// If the index is invalid, the data file
		// is probably corrupt.
		if ( index < 0 || index >= ST_MAX_STRINGS ) {
			
			empty();
			sprintf(errorMsg, "Invalid index (%d)", index);
			infile.close();
			return;
		}
		
		
		// If this returns false, we're out of memory.
		// That's the only reason this may return false,
		// since we already checked the validity of the index.
		if ( !insert(index, buffer) ) {
			
			empty();
			sprintf(errorMsg, "Out of memory");
			infile.close();
			return;
		}
	}
	
	
	infile.close();
	loaded = true;
	
}

// ==================================================== //

// Helper function for loadBin().
// Reads the index and the string from the file, and
// returns true if the read was successful.

bool cStringTable::readString(std::ifstream &infile, char *buffer, int &index)
{
	
	// Read the index
	index =  infile.get();
	index |= infile.get() << 8;
	index |= infile.get() << 16;
	index |= infile.get() << 24;
	
	
	// Even if the string is empty, it has the null character
	// next to the index, so if we got eof here, the file is corrupt
	// or we tried to read from the end of the file
	if ( infile.eof() )
		return false;
	
		
		
	memset(buffer, 0, ST_MAX_STRING_SIZE);
	int pos = 0;
	char c;
	
	// Read chars from the file until a null character
	// or eof is encountered
	while (1) {
		

		c = infile.get() ^ 85;
		
		if ( infile.eof() ||  c == 0 )
			return true;
		
		// Add the read char to the buffer only if the position
		// is before the last character to avoid overflow.
		if ( pos < ST_MAX_STRING_SIZE - 1 )
			buffer[pos++] = c;
		
	}
	
	
}

// ==================================================== //
//	                                                    //
//	                                                    //
//	                                                    //
// ==================================================== //

// Write all allocated strings and their indices
// to the specified file.

bool cStringTable::saveBin(const char *file)
{
	
	std::ofstream outfile(file, std::ios::binary);
	
	if ( !outfile ) {
		
		sprintf(errorMsg, "Error opening \"%s\"", file);
		return false;
	}
	
	
	for ( int i=0; i<ST_MAX_STRINGS; i++ ) {
		
		if ( stringTable[i] ) {
			
			// Write the index
			outfile.put(i);
			outfile.put(i >> 8);
			outfile.put(i >> 16);
			outfile.put(i >> 24);
			
			// And then the string
			for ( int j=0; j<=strlen(stringTable[i]); j++ ) {
				
				// Simple xor-encryption so that the strings
				// won't be just plain text in the data file.
				outfile.put( stringTable[i][j] ^ 85 );
			}

		}
	}
	
	
	outfile.close();
	return true;
	
}

// ==================================================== //
//	                                                    //
//	                                                    //
//	                                                    //
// ==================================================== //

// Allocates the given string to the table and
// returns the index, or a negative number on error.
// -1 = No room left in the table
// -2 = Out of memory

int cStringTable::add(const char *string)
{
	
	if ( stringCount >= ST_MAX_STRINGS )
		return -1;
		
	// Go through the table and insert into the first free slot.
	for ( int i=0; i<ST_MAX_STRINGS; i++ ) {
		
		if ( !stringTable[i] ) {
			
			if ( insert(i, string) )	return i;
			else						return -2;

		}
	}
	
	return -1; // This should never be reached.
	
}

// ==================================================== //
//	                                                    //
//	                                                    //
//	                                                    //
// ==================================================== //

// Inserts the string into the index, removing whatever
// there was before.

bool cStringTable::insert(int index, const char *string)
{

	// Check that the index is valid
	if ( index < 0 || index >= ST_MAX_STRINGS )
		return false;
		
	// Remove the old string, if there is one
	remove(index);	
	
		
	// If the given string is as long as or longer than the
	// maximum size, allocate that much memory. Otherwise
	// allocate only as much as is needed.
	int memory;
	if ( strlen(string) >= ST_MAX_STRING_SIZE - 1 )
		memory = ST_MAX_STRING_SIZE;
	else
		memory = strlen(string) + 1;
		
		
	// Allocate & check
	stringTable[index] = new char[memory];
	if ( !stringTable[index] )
		return false;
	

	// Zero out & copy the string
	memset(stringTable[index], 0, memory);
	strncpy(stringTable[index], string, memory - 1);
	
	stringCount++;
	usedMemory += memory;
	
	return true;
	
}

// ==================================================== //
//	                                                    //
//	                                                    //
//	                                                    //
// ==================================================== //

// Deallocates the given index.

bool cStringTable::remove(int index)
{
	
	// Check the index first
	if ( index < 0 || index >= ST_MAX_STRINGS )
		return false;
	
		
	if ( stringTable[index] ) {

		usedMemory -= strlen(stringTable[index]) + 1;
		delete [] stringTable[index];
		stringTable[index] = NULL;
		stringCount--;
		return true;
	}
	else return false;
	
}

// ==================================================== //
//	                                                    //
//	                                                    //
//	                                                    //
// ==================================================== //

// Empties the string table.

void cStringTable::empty()
{
	
	for ( int i=0; i<ST_MAX_STRINGS; i++ ) {
		
		if ( stringTable[i] ) {
			
			delete [] stringTable[i];
			stringTable[i] = 0;
		}
	}
	
	stringCount = 0;
	memset(errorMsg, 0, sizeof(errorMsg));
	loaded = false;
	usedMemory = sizeof(*this);
	
}

// ==================================================== //
//	                                                    //
//	                                                    //
//	                                                    //
// ==================================================== //

// Returns a pointer to the string in the index, or null.

const char * cStringTable::getString(int index)
{
	
	if ( index < 0 || index >= ST_MAX_STRINGS )
		return nullStr;
		
	if ( !stringTable[index] )
		return nullStr;
	
	return stringTable[index];
}

// ==================================================== //

const char * cStringTable::operator[](int index)
{
	
	if ( index < 0 || index >= ST_MAX_STRINGS )
		return nullStr;
	
	if ( !stringTable[index] )
		return nullStr;
		
	return stringTable[index];
	
}

// ==================================================== //
//	                                                    //
//	                                                    //
//	                                                    //
// ==================================================== //

// Returns true if the given index is unallocated.

bool cStringTable::isNull(int index)
{
	
	if ( index < 0 || index >= ST_MAX_STRINGS )
		return true;
		
	if ( !stringTable[index] )
		return true;
	
	return false;
		
}

// ==================================================== //
//	                                                    //
//	                                                    //
//	                                                    //
// ==================================================== //

// Loads strings from a text file.
// Format:
// 1234 = "This is a string"

void cStringTable::loadTxt(const char *file)
{
	
	// Empty the string table first
	empty();

	
	// Open & check for errors
	std::ifstream infile(file);
	if (!infile) {
		
		sprintf(errorMsg, "Error opening \"%s\"", file);
		return;
	}
	
	
	char buffer[ST_MAX_STRING_SIZE];
	int index;
	
	while (1) {
		
		// Read the string and index
		if ( !readLine(infile, buffer, index) )
			break;
			
			
		// If the index is invalid, the data file
		// is probably corrupt.
		if ( index < 0 || index >= ST_MAX_STRINGS ) {
			
			empty();
			sprintf(errorMsg, "Invalid index (%d)", index);
			infile.close();
			return;
		}
		
		
		// If this returns false, we're out of memory.
		// That's the only reason this may return false,
		// since we already checked the validity of the index.
		if ( !insert(index, buffer) ) {

			empty();			
			sprintf(errorMsg, "Out of memory");
			infile.close();
			return;
		}
	}
	
	
	infile.close();
	loaded = true;
	
}

// ==================================================== //

// Helper function for loadTxt().
// Parses a line from the file stream and returns
// true if the read was successful.

bool cStringTable::readLine(std::ifstream &infile, char *buffer, int &index )
{
    
    char indexBuf[12];   // Limit the index number to 11 chars
    char c;
    
    memset(indexBuf, 0, 12);
    memset(buffer, 0, ST_MAX_STRING_SIZE); 

    
    bool readingIndex=true, readingString=false,
         gotIndex=false, gotString=false,
         stop=false, successful=false;
    int pos = 0;
    
    
    while ( !infile.eof() ) {
        
        c = infile.get();
        

        // ===================== //
        //                       //
        // ===================== //
        if (readingIndex) {
            
            switch (c) {
                
	            // Ignore whitespaces after the index and before the '='
	            // because the index can't contain spaces.
                case '\t':
                case ' ':   if ( pos ) gotIndex = true;
                            break;
                
                // If we got an endline here, the line is obviously
                // not a index = "string" pair.
                case '\n':  return false;
                
                // We got the index.
                case '=':   gotIndex = true;
                            readingIndex = false;
                            break;
                
                // Accept only numbers in the index
                case '0': case '1':
                case '2': case '3':
                case '4': case '5':
                case '6': case '7':
                case '8': case '9':
                	if (!gotIndex) indexBuf[pos++] = c;
                	break;
                
                // Any other character in the index
                // means an error in the file
                default:	stop = true;
                			break;
            }
            
            // If we got the index, convert it into an int
            index = atoi(indexBuf);

            // If the index is too long, stop.
            if ( pos >= 12 ) stop = true;

            
        }
        // ===================== //
        //                       //
        // ===================== //
        else if (readingString) {
            
            switch (c) {
                
	            // Since we are reading the string, we already found one quote,
	            // so the next one means we got the string.
                case '\"':  gotString = true;
                            if ( gotIndex && gotString ) successful = true;
                            stop = true;
                            break;
                

                case '\n':  if ( gotIndex && gotString ) {
	                			successful = true;
	                			return true;
                			}
                            else return false;
                
                default:    buffer[pos++] = c;
            }

            // If the string is too long, stop.
            if ( pos >= ST_MAX_STRING_SIZE ) stop = successful = true;
            
        }
        // ===================== //
        //                       //
        // ===================== //
        else {
            
            switch (c) {
                
                case '#':	if ( gotIndex && gotString ) successful = true;
                			stop = true;
                			break;
                
                case '\n':  if ( gotIndex && gotString ) {
	                
                				successful = true;
                				return true;
            				}
            				else return false;

                
                case '\"':  if ( gotIndex && !gotString ) {
                                readingString = true;
                                pos = 0;
                            }
                            break;
            }
            
        }
        // ===================== //
        //                       //
        // ===================== //
        
        
        // If told to stop, go to the next endline or eof
        // and return.
        if (stop) {
                    
            while (1) {
	            
                if ( infile.get() == '\n' || infile.eof() )
                	if ( successful )	return true;
                	else				return false;
            }
        }
        
    } // while
 
    
    return false;
       
}

// ==================================================== //
//	                                                    //
//	                                                    //
//	                                                    //
// ==================================================== //

// Saves the strings into a text file.

bool cStringTable::saveTxt(const char *file)
{
	
	std::ofstream outfile(file);
	if ( !outfile ) {
		
		sprintf(errorMsg, "Error opening \"%s\"", file);
		return false;
	}
	
	
	for ( int i=0; i<ST_MAX_STRINGS; i++ ) {
		
		if ( stringTable[i] )
			outfile << i << " = \"" << stringTable[i] << '\"' << std::endl;
	}
	
	
	outfile.close();
	return true;
	
}

// ==================================================== //
//	                                                    //
//	                                                    //
//	                                                    //
// ==================================================== //
