#include "stdafx.h"
#include "HexData.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

namespace HexData
{
	CString	Dec2HexRan (CString strCnv )
	{
		if ( strCnv == "1" )	return "1E";
		if ( strCnv == "2" )	return "17";
		if ( strCnv == "3" )	return "1B";
		if ( strCnv == "4" )	return "1D";
		if ( strCnv == "5" )	return "13";
		if ( strCnv == "6" )	return "1C";
		if ( strCnv == "7" )	return "16";
		if ( strCnv == "8" )	return "19";
		if ( strCnv == "9" )	return "09";
		if ( strCnv == "10" )	return "0A";
		if ( strCnv == "11" )	return "12";
		if ( strCnv == "12" )	return "1F";
		if ( strCnv == "13" )	return "0D";
		if ( strCnv == "14" )	return "07";
		if ( strCnv == "15" )	return "15";
		if ( strCnv == "16" )	return "7F";
		if ( strCnv == "17" )	return "10";
		if ( strCnv == "18" )	return "0E";
		if ( strCnv == "19" )	return "02";
		if ( strCnv == "20" )	return "18";
		if ( strCnv == "21" )	return "03";
		if ( strCnv == "22" )	return "04";
		if ( strCnv == "23" )	return "06";
		if ( strCnv == "24" )	return "05";
		if ( strCnv == "25" )	return "01";
		if ( strCnv == "26" )	return "1A";
		if ( strCnv == "27" )	return "08";
		if ( strCnv == "28" )	return "14";
		if ( strCnv == "29" )	return "0F";
		if ( strCnv == "30" )	return "0B";
		if ( strCnv == "31" )	return "0C";
		if ( strCnv == "32" )	return "20";
		if ( strCnv == "33" )	return "21";
		if ( strCnv == "34" )	return "22";
		if ( strCnv == "35" )	return "23";
		if ( strCnv == "36" )	return "24";
		if ( strCnv == "37" )	return "25";
		if ( strCnv == "38" )	return "26";
		if ( strCnv == "39" )	return "27";
		if ( strCnv == "40" )	return "28";
		if ( strCnv == "41" )	return "29";
		if ( strCnv == "42" )	return "2A";
		if ( strCnv == "43" )	return "2B";
		if ( strCnv == "44" )	return "2C";
		if ( strCnv == "45" )	return "2D";
		if ( strCnv == "46" )	return "2E";
		if ( strCnv == "47" )	return "2F";
		if ( strCnv == "48" )	return "30";
		if ( strCnv == "49" )	return "31";
		if ( strCnv == "50" )	return "32";
		if ( strCnv == "51" )	return "33";
		if ( strCnv == "52" )	return "34";
		if ( strCnv == "53" )	return "35";
		if ( strCnv == "54" )	return "36";
		if ( strCnv == "55" )	return "37";
		if ( strCnv == "56" )	return "38";
		if ( strCnv == "57" )	return "39";
		if ( strCnv == "58" )	return "3A";
		if ( strCnv == "59" )	return "3B";
		if ( strCnv == "60" )	return "3C";
		if ( strCnv == "61" )	return "3D";
		if ( strCnv == "62" )	return "3E";
		if ( strCnv == "63" )	return "3F";
		if ( strCnv == "64" )	return "40";
		if ( strCnv == "65" )	return "41";
		if ( strCnv == "66" )	return "42";
		if ( strCnv == "67" )	return "43";
		if ( strCnv == "68" )	return "44";
		if ( strCnv == "69" )	return "45";
		if ( strCnv == "70" )	return "46";
		if ( strCnv == "71" )	return "47";
		if ( strCnv == "72" )	return "48";
		if ( strCnv == "73" )	return "49";
		if ( strCnv == "74" )	return "4A";
		if ( strCnv == "75" )	return "4B";
		if ( strCnv == "76" )	return "4C";
		if ( strCnv == "77" )	return "4D";
		if ( strCnv == "78" )	return "4E";
		if ( strCnv == "79" )	return "4F";
		if ( strCnv == "80" )	return "50";
		if ( strCnv == "81" )	return "51";
		if ( strCnv == "82" )	return "52";
		if ( strCnv == "83" )	return "53";
		if ( strCnv == "84" )	return "54";
		if ( strCnv == "85" )	return "55";
		if ( strCnv == "86" )	return "56";
		if ( strCnv == "87" )	return "57";
		if ( strCnv == "88" )	return "58";
		if ( strCnv == "89" )	return "59";
		if ( strCnv == "90" )	return "5A";
		if ( strCnv == "91" )	return "5B";
		if ( strCnv == "92" )	return "5C";
		if ( strCnv == "93" )	return "5D";
		if ( strCnv == "94" )	return "5E";
		if ( strCnv == "95" )	return "5F";
		if ( strCnv == "96" )	return "60";
		if ( strCnv == "97" )	return "61";
		if ( strCnv == "98" )	return "62";
		if ( strCnv == "99" )	return "63";
		if ( strCnv == "100" )	return "64";
		if ( strCnv == "101" )	return "65";
		if ( strCnv == "102" )	return "66";
		if ( strCnv == "103" )	return "67";
		if ( strCnv == "104" )	return "68";
		if ( strCnv == "105" )	return "69";
		if ( strCnv == "106" )	return "6A";
		if ( strCnv == "107" )	return "6B";
		if ( strCnv == "108" )	return "6C";
		if ( strCnv == "109" )	return "6D";
		if ( strCnv == "110" )	return "6E";
		if ( strCnv == "111" )	return "6F";
		if ( strCnv == "112" )	return "70";
		if ( strCnv == "113" )	return "71";
		if ( strCnv == "114" )	return "72";
		if ( strCnv == "115" )	return "73";
		if ( strCnv == "116" )	return "74";
		if ( strCnv == "117" )	return "75";
		if ( strCnv == "118" )	return "76";
		if ( strCnv == "119" )	return "77";
		if ( strCnv == "120" )	return "78";
		if ( strCnv == "121" )	return "79";
		if ( strCnv == "122" )	return "7A";
		if ( strCnv == "123" )	return "7B";
		if ( strCnv == "124" )	return "7C";
		if ( strCnv == "125" )	return "7D";
		if ( strCnv == "126" )	return "7E";
		if ( strCnv == "127" )	return "7F";
		if ( strCnv == "128" )	return "80";
		if ( strCnv == "129" )	return "81";
		if ( strCnv == "130" )	return "82";
		if ( strCnv == "131" )	return "83";
		if ( strCnv == "132" )	return "84";
		if ( strCnv == "133" )	return "85";
		if ( strCnv == "134" )	return "86";
		if ( strCnv == "135" )	return "87";
		if ( strCnv == "136" )	return "88";
		if ( strCnv == "137" )	return "89";
		if ( strCnv == "138" )	return "8A";
		if ( strCnv == "139" )	return "8B";
		if ( strCnv == "140" )	return "8C";
		if ( strCnv == "141" )	return "8D";
		if ( strCnv == "142" )	return "8E";
		if ( strCnv == "143" )	return "8F";
		if ( strCnv == "144" )	return "90";
		if ( strCnv == "145" )	return "91";
		if ( strCnv == "146" )	return "92";
		if ( strCnv == "147" )	return "93";
		if ( strCnv == "148" )	return "94";
		if ( strCnv == "149" )	return "95";
		if ( strCnv == "150" )	return "96";
		if ( strCnv == "151" )	return "97";
		if ( strCnv == "152" )	return "98";
		if ( strCnv == "153" )	return "99";
		if ( strCnv == "154" )	return "9A";
		if ( strCnv == "155" )	return "9B";
		if ( strCnv == "156" )	return "9C";
		if ( strCnv == "157" )	return "9D";
		if ( strCnv == "158" )	return "9E";
		if ( strCnv == "159" )	return "9F";
		if ( strCnv == "160" )	return "A0";
		if ( strCnv == "161" )	return "A1";
		if ( strCnv == "162" )	return "A2";
		if ( strCnv == "163" )	return "A3";
		if ( strCnv == "164" )	return "A4";
		if ( strCnv == "165" )	return "A5";
		if ( strCnv == "166" )	return "A6";
		if ( strCnv == "167" )	return "A7";
		if ( strCnv == "168" )	return "A8";
		if ( strCnv == "169" )	return "A9";
		if ( strCnv == "170" )	return "AA";
		if ( strCnv == "171" )	return "AB";
		if ( strCnv == "172" )	return "AC";
		if ( strCnv == "173" )	return "AD";
		if ( strCnv == "174" )	return "AE";
		if ( strCnv == "175" )	return "AF";
		if ( strCnv == "176" )	return "B0";
		if ( strCnv == "177" )	return "B1";
		if ( strCnv == "178" )	return "B2";
		if ( strCnv == "179" )	return "B3";
		if ( strCnv == "180" )	return "B4";
		if ( strCnv == "181" )	return "B5";
		if ( strCnv == "182" )	return "B6";
		if ( strCnv == "183" )	return "B7";
		if ( strCnv == "184" )	return "B8";
		if ( strCnv == "185" )	return "B9";
		if ( strCnv == "186" )	return "BA";
		if ( strCnv == "187" )	return "BB";
		if ( strCnv == "188" )	return "BC";
		if ( strCnv == "189" )	return "BD";
		if ( strCnv == "190" )	return "BE";
		if ( strCnv == "191" )	return "BF";
		if ( strCnv == "192" )	return "C0";
		if ( strCnv == "193" )	return "C1";
		if ( strCnv == "194" )	return "C2";
		if ( strCnv == "195" )	return "C3";
		if ( strCnv == "196" )	return "C4";
		if ( strCnv == "197" )	return "C5";
		if ( strCnv == "198" )	return "C6";
		if ( strCnv == "199" )	return "C7";
		if ( strCnv == "200" )	return "C8";
		if ( strCnv == "201" )	return "C9";
		if ( strCnv == "202" )	return "CA";
		if ( strCnv == "203" )	return "CB";
		if ( strCnv == "204" )	return "CC";
		if ( strCnv == "205" )	return "CD";
		if ( strCnv == "206" )	return "CE";
		if ( strCnv == "207" )	return "CF";
		if ( strCnv == "208" )	return "D0";
		if ( strCnv == "209" )	return "D1";
		if ( strCnv == "210" )	return "D2";
		if ( strCnv == "211" )	return "D3";
		if ( strCnv == "212" )	return "D4";
		if ( strCnv == "213" )	return "D5";
		if ( strCnv == "214" )	return "D6";
		if ( strCnv == "215" )	return "D7";
		if ( strCnv == "216" )	return "D8";
		if ( strCnv == "217" )	return "D9";
		if ( strCnv == "218" )	return "DA";
		if ( strCnv == "219" )	return "DB";
		if ( strCnv == "220" )	return "DC";
		if ( strCnv == "221" )	return "DD";
		if ( strCnv == "222" )	return "DE";
		if ( strCnv == "223" )	return "DF";
		if ( strCnv == "224" )	return "E0";
		if ( strCnv == "225" )	return "E1";
		if ( strCnv == "226" )	return "E2";
		if ( strCnv == "227" )	return "E3";
		if ( strCnv == "228" )	return "E4";
		if ( strCnv == "229" )	return "E5";
		if ( strCnv == "230" )	return "E6";
		if ( strCnv == "231" )	return "E7";
		if ( strCnv == "232" )	return "E8";
		if ( strCnv == "233" )	return "E9";
		if ( strCnv == "234" )	return "EA";
		if ( strCnv == "235" )	return "EB";
		if ( strCnv == "236" )	return "EC";
		if ( strCnv == "237" )	return "ED";
		if ( strCnv == "238" )	return "EE";
		if ( strCnv == "239" )	return "EF";
		if ( strCnv == "240" )	return "F0";
		if ( strCnv == "241" )	return "F1";
		if ( strCnv == "242" )	return "F2";
		if ( strCnv == "243" )	return "F3";
		if ( strCnv == "244" )	return "F4";
		if ( strCnv == "245" )	return "F5";
		if ( strCnv == "246" )	return "F6";
		if ( strCnv == "247" )	return "F7";
		if ( strCnv == "248" )	return "F8";
		if ( strCnv == "249" )	return "F9";
		if ( strCnv == "250" )	return "FA";
		if ( strCnv == "251" )	return "FB";
		if ( strCnv == "252" )	return "FC";
		if ( strCnv == "253" )	return "FD";
		if ( strCnv == "254" )	return "FE";
		if ( strCnv == "255" )	return "FF";

		else	return "DataError";
	}

