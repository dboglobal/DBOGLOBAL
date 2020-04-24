//-- include ------------------------------------------------------------------------
#include "StdAfx.h"
#include "NtlPetition.h"
#include <vector>
//-----------------------------------------------------------------------------------

////------------------------------------------------------------------------------
////	author:  sooshia
////	date:    2008-09-04
////	Purpose: 선택된 카테고리의 enum값을 반환
////------------------------------------------------------------------------------
//int Category2SelToEnum( int nCurrentSel1, int nCurrentSel2 )
//{
//	int nCategoryIndexStart[PETITION_CATEGORY_1_COUNT+1] = {0,};
//	for (int i=0 ; i<PETITION_CATEGORY_1_COUNT ; i++)
//	{
//		nCategoryIndexStart[i+1] = nCategoryIndexStart[i] + g_nPetitionCategory1to2[i];
//	}
//
//	return nCategoryIndexStart[nCurrentSel1] + nCurrentSel2;
//}

//------------------------------------------------------------------------------
//	author:  sooshia
//	date:    2008-09-04
//	Purpose: 카테고리2의 enum값을 받아서 몇 번째 selection에 해당하는지 반환
//------------------------------------------------------------------------------
int PetitionCategory2ToSelectionPos( const int nCategory2 )
{
	if ( nCategory2<0 || nCategory2>=PETITION_CATEGORY_2_COUNT )
	{
		return -1; // error
	}

	int nCategoryIndexStart[PETITION_CATEGORY_1_COUNT+1] = {0,};
	for (int i=0 ; i<PETITION_CATEGORY_1_COUNT ; i++)
	{
		nCategoryIndexStart[i+1] = nCategoryIndexStart[i] + g_nPetitionCategory1to2[i];
		if ( nCategory2>=nCategoryIndexStart[i] && nCategory2<nCategoryIndexStart[i+1] )
		{
			return nCategory2-nCategoryIndexStart[i];
		}
	}
	return -1; // invalid
}

//------------------------------------------------------------------------------
//	author:  sooshia
//	date:    2008-10-24
//	Purpose: 카테고리1의 enum값을 받아서 이에 해당하는 카테고리2의 enum값을 반환
//------------------------------------------------------------------------------
std::vector<int> PetitionCategory1To2( const int nCategory1 )
{
	std::vector<int> vecCategory2Value;
	if ( nCategory1<0 || nCategory1>=PETITION_CATEGORY_1_COUNT )
	{
		vecCategory2Value.clear();
		return vecCategory2Value; // error
	}

	int nCategoryIndexStart[PETITION_CATEGORY_1_COUNT+1] = {0,};
	for (int i=0 ; i<PETITION_CATEGORY_1_COUNT ; i++)
	{
		nCategoryIndexStart[i+1] = nCategoryIndexStart[i] + g_nPetitionCategory1to2[i];
	}

	for (int nCategory2=nCategoryIndexStart[nCategory1] 
			; nCategory2<nCategoryIndexStart[nCategory1+1] 
			; nCategory2++)
	{
		vecCategory2Value.push_back(nCategory2);
	}

	return vecCategory2Value;
}

//------------------------------------------------------------------------------
//	author:  sooshia
//	date:    2008-10-24
//	Purpose: 카테고리2의 enum값을 받아서 이에 해당하는 카테고리1의 enum값을 반환
//------------------------------------------------------------------------------
int PetitionCategory2To1( const int nCategory2 )
{
	if ( nCategory2<0 || nCategory2>=PETITION_CATEGORY_2_COUNT )
	{
		return -1; // error
	}

	int nCategoryIndexStart[PETITION_CATEGORY_1_COUNT+1] = {0,};
	for (int i=0 ; i<PETITION_CATEGORY_1_COUNT ; i++)
	{
		nCategoryIndexStart[i+1] = nCategoryIndexStart[i] + g_nPetitionCategory1to2[i];
		if ( nCategory2>=nCategoryIndexStart[i] && nCategory2<nCategoryIndexStart[i+1] )
		{
			return i;
		}
	}
	return -1; // invalid
}