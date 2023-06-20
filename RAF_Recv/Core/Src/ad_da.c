/*
 * ad_da.c
 *
 *  Created on: Mar 28, 2022
 *      Author: ��־��
 */

/**
 * @brief �첨��ɼ���ѹ�������
 * @retval None
 */
#include "ad_da.h"
#include "main.h"
#include "base.h"
#include "tim.h"
#include "adc.h"
#include "dac.h"
#include "hmi_driver.h"
#include "hmi_user_uart.h"
#include "cmd_queue.h"
#include "cmd_process.h"

extern DMA_HandleTypeDef hdma_adc1;

uint16_t adData[ad_daLength];
uint16_t daData[ad_daLength];
uint8 uartData[ad_daLength];
float tempData[ad_daLength];
uint8_t getKey;
uint16_t Jag[2000]={
		0,3,6,9,12,15,18,21,24,27,30,33,36,39,42,
		45,48,51,54,57,60,63,66,69,72,75,78,81,84,87,
		90,93,96,99,102,105,108,111,114,117,120,123,126,129,132,
		135,138,141,144,147,150,153,156,159,162,165,168,171,174,177,
		180,183,186,189,192,195,198,201,204,207,210,213,216,219,222,
		225,228,231,234,237,240,243,246,249,252,255,258,261,264,267,
		270,273,276,279,282,285,288,291,294,297,300,303,306,309,312,
		315,318,321,324,327,330,333,336,339,342,345,348,351,354,357,
		360,363,366,369,372,375,378,381,384,387,390,393,396,399,402,
		405,408,411,414,417,420,423,426,429,432,435,438,441,444,447,
		450,453,456,459,462,465,468,471,474,477,480,483,486,489,492,
		495,498,501,504,507,510,513,516,519,522,525,528,531,534,537,
		540,543,546,549,552,555,558,561,564,567,570,573,576,579,582,
		585,588,591,594,597,600,603,606,609,612,615,618,621,624,627,
		630,633,636,639,642,645,648,651,654,657,660,663,666,669,672,
		675,678,681,684,687,690,693,696,699,702,705,708,711,714,717,
		720,723,726,729,732,735,738,741,744,747,750,753,756,759,762,
		765,768,771,774,777,780,783,786,789,792,795,798,801,804,807,
		810,813,816,819,822,825,828,831,834,837,840,843,846,849,852,
		855,858,861,864,867,870,873,876,879,882,885,888,891,894,897,
		900,903,906,909,912,915,918,921,924,927,930,933,936,939,942,
		945,948,951,954,957,960,963,966,969,972,975,978,981,984,987,
		990,993,996,999,1002,1005,1008,1011,1014,1017,1020,1023,1026,1029,1032,
		1035,1038,1041,1044,1047,1050,1053,1056,1059,1062,1065,1068,1071,1074,1077,
		1080,1083,1086,1089,1092,1095,1098,1101,1104,1107,1110,1113,1116,1119,1122,
		1125,1128,1131,1134,1137,1140,1143,1146,1149,1152,1155,1158,1161,1164,1167,
		1170,1173,1176,1179,1182,1185,1188,1191,1194,1197,1200,1203,1206,1209,1212,
		1215,1218,1221,1224,1227,1230,1233,1236,1239,1242,1245,1248,1251,1254,1257,
		1260,1263,1266,1269,1272,1275,1278,1281,1284,1287,1290,1293,1296,1299,1302,
		1305,1308,1311,1314,1317,1320,1323,1326,1329,1332,1335,1338,1341,1344,1347,
		1350,1353,1356,1359,1362,1365,1368,1371,1374,1377,1380,1383,1386,1389,1392,
		1395,1398,1401,1404,1407,1410,1413,1416,1419,1422,1425,1428,1431,1434,1437,
		1440,1443,1446,1449,1452,1455,1458,1461,1464,1467,1470,1473,1476,1479,1482,
		1485,1488,1491,1494,1497,1500,1503,1506,1509,1512,1515,1518,1521,1524,1527,
		1530,1533,1536,1539,1542,1545,1548,1551,1554,1557,1560,1563,1566,1569,1572,
		1575,1578,1581,1584,1587,1590,1593,1596,1599,1602,1605,1608,1611,1614,1617,
		1620,1623,1626,1629,1632,1635,1638,1641,1644,1647,1650,1653,1656,1659,1662,
		1665,1668,1671,1674,1677,1680,1683,1686,1689,1692,1695,1698,1701,1704,1707,
		1710,1713,1716,1719,1722,1725,1728,1731,1734,1737,1740,1743,1746,1749,1752,
		1755,1758,1761,1764,1767,1770,1773,1776,1779,1782,1785,1788,1791,1794,1797,
		1800,1803,1806,1809,1812,1815,1818,1821,1824,1827,1830,1833,1836,1839,1842,
		1845,1848,1851,1854,1857,1860,1863,1866,1869,1872,1875,1878,1881,1884,1887,
		1890,1893,1896,1899,1902,1905,1908,1911,1914,1917,1920,1923,1926,1929,1932,
		1935,1938,1941,1944,1947,1950,1953,1956,1959,1962,1965,1968,1971,1974,1977,
		1980,1983,1986,1989,1992,1995,1998,2001,2004,2007,2010,2013,2016,2019,2022,
		2025,2028,2031,2034,2037,2040,2043,2046,2049,2052,2055,2058,2061,2064,2067,
		2070,2073,2076,2079,2082,2085,2088,2091,2094,2097,2100,2103,2106,2109,2112,
		2115,2118,2121,2124,2127,2130,2133,2136,2139,2142,2145,2148,2151,2154,2157,
		2160,2163,2166,2169,2172,2175,2178,2181,2184,2187,2190,2193,2196,2199,2202,
		2205,2208,2211,2214,2217,2220,2223,2226,2229,2232,2235,2238,2241,2244,2247,
		2250,2253,2256,2259,2262,2265,2268,2271,2274,2277,2280,2283,2286,2289,2292,
		2295,2298,2301,2304,2307,2310,2313,2316,2319,2322,2325,2328,2331,2334,2337,
		2340,2343,2346,2349,2352,2355,2358,2361,2364,2367,2370,2373,2376,2379,2382,
		2385,2388,2391,2394,2397,2400,2403,2406,2409,2412,2415,2418,2421,2424,2427,
		2430,2433,2436,2439,2442,2445,2448,2451,2454,2457,2460,2463,2466,2469,2472,
		2475,2478,2481,2484,2487,2490,2493,2496,2499,2502,2505,2508,2511,2514,2517,
		2520,2523,2526,2529,2532,2535,2538,2541,2544,2547,2550,2553,2556,2559,2562,
		2565,2568,2571,2574,2577,2580,2583,2586,2589,2592,2595,2598,2601,2604,2607,
		2610,2613,2616,2619,2622,2625,2628,2631,2634,2637,2640,2643,2646,2649,2652,
		2655,2658,2661,2664,2667,2670,2673,2676,2679,2682,2685,2688,2691,2694,2697,
		2700,2703,2706,2709,2712,2715,2718,2721,2724,2727,2730,2733,2736,2739,2742,
		2745,2748,2751,2754,2757,2760,2763,2766,2769,2772,2775,2778,2781,2784,2787,
		2790,2793,2796,2799,2802,2805,2808,2811,2814,2817,2820,2823,2826,2829,2832,
		2835,2838,2841,2844,2847,2850,2853,2856,2859,2862,2865,2868,2871,2874,2877,
		2880,2883,2886,2889,2892,2895,2898,2901,2904,2907,2910,2913,2916,2919,2922,
		2925,2928,2931,2934,2937,2940,2943,2946,2949,2952,2955,2958,2961,2964,2967,
		2970,2973,2976,2979,2982,2985,2988,2991,2994,2997,
		3000,2997,2994,2991,2988,2985,2982,2979,2976,2973,2970,2967,2964,2961,2958,
		2955,2952,2949,2946,2943,2940,2937,2934,2931,2928,2925,2922,2919,2916,2913,
		2910,2907,2904,2901,2898,2895,2892,2889,2886,2883,2880,2877,2874,2871,2868,
		2865,2862,2859,2856,2853,2850,2847,2844,2841,2838,2835,2832,2829,2826,2823,
		2820,2817,2814,2811,2808,2805,2802,2799,2796,2793,2790,2787,2784,2781,2778,
		2775,2772,2769,2766,2763,2760,2757,2754,2751,2748,2745,2742,2739,2736,2733,
		2730,2727,2724,2721,2718,2715,2712,2709,2706,2703,2700,2697,2694,2691,2688,
		2685,2682,2679,2676,2673,2670,2667,2664,2661,2658,2655,2652,2649,2646,2643,
		2640,2637,2634,2631,2628,2625,2622,2619,2616,2613,2610,2607,2604,2601,2598,
		2595,2592,2589,2586,2583,2580,2577,2574,2571,2568,2565,2562,2559,2556,2553,
		2550,2547,2544,2541,2538,2535,2532,2529,2526,2523,2520,2517,2514,2511,2508,
		2505,2502,2499,2496,2493,2490,2487,2484,2481,2478,2475,2472,2469,2466,2463,
		2460,2457,2454,2451,2448,2445,2442,2439,2436,2433,2430,2427,2424,2421,2418,
		2415,2412,2409,2406,2403,2400,2397,2394,2391,2388,2385,2382,2379,2376,2373,
		2370,2367,2364,2361,2358,2355,2352,2349,2346,2343,2340,2337,2334,2331,2328,
		2325,2322,2319,2316,2313,2310,2307,2304,2301,2298,2295,2292,2289,2286,2283,
		2280,2277,2274,2271,2268,2265,2262,2259,2256,2253,2250,2247,2244,2241,2238,
		2235,2232,2229,2226,2223,2220,2217,2214,2211,2208,2205,2202,2199,2196,2193,
		2190,2187,2184,2181,2178,2175,2172,2169,2166,2163,2160,2157,2154,2151,2148,
		2145,2142,2139,2136,2133,2130,2127,2124,2121,2118,2115,2112,2109,2106,2103,
		2100,2097,2094,2091,2088,2085,2082,2079,2076,2073,2070,2067,2064,2061,2058,
		2055,2052,2049,2046,2043,2040,2037,2034,2031,2028,2025,2022,2019,2016,2013,
		2010,2007,2004,2001,1998,1995,1992,1989,1986,1983,1980,1977,1974,1971,1968,
		1965,1962,1959,1956,1953,1950,1947,1944,1941,1938,1935,1932,1929,1926,1923,
		1920,1917,1914,1911,1908,1905,1902,1899,1896,1893,1890,1887,1884,1881,1878,
		1875,1872,1869,1866,1863,1860,1857,1854,1851,1848,1845,1842,1839,1836,1833,
		1830,1827,1824,1821,1818,1815,1812,1809,1806,1803,1800,1797,1794,1791,1788,
		1785,1782,1779,1776,1773,1770,1767,1764,1761,1758,1755,1752,1749,1746,1743,
		1740,1737,1734,1731,1728,1725,1722,1719,1716,1713,1710,1707,1704,1701,1698,
		1695,1692,1689,1686,1683,1680,1677,1674,1671,1668,1665,1662,1659,1656,1653,
		1650,1647,1644,1641,1638,1635,1632,1629,1626,1623,1620,1617,1614,1611,1608,
		1605,1602,1599,1596,1593,1590,1587,1584,1581,1578,1575,1572,1569,1566,1563,
		1560,1557,1554,1551,1548,1545,1542,1539,1536,1533,1530,1527,1524,1521,1518,
		1515,1512,1509,1506,1503,1500,1497,1494,1491,1488,1485,1482,1479,1476,1473,
		1470,1467,1464,1461,1458,1455,1452,1449,1446,1443,1440,1437,1434,1431,1428,
		1425,1422,1419,1416,1413,1410,1407,1404,1401,1398,1395,1392,1389,1386,1383,
		1380,1377,1374,1371,1368,1365,1362,1359,1356,1353,1350,1347,1344,1341,1338,
		1335,1332,1329,1326,1323,1320,1317,1314,1311,1308,1305,1302,1299,1296,1293,
		1290,1287,1284,1281,1278,1275,1272,1269,1266,1263,1260,1257,1254,1251,1248,
		1245,1242,1239,1236,1233,1230,1227,1224,1221,1218,1215,1212,1209,1206,1203,
		1200,1197,1194,1191,1188,1185,1182,1179,1176,1173,1170,1167,1164,1161,1158,
		1155,1152,1149,1146,1143,1140,1137,1134,1131,1128,1125,1122,1119,1116,1113,
		1110,1107,1104,1101,1098,1095,1092,1089,1086,1083,1080,1077,1074,1071,1068,
		1065,1062,1059,1056,1053,1050,1047,1044,1041,1038,1035,1032,1029,1026,1023,
		1020,1017,1014,1011,1008,1005,1002,999,996,993,990,987,984,981,978,
		975,972,969,966,963,960,957,954,951,948,945,942,939,936,933,
		930,927,924,921,918,915,912,909,906,903,900,897,894,891,888,
		885,882,879,876,873,870,867,864,861,858,855,852,849,846,843,
		840,837,834,831,828,825,822,819,816,813,810,807,804,801,798,
		795,792,789,786,783,780,777,774,771,768,765,762,759,756,753,
		750,747,744,741,738,735,732,729,726,723,720,717,714,711,708,
		705,702,699,696,693,690,687,684,681,678,675,672,669,666,663,
		660,657,654,651,648,645,642,639,636,633,630,627,624,621,618,
		615,612,609,606,603,600,597,594,591,588,585,582,579,576,573,
		570,567,564,561,558,555,552,549,546,543,540,537,534,531,528,
		525,522,519,516,513,510,507,504,501,498,495,492,489,486,483,
		480,477,474,471,468,465,462,459,456,453,450,447,444,441,438,
		435,432,429,426,423,420,417,414,411,408,405,402,399,396,393,
		390,387,384,381,378,375,372,369,366,363,360,357,354,351,348,
		345,342,339,336,333,330,327,324,321,318,315,312,309,306,303,
		300,297,294,291,288,285,282,279,276,273,270,267,264,261,258,
		255,252,249,246,243,240,237,234,231,228,225,222,219,216,213,
		210,207,204,201,198,195,192,189,186,183,180,177,174,171,168,
		165,162,159,156,153,150,147,144,141,138,135,132,129,126,123,
		120,117,114,111,108,105,102,99,96,93,90,87,84,81,78,
				75,72,69,66,63,60,57,54,51,48,45,42,39,36,33,
				30,27,24,21,18,15,12,9,6,3
};