	CString	Hex2DecRan (CString strCnv )
	{
		if ( strCnv == "1E" )	return "1";
		if ( strCnv == "17" )	return "2";
		if ( strCnv == "1B" )	return "3";
		if ( strCnv == "1D" )	return "4";
		if ( strCnv == "13" )	return "5";
		if ( strCnv == "1C" )	return "6";
		if ( strCnv == "16" )	return "7";
		if ( strCnv == "19" )	return "8";
		if ( strCnv == "09" )	return "9";
		if ( strCnv == "9" )	return "9";
		if ( strCnv == "0A" )	return "10";
		if ( strCnv == "12" )	return "11";
		if ( strCnv == "1F" )	return "12";
		if ( strCnv == "0D" )	return "13";
		if ( strCnv == "07" )	return "14";
		if ( strCnv == "15" )	return "15";
		if ( strCnv == "7F" )	return "16";
		if ( strCnv == "10" )	return "17";
		if ( strCnv == "0E" )	return "18";
		if ( strCnv == "02" )	return "19";
		if ( strCnv == "2" )	return "19";
		if ( strCnv == "18" )	return "20";
		if ( strCnv == "03" )	return "21";
		if ( strCnv == "3" )	return "21";
		if ( strCnv == "04" )	return "22";
		if ( strCnv == "4" )	return "22";
		if ( strCnv == "06" )	return "23";
		if ( strCnv == "6" )	return "23";
		if ( strCnv == "05" )	return "24";
		if ( strCnv == "5" )	return "24";
		if ( strCnv == "01" )	return "25";
		if ( strCnv == "1" )	return "25";
		if ( strCnv == "1A" )	return "26";
		if ( strCnv == "08" )	return "27";
		if ( strCnv == "8" )	return "27";
		if ( strCnv == "14" )	return "28";
		if ( strCnv == "0F" )	return "29";
		if ( strCnv == "0B" )	return "30";
		if ( strCnv == "0C" )	return "31";
		if ( strCnv == "20" )	return "32";
		if ( strCnv == "21" )	return "33";
		if ( strCnv == "22" )	return "34";
		if ( strCnv == "23" )	return "35";
		if ( strCnv == "24" )	return "36";
		if ( strCnv == "25" )	return "37";
		if ( strCnv == "26" )	return "38";
		if ( strCnv == "27" )	return "39";
		if ( strCnv == "28" )	return "40";
		if ( strCnv == "29" )	return "41";
		if ( strCnv == "2A" )	return "42";
		if ( strCnv == "2B" )	return "43";
		if ( strCnv == "2C" )	return "44";
		if ( strCnv == "2D" )	return "45";
		if ( strCnv == "2E" )	return "46";
		if ( strCnv == "2F" )	return "47";
		if ( strCnv == "30" )	return "48";
		if ( strCnv == "31" )	return "49";
		if ( strCnv == "32" )	return "50";
		if ( strCnv == "33" )	return "51";
		if ( strCnv == "34" )	return "52";
		if ( strCnv == "35" )	return "53";
		if ( strCnv == "36" )	return "54";
		if ( strCnv == "37" )	return "55";
		if ( strCnv == "38" )	return "56";
		if ( strCnv == "39" )	return "57";
		if ( strCnv == "3A" )	return "58";
		if ( strCnv == "3B" )	return "59";
		if ( strCnv == "3C" )	return "60";
		if ( strCnv == "3D" )	return "61";
		if ( strCnv == "3E" )	return "62";
		if ( strCnv == "3F" )	return "63";
		if ( strCnv == "40" )	return "64";
		if ( strCnv == "41" )	return "65";
		if ( strCnv == "42" )	return "66";
		if ( strCnv == "43" )	return "67";
		if ( strCnv == "44" )	return "68";
		if ( strCnv == "45" )	return "69";
		if ( strCnv == "46" )	return "70";
		if ( strCnv == "47" )	return "71";
		if ( strCnv == "48" )	return "72";
		if ( strCnv == "49" )	return "73";
		if ( strCnv == "4A" )	return "74";
		if ( strCnv == "4B" )	return "75";
		if ( strCnv == "4C" )	return "76";
		if ( strCnv == "4D" )	return "77";
		if ( strCnv == "4E" )	return "78";
		if ( strCnv == "4F" )	return "79";
		if ( strCnv == "50" )	return "80";
		if ( strCnv == "51" )	return "81";
		if ( strCnv == "52" )	return "82";
		if ( strCnv == "53" )	return "83";
		if ( strCnv == "54" )	return "84";
		if ( strCnv == "55" )	return "85";
		if ( strCnv == "56" )	return "86";
		if ( strCnv == "57" )	return "87";
		if ( strCnv == "58" )	return "88";
		if ( strCnv == "59" )	return "89";
		if ( strCnv == "5A" )	return "90";
		if ( strCnv == "5B" )	return "91";
		if ( strCnv == "5C" )	return "92";
		if ( strCnv == "5D" )	return "93";
		if ( strCnv == "5E" )	return "94";
		if ( strCnv == "5F" )	return "95";
		if ( strCnv == "60" )	return "96";
		if ( strCnv == "61" )	return "97";
		if ( strCnv == "62" )	return "98";
		if ( strCnv == "63" )	return "99";
		if ( strCnv == "64" )	return "100";
		if ( strCnv == "65" )	return "101";
		if ( strCnv == "66" )	return "102";
		if ( strCnv == "67" )	return "103";
		if ( strCnv == "68" )	return "104";
		if ( strCnv == "69" )	return "105";
		if ( strCnv == "6A" )	return "106";
		if ( strCnv == "6B" )	return "107";
		if ( strCnv == "6C" )	return "108";
		if ( strCnv == "6D" )	return "109";
		if ( strCnv == "6E" )	return "110";
		if ( strCnv == "6F" )	return "111";
		if ( strCnv == "70" )	return "112";
		if ( strCnv == "71" )	return "113";
		if ( strCnv == "72" )	return "114";
		if ( strCnv == "73" )	return "115";
		if ( strCnv == "74" )	return "116";
		if ( strCnv == "75" )	return "117";
		if ( strCnv == "76" )	return "118";
		if ( strCnv == "77" )	return "119";
		if ( strCnv == "78" )	return "120";
		if ( strCnv == "79" )	return "121";
		if ( strCnv == "7A" )	return "122";
		if ( strCnv == "7B" )	return "123";
		if ( strCnv == "7C" )	return "124";
		if ( strCnv == "7D" )	return "125";
		if ( strCnv == "7E" )	return "126";
		if ( strCnv == "7F" )	return "127";
		if ( strCnv == "80" )	return "128";
		if ( strCnv == "81" )	return "129";
		if ( strCnv == "82" )	return "130";
		if ( strCnv == "83" )	return "131";
		if ( strCnv == "84" )	return "132";
		if ( strCnv == "85" )	return "133";
		if ( strCnv == "86" )	return "134";
		if ( strCnv == "87" )	return "135";
		if ( strCnv == "88" )	return "136";
		if ( strCnv == "89" )	return "137";
		if ( strCnv == "8A" )	return "138";
		if ( strCnv == "8B" )	return "139";
		if ( strCnv == "8C" )	return "140";
		if ( strCnv == "8D" )	return "141";
		if ( strCnv == "8E" )	return "142";
		if ( strCnv == "8F" )	return "143";
		if ( strCnv == "90" )	return "144";
		if ( strCnv == "91" )	return "145";
		if ( strCnv == "92" )	return "146";
		if ( strCnv == "93" )	return "147";
		if ( strCnv == "94" )	return "148";
		if ( strCnv == "95" )	return "149";
		if ( strCnv == "96" )	return "150";
		if ( strCnv == "97" )	return "151";
		if ( strCnv == "98" )	return "152";
		if ( strCnv == "99" )	return "153";
		if ( strCnv == "9A" )	return "154";
		if ( strCnv == "9B" )	return "155";
		if ( strCnv == "9C" )	return "156";
		if ( strCnv == "9D" )	return "157";
		if ( strCnv == "9E" )	return "158";
		if ( strCnv == "9F" )	return "159";
		if ( strCnv == "A0" )	return "160";
		if ( strCnv == "A1" )	return "161";
		if ( strCnv == "A2" )	return "162";
		if ( strCnv == "A3" )	return "163";
		if ( strCnv == "A4" )	return "164";
		if ( strCnv == "A5" )	return "165";
		if ( strCnv == "A6" )	return "166";
		if ( strCnv == "A7" )	return "167";
		if ( strCnv == "A8" )	return "168";
		if ( strCnv == "A9" )	return "169";
		if ( strCnv == "AA" )	return "170";
		if ( strCnv == "AB" )	return "171";
		if ( strCnv == "AC" )	return "172";
		if ( strCnv == "AD" )	return "173";
		if ( strCnv == "AE" )	return "174";
		if ( strCnv == "AF" )	return "175";
		if ( strCnv == "B0" )	return "176";
		if ( strCnv == "B1" )	return "177";
		if ( strCnv == "B2" )	return "178";
		if ( strCnv == "B3" )	return "179";
		if ( strCnv == "B4" )	return "180";
		if ( strCnv == "B5" )	return "181";
		if ( strCnv == "B6" )	return "182";
		if ( strCnv == "B7" )	return "183";
		if ( strCnv == "B8" )	return "184";
		if ( strCnv == "B9" )	return "185";
		if ( strCnv == "BA" )	return "186";
		if ( strCnv == "BB" )	return "187";
		if ( strCnv == "BC" )	return "188";
		if ( strCnv == "BD" )	return "189";
		if ( strCnv == "BE" )	return "190";
		if ( strCnv == "BF" )	return "191";
		if ( strCnv == "C0" )	return "192";
		if ( strCnv == "C1" )	return "193";
		if ( strCnv == "C2" )	return "194";
		if ( strCnv == "C3" )	return "195";
		if ( strCnv == "C4" )	return "196";
		if ( strCnv == "C5" )	return "197";
		if ( strCnv == "C6" )	return "198";
		if ( strCnv == "C7" )	return "199";
		if ( strCnv == "C8" )	return "200";
		if ( strCnv == "C9" )	return "201";
		if ( strCnv == "CA" )	return "202";
		if ( strCnv == "CB" )	return "203";
		if ( strCnv == "CC" )	return "204";
		if ( strCnv == "CD" )	return "205";
		if ( strCnv == "CE" )	return "206";
		if ( strCnv == "CF" )	return "207";
		if ( strCnv == "D0" )	return "208";
		if ( strCnv == "D1" )	return "209";
		if ( strCnv == "D2" )	return "210";
		if ( strCnv == "D3" )	return "211";
		if ( strCnv == "D4" )	return "212";
		if ( strCnv == "D5" )	return "213";
		if ( strCnv == "D6" )	return "214";
		if ( strCnv == "D7" )	return "215";
		if ( strCnv == "D8" )	return "216";
		if ( strCnv == "D9" )	return "217";
		if ( strCnv == "DA" )	return "218";
		if ( strCnv == "DB" )	return "219";
		if ( strCnv == "DC" )	return "220";
		if ( strCnv == "DD" )	return "221";
		if ( strCnv == "DE" )	return "222";
		if ( strCnv == "DF" )	return "223";
		if ( strCnv == "E0" )	return "224";
		if ( strCnv == "E1" )	return "225";
		if ( strCnv == "E2" )	return "226";
		if ( strCnv == "E3" )	return "227";
		if ( strCnv == "E4" )	return "228";
		if ( strCnv == "E5" )	return "229";
		if ( strCnv == "E6" )	return "230";
		if ( strCnv == "E7" )	return "231";
		if ( strCnv == "E8" )	return "232";
		if ( strCnv == "E9" )	return "233";
		if ( strCnv == "EA" )	return "234";
		if ( strCnv == "EB" )	return "235";
		if ( strCnv == "EC" )	return "236";
		if ( strCnv == "ED" )	return "237";
		if ( strCnv == "EE" )	return "238";
		if ( strCnv == "EF" )	return "239";
		if ( strCnv == "F0" )	return "240";
		if ( strCnv == "F1" )	return "241";
		if ( strCnv == "F2" )	return "242";
		if ( strCnv == "F3" )	return "243";
		if ( strCnv == "F4" )	return "244";
		if ( strCnv == "F5" )	return "245";
		if ( strCnv == "F6" )	return "246";
		if ( strCnv == "F7" )	return "247";
		if ( strCnv == "F8" )	return "248";
		if ( strCnv == "F9" )	return "249";
		if ( strCnv == "FA" )	return "250";
		if ( strCnv == "FB" )	return "251";
		if ( strCnv == "FC" )	return "252";
		if ( strCnv == "FD" )	return "253";
		if ( strCnv == "FE" )	return "254";
		if ( strCnv == "FF" )	return "255";

		else	return "DataError";
	}

	CString	Dec2HexNor (CString strCnv )
	{
		int nVal = (int) atoi ( strCnv.GetString() );
		CString strTemp;
		strTemp.Format ("%02x",nVal);

		return strTemp;
	}

	CString	Hex2DecNor (CString strCnv )
	{
		std::string s = strCnv.GetString();
		std::stringstream ss(s);
		int i;
		ss >> std::hex >> i;
		CString strTemp;
		strTemp.Format ("%02d", i );
		return strTemp;
	}
};
