//**********************************************************************
//
// Copyright (c) 2000-2005
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

#include "platform_common/Header.h"
#include "DynamicInteger.h"
#include "platform_common/AssemblyMath.h"
#include <memory.h>

void cDynamicInteger::normalise()
{
// remove any number of leading zeros
	cDigitBlock *blockptr;
	int index;
	index=(topdigit&DIGITSPERBLOCKMASK);
	blockptr=lastblockptr;
	while(blockptr->digitarray[index]==0 && (blockptr->prev || index))
	{
		assertD(topdigit);
		topdigit--;
		if(!index)
		{
			index=DIGITSPERBLOCK;
			blockptr=blockptr->prev;
			delete blockptr->next;
			blockptr->next=0;
		}
		index--;
	}
	lastblockptr=blockptr;
	if(blockptr->digitarray[index]==0)
		negativeFlag=0;
}


cDynamicInteger cDynamicInteger::operator+= (const cDynamicInteger &di2)
{
	const cDigitBlock *srcptr;
	cDigitBlock *destptr;
	tUnsigned32 srcendindex,destendindex;
	tUnsigned32 i;
	tUnsigned32 afteraddition;
	tUnsigned32 before;
	int carry;

//	static profileentry additionprofile("addition");
//	profiler aprofiler(&additionprofile);

	srcptr=&(di2.firstblock);
	destptr=&firstblock;
	carry=0;
	srcendindex=(di2.topdigit&DIGITSPERBLOCKMASK);
	destendindex=(topdigit&DIGITSPERBLOCKMASK);
	i=0;

	if(negativeFlag==di2.negativeFlag)
	{

	//addition

		do
		{
		// loop for digits in both src and dest
			afteraddition=destptr->digitarray[i];
			afteraddition+=carry;
			carry=afteraddition<destptr->digitarray[i];
			destptr->digitarray[i]=afteraddition;
			afteraddition+=srcptr->digitarray[i];
			carry+=afteraddition<destptr->digitarray[i];
			destptr->digitarray[i]=afteraddition;
			i++;
			if(i==DIGITSPERBLOCK)
			{
				srcptr=srcptr->next;
				destptr=destptr->next;
				i=0;
			}
		}
		while(srcptr && destptr && (srcptr->next || i<=srcendindex) && (destptr->next || i<=destendindex));

		while(carry && destptr && (destptr->next || i<=destendindex))
		{
		// loop for extra digits in dest while carry flag is set
			afteraddition=destptr->digitarray[i];
			afteraddition+=carry;
			carry=afteraddition<destptr->digitarray[i];
			destptr->digitarray[i]=afteraddition;

		//.. i is no longer in sync with srcptr
			srcptr=0;

			i++;
			if(i==DIGITSPERBLOCK)
			{
				destptr=destptr->next;
				i=0;
			}
		}
		// (or)
		while(srcptr && (srcptr->next || i<=srcendindex))
		{
		// loop for extra digits in src
			destptr=adddigit(destptr,i);
			afteraddition=srcptr->digitarray[i];
			afteraddition+=carry;
			carry=afteraddition<srcptr->digitarray[i];
			destptr->digitarray[i]=afteraddition;
			i++;
			if(i==DIGITSPERBLOCK)
			{
				srcptr=srcptr->next;
				destptr=destptr->next;
				i=0;
			}
		}
		if(carry)
		{
		// extra digit from carry
			destptr=adddigit(destptr,i);
			destptr->digitarray[i]=carry;
		}
		return *this;
	}

	//subtraction

	//.. optimise by combining compare with subtraction
	if(absoluteValueGreaterThan(*this,di2))
	{

	// subtraction without change of sign

		do
		{
			assertD(destptr && (destptr->next || i<=destendindex));
		// loop for digits in both src and dest
			afteraddition=destptr->digitarray[i];
			afteraddition-=carry;
			carry=afteraddition>destptr->digitarray[i];
			destptr->digitarray[i]=afteraddition;
			afteraddition-=srcptr->digitarray[i];
			carry+=afteraddition>destptr->digitarray[i];
			destptr->digitarray[i]=afteraddition;
			i++;
			if(i==DIGITSPERBLOCK)
			{
				srcptr=srcptr->next;
				destptr=destptr->next;
				i=0;
			}
		}
		while(srcptr && (srcptr->next || i<=srcendindex));

		while(carry)
		{
			assertD(destptr && (destptr->next || i<=destendindex));
		// loop for extra digits in dest while carry flag is set
			afteraddition=destptr->digitarray[i];
			afteraddition-=carry;
			carry=afteraddition>destptr->digitarray[i];
			destptr->digitarray[i]=afteraddition;
			i++;
			if(i==DIGITSPERBLOCK)
			{
				destptr=destptr->next;
				i=0;
			}
		}

		if(destptr==0 || (destptr->next==0 && i>destendindex))
			normalise();

		return *this;
	}

	// subtraction with change of sign

	negativeFlag=!negativeFlag;

	do
	{
		assertD(srcptr && (srcptr->next || i<=srcendindex));
	// loop for digits in both src and dest
		afteraddition=srcptr->digitarray[i];
		afteraddition-=carry;
		carry=afteraddition>srcptr->digitarray[i];
		before=afteraddition;
		afteraddition-=destptr->digitarray[i];
		carry+=afteraddition>before;
		destptr->digitarray[i]=afteraddition;
		i++;
		if(i==DIGITSPERBLOCK)
		{
			srcptr=srcptr->next;
			destptr=destptr->next;
			i=0;
		}
	}
	while(destptr && (destptr->next || i<=destendindex));

	while(carry && srcptr && (srcptr->next || i<=srcendindex))
	{
	// loop for extra digits in src while carry flag is set
		destptr=adddigit(destptr,i);
		afteraddition=srcptr->digitarray[i];
		afteraddition-=carry;
		carry=afteraddition>srcptr->digitarray[i];
		destptr->digitarray[i]=afteraddition;
		i++;
		if(i==DIGITSPERBLOCK)
		{
			srcptr=srcptr->next;
			destptr=destptr->next;
			i=0;
		}
	}

	if(srcptr==0 || (srcptr->next==0 && i>srcendindex))
	{
		normalise();
		return *this;
	}

	do
	{
	// copy extra digits now as there is no carry
		destptr=adddigit(destptr,i);
		destptr->digitarray[i]=srcptr->digitarray[i];
		i++;
		if(i==DIGITSPERBLOCK)
		{
			srcptr=srcptr->next;
			destptr=destptr->next;
			i=0;
		}
	}
	while(srcptr && (srcptr->next || i<=srcendindex));

	return *this;
}