/**
 * @brief ADC_DACʹ�ܺ���
 * @retval None
 */
void ad_daStartGet(){

	uint16_t adMAX = 0;
	int Flag = 0;			//���ֵ��־λ

	HAL_TIM_Base_Start(&htim2);
	HAL_TIM_Base_Start(&htim4);

	while(1){
		getKey = KEY_Scan(0);
		if(getKey==1){
			LED0 = !LED0;
			HAL_ADC_Start_DMA(&hadc1,(uint32_t *)adData,ad_daLength);
			HAL_Delay(1000);
			break;
		}
	}

	/*���²���Ϊ��ADC�ɼ������ݽ�������ƫ��*/
	for(int i = 0;i < 40;i++){
		if(adData[i] > adMAX){
			adMAX = adData[i];
			Flag = i;
		}
	}
	for(int j = 0;j < 40;j++){
		daData[j] = (uint16_t)adData[(j+Flag)%80];
	}
//	daData[0]=daData[2];
//	daData[1]=daData[2];
	for(int k = 0;k < 80;k++){
		tempData[k] = (float)daData[k];
	}
	for(int m = 0;m < 80;m++){
		uartData[m] = (uint8)(tempData[m]/adMAX*200);
	}
	HAL_DAC_Start_DMA(&hdac,DAC1_CHANNEL_2,(uint32_t *)Jag,1000,DAC_ALIGN_12B_R);
	HAL_Delay(784);
	HAL_DAC_Start_DMA(&hdac,DAC1_CHANNEL_1,(uint32_t *)daData,ad_daLength/2,DAC_ALIGN_12B_R);
}

/**
 * @brief DMA����������
 * @retval None
 */
//void DMA2_Stream0_Handler(){
//
//	/*�ж��Ƿ�������*/
//	if(__HAL_DMA_GET_FLAG(&hdma_adc1,DMA_FLAG_TCIF1_5) != RESET){
//		__HAL_DMA_CLEAR_FLAG(&hdma_adc1,DMA_FLAG_TCIF1_5);		//���DMA������ɱ�־
//
//		for(int i = 40;i < 80;i++){
//			daData[i] = (uint16_t)adData[i];
//		}
//	}
//
//	/*�ж��Ƿ���հ����*/
//	if(__HAL_DMA_GET_FLAG(&hdma_adc1,DMA_FLAG_HTIF1_5) != RESET){
//		__HAL_DMA_CLEAR_FLAG(&hdma_adc1,DMA_FLAG_HTIF1_5);
//
//		for(int i = 0;i < 40;i++)
//		{
//			daData[i] = (uint16_t)adData[i];
//		}
//	}
//}