
/* -------------------------------------------------- //

cStringTable v1.2

A class that holds a number of strings in an array.
The strings can be saved to/loaded from a file with
the string indices preserved.
If a string is longer than the maximum string size
specified in ST_MAX_STRING_SIZE, it will be truncated
on inserting.
The strings in the binary file are xorred with 85
so that they are not plain text in the data file.

Usage:

	cStringTable tbl;
	
	tbl.loadBin("strings.dat");    // Load from binary file
	//tbl.loadTxt("strings.txt");  // Load from text file
	
	if ( !tbl ) {
		
		printf( "Error loading string table: %s\n", tbl.getErrorMsg() );
		return 1;
	}
	
	tbl.add("test"); // Add to the first available slot
	
	tbl.insert(1, "test2"); // Insert to a specific slot
	
	tbl.remove(1);

	// Print all
	for ( int i=0; i<tbl.getMax(); i++ ) {
		
		if ( !tbl.isNull(i) )
			printf( "String %d: %s\n", i, tbl.getString(i) );
		
		// or
		//if ( !tbl.isNull(i) )
		//	printf( "String %d: %s\n", i, tbl[i] );
	}
	
		
	if ( tbl.saveBin("strings.dat") )
	//if ( tbl.saveTxt("strings.txt") )
		printf("strings.dat saved successfully\n");
	else
		printf("Failed to save strings.dat\n");
		

Version history:

1.1
- Modified getString() to never return a null pointer
- Added isNull() function for testing
- Eliminated the need for "using namespace std;"

1.2
- Corrected the funny .hpp/.cpp relationship
- Made error message buffer static
- Changed char* params and return values to const char*
  where possible

// -------------------------------------------------- */


#ifndef _CLASS_STRINGTABLE_HPP_
#define _CLASS_STRINGTABLE_HPP_

#include <fstream>
#include <cstring>


#define ST_MAX_STRINGS 512     // Maximum number of strings stored
#define ST_MAX_STRING_SIZE 114 // Maximum size of one string


class cStringTable
{
	
	public:
	
		cStringTable();
		~cStringTable();
		

		void	loadBin(const char *file);
		bool	saveBin(const char *file);
		void	loadTxt(const char *file);
		bool	saveTxt(const char *file);
		
		int  	add(const char *string);
		bool 	insert(int index, const char *string);
		bool 	remove(int index);
		void 	empty();

		
		const char * getString(int index);
		const char * operator[](int index);
		bool         isNull(int index);
		

		int	         getMax()        { return ST_MAX_STRINGS; }
		int	         getCount()      { return stringCount; }
        						
		bool         operator!()	 { return !loaded; }
		const char * getErrorMsg()	 { return errorMsg; }
		int	         getUsedMemory() { return usedMemory; }
		
		
	private:
	
		bool readString(std::ifstream &infile, char *buffer, int &index);
		bool readLine(std::ifstream &inFile, char *buffer, int &index );
	
		char *stringTable[ST_MAX_STRINGS];	
		int  stringCount;
		
		bool loaded;
		int  usedMemory;
		
		static char errorMsg[256];
		static char nullStr[5];
	
};

#endif