bool operator== (const cDynamicInteger &s1, const cDynamicInteger &s2)
{
	const cDigitBlock *blockptr1,*blockptr2;

	if(s1.negativeFlag!=s2.negativeFlag)
		return false;

//... use of topdigit like this gives a limit to size of cDynamicInteger
	if(s1.topdigit!=s2.topdigit)
		return false;

	blockptr1=&s1.firstblock;
	blockptr2=&s2.firstblock;

	if(s1.topdigit==0)
		return blockptr1->digitarray[0]==blockptr2->digitarray[0];

	while(1)
	{
		assertD(blockptr2);
		//.. optimise - dont need to compare all of last block
		if(memcmp(blockptr1->digitarray,blockptr2->digitarray,DIGITSPERBLOCK*4))
			return false;
		blockptr1=blockptr1->next;
		if(!blockptr1)
			break;
		blockptr2=blockptr2->next;
	}
	return true;
}

float
cDynamicInteger::asFloat() const
{
    tUnsigned32 digit1 = lastblockptr->digitarray[topdigit & DIGITSPERBLOCKMASK];
    float digit1F = static_cast<float>(digit1);
    float result = digit1F;
    if(topdigit > 0)
    {
        result *= 4294967296.f;
        tUnsigned32 digitsLeft = topdigit - 1;

        tUnsigned32 digit2Index = topdigit - 1;
        tUnsigned32 digit2;
        if(topdigit & DIGITSPERBLOCKMASK)
        {
          // digit 2 is also in last block
            digit2 = lastblockptr->digitarray[(topdigit & DIGITSPERBLOCKMASK) - 1];
        }
        else
        {
            digit2 = lastblockptr->prev->digitarray[DIGITSPERBLOCK - 1];
        }
        float digit2F = static_cast<float>(digit2);
        result += digit2F;
        while(digitsLeft)
        {
            result *= 4294967296.f;
            digitsLeft--;
        }
    }
    if(isNegative())
    {
        result = -result;
    }
    return result;
}


bool absoluteValueGreaterThan (const cDynamicInteger &s1, const cDynamicInteger &s2)
{
// returns 1 if s1>s2 or 0 otherwise

//	static profileentry aprofile("abs>");
//	profiler aprofiler(&aprofile);

	cDigitBlock *blockptr1,*blockptr2;
	int i;

//... use of topdigit like this gives a limit to size of cDynamicInteger
	if(s1.topdigit!=s2.topdigit)
		return (s1.topdigit>s2.topdigit);

	blockptr1=s1.lastblockptr;
	blockptr2=s2.lastblockptr;
	do
	{
		assertD(blockptr2);
		//.. optimise - dont need to compare all of last block
		for(i=0;i<DIGITSPERBLOCK;i++)
		{
			if(blockptr1->digitarray[DIGITSPERBLOCK-1-i]>blockptr2->digitarray[DIGITSPERBLOCK-1-i])
				return true;
			if(blockptr1->digitarray[DIGITSPERBLOCK-1-i]<blockptr2->digitarray[DIGITSPERBLOCK-1-i])
				return false;
		}
		blockptr1=blockptr1->prev;
		blockptr2=blockptr2->prev;
	}
	while(blockptr1);
	return false;
}


// ======= derived operators ======

int
cDynamicInteger::compareAbsoluteValues(const cDynamicInteger& rhs) const
{
    if(*this == rhs)
    {
        return 0;
    }
    int result = 1;
    if(absoluteValueGreaterThan(*this, rhs))
    {
        result = -1;
    }
    return result;
}

int
cDynamicInteger::compare(const cDynamicInteger& rhs) const
{
    if(negativeFlag != rhs.negativeFlag)
    {
        if(negativeFlag)
        {
            return 1;
        }
        return -1;
    }
    if(*this == rhs)
    {
        return 0;
    }
    int result = 1;
    if(absoluteValueGreaterThan(*this, rhs))
    {
        result = -1;
    }
    if(negativeFlag)
    {
        result = -result;
    }
    return result;
}

//bool operator> (const cDynamicInteger& s1, const cDynamicInteger& s2)
//{
//	if(s1.negativeFlag != s2.negativeFlag)
//		return !s1.negativeFlag;
//	if(s1==s2)
//		return false;
//	return absoluteValueGreaterThan(s1,s2) ^ s1.negativeFlag;
//}
cDynamicInteger operator+(const cDynamicInteger &di1,const cDynamicInteger &di2)
{
	cDynamicInteger result=di1;
	return result+=di2;
}
cDynamicInteger operator-(const cDynamicInteger &di1,const cDynamicInteger &di2)
{
	cDynamicInteger result=di1;
	return result-=di2;
}
// ================================

void cDynamicInteger::shiftleft()
{
	cDigitBlock *ptr;
	int endindex;
	int index;
	int nextcarry;
	int carry;

	ptr=&firstblock;
	index=0;
	endindex=(topdigit&DIGITSPERBLOCKMASK);
	carry=0;
	do
	{
		nextcarry=ptr->digitarray[index]>=0x80000000;
		ptr->digitarray[index]&=0x7fffffff;
		ptr->digitarray[index]<<=1;
		ptr->digitarray[index]+=carry;
		carry=nextcarry;
		index++;
		if(index==DIGITSPERBLOCK)
		{
			ptr=ptr->next;
			index=0;
		}
	}
	while(ptr && (ptr->next || index<=endindex));
	if(carry)
	{
		ptr=adddigit(ptr,index);
		ptr->digitarray[index]=1;
	}
}
void cDynamicInteger::shiftright()
{
	cDigitBlock *ptr;
	int index;
	int carry;
	int nextcarry;

	if(isZero())
		return;
	if(lastblockptr==&firstblock && (topdigit&DIGITSPERBLOCKMASK)==0 && firstblock.digitarray[0]==1)
	{
		negativeFlag=0;
		firstblock.digitarray[0]=0;
		return;
	}

	ptr=lastblockptr;
	index=(topdigit&DIGITSPERBLOCKMASK);
	carry=0;
	do
	{
		nextcarry=ptr->digitarray[index]&1;
		ptr->digitarray[index]>>=1;
		if(carry)
			ptr->digitarray[index]+=0x80000000;
		carry=nextcarry;
		if(!index--)
		{
			index=DIGITSPERBLOCK-1;
			ptr=ptr->prev;
		}
	}
	while(ptr);

	//.. optimise - do this first
	ptr=lastblockptr;
	index=(topdigit&DIGITSPERBLOCKMASK);
	if((index || lastblockptr!=&firstblock) && ptr->digitarray[index]==0)
	{
	// array loses top digit
		assertD(topdigit);
		topdigit--;
		if(!index)
		{
			lastblockptr=ptr->prev;
			lastblockptr->next=0;
			delete ptr;
		}
	}
}

