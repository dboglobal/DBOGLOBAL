#ifndef __FIELD_H__
#define __FIELD_H__

class Field
{
public:

	inline void SetValue(char* value) { mValue = value; }

	inline const char *GetString() { return mValue ? mValue : ""; }
	inline float GetFloat() { return mValue ? static_cast<float>(atof(mValue)) : 0; }
	inline bool GetBool() { return mValue ? atoi(mValue) > 0 : false; }
	inline BYTE GetBYTE() { return mValue ? static_cast<BYTE>(atol(mValue)) : 0; }
	inline WORD GetWORD() { return mValue ? static_cast<WORD>(atol(mValue)) : 0; }
	inline int GetINT() { return mValue ? static_cast<int>(atol(mValue)) : 0; }
	inline unsigned int GetUInt32() { return mValue ? static_cast<unsigned int>(atoll(mValue)) : 0; }
	inline DWORD GetDWORD() { return mValue ? static_cast<DWORD>(atoll(mValue)) : 0; }
	UINT64 GetUInt64()
	{
		if (mValue)
		{
			UINT64 value;
			sscanf_s(mValue, "%I64u", &value);
			return value;
		}
		else
			return 0;
	}

private:
	char *mValue;
};

#endif