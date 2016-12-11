#include "HaffmanCode.h"
#include "Node.h"

//-------------------------------------------------------------------------//

bool SortNode( const Node *left , const Node *right )
{
	return left->iCount < right->iCount;
}

//-------------------------------------------------------------------------//

void HaffmanCode::BuildTable( Node* root , string namefile , vector<bool>& code )
{
	if ( root->left != NULL )
	{
		code.push_back( 0 );
		BuildTable( root->left , namefile , code );
	}

	if ( root->right != NULL )
	{
		code.push_back( 1 );
		BuildTable( root->right , namefile , code );
	}

	if ( root->left == NULL && root->right == NULL )
		mTables[ namefile ][ root->_char ] = code;

	if ( !code.empty() )
		code.pop_back();
}

void HaffmanCode::BuildTable( Node* root , map<char , vector<bool> >& mTable , vector<bool>& code )
{
	if ( root->left != NULL )
	{
		code.push_back( 0 );
		BuildTable( root->left , mTable , code );
	}

	if ( root->right != NULL )
	{
		code.push_back( 1 );
		BuildTable( root->right , mTable , code );
	}

	if ( root->left == NULL && root->right == NULL )
		mTable[ root->_char ] = code;

	if ( !code.empty() )
		code.pop_back();
}

//-------------------------------------------------------------------------//

bool HaffmanCode::LoadFromFile( string route , string namefile )
{
	mRoutesLoadFile[ namefile ] = route;
	map<char , int> mCharEnters;

	ifstream fi( route , ios::out | ios::binary );

	if ( !fi.is_open() ) return false;

	char _char;
	while ( fi.get( _char ) )
		mCharEnters[ _char ]++;

	fi.close();

	for ( auto it = mCharEnters.begin(); it != mCharEnters.end(); ++it )
	{
		Node *node = new Node;
		node->_char = it->first;
		node->iCount = it->second;
		lNodes.push_back( node );
	}

	while ( lNodes.size() != 1 )
	{
		lNodes.sort( SortNode );

		Node *SonL = lNodes.front();
		lNodes.pop_front();

		Node *SonR = lNodes.front();
		lNodes.pop_front();

		Node *parent = new Node( SonL , SonR );
		lNodes.push_back( parent );

	}

	Node* root = lNodes.front();

	vector<bool> code;
	BuildTable( root , namefile , code );

	return true;
}

void HaffmanCode::CompressedFromMemory( string route , string text )
{
	stringstream sstream;
	sstream << text;

	map<char , int> mCharEnters;
	char _char;

	while ( sstream.get( _char ) )
		mCharEnters[ _char ]++;

	for ( auto it = mCharEnters.begin(); it != mCharEnters.end(); ++it )
	{
		Node *node = new Node;
		node->_char = it->first;
		node->iCount = it->second;
		lNodes.push_back( node );
	}

	while ( lNodes.size() != 1 )
	{
		lNodes.sort( SortNode );

		Node *SonL = lNodes.front();
		lNodes.pop_front();

		Node *SonR = lNodes.front();
		lNodes.pop_front();

		Node *parent = new Node( SonL , SonR );
		lNodes.push_back( parent );

	}

	Node* root = lNodes.front();

	vector<bool> code;
	map<char , vector<bool> > mTable;
	BuildTable( root , mTable , code );

	sstream.clear();
	sstream.seekg( 0 );

	ofstream fo( route , ios::out | ios::binary );

	//----------«¿œ»—€¬¿≈Ã-“¿¡À»÷”-’¿‘‘Ã¿Õ¿-¬-‘¿…À----------//

	int sizeTable = mTable.size();

	fo.write( ( char* ) &sizeTable , sizeof( sizeTable ) );

	for ( auto it = mTable.begin(); it != mTable.end(); it++ )
	{
		int sizeVector = it->second.size();

		fo.write( ( char* ) &sizeVector , sizeof( sizeVector ) );
		for ( int i = 0; i < sizeVector; i++ )
		{
			bool j = it->second[ i ];
			fo.write( ( char* ) &j , sizeof( j ) );
		}

		char _char = it->first;
		fo.write( ( char* ) &_char , sizeof( _char ) );
	}

	//------------------------------------------------------//

	int iCount = 0;
	char cBuff = 0;

	while ( sstream.get( _char ) )
	{
		vector<bool> code = mTable[ _char ];

		for ( int i = 0; i < code.size(); i++ )
		{
			cBuff = cBuff | code[ i ] << ( 7 - iCount );
			iCount++;

			if ( iCount == 8 )
			{
				iCount = 0;
				fo << cBuff;
				cBuff = 0;
			}
		}
	}

	if ( iCount > 0 )
		fo << cBuff;

	fo.close();

	return;
}