void cDynamicInteger::shiftbitsright(int bits)
{
	assertD(bits>0 && bits<32);
	cDigitBlock *ptr;
	int index;
	tUnsigned32 value;
	tUnsigned32 shiftthrough;
	bool normaliseflag;

//#ifdef ASSERTIONS_ON
//	cDynamicInteger compare;
//    {
//	    compare=*this;
//	    int i;
//	    for(i=0;i<bits;i++)
//		    compare.shiftright();
//    }
//#endif

	ptr=lastblockptr;
	index=(topdigit&DIGITSPERBLOCKMASK);
	value=ptr->digitarray[index];
	shiftthrough=value;
	value>>=bits;
	normaliseflag = (value==0);
	ptr->digitarray[index]=value;
	if(index==0)
	{
		index=DIGITSPERBLOCK;
		ptr=ptr->prev;
	}
	while(ptr)
	{
		index--;
		value=ptr->digitarray[index];
		ptr->digitarray[index]=((value>>bits)|(shiftthrough<<(32-bits)));
		shiftthrough=value;
		if(index==0)
		{
			index=DIGITSPERBLOCK;
			ptr=ptr->prev;
		}
	}
	if(normaliseflag)
	{
		if((topdigit&DIGITSPERBLOCKMASK)==0)
		{
			if(topdigit)
			{
				lastblockptr=lastblockptr->prev;
				assertD(lastblockptr);
				lastblockptr->next=0;
			}
			else
			{
				topdigit=1;
				negativeFlag=0;
			}
		}
		topdigit--;
	}

//	assertD(compare==*this);
}
void cDynamicInteger::shiftbitsleft(int bits)
{
	cDigitBlock *ptr;
	int index;
	int endindex;
	tUnsigned32 value;
	tUnsigned32 shiftthrough;

//...
//	cDynamicInteger compare;
//	compare=*this;
//	int i;
//	for(i=0;i<bits;i++)
//		compare.shiftleft();


	ptr=&firstblock;
	index=0;
	endindex=(topdigit&DIGITSPERBLOCKMASK);
	shiftthrough=ptr->digitarray[index];
	ptr->digitarray[index]=shiftthrough<<bits;
	index++;
	if(index==DIGITSPERBLOCK)
	{
		index=0;
		ptr=ptr->next;
	}
//.. optimise - repeat loop to separate endconditions
	while(ptr && (ptr->next || index<=endindex))
	{
		value=ptr->digitarray[index];
		ptr->digitarray[index]=((value<<bits)|(shiftthrough>>(32-bits)));
		shiftthrough=value;
		index++;
		if(index==DIGITSPERBLOCK)
		{
			index=0;
			ptr=ptr->next;
		}
	}
	shiftthrough>>=(32-bits);
	if(shiftthrough)
	{
	// add digit on end
		ptr=adddigit(ptr,index);
		ptr->digitarray[index]=shiftthrough;
	}

//...
//	assertD(compare==*this);
}
void cDynamicInteger::shiftdigitright()
{
	cDigitBlock *ptr;
	tUnsigned32 *shiftdownptr;
	int i;
	int endi;

//...
//	cDynamicInteger compare;
//	compare=*this;
//	for(i=0;i<32;i++)
//		compare.shiftright();


	ptr=&firstblock;
	while(ptr->next)
	{
		if(ptr!=&firstblock)
			*shiftdownptr=ptr->digitarray[0];
		for(i=0;i<DIGITSPERBLOCK-1;i++)
			ptr->digitarray[i]=ptr->digitarray[i+1];
		shiftdownptr=ptr->digitarray+DIGITSPERBLOCK-1;
		ptr=ptr->next;
	}
	endi=(topdigit&DIGITSPERBLOCKMASK);
	i=0;
	if(ptr!=&firstblock)
		*shiftdownptr=ptr->digitarray[0];
	while(i<endi)
	{
		ptr->digitarray[i]=ptr->digitarray[i+1];
		i++;
	}
	ptr->digitarray[i]=0;
	if(i==0)
	{
		if(topdigit)
		{
			lastblockptr=lastblockptr->prev;
			assertD(lastblockptr);
            delete lastblockptr->next;
			lastblockptr->next=0;
		}
		else
		{
			topdigit++;
			negativeFlag=0;
		}
	}
	topdigit--;

//...
//	assertD(compare==*this);
}
void cDynamicInteger::shiftdigitleft()
{
	cDigitBlock *ptr;
	tUnsigned32 shiftup;
	tUnsigned32 nextshiftup;
	int i;
	int endi;
	if(isZero())
		return;
	shiftup=0;
	ptr=&firstblock;
	while(ptr->next)
	{
		nextshiftup=ptr->digitarray[DIGITSPERBLOCK-1];
		for(i=DIGITSPERBLOCK-1;i;i--)
			ptr->digitarray[i]=ptr->digitarray[i-1];
		ptr->digitarray[0]=shiftup;
		shiftup=nextshiftup;
		ptr=ptr->next;
	}
	i=0;
	endi=(topdigit&DIGITSPERBLOCKMASK);
	while(i<=endi)
	{
	//.. optimise - copy directly
		nextshiftup=ptr->digitarray[i];
		ptr->digitarray[i]=shiftup;
		shiftup=nextshiftup;
		i++;
	}
	if(i==DIGITSPERBLOCK)
	{
		i=0;
		ptr=0;
	}
	ptr=adddigit(ptr,i);
	ptr->digitarray[i]=shiftup;
}




void cDynamicInteger::initialiseFrom( const cDynamicInteger &di)
{
//	static profileentry aprofile("copy");
//	profiler aprofiler(&aprofile);

	int i;
	cDigitBlock *srcptr,*destptr;

	negativeFlag=di.negativeFlag;
	topdigit=di.topdigit;

////... memcpy
//	for(i=0;i<DIGITSPERBLOCK;i++)
//		firstblock.digitarray[i]=di.firstblock.digitarray[i];
	memcpy(firstblock.digitarray,di.firstblock.digitarray,DIGITSPERBLOCK*4);

	destptr=&firstblock;
	srcptr=di.firstblock.next;
	while(srcptr)
	{
		destptr->next=new cDigitBlock;
		destptr->next->prev=destptr;
		destptr=destptr->next;
		for(i=0;i<DIGITSPERBLOCK;i++)
			destptr->digitarray[i]=srcptr->digitarray[i];
		srcptr=srcptr->next;
	}
	destptr->next=0;
	lastblockptr=destptr;
}
void cDynamicInteger::initialiseFrom(tUnsigned32 firstdigit, tUnsigned32 seconddigit, tUnsigned32 topdigit)
{
//.. use of topdigit imposes limit on size of cDynamicInteger
	cDigitBlock *ptr;

// for initial quotient in division
	assertD(firstdigit || seconddigit);
	assertD(topdigit || seconddigit==0);

	negativeFlag=0;
	if(!firstdigit)
	{
		topdigit--;
		firstdigit=seconddigit;
		seconddigit=0;
	}
	this->topdigit=topdigit;

	ptr=&firstblock;
	while(topdigit>=DIGITSPERBLOCK)
	{
		memset(ptr->digitarray,0,DIGITSPERBLOCK*4);
		ptr->next=new cDigitBlock;
		ptr->next->prev=ptr;
		ptr=ptr->next;
		topdigit-=DIGITSPERBLOCK;
	}
	ptr->next=0;
	lastblockptr=ptr;
	memset(ptr->digitarray,0,DIGITSPERBLOCK*4);

	topdigit&=DIGITSPERBLOCKMASK;
	ptr->digitarray[topdigit]=firstdigit;
	if(topdigit)
		ptr->digitarray[topdigit-1]=seconddigit;
	else
	if(seconddigit)
	{
		assertD(ptr->prev);
		ptr->prev->digitarray[DIGITSPERBLOCK-1]=seconddigit;
	}
}

