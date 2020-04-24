CNtlTSTrigger
{
	sm = 0;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 11606;
	title = 1100403;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 105;
			elnk = 255;
			nextlnk = 36;
			prelnk = "104;";

			CDboTSActTWaitTS
			{
				taid = 1;
				time = 3000;
			}
		}
		CDboTSContGAct
		{
			cid = 104;
			elnk = 255;
			nextlnk = 105;
			prelnk = "36;";

			CDboTSActPCConv
			{
				taid = 1;
				tblidx = 1101059;
			}
		}
		CDboTSContGAct
		{
			cid = 103;
			elnk = 255;
			nextlnk = 9;
			prelnk = "102;";

			CDboTSActTWaitTS
			{
				taid = 1;
				time = 3000;
			}
		}
		CDboTSContGAct
		{
			cid = 102;
			elnk = 255;
			nextlnk = 103;
			prelnk = "9;";

			CDboTSActPCConv
			{
				taid = 1;
				tblidx = 1101059;
			}
		}
		CDboTSContGAct
		{
			cid = 101;
			elnk = 255;
			nextlnk = 10;
			prelnk = "100;";

			CDboTSActTWaitTS
			{
				taid = 1;
				time = 3000;
			}
		}
		CDboTSContGAct
		{
			cid = 100;
			elnk = 255;
			nextlnk = 101;
			prelnk = "10;";

			CDboTSActPCConv
			{
				taid = 1;
				tblidx = 1101059;
			}
		}
		CDboTSContGCond
		{
			cid = 44;
			prelnk = "24;";
			nolnk = 255;
			rm = 0;
			yeslnk = 45;

			CDboTSRcvSvrEvt
			{
				id = 16310;
			}
		}
		CDboTSContGAct
		{
			cid = 45;
			elnk = 255;
			nextlnk = 254;
			prelnk = "44;";

			CDboTSActSendSvrEvt
			{
				id = 16230;
				tblidx = -1;
				stradius = 0;
				stype = 0;
				taid = 1;
				ttype = 0;
				type = 5;
			}
		}
		CDboTSContGCond
		{
			cid = 41;
			prelnk = "26;20;22;21;19;18;17;16;15;14;13;";
			nolnk = 255;
			rm = 0;
			yeslnk = 13;

			CDboTSClickNPC
			{
				npcidx = 6202101;
			}
			CDboTSCheckProgQuest
			{
				qid = 11610;
			}
		}
		CDboTSContGAct
		{
			cid = 36;
			elnk = 104;
			nextlnk = 11;
			prelnk = "105;8;";

			CDboTSActRegQInfo
			{
				cont = 1100405;
				gtype = -1;
				area = 1100407;
				goal = 1100404;
				m0fz = "-963.710022";
				m0widx = 160000;
				sort = 1100406;
				stype = 32;
				m0fy = "0.000000";
				m0pat = 2;
				grade = -1;
				m0fx = "915.429993";
				m0ttip = 1100403;
				rwd = 255;
				taid = 2;
				title = 1100403;
			}
			CDboTSActSToCEvt
			{
				apptype = 1;
				cd0 = 92009;
				eitype = 5;
				idx0 = -1;
				cnt0 = 0;
				ectype = 0;
				etype = 0;
				qtidx = -1;
				taid = 1;
				esctype = 0;
			}
		}
		CDboTSContGCond
		{
			cid = 34;
			prelnk = "2;";
			nolnk = 35;
			rm = 0;
			yeslnk = 4;

			CDboTSCheckInWorld
			{
				widx = 150000;
			}
		}
		CDboTSContGCond
		{
			cid = 6;
			prelnk = "7;";
			nolnk = 8;
			rm = 0;
			yeslnk = 10;

			CDboTSCheckInWorld
			{
				widx = 120000;
			}
		}
		CDboTSContGCond
		{
			cid = 35;
			prelnk = "34;";
			nolnk = 255;
			rm = 0;
			yeslnk = 5;

			CDboTSClickObject
			{
				objidx = "6;";
				widx = 160000;
			}
		}
		CDboTSContGCond
		{
			cid = 2;
			prelnk = "1;1;";
			nolnk = 34;
			rm = 0;
			yeslnk = 3;

			CDboTSCheckInWorld
			{
				widx = 120000;
			}
		}
		CDboTSContGAct
		{
			cid = 24;
			elnk = 255;
			nextlnk = 44;
			prelnk = "23;23;";

			CDboTSActSendSvrEvt
			{
				id = 16180;
				tblidx = 6202101;
				stradius = 0;
				stype = 0;
				taid = 1;
				ttype = 0;
				type = 0;
			}
			CDboTSActSendSvrEvt
			{
				id = 16150;
				tblidx = -1;
				stradius = 0;
				stype = 0;
				taid = 2;
				ttype = 0;
				type = 5;
			}
			CDboTSActSendSvrEvt
			{
				id = 16220;
				tblidx = -1;
				stradius = 0;
				stype = 0;
				taid = 3;
				ttype = 0;
				type = 5;
			}
		}
		CDboTSContUsrSel
		{
			cancellnk = 41;
			cid = 20;
			lilnk = 255;
			ndesc0 = 1100417;
			uspt = 2;
			desc = -1;
			type = -1;
			ust = 1;
			idx = -1;
			nid1 = 22;
			prelnk = "19;";
			ndesc1 = 1100416;
			nid0 = 21;
		}
		CDboTSContGCond
		{
			cid = 4;
			prelnk = "34;";
			nolnk = 255;
			rm = 0;
			yeslnk = 5;

			CDboTSClickObject
			{
				objidx = "6;";
				widx = 150000;
			}
		}
		CDboTSContGCond
		{
			cid = 8;
			prelnk = "6;";
			nolnk = 36;
			rm = 0;
			yeslnk = 9;

			CDboTSCheckInWorld
			{
				widx = 150000;
			}
		}
		CDboTSContGAct
		{
			cid = 9;
			elnk = 102;
			nextlnk = 11;
			prelnk = "103;8;";

			CDboTSActRegQInfo
			{
				cont = 1100405;
				gtype = -1;
				area = 1100407;
				goal = 1100404;
				m0fz = "-963.710022";
				m0widx = 150000;
				sort = 1100406;
				stype = 32;
				m0fy = "0.000000";
				m0pat = 2;
				grade = -1;
				m0fx = "915.429993";
				m0ttip = 1100403;
				rwd = 255;
				taid = 2;
				title = 1100403;
			}
			CDboTSActSToCEvt
			{
				apptype = 1;
				cd0 = 92009;
				eitype = 5;
				idx0 = -1;
				cnt0 = 0;
				ectype = 0;
				etype = 0;
				qtidx = -1;
				taid = 1;
				esctype = 0;
			}
		}
		CDboTSContReward
		{
			canclnk = 41;
			cid = 26;
			desc = 1100763;
			nextlnk = 23;
			rwdtbl = 1160601;
			rwdtype = 0;
			ltime = -1;
			prelnk = "22;";
			usetbl = 1;
		}
		CDboTSContNarration
		{
			cancellnk = 24;
			cid = 23;
			dt = 2;
			lilnk = 255;
			oklnk = 24;
			ot = 0;
			pt = 3;
			mlt = 10000;
			os = 0;
			prelnk = "26;";
			dg = 1100421;
			gt = 0;
			oi = 6201101;
		}
		CDboTSContGAct
		{
			cid = 7;
			elnk = 255;
			nextlnk = 6;
			prelnk = "5;5;";

			CDboTSActSendSvrEvt
			{
				id = 16110;
				tblidx = 11604;
				stradius = 0;
				stype = 0;
				taid = 1;
				ttype = 255;
				type = 6;
			}
			CDboTSActSendSvrEvt
			{
				id = 16130;
				tblidx = -1;
				stradius = 0;
				stype = 0;
				taid = 2;
				ttype = 0;
				type = 5;
			}
		}
		CDboTSContGCond
		{
			cid = 3;
			prelnk = "2;";
			nolnk = 255;
			rm = 0;
			yeslnk = 5;

			CDboTSClickObject
			{
				objidx = "6;";
				widx = 120000;
			}
		}
		CDboTSContNarration
		{
			cancellnk = 41;
			cid = 21;
			dt = 2;
			lilnk = 255;
			oklnk = 16;
			ot = 0;
			pt = 1;
			mlt = 10000;
			os = 0;
			prelnk = "20;20;";
			dg = 1100420;
			gt = 0;
			oi = 6201101;
		}
		CDboTSContNarration
		{
			cancellnk = 41;
			cid = 22;
			dt = 2;
			lilnk = 255;
			oklnk = 26;
			ot = 0;
			pt = 2;
			mlt = 10000;
			os = 0;
			prelnk = "20;";
			dg = 1100419;
			gt = 0;
			oi = 6201101;
		}
		CDboTSContNarration
		{
			cancellnk = 41;
			cid = 19;
			dt = 2;
			lilnk = 255;
			oklnk = 20;
			ot = 0;
			pt = 1;
			mlt = 10000;
			os = 0;
			prelnk = "18;";
			dg = 1100415;
			gt = 0;
			oi = 6201101;
		}
		CDboTSContNarration
		{
			cancellnk = 41;
			cid = 18;
			dt = 2;
			lilnk = 255;
			oklnk = 19;
			ot = 0;
			pt = 1;
			mlt = 10000;
			os = 0;
			prelnk = "17;";
			dg = 1100414;
			gt = 0;
			oi = 6201101;
		}
		CDboTSContNarration
		{
			cancellnk = 41;
			cid = 17;
			dt = 2;
			lilnk = 255;
			oklnk = 18;
			ot = 0;
			pt = 1;
			mlt = 10000;
			os = 0;
			prelnk = "16;";
			dg = 1100413;
			gt = 0;
			oi = 6201101;
		}
		CDboTSContNarration
		{
			cancellnk = 41;
			cid = 16;
			dt = 2;
			lilnk = 255;
			oklnk = 17;
			ot = 2;
			pt = 0;
			mlt = 10000;
			os = 0;
			prelnk = "21;15;";
			dg = 1100412;
			gt = 0;
			oi = -1;
		}
		CDboTSContNarration
		{
			cancellnk = 41;
			cid = 15;
			dt = 2;
			lilnk = 255;
			oklnk = 16;
			ot = 0;
			pt = 1;
			mlt = 10000;
			os = 0;
			prelnk = "14;";
			dg = 1100411;
			gt = 0;
			oi = 6201101;
		}
		CDboTSContNarration
		{
			cancellnk = 41;
			cid = 14;
			dt = 2;
			lilnk = 255;
			oklnk = 15;
			ot = 0;
			pt = 1;
			mlt = 10000;
			os = 0;
			prelnk = "13;";
			dg = 1100410;
			gt = 0;
			oi = 6201101;
		}
		CDboTSContNarration
		{
			cancellnk = 41;
			cid = 13;
			dt = 2;
			lilnk = 255;
			oklnk = 14;
			ot = 0;
			pt = 0;
			mlt = 10000;
			os = 0;
			prelnk = "41;12;";
			dg = 1100409;
			gt = 0;
			oi = 6201101;
		}
		CDboTSContGAct
		{
			cid = 12;
			elnk = 255;
			nextlnk = 13;
			prelnk = "11;";

			CDboTSActSendSvrEvt
			{
				id = 16170;
				tblidx = 11604;
				stradius = 0;
				stype = 0;
				taid = 2;
				ttype = 255;
				type = 6;
			}
			CDboTSActSToCEvt
			{
				apptype = 1;
				cd0 = 92009;
				eitype = 5;
				idx0 = -1;
				cnt0 = 0;
				ectype = 0;
				etype = 1;
				qtidx = -1;
				taid = 1;
				esctype = 0;
			}
		}
		CDboTSContGCond
		{
			cid = 11;
			prelnk = "36;9;10;";
			nolnk = 255;
			rm = 0;
			yeslnk = 12;

			CDboTSClickNPC
			{
				npcidx = 6201101;
			}
			CDboTSCheckProgQuest
			{
				qid = 11610;
			}
		}
		CDboTSContNarration
		{
			cancellnk = 7;
			cid = 5;
			dt = 2;
			lilnk = 255;
			oklnk = 7;
			ot = 0;
			pt = 3;
			mlt = 10000;
			os = 0;
			prelnk = "35;4;3;";
			dg = 1100401;
			gt = 0;
			oi = 6201101;
		}
		CDboTSContNarration
		{
			cancellnk = 2;
			cid = 1;
			dt = 2;
			lilnk = 255;
			oklnk = 2;
			ot = 0;
			pt = 3;
			mlt = 10000;
			os = 0;
			prelnk = "0;";
			dg = 1100400;
			gt = 0;
			oi = 6201101;
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 1100402;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSClickNPC
			{
				npcidx = 6201101;
			}
			CDboTSCheckProgQuest
			{
				qid = 11605;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "45;";
			type = 1;
		}
		CDboTSContGAct
		{
			cid = 10;
			elnk = 100;
			nextlnk = 11;
			prelnk = "101;6;";

			CDboTSActRegQInfo
			{
				cont = 1100405;
				gtype = -1;
				area = 1100407;
				goal = 1100404;
				m0fz = "-963.710022";
				m0widx = 120000;
				sort = 1100406;
				stype = 32;
				m0fy = "0.000000";
				m0pat = 2;
				grade = -1;
				m0fx = "915.429993";
				m0ttip = 1100403;
				rwd = 255;
				taid = 2;
				title = 1100403;
			}
			CDboTSActSToCEvt
			{
				apptype = 1;
				cd0 = 92009;
				eitype = 5;
				idx0 = -1;
				cnt0 = 0;
				ectype = 0;
				etype = 0;
				qtidx = -1;
				taid = 1;
				esctype = 0;
			}
		}
	}
	CNtlTSGroup
	{
		gid = 254;

		CDboTSContEnd
		{
			cid = 254;
			prelnk = "1;";
			type = 0;
		}
		CDboTSContGAct
		{
			cid = 1;
			elnk = 255;
			nextlnk = 254;
			prelnk = "0;";

			CDboTSActItem
			{
				iidx1 = 92009;
				stype0 = 0;
				iidx0 = 92009;
				stype1 = 1;
				taid = 1;
				type = 1;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 0;
			nolnk = 255;
			rm = 0;
			yeslnk = 1;
		}
	}
}

