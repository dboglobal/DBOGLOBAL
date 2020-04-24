CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 2950;
	title = 295002;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContEnd
		{
			cid = 254;
			prelnk = "101;";
			type = 1;
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "1;0;252;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 295007;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 56;
			}
			CDboTSClickNPC
			{
				npcidx = "4752110;";
			}
			CDboTSCheckClrQst
			{
				and = "2936;";
				flink = 0;
				flinknextqid = "2951;";
				not = 0;
			}
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 7;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 295009;
				ctype = 1;
				idx = 4752110;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 295008;
				m3widx = 1;
				m1fy = "0.000000";
				m2widx = 1;
				grade = 132203;
				m1pat = 2;
				m1widx = 1;
				m3fx = "3618.199951";
				m4fy = "0.000000";
				rwd = 100;
				taid = 1;
				gtype = 2;
				m3fy = "0.000000";
				m4fx = "3432.439941";
				m4ttip = 295016;
				area = 295001;
				goal = 295004;
				m0fz = "-4149.439941";
				m0widx = 1;
				m1ttip = 295015;
				m2fy = "0.000000";
				m2pat = 2;
				m3fz = "-4139.770020";
				m3ttip = 295016;
				scitem = -1;
				stype = 2;
				m0fy = "0.000000";
				m0pat = 2;
				m1fz = "-4253.209961";
				m2fx = "3216.060059";
				m2ttip = 295015;
				m4widx = 1;
				m0fx = "3650.959961";
				m0ttip = 295015;
				m3pat = 2;
				m4pat = 2;
				title = 295002;
				m1fx = "3350.909912";
				m2fz = "-4051.969971";
				sort = 295005;
				m4fz = "-3938.969971";
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 11;
				eitype = 0;
				idx0 = 16613400;
				cnt0 = 9;
				cnt2 = 0;
				ectype = -1;
				etype = 0;
				idx1 = 18441100;
				taid = 3;
				esctype = 0;
				idx2 = -1;
			}
		}
		CDboTSContGCond
		{
			cid = 7;
			prelnk = "2;";
			nolnk = 255;
			rm = 0;
			yeslnk = 100;

			CDboTSCheckSToCEvt
			{
				itype = 0;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 295014;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 295001;
			rwdtype = 0;
			ltime = -1;
			prelnk = "7;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4752110;";
			}
		}
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "2;";
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 295008;
			gtype = 2;
			oklnk = 2;
			area = 295001;
			goal = 295004;
			sort = 295005;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 295002;
		}
		CDboTSContGAct
		{
			cid = 101;
			elnk = 255;
			nextlnk = 254;
			prelnk = "100;";

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
	}
}