void cofactors(const cDynamicInteger &di1, const cDynamicInteger &di2, cDynamicInteger &factor1, cDynamicInteger &factor2)
{
//.. optimise - obtain directly from hcf algorithm
	int remainderflag;
	cDynamicInteger hcf=highestcommonfactor(di1,di2);
//.. optimise - dividing by the same divisor twice
	factor1=di2;
//.. optimise - drop out early from division as it is known there will be no remainder?
	remainderflag=factor1.divideBy(hcf);
	assertD(!remainderflag);
	factor2=di1;
	remainderflag=factor2.divideBy(hcf);
	assertD(!remainderflag);
}



cDynamicInteger operator*(const cDynamicInteger &di1,const cDynamicInteger &di2)
{
//	static profileentry aprofile("multiply");
//	profiler aprofiler(&aprofile);

	cDynamicInteger result;
	const cDigitBlock *src1ptr;
	const cDigitBlock *src2ptr;
	tUnsigned32 src1endindex,src2endindex;
	tUnsigned32 src1index,src2index;

	tUnsigned32 destendindex;
	cDigitBlock *destptr;
	cDigitBlock *destptr2;
	tUnsigned32 destindex;
	tUnsigned32 destindex2;

	tUnsigned32 multiplier;
	tUnsigned32 product;
	tUnsigned32 producthigh;
	tUnsigned32 carry;
	tUnsigned32 carryhigh;
	tUnsigned32 nextcarryhigh;

	result.negativeFlag=di1.negativeFlag^di2.negativeFlag;

	src1endindex=(di1.topdigit&DIGITSPERBLOCKMASK);
	src2endindex=(di2.topdigit&DIGITSPERBLOCKMASK);

	destendindex=(result.topdigit&DIGITSPERBLOCKMASK);
	destptr=&result.firstblock;
	destindex=0;

	src1ptr=&di1.firstblock;
	src1index=0;
	do
	{
	// loop for digits in src1

		carry=0;
		carryhigh=0;

		multiplier=src1ptr->digitarray[src1index];

		destptr2=destptr;
		destindex2=destindex;

		src2ptr=&di2.firstblock;
		src2index=0;
		do
		{
		// loop for digits in src2
            nAssemblyMath::mul(multiplier,src2ptr->digitarray[src2index],producthigh,product);

			nextcarryhigh=0;

			tUnsigned32 previous;

			if(destptr2==0 || (destptr2->next==0 && destindex2>destendindex))
			{
				destptr2=result.adddigit(destptr2,destindex2);
				if(destptr==0)
					destptr=destptr2;
				destendindex=(result.topdigit&DIGITSPERBLOCKMASK);
			}
			
			previous=destptr2->digitarray[destindex2];

//			__asm
//			{
//				mov eax,product
//				mov ebx,producthigh
//				add	eax,carry
//				adc	ebx,carryhigh
//				adc	nextcarryhigh,0
//				add	eax,previous
//				adc	ebx,0
//				adc	nextcarryhigh,0
//				mov product,eax
//				mov producthigh,ebx
//			}

            {
                tUnsigned32 carryFlag;
                nAssemblyMath::add(carry, product, carryFlag);
                nAssemblyMath::adc(carryhigh, producthigh, carryFlag);
                nAssemblyMath::adc(0, nextcarryhigh, carryFlag);
            }
            {
                tUnsigned32 carryFlag;
                nAssemblyMath::add(previous, product, carryFlag);
                nAssemblyMath::adc(0, producthigh, carryFlag);
                nAssemblyMath::adc(0, nextcarryhigh, carryFlag);
            }

			destptr2->digitarray[destindex2]=product;

			destindex2++;
			if(destindex2==DIGITSPERBLOCK)
			{
				destptr2=destptr2->next;
				destindex2=0;
			}

			carry=producthigh;
			carryhigh=nextcarryhigh;
		
			src2index++;
			if(src2index==DIGITSPERBLOCK)
			{
				src2ptr=src2ptr->next;
				src2index=0;
			}
		}
		while(src2ptr && (src2ptr->next || src2index<=src2endindex));

		if(carry || carryhigh)
		{
			if(destptr2==0 || (destptr2->next==0 && destindex2>destendindex))
			{
				destptr2=result.adddigit(destptr2,destindex2);
				destendindex=(result.topdigit&DIGITSPERBLOCKMASK);
			}
			destptr2->digitarray[destindex2]=carry;
		}
		if(carryhigh)
		{
			if(destptr2==0 || (destptr2->next==0 && destindex2>destendindex))
			{
				destptr2=result.adddigit(destptr2,destindex2);
				destendindex=(result.topdigit&DIGITSPERBLOCKMASK);
			}
			destptr2->digitarray[destindex2]=carryhigh;
		}

		destindex++;
		if(destindex==DIGITSPERBLOCK)
		{
			destptr=destptr->next;
			destindex=0;
		}
		
		src1index++;
		if(src1index==DIGITSPERBLOCK)
		{
			src1ptr=src1ptr->next;
			src1index=0;
		}
	}
	while(src1ptr && (src1ptr->next || src1index<=src1endindex));


	result.normalise();
	return result;
}
cDynamicInteger cDynamicInteger::operator*= (const cDynamicInteger &di2)
{
	return *this=*this*di2;
}

