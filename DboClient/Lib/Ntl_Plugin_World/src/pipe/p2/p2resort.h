#ifndef _INC_RESORT_H

#define _INC_RESORT_H

#ifdef    __cplusplus
extern "C"
{
#endif /* __cplusplus */

extern void
_rx_rxRadixExchangeSort(void *elements, 
                  RwUInt32 numElements, 
                  RwUInt32 elementSize,
                  RwUInt32 elementKeyOffset,
                  RwUInt32 keyLo, 
                  RwUInt32 keyHi);

/*
 * a less sophisticated alternative, but good news when swap cost is high,
 * coz _rxSelectionSort performs many less swaps than other algorithms.
 * it should be pointed out, though, that on large data sets (1000+ items),
 * _rxSelectionSort() will perform horribly 
 */

extern void
_rxSelectionSort(void *elements, 
              RwUInt32 numElements, 
              RwUInt32 elementSize,
              RwUInt32 elementKeyOffset,
              RwUInt32 keyLo, 
              RwUInt32 keyHi);

#ifdef    __cplusplus
}
#endif /* __cplusplus */

#endif /* _INC_RESORT_H */
