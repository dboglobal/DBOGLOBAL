//**********************************************************************
//
// Copyright (c) 2006
// PathEngine
// Lyon, France
//
// All Rights Reserved
//
//**********************************************************************

void
QuantisedRotationCoefficients(tSigned32 rotation, tSigned32& sinOf, tSigned32& cosOf);

inline tSigned32
QuantisedRotationDivisor()
{
    return 4096; // 2 ^ 12
}
inline tSigned32
QuantisedRotationAsPathEngineAngle(tSigned32 rotation)
{
    return rotation * (1048576 / 4096);
}
inline tSigned32
MaximumValueSupportedByQuantisedRotation()
{
    return 524287; // 2^19 - 1
}
inline tSigned32
QuantisedRotationNumberOfRotations()
{
    return 64;
}