bool
cDynamicInteger::cansubtractresidueproduct(const cDynamicInteger &divisor, tUnsigned32 partialquotient, tUnsigned32 partialremainder) const
{
	const cDynamicInteger &dividend=*this;
//.. optimise - dont do subtraction twice ?
	assertD(partialquotient>=0x40000000);
	assertD(partialremainder<0xffffffff);

	cDigitBlock *destptr;
	const cDigitBlock *srcptr;
	tUnsigned32 destindex,srcindex;
	tUnsigned32 producthigh,productlow;

	if(!divisor.topdigit)
		return true;

	srcptr=divisor.lastblockptr;
	srcindex=(divisor.topdigit&DIGITSPERBLOCKMASK);

	destptr=dividend.lastblockptr;
	destindex=(dividend.topdigit&DIGITSPERBLOCKMASK);

	int carrypermitted;
	tUnsigned32 subtractfrom;
	tUnsigned32 previousproductlow;
	tUnsigned32 beforesubtract;
	subtractfrom=partialremainder;
	carrypermitted=0;
	previousproductlow=0;

	if(srcindex==0)
	{
		srcindex=DIGITSPERBLOCK;
		srcptr=srcptr->prev;
		assertD(srcptr);
	}
	srcindex--;

	if(destindex==0)
	{
		destindex=DIGITSPERBLOCK;
		destptr=destptr->prev;
		assertD(destptr);
	}
	destindex--;

	bool failed = false;
	do
	{
		assertD(destptr);

		beforesubtract=subtractfrom;
		subtractfrom-=previousproductlow;
		if(subtractfrom>beforesubtract)
		{
			if(!carrypermitted)
			// failed
				return false;
			carrypermitted=0;
		}
        nAssemblyMath::mul(partialquotient,srcptr->digitarray[srcindex],producthigh,productlow);
		beforesubtract=subtractfrom;
		subtractfrom-=producthigh;
		if(subtractfrom>beforesubtract)
		{
			if(!carrypermitted)
				return false;
			carrypermitted=0;
		}
		if(subtractfrom)
		// succeeded
			return true;

		previousproductlow=productlow;
		if(srcindex==0)
		{
			srcindex=DIGITSPERBLOCK;
			srcptr=srcptr->prev;
		}
		srcindex--;
		if(destindex==0)
		{
			destindex=DIGITSPERBLOCK;
			destptr=destptr->prev;
		}
		destindex--;

		assertD(destptr);
		subtractfrom=destptr->digitarray[destindex];
	}
	while(srcptr);

	beforesubtract=subtractfrom;
	subtractfrom-=previousproductlow;
	if(subtractfrom>beforesubtract)
	{
		if(!carrypermitted)
		// failed
			return false;
		carrypermitted=0;
	}

	return true;
}

void cDynamicInteger::subtractresidueproduct(const cDynamicInteger &divisor, tUnsigned32 partialquotient, tUnsigned32 partialremainderhigh, tUnsigned32 partialremainderlow)
{
	cDynamicInteger &dividend=*this;
//.. optimise - dont do subtraction twice ?
	assertD(partialquotient>=0x3fffffff);

	cDigitBlock *destptr;
	const cDigitBlock *srcptr;
	tUnsigned32 destindex,srcindex;
	const cDigitBlock *destendptr;
	tUnsigned32 destendindex;
	tUnsigned32 previousproducthigh;
	int carry;
	tUnsigned32 producthigh,productlow;
	tUnsigned32 beforesubtraction;

	destptr=dividend.lastblockptr;
	destindex=(dividend.topdigit&DIGITSPERBLOCKMASK);

	destptr->digitarray[destindex]=partialremainderhigh;
	if(destindex==0)
	{
		destindex=DIGITSPERBLOCK;
		destptr=destptr->prev;
		assertD(destptr);
	}
	destindex--;
	destptr->digitarray[destindex]=partialremainderlow;

	if(!divisor.topdigit)
	{
		normalise();
		return;
	}
	
	srcptr=divisor.lastblockptr;
	srcindex=(divisor.topdigit&DIGITSPERBLOCKMASK);

	destendptr=destptr;
	destendindex=destindex;

// move to first digit for subtraction
	while(srcindex || srcptr->prev)
	{
		assertD(destindex || destptr->prev);

		if(destindex==0)
		{
			destindex=DIGITSPERBLOCK;
			destptr=destptr->prev;
		}
		destindex--;

		if(srcindex==0)
		{
			srcindex=DIGITSPERBLOCK;
			srcptr=srcptr->prev;
		}
		srcindex--;
	}

	carry=0;
	previousproducthigh=0;

	while(destptr!=destendptr || destindex!=destendindex)
	{
        nAssemblyMath::mul(partialquotient,srcptr->digitarray[srcindex],producthigh,productlow);
		beforesubtraction=destptr->digitarray[destindex];
		destptr->digitarray[destindex]-=carry;
		carry=destptr->digitarray[destindex]>beforesubtraction;
		beforesubtraction=destptr->digitarray[destindex];
		destptr->digitarray[destindex]-=previousproducthigh;
		carry+=destptr->digitarray[destindex]>beforesubtraction;
		beforesubtraction=destptr->digitarray[destindex];
		destptr->digitarray[destindex]-=productlow;
		carry+=destptr->digitarray[destindex]>beforesubtraction;

		previousproducthigh=producthigh;

		destindex++;
		if(destindex==DIGITSPERBLOCK)
		{
			destptr=destptr->next;
			destindex=0;
		}
		srcindex++;
		if(srcindex==DIGITSPERBLOCK)
		{
			srcptr=srcptr->next;
			srcindex=0;
		}
	}

	beforesubtraction=destptr->digitarray[destindex];
	destptr->digitarray[destindex]-=carry;
	carry=destptr->digitarray[destindex]>beforesubtraction;
	beforesubtraction=destptr->digitarray[destindex];
	destptr->digitarray[destindex]-=previousproducthigh;
	carry+=destptr->digitarray[destindex]>beforesubtraction;
	
	destindex++;
	if(destindex==DIGITSPERBLOCK)
	{
		destptr=destptr->next;
		destindex=0;
	}
 
	beforesubtraction=destptr->digitarray[destindex];
	destptr->digitarray[destindex]-=carry;
	carry=destptr->digitarray[destindex]>beforesubtraction;
	assertD(carry==0);

	normalise();
}



