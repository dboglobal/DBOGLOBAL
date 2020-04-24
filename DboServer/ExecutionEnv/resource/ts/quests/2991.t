CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 2991;
	title = 299102;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGCond
		{
			cid = 3;
			prelnk = "2;";
			nolnk = 255;
			rm = 0;
			yeslnk = 100;

			CDboTSCheckSToCEvt
			{
				itype = 5;
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;1;252;";
			type = 0;
		}
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "2;";
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 3;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 299109;
				ctype = 1;
				idx = 4651212;
				taid = 2;
			}
			CDboTSActItem
			{
				iidx0 = 99157;
				stype0 = 1;
				taid = 4;
				type = 0;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				eitype = 5;
				idx0 = 2991;
				cnt0 = 5;
				ectype = -1;
				etype = 0;
				qtidx = 299115;
				taid = 3;
				esctype = 0;
			}
			CDboTSActRegQInfo
			{
				cont = 299108;
				m1fx = "4325.410156";
				m2fz = "-3831.449951";
				m1fy = "0.000000";
				sort = 299105;
				m2widx = 1;
				grade = -1;
				m1pat = 2;
				m1widx = 1;
				rwd = 100;
				taid = 1;
				gtype = 2;
				area = 299101;
				goal = 299104;
				m0fz = "-3950.979980";
				m0widx = 1;
				m1ttip = 299115;
				m2fy = "0.000000";
				m2pat = 2;
				scitem = 99157;
				stype = 1;
				m0fy = "0.000000";
				m0pat = 2;
				m1fz = "-3973.790039";
				m2fx = "4176.350098";
				m2ttip = 299115;
				m0fx = "4042.790039";
				m0ttip = 299115;
				title = 299102;
			}
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
				eitype = -1;
				esctype = 0;
				ectype = -1;
				etype = 1;
				taid = 1;
			}
			CDboTSActItem
			{
				iidx0 = 99157;
				stype0 = 1;
				taid = 2;
				type = 1;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 299107;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 58;
			}
			CDboTSCheckClrQst
			{
				and = "2990;";
				flink = 0;
				not = 0;
			}
			CDboTSClickNPC
			{
				npcidx = "4651212;";
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 299114;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 299101;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4651212;";
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "101;";
			type = 1;
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 299108;
			gtype = 2;
			oklnk = 2;
			area = 299101;
			goal = 299104;
			sort = 299105;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 299102;
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
		CDboTSContStart
		{
			cid = 0;
			stdiag = 0;
			nolnk = 255;
			rm = 0;
			yeslnk = 1;
		}
		CDboTSContGAct
		{
			cid = 1;
			elnk = 255;
			nextlnk = 254;
			prelnk = "0;";

			CDboTSActItem
			{
				iidx0 = 99157;
				stype0 = 1;
				taid = 1;
				type = 1;
			}
		}
	}
}

