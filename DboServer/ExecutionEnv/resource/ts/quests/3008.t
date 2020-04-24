CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 3008;
	title = 300802;

	CNtlTSGroup
	{
		gid = 0;

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
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "1;0;252;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 300807;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 58;
			}
			CDboTSClickNPC
			{
				npcidx = "4512301;";
			}
			CDboTSCheckClrQst
			{
				and = "3004;";
				flink = 0;
				flinknextqid = "3009;";
				not = 0;
			}
		}
		CDboTSContGCond
		{
			cid = 3;
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
			desc = 300814;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 300801;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4512301;";
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
			prelnk = "101;";
			type = 1;
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 300808;
			gtype = 2;
			oklnk = 2;
			area = 300801;
			goal = 300804;
			sort = 300805;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 300802;
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 3;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 300809;
				ctype = 1;
				idx = 4512301;
				taid = 2;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 10;
				eitype = 0;
				idx0 = 13591100;
				cnt0 = 10;
				cnt2 = 0;
				ectype = -1;
				etype = 0;
				idx1 = 17112300;
				taid = 3;
				esctype = 0;
				idx2 = -1;
			}
			CDboTSActRegQInfo
			{
				cont = 300808;
				m3widx = 6;
				m1fy = "0.000000";
				m2widx = 6;
				grade = 132203;
				m1pat = 2;
				m1widx = 6;
				m3fx = "80.480003";
				m4fy = "0.000000";
				rwd = 100;
				taid = 1;
				gtype = 2;
				m3fy = "0.000000";
				m4fx = "-167.610001";
				m4ttip = 300816;
				area = 300801;
				goal = 300804;
				m0fz = "-7.670000";
				m0widx = 6;
				m1ttip = 300815;
				m2fy = "0.000000";
				m2pat = 2;
				m3fz = "-38.090000";
				m3ttip = 300816;
				scitem = -1;
				stype = 1;
				m0fy = "0.000000";
				m0pat = 2;
				m1fz = "-80.389999";
				m2fx = "24.120001";
				m2ttip = 300815;
				m4widx = 6;
				m0fx = "-190.050003";
				m0ttip = 300815;
				m3pat = 2;
				m4pat = 2;
				title = 300802;
				m1fx = "-71.919998";
				m2fz = "46.470001";
				sort = 300805;
				m4fz = "-47.650002";
			}
		}
	}
}