bool
cDynamicInteger::divideBy(const cDynamicInteger &di)
{
// returns true if there was a remainder or false on exact division

// absolute values of di and *this
	cDynamicInteger divisor;
	tUnsigned32 divisortop;
	cDynamicInteger quotient;
	tUnsigned32 partialremainder;
	tUnsigned32 partialremainderhigh;
	tUnsigned32 topdigitbefore;
	tUnsigned32 digitsretired;
	tUnsigned32 i;
	tUnsigned32 digitstoretire;
	int bitstoretire;
	tUnsigned32 dividendtophigh;
	tUnsigned32 dividendtoplow;
	tUnsigned32 partialquotient;
	bool newnegativeFlag;

	assertD(!di.isZero());

	newnegativeFlag = negativeFlag;
	if(!absoluteValueGreaterThan(*this,di))
	{
		if(!absoluteValueGreaterThan(di,*this))
		{
			deleteall();
			newnegativeFlag^=di.negativeFlag;
			initialiseFrom((tUnsigned32)1);
			negativeFlag=newnegativeFlag;
			return false;
		}
		deleteall();
		initialiseFrom((tUnsigned32)0);
		return true;
	}

	divisor=di;
	if(divisor.isNegative())
		divisor.negate();
	if(divisor.isIdentity())
		return false;

//#ifdef ASSERTIONS_ON
//	cDynamicInteger dividend=*this;
//#endif
//#ifndef ASSERTIONS_ON
	cDynamicInteger &dividend=*this;
//#endif
	if(dividend.isNegative())
		dividend.negate();

	{
		int topbit;
        topbit = nAssemblyMath::indexForTopBit(divisor.getfirstdigit());
		if(topbit<31)
		{
			divisor.shiftbitsleft(31-topbit);
			dividend.shiftbitsleft(31-topbit);
		}
		divisortop=divisor.getfirstdigit();
	}

	//.. limits size of cDynamicInteger
	if(dividend.topdigit==divisor.topdigit && dividend.getfirstdigit()>=divisortop)
	{
		assertD(dividend.lastblockptr->digitarray[dividend.topdigit&DIGITSPERBLOCKMASK]>divisortop);
		newnegativeFlag^=di.negativeFlag;
		deleteall();
		initialiseFrom((tUnsigned32)1);
        if(newnegativeFlag)
        {
            negate();
        }
		return true;
	}

	digitstoretire=dividend.topdigit-divisor.topdigit;
	bitstoretire=0;

	do
	{
		if(dividend.getfirstdigit()>=divisortop)
		{
			dividend.shiftleft();
			bitstoretire+=31;
			if(bitstoretire>=32)
			{
				bitstoretire-=32;
				digitstoretire++;
			}
		//.. optimise - it is known how many shifts will be immediately below
		//.. shouldn't get here very often, though
		}
	
		{
			assertD(dividend.topdigit);

			int topbit;
            topbit = nAssemblyMath::indexForTopBit(dividend.getfirstdigit());
			if(topbit<30)
			{
				dividend.shiftbitsleft(30-topbit);
				quotient.shiftbitsleft(30-topbit);
				bitstoretire-=30-topbit;
				if(bitstoretire<0)
				{
					assertD(digitstoretire);
					digitstoretire--;
					bitstoretire+=32;
				}
			}			
		}

		dividendtophigh=dividend.getfirstdigit();
		dividendtoplow=dividend.getseconddigit();

        nAssemblyMath::div(dividendtophigh,dividendtoplow,divisortop,partialquotient,partialremainder);

		partialremainderhigh=0;
		if(!dividend.cansubtractresidueproduct(divisor,partialquotient,partialremainder))
		{
			tUnsigned32 beforeaddition;
			assertD(partialquotient);
			partialquotient--;
			beforeaddition=partialremainder;
			partialremainder+=divisortop;
			partialremainderhigh=(int)(partialremainder<beforeaddition);
			if(!partialremainderhigh && !dividend.cansubtractresidueproduct(divisor,partialquotient,partialremainder))
			{
				assertD(partialquotient);
				partialquotient--;
				beforeaddition=partialremainder;
				partialremainder+=divisortop;
				partialremainderhigh=(int)(partialremainder<beforeaddition);
			}
		}
		//.. restricts size of cDynamicInteger
		topdigitbefore=dividend.topdigit;
		dividend.subtractresidueproduct(divisor,partialquotient,partialremainderhigh,partialremainder);

	//.. optimise - build quotient directly
		quotient.shiftdigitleft();
		quotient+=cDynamicInteger(partialquotient);

		digitsretired=topdigitbefore-dividend.topdigit;
		assertD(digitsretired);

	// impose max limit on digitsretired
		if(bitstoretire)
		{
			if(digitstoretire+1<digitsretired)
				digitsretired=digitstoretire+1;
		}
		else
			if(digitstoretire<digitsretired)
				digitsretired=digitstoretire;

		for(i=0;i<digitsretired-1;i++)
			quotient.shiftdigitleft();

		if(digitstoretire<digitsretired)
		{
			assertD(digitstoretire+1==digitsretired);
			digitstoretire=0;
			assertD(bitstoretire);
			bitstoretire-=32;
			break;
		}
		digitstoretire-=digitsretired;
		if(digitstoretire==0 && bitstoretire==0)
			break;
	}
	while(1);

	bool remainderflag = !dividend.isZero();

	while(bitstoretire<0)
	{
		if(!quotient.isEven())
        {
			remainderflag = true;
        }
		quotient.shiftright();
		bitstoretire++;
	}

	quotient.negativeFlag=newnegativeFlag^di.negativeFlag;
	*this=quotient;

	return remainderflag;
}

//-----------------------------------
// support routines for highestcommonfactor()

int cDynamicInteger::comparebitstring(const cDigitBlock *endptr, const int endindex, const cDynamicInteger &test, const int shift) const
{
// 0 = equal
// 1 = test>this
// -1 = test<this
	cDigitBlock *ptr;
	int index;
	cDigitBlock *testptr;
	int testindex;
	tUnsigned32 digittocompare;

	assertD(shift>=0);
//.. optimise - special case for shift==0?

	ptr=lastblockptr;
	index=(topdigit&DIGITSPERBLOCKMASK);
	testptr=test.lastblockptr;
	testindex=(test.topdigit&DIGITSPERBLOCKMASK);

	if(shift)
	{
		digittocompare=testptr->digitarray[testindex]>>(32-shift);
		if(digittocompare)
		{
		// this has an extra digit
			if(digittocompare>ptr->digitarray[index])
				return 1;
			if(digittocompare<ptr->digitarray[index])
				return -1;

			assertD(ptr!=endptr || index!=endindex);
			index--;
			if(index<0)
			{
				index=DIGITSPERBLOCK-1;
				ptr=ptr->prev;
				assertD(ptr);
			}
		}
	}

	digittocompare=testptr->digitarray[testindex]<<shift;
	while(ptr!=endptr || index!=endindex)
	{
		testindex--;
		if(testindex<0)
		{
			testindex=DIGITSPERBLOCK-1;
			testptr=testptr->prev;
			assertD(testptr);
		}

		if(shift)
			digittocompare|=testptr->digitarray[testindex]>>(32-shift);

		if(digittocompare>ptr->digitarray[index])
			return 1;
		if(digittocompare<ptr->digitarray[index])
			return -1;

		digittocompare=testptr->digitarray[testindex]<<shift;

		index--;
		if(index<0)
		{
			index=DIGITSPERBLOCK-1;
			ptr=ptr->prev;
			assertD(ptr);
		}
	}

	testindex--;
	if(testindex<0)
	{
		testindex=DIGITSPERBLOCK-1;
		testptr=testptr->prev;
	}
	if(testptr)
		if(shift)
			digittocompare|=testptr->digitarray[testindex]>>(32-shift);

	if(digittocompare>ptr->digitarray[index])
		return 1;
	if(digittocompare<ptr->digitarray[index])
		return -1;

	return 0;
}

