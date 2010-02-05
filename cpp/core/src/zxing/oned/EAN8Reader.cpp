/*
 *  EAN8Reader.cpp
 *  ZXing
 *
 *  Created by Lukasz Warchol on 10-01-25.
 *  Copyright 2010 ZXing authors All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "EAN8Reader.h"
#include <zxing/ReaderException.h>

namespace zxing {
	namespace oned {
		
		EAN8Reader::EAN8Reader(){
			decodeMiddleCounters = new int[4];
			for (int i=0; i<4; i++) {
				decodeMiddleCounters[i] = 0;
			}
		}
		
		int EAN8Reader::decodeMiddle(Ref<BitArray> row, int startRange[], int startRangeLen, std::string& resultString){
			int countersLen = 4;
			int* counters = decodeMiddleCounters;
			counters[0] = 0;
			counters[1] = 0;
			counters[2] = 0;
			counters[3] = 0;
			
			
			int end = row->getSize();
			int rowOffset = startRange[1];
			
			for (int x = 0; x < 4 && rowOffset < end; x++) {
				int bestMatch = decodeDigit(row, counters, countersLen, rowOffset, UPC_EAN_PATTERNS_L_PATTERNS);
				resultString.append(1, (char) ('0' + bestMatch));
				for (int i = 0; i < countersLen; i++) {
					rowOffset += counters[i];
				}
			}
			
			int* middleRange = findGuardPattern(row, rowOffset, true, (int*)getMIDDLE_PATTERN(), getMIDDLE_PATTERN_LEN());
			rowOffset = middleRange[1];
			
			for (int x = 0; x < 4 && rowOffset < end; x++) {
				int bestMatch = decodeDigit(row, counters, countersLen, rowOffset, UPC_EAN_PATTERNS_L_PATTERNS);
				resultString.append(1, (char) ('0' + bestMatch));
				for (int i = 0; i < countersLen; i++) {
					rowOffset += counters[i];
				}
			}
			
			return rowOffset;
		}
		
		BarcodeFormat EAN8Reader::getBarcodeFormat(){
			return BarcodeFormat_EAN_8;
		}
		EAN8Reader::~EAN8Reader(){
			delete [] decodeMiddleCounters;
		}
	}
}