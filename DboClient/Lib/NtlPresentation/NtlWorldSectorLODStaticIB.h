#ifndef _STATICIB_H_
#define _STATICIB_H_

#include <d3d9.h>
#include <assert.h>

/////////////////////////////
// D. Sim Dietrich Jr.
// sim.dietrich@nvidia.com
//////////////////////

template < class IndexType > class StaticIB
{
	private :

		LPDIRECT3DINDEXBUFFER9 mpIB;

		unsigned int mIndexCount;

		unsigned int mPosition;

		bool		 mbLocked;

		D3DINDEXBUFFER_DESC mDesc;

	public :

		unsigned int GetIndexCount() const 
		{ 
			return mIndexCount; 
		}

		StaticIB( const LPDIRECT3DDEVICE9 pD3D, const unsigned int& theIndexCount )
		{
			mpIB = 0;
			mPosition = 0;

			mbLocked = false;

			mIndexCount = theIndexCount;
		
			memset( &mDesc, 0x00, sizeof( mDesc ) );
			if ( sizeof( IndexType ) == 2 )
			{
				mDesc.Format = D3DFMT_INDEX16;
			}
			else
			{
				assert( sizeof( IndexType ) == 4 );
				mDesc.Format = D3DFMT_INDEX32;
			}

			mDesc.Size = sizeof( IndexType ) * theIndexCount;
			mDesc.Type = D3DRTYPE_INDEXBUFFER;
			mDesc.Pool = D3DPOOL_MANAGED;//D3DPOOL_DEFAULT;

			mDesc.Usage = D3DUSAGE_WRITEONLY;

			HRESULT hr = pD3D->CreateIndexBuffer( mIndexCount * sizeof( IndexType ),
				                                   mDesc.Usage,
												   mDesc.Format,
												   mDesc.Pool,
												   &mpIB,
												   NULL);

			assert( ( hr == D3D_OK ) && ( mpIB ) );
		}

		LPDIRECT3DINDEXBUFFER9 GetInterface() const { return mpIB; }

		IndexType* Lock( const unsigned int& theLockCount, unsigned int& theStartIndex )
		{
			theStartIndex = 0;
			IndexType* pLockedData = 0;

			// Ensure there is enough space in the IB for this data
			if ( theLockCount > mIndexCount ) { assert( false ); return 0; }

			if ( mpIB )
			{
				DWORD dwFlags = D3DLOCK_NOSYSLOCK;

				DWORD dwSize = 0;

				HRESULT hr = mpIB->Lock( mPosition * sizeof( IndexType ), 
										 theLockCount * sizeof( IndexType ), 
										 reinterpret_cast< VOID** >( &pLockedData ), 
										 dwFlags );

				assert( hr == D3D_OK );
				if ( hr == D3D_OK )
				{
					assert( pLockedData != 0 );
					mbLocked = true;
					theStartIndex = mPosition;
					mPosition += theLockCount;
				}
			}

			return pLockedData;
		}

		void Unlock()
		{
			if ( ( mbLocked ) && ( mpIB ) )
			{
				HRESULT hr = mpIB->Unlock();				
				assert( hr == D3D_OK );
				mbLocked = false;
			}
		}

		~StaticIB()
		{
			Unlock();
			if ( mpIB )
			{
				mpIB->Release();
			}
		}
	
};

typedef StaticIB< unsigned short > StaticIB16;
typedef StaticIB< unsigned int > StaticIB32;

#endif  _STATICIB_H_