void
cDynamicInteger::subtractbitstring(cDigitBlock *ptr, int index, const cDynamicInteger &src, cDigitBlock *srcptr, int srcindex, const int srcshift)
{
	tUnsigned32 beforesubtraction;
	tUnsigned32 tosubtract;
	int borrow;
	int srcendindex;

	srcendindex=(src.topdigit&DIGITSPERBLOCKMASK);

//#ifdef ASSERTIONS_ON
//	int endindex;
//	endindex=(topdigit&DIGITSPERBLOCKMASK);
//#endif

	borrow=0;
	if(srcshift>=0)
	{
		// source shifts left

		tosubtract=0;
		while(srcptr->next || srcindex!=srcendindex)
		{
//			assertD(ptr->next || index!=endindex)
			tosubtract|=srcptr->digitarray[srcindex]<<srcshift;

			beforesubtraction=ptr->digitarray[index];
			ptr->digitarray[index]-=borrow;
			borrow=(int)(ptr->digitarray[index]>beforesubtraction);
			beforesubtraction=ptr->digitarray[index];
			ptr->digitarray[index]-=tosubtract;
			borrow+=(int)(ptr->digitarray[index]>beforesubtraction);

			if(srcshift)
				tosubtract=srcptr->digitarray[srcindex]>>(32-srcshift);
			else
				tosubtract=0;

			srcindex++;
			if(srcindex==DIGITSPERBLOCK)
			{
				srcindex=0;
				srcptr=srcptr->next;
				assertD(srcptr);
			}

			index++;
			if(index==DIGITSPERBLOCK)
			{
				index=0;
				ptr=ptr->next;
				assertD(ptr);
			}
		}

//		assertD(ptr && (ptr->next || index<=endindex));

		tosubtract|=srcptr->digitarray[srcindex]<<srcshift;

		beforesubtraction=ptr->digitarray[index];
		ptr->digitarray[index]-=borrow;
		borrow=(int)(ptr->digitarray[index]>beforesubtraction);
		beforesubtraction=ptr->digitarray[index];
		ptr->digitarray[index]-=tosubtract;
		borrow+=(int)(ptr->digitarray[index]>beforesubtraction);

		if(srcshift)
			tosubtract=srcptr->digitarray[srcindex]>>(32-srcshift);
		else
			tosubtract=0;

		if(tosubtract || borrow)
		{
//			assertD(ptr->next || index!=endindex)
			index++;
			if(index==DIGITSPERBLOCK)
			{
				index=0;
				ptr=ptr->next;
			}
			beforesubtraction=ptr->digitarray[index];
			ptr->digitarray[index]-=borrow;
			borrow=(int)(ptr->digitarray[index]>beforesubtraction);
			beforesubtraction=ptr->digitarray[index];
			ptr->digitarray[index]-=tosubtract;
			borrow+=(int)(ptr->digitarray[index]>beforesubtraction);
		}	
	}
	else
	{
		// source shifts right

		tosubtract=srcptr->digitarray[srcindex]>>(-srcshift);
		while(srcptr->next || srcindex!=srcendindex)
		{
			srcindex++;
			if(srcindex==DIGITSPERBLOCK)
			{
				srcindex=0;
				srcptr=srcptr->next;
			}
			tosubtract|=srcptr->digitarray[srcindex]<<(32+srcshift);

//			assertD(ptr && (ptr->next || index<=endindex));

			beforesubtraction=ptr->digitarray[index];
			ptr->digitarray[index]-=borrow;
			borrow=(int)(ptr->digitarray[index]>beforesubtraction);
			beforesubtraction=ptr->digitarray[index];
			ptr->digitarray[index]-=tosubtract;
			borrow+=(int)(ptr->digitarray[index]>beforesubtraction);

			tosubtract=srcptr->digitarray[srcindex]>>(-srcshift);

			index++;
			if(index==DIGITSPERBLOCK)
			{
				index=0;
				ptr=ptr->next;
			}
		}

		if(tosubtract || borrow)
		{
//			assertD(ptr && (ptr->next || index<=endindex));

			beforesubtraction=ptr->digitarray[index];
			ptr->digitarray[index]-=borrow;
			borrow=(int)(ptr->digitarray[index]>beforesubtraction);
			beforesubtraction=ptr->digitarray[index];
			ptr->digitarray[index]-=tosubtract;
			borrow+=(int)(ptr->digitarray[index]>beforesubtraction);
		}
	}
	while(borrow)
	{
//		assertD(ptr->next || index!=endindex)
		index++;
		if(index==DIGITSPERBLOCK)
		{
			index=0;
			ptr=ptr->next;
		}
		beforesubtraction=ptr->digitarray[index];
		ptr->digitarray[index]-=borrow;
		borrow=(int)(ptr->digitarray[index]>beforesubtraction);
	}
//.. optimise - it is known in advance whether top digit will change
//	if(ptr->next==0 && index==endindex)
		normalise();
}

cDynamicInteger::cDynamicInteger(const cLargeInteger2& value)
{
    firstblock.prev = 0;
    
}

class hcfinfo
{
public:
	tUnsigned32 &digit() const
	{
		return ptr->digitarray[index];
	}
	int bit() const
	{
		return (digit()&bitmask)!=0;
	}
	tUnsigned32 &digit(cDigitBlock *const ptr) const
	{
		return ptr->digitarray[index];
	}
	int bit(cDigitBlock *const ptr) const
	{
		return (digit(ptr)&bitmask)!=0;
	}
	void shiftleft()
	{
		bitmask<<=1;
		shift++;
		if(bitmask==0)
		{
			bitmask=1;
			shift=0;
			digitshift++;
			index++;
			if(index==DIGITSPERBLOCK)
			{
				index=0;
				ptr=ptr->next;
				assertD(ptr);
			}
		}
	}
	void shiftleft(cDigitBlock *(&ptr2))
	{
		bitmask<<=1;
		shift++;
		if(bitmask==0)
		{
			bitmask=1;
			shift=0;
			digitshift++;
			index++;
			if(index==DIGITSPERBLOCK)
			{
				index=0;
				ptr=ptr->next;
				ptr2=ptr2->next;
				assertD(ptr);
				assertD(ptr2);
			}
		}
	}
	int gettopbit() const
	{
		int topbits;
        topbits = nAssemblyMath::indexForTopBit(value.getfirstdigit())+1;
//		tUnsigned32 top;
//		top=value.getfirstdigit();
//		topbits=0;
//		while(top)
//		{
//			topbits++;
//			top>>=1;
//		}
		return topbits;
	}
	void getbitsleft(tUnsigned32 &digits, int &bits) const
	{
		digits = value.getTopDigitIndex() - digitshift;
		bits=gettopbit()-shift;
		if(bits<0)
		{
			assertD(digits);
			digits--;
			bits+=32;
		}
	}
	cDynamicInteger value;
	tUnsigned32 bitmask;
	tUnsigned32 digitshift;
	int shift;
	cDigitBlock *ptr;
	int index;
};

