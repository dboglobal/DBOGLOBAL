CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 4;
	rq = 0;
	tid = 1089;
	title = 108902;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 105;
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
			prelnk = "105;";
			type = 1;
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
			stdiag = 108907;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 40;
			}
			CDboTSClickNPC
			{
				npcidx = "1655301;";
			}
			CDboTSCheckClrQst
			{
				and = "1088;";
				flink = 1;
				flinknextqid = "1094;";
				not = 0;
			}
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 3;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 108909;
				ctype = 1;
				idx = 1655301;
				taid = 2;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 4;
				eitype = 0;
				idx0 = 1681100;
				cnt0 = 1;
				cnt2 = 0;
				ectype = -1;
				etype = 0;
				idx1 = 7813201;
				taid = 3;
				esctype = 0;
				idx2 = -1;
			}
			CDboTSActRegQInfo
			{
				cont = 108908;
				m1fx = "-185.940002";
				m1fy = "0.000000";
				sort = 108905;
				m0fy = "0.000000";
				m0pat = 1;
				m1fz = "436.709991";
				grade = 132203;
				m0fx = "-177.910004";
				m0ttip = 108915;
				m1pat = 2;
				m1widx = 4;
				rwd = 100;
				taid = 1;
				title = 108902;
				gtype = 3;
				area = 108901;
				goal = 108904;
				m0fz = "430.119995";
				m0widx = 4;
				m1ttip = 108916;
				scitem = -1;
				stype = 2;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 108914;
			nextlnk = 105;
			rwdexp = 0;
			rwdtbl = 108901;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSCheckQItem
			{
				ct = 0;
				icnt = 0;
				iidx = -1;
			}
			CDboTSClickNPC
			{
				npcidx = "1655301;";
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 108908;
			gtype = 3;
			oklnk = 2;
			area = 108901;
			goal = 108904;
			sort = 108905;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 108902;
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
	}
}

