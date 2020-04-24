CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 3;
	rq = 0;
	tid = 694;
	title = 69402;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGCond
		{
			cid = 101;
			prelnk = "100;";
			nolnk = 255;
			rm = 1;
			yeslnk = 100;

			CDboTSClickNPC
			{
				npcidx = "1551105;";
			}
		}
		CDboTSContGCond
		{
			cid = 4;
			prelnk = "3;";
			nolnk = 255;
			rm = 1;
			yeslnk = 5;

			CDboTSClickNPC
			{
				npcidx = "1551105;";
			}
		}
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "2;";
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "100;";
			type = 1;
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 69408;
			gtype = 2;
			oklnk = 2;
			area = 69401;
			goal = 69404;
			sort = 69405;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 69402;
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 3;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 69409;
				ctype = 1;
				idx = 1551105;
				taid = 2;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cd0 = 92028;
				cnt1 = 0;
				eitype = 0;
				idx0 = 1712102;
				cnt0 = 12;
				ectype = 0;
				esctype = 0;
				idx2 = -1;
				cnt2 = 0;
				etype = 0;
				idx1 = -1;
				taid = 3;
			}
			CDboTSActRegQInfo
			{
				cont = 69408;
				m1fx = "6551.660156";
				m1fy = "0.000000";
				sort = 69405;
				m0fy = "-108.000000";
				m0pat = 2;
				m1fz = "484.779999";
				grade = 132203;
				m0fx = "6596.859863";
				m0ttip = 69415;
				m1pat = 2;
				m1widx = 1;
				rwd = 100;
				taid = 1;
				title = 69402;
				gtype = 2;
				area = 69401;
				goal = 69404;
				m0fz = "-216.550003";
				m0widx = 1;
				m1ttip = 69415;
				scitem = -1;
				stype = 2;
			}
		}
		CDboTSContGAct
		{
			cid = 5;
			elnk = 255;
			nextlnk = 100;
			prelnk = "4;";

			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 0;
				eitype = 0;
				idx0 = -1;
				cnt0 = 0;
				cnt2 = 0;
				ectype = -1;
				etype = 1;
				idx1 = -1;
				taid = 1;
				esctype = 0;
				idx2 = -1;
			}
		}
		CDboTSContReward
		{
			canclnk = 101;
			cid = 100;
			rwdzeny = 0;
			desc = 69414;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 69401;
			rwdtype = 0;
			ltime = -1;
			prelnk = "101;5;";
			usetbl = 1;
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;252;1;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 69407;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckPCRace
			{
				raceflg = 4;
			}
			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 23;
			}
			CDboTSClickNPC
			{
				npcidx = "1551105;";
			}
			CDboTSCheckClrQst
			{
				and = "693;";
				flink = 0;
				not = 0;
			}
		}
		CDboTSContGCond
		{
			cid = 3;
			prelnk = "2;";
			nolnk = 255;
			rm = 0;
			yeslnk = 4;

			CDboTSCheckSToCEvt
			{
				itype = 0;
			}
		}
	}
}