cDynamicInteger highestcommonfactor( const cDynamicInteger &d1, const cDynamicInteger &d2)
{
//	static profileentry aprofile("highestcommonfactor");
//	profiler aprofiler(&aprofile);
//	VtResumeSampling();

	hcfinfo hcfinfo1,hcfinfo2;
	hcfinfo *large,*small;
	tSigned32 extradigits;
	int extrabits;
	int equal;
	tUnsigned32 resultshift;
	hcfinfo *temp;

	assertD(!d1.isZero() && !d2.isZero());
	hcfinfo1.value=d1;
	if(hcfinfo1.value.isNegative())
		hcfinfo1.value.negate();
	hcfinfo2.value=d2;
	if(hcfinfo2.value.isNegative())
		hcfinfo2.value.negate();

// these will be swapped if necessary after removal of leading zeros
	large=&hcfinfo1;
	small=&hcfinfo2;

	large->bitmask=1;
	large->digitshift=0;
	large->shift=0;
	large->index=0;
	large->ptr=&(large->value.firstblock);
	small->ptr=&(small->value.firstblock);

// lose any trailing zeros across both
	resultshift=0;
	while(!large->bit() && !large->bit(small->ptr))
	{
		resultshift++;
		large->shiftleft(small->ptr);
	}

	small->bitmask=large->bitmask;
	small->digitshift=large->digitshift;
	small->shift=large->shift;
	small->index=large->index;

// remove remaining trailing zeros from whichever has any
	if(!large->bit())
	{
		do
			large->shiftleft();
		while(!large->bit());
	}
	else
	while(!small->bit())
		small->shiftleft();

	{
	// determine number of extra digits and bits in large value
		tUnsigned32 digitsleft1,digitsleft2;
		int bitsleft1,bitsleft2;
		large->getbitsleft(digitsleft1,bitsleft1);
		small->getbitsleft(digitsleft2,bitsleft2);

		extradigits=digitsleft1-digitsleft2;
		extrabits=bitsleft1-bitsleft2;
	}

	equal=0;

	// swap if necessary
	if(extrabits<0)
	{
		extradigits--;
		extrabits+=32;
	}
	if(extradigits<0)
	{
		// swap
		extradigits=-extradigits;
		if(extrabits)
		{
			extrabits=32-extrabits;
			extradigits--;
		}
		temp=large;
		large=small;
		small=temp;
	}

	// check for equality
	if(extradigits==0 && extrabits==0)
	{
		int result;
		if(large->shift>=small->shift)
			result=large->value.comparebitstring(large->ptr,large->index,small->value,large->shift-small->shift);
		else
			result=-small->value.comparebitstring(small->ptr,small->index,large->value,small->shift-large->shift);
		if(result==1)
		{
			// swap
			temp=large;
			large=small;
			small=temp;
		}
		equal=(int)(result==0);
	}

	while(!equal)
	{
		// subtract small from large
		// update extra digits and extra bits accordingly
		//.. optimise - return digits lost in normalise operation from subtractbitstring
		tUnsigned32 digitsremoved;
		int bitsremoved;
		digitsremoved=large->value.topdigit;
		bitsremoved=large->gettopbit();
//#ifdef ASSERTIONS_ON
////.. slows down debug verion
//		cDynamicInteger beforesubtract=large->value;
//#endif;
		large->value.subtractbitstring(large->ptr, large->index, small->value, small->ptr, small->index, large->shift-small->shift);
		digitsremoved-=large->value.topdigit;
		bitsremoved-=large->gettopbit();
		if(bitsremoved<0)
		{
			bitsremoved+=32;
			assertD(digitsremoved);
			digitsremoved--;
		}
		extradigits-=digitsremoved;
		extrabits-=bitsremoved;
		if(extrabits<0)
		{
			extrabits+=32;
			extradigits--;
		}
		else
		if(extrabits>=32)
		{
			extrabits-=32;
			extradigits++;
		}
		assertD(extrabits>=0 && extrabits<32);

		// remove trailing zeros on large resulting from this subtraction
		do
		{
			large->shiftleft();
			extrabits--;
			if(extrabits<0)
			{
				extrabits+=32;
				extradigits--;
			}
		}
		while(!large->bit());

		// swap if necessary
		if(extrabits<0)
		{
			extradigits--;
			extrabits+=32;
		}
		if(extradigits<0)
		{
			// swap
			extradigits=-extradigits;
			if(extrabits)
			{
				extrabits=32-extrabits;
				extradigits--;
			}
			temp=large;
			large=small;
			small=temp;
		}

		// check for equality
		if(extradigits==0 && extrabits==0)
		{
			int result;
			if(large->shift>=small->shift)
				result=large->value.comparebitstring(large->ptr,large->index,small->value,large->shift-small->shift);
			else
				result=-small->value.comparebitstring(small->ptr,small->index,large->value,small->shift-large->shift);
			if(result==1)
			{
				// swap
				temp=large;
				large=small;
				small=temp;
			}
			equal=(int)(result==0);
		}
	}

	//.. optimise - shift and subtract
	while(resultshift>=32)
	{
		assertD(large->digitshift);
		large->digitshift--;
		resultshift-=32;
	}
	while(resultshift)
	{
		large->shift--;
		if(large->shift<0)
		{
			large->shift+=32;
			assertD(large->digitshift);
			large->digitshift--;
		}
		resultshift--;
	}

	//.. optimise - barrel shift
	int i;
//	int j;
//	for(i=0;i<(int)large->digitshift;i++)
//		for(j=0;j<32;j++)
//			large->value.shiftright();
	for(i=0;i<(int)large->digitshift;i++)
		large->value.shiftdigitright();
//	for(i=0;i<large->shift;i++)
//		large->value.shiftright();
	if(large->shift)
		large->value.shiftbitsright(large->shift);
	return large->value;
}

//cDynamicInteger highestcommonfactor( const cDynamicInteger &d1, const cDynamicInteger &d2)
//{
//	static profileentry aprofile("highestcommonfactor");
//	profiler aprofiler(&aprofile);
//
////VtResumeSampling();
//
//	cDynamicInteger result((tUnsigned32)1);
//	cDynamicInteger absd1,absd2;
//
//	assertD(!d1.isZero() && !d2.isZero());
//
//	absd1=d1;
//	if(absd1.isNegative())
//		absd1.negate();
//	absd2=d2;
//	if(absd2.isNegative())
//		absd2.negate();
//
//	while(absd1.isEven() && absd2.isEven())
//	{
//		absd1.shiftright();
//		absd2.shiftright();
//		result.shiftleft();
//	}
//
//	while(!absd1.isZero() && !absd2.isZero())
//	{
//		if(absd1.isEven())
//			absd1.shiftright();
//		else
//		if(absd2.isEven())
//			absd2.shiftright();
//		else
//		if(absd1>absd2)
//		{
//			absd1-=absd2;
//			assertD(absd1.isEven());
//			absd1.shiftright();
//		}
//		else
//		{
//			absd2-=absd1;
//			//.. might be zero
//			assertD(absd2.isEven());
//			absd2.shiftright();
//		}
//	}
//
//	if(absd1.isZero())
//		result*=absd2;
//	else
//		result*=absd1;
//
////VtPauseSampling();
//
//	return result;
//}