//-------------------------------------------------------------------------//

void HaffmanCode::CompressedFromFile( string route , string namefile )
{
	for ( auto it = mRoutesLoadFile.begin(); it != mRoutesLoadFile.end(); it++ )
	{
		if ( it->first == namefile )
		{
			ifstream fi( it->second , ios::out | ios::binary );
			ofstream fo( route , ios::out | ios::binary );

			//----------«¿œ»—€¬¿≈Ã-“¿¡À»÷”-’¿‘‘Ã¿Õ¿-¬-‘¿…À----------//

			int sizeTable = mTables[ namefile ].size();
			map<char , vector<bool> > mTable = mTables[ namefile ];

			fo.write( ( char* ) &sizeTable , sizeof( sizeTable ) );

			for ( auto it = mTable.begin(); it != mTable.end(); it++ )
			{
				char _char = it->first;
				int sizeVector = it->second.size();

				fo.write( ( char* ) &_char , sizeof( _char ) );
				fo.write( ( char* ) &sizeVector , sizeof( sizeVector ) );
				for ( int i = 0; i < sizeVector; i++ )
				{
					bool j = it->second[ i ];
					fo.write( ( char* ) &j , sizeof( j ) );
				}
			}

			//------------------------------------------------------//

			int iCount = 0;
			char cBuff = 0;
			char _char;

			while ( fi.get( _char ) )
			{
				vector<bool> code = mTable[ _char ];

				for ( int i = 0; i < code.size(); i++ )
				{
					cBuff = cBuff | code[ i ] << ( 7 - iCount );
					iCount++;

					if ( iCount == 8 )
					{
						iCount = 0;
						fo << cBuff;
						cBuff = 0;
					}
				}
			}

			if ( iCount > 0 )
				fo << cBuff;

			fi.close();
			fo.close();

			return;
		}
	}
}

//-------------------------------------------------------------------------//

string HaffmanCode::DecompressedFromFile( string route )
{
	ifstream fi( route , ios::in | ios::binary );

	if ( !fi.is_open() ) return "ERROR";

	//----------—◊»“€¬¿≈Ã-“¿¡À»÷”-’¿‘‘Ã¿Õ¿-—-‘¿…À¿----------//

	int tableSize;
	fi.read( ( char* ) &tableSize , sizeof( tableSize ) );

	map<vector<bool> , char> mTable;

	for ( int i = 0; i < tableSize; i++ )
	{
		char _char;
		int sizeVec;
		vector<bool> vec;

		fi.read( ( char* ) &sizeVec , sizeof( sizeVec ) );

		for ( int j = 0; j < sizeVec; j++ )
		{
			bool _bool;
			fi.read( ( char* ) &_bool , sizeof( _bool ) );
			vec.push_back( _bool );
		}

		fi.read( ( char* ) &_char , sizeof( _char ) );

		mTable[ vec ] = _char;
	}

	//------------------------------------------------------//

	//----------------ƒ≈ Œƒ»–”≈Ã-‘¿…À----------------------//

	int count = 0;
	char byte;
	stringstream strStream;
	vector<bool> code;
	byte = fi.get();

	while ( !fi.eof() )
	{
		code.push_back( byte & ( 1 << ( 7 - count ) ) );

		if ( mTable[ code ] )
		{
			strStream << mTable[ code ];
			code.clear();
		}

		++count;

		if ( count == 8 )
		{
			count = 0;
			byte = fi.get();
		}
	}

	fi.close();

	return string( strStream.str() );
	//------------------------------------------------------//
}

//-------------------------------------------------------------------------//