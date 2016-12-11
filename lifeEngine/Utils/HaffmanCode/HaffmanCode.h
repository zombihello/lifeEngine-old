#ifndef HAFFMAN_CODE
#define HAFFMAN_CODE

#define COMPILING_LIBRARY
#include "../../DllGlobal.h"

/////////////////////
// SYSTEM
////////////////////
#include <vector>
#include <map>
#include <list>
#include <fstream>
#include <sstream>
#include <string>
using namespace std;

//-------------------------------------------------------------------------//

class Node;

//-------------------------------------------------------------------------//

class DLL_API HaffmanCode
{
public:
	bool	LoadFromFile( string route , string namefile );
	void	CompressedFromFile( string route , string namefile );
	void	CompressedFromMemory( string route , string text );
	string	DecompressedFromFile( string route );

private:
	void BuildTable( Node* root , string namefile , vector<bool>& code );
	void BuildTable( Node* root , map<char , vector<bool> >& mTable , vector<bool>& code );

	map<string , string>							mRoutesLoadFile;
	map<string , map<char , vector<bool> > >		mTables;
	list<Node*>										lNodes;
};

//-------------------------------------------------------------------------//

#endif // HAFFMAN_CODE
