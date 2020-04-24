CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 3;
	rq = 0;
	tid = 829;
	title = 82902;

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
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "3;";
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "101;";
			type = 1;
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;252;2;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 82907;
			nolnk = 253;
			rm = 0;
			yeslnk = 2;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 29;
			}
			CDboTSClickNPC
			{
				npcidx = "1653108;";
			}
			CDboTSCheckClrQst
			{
				flink = 1;
				flinknextqid = "830;";
				not = 0;
			}
		}
		CDboTSContGAct
		{
			cid = 3;
			elnk = 252;
			nextlnk = 4;
			prelnk = "2;";

			CDboTSActNPCConv
			{
				conv = 82909;
				ctype = 1;
				idx = 1653108;
				taid = 2;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 0;
				eitype = 0;
				idx0 = 2132102;
				cnt0 = 13;
				cnt2 = 0;
				ectype = -1;
				etype = 0;
				idx1 = -1;
				taid = 3;
				esctype = 0;
				idx2 = -1;
			}
			CDboTSActRegQInfo
			{
				cont = 82908;
				m1fx = "544.140015";
				m2fz = "419.890015";
				m1fy = "0.000000";
				sort = 82905;
				m2widx = 3;
				grade = 132203;
				m1pat = 2;
				m1widx = 3;
				rwd = 100;
				taid = 1;
				gtype = 3;
				area = 82901;
				goal = 82904;
				m0fz = "1565.000000";
				m0widx = 1;
				m1ttip = 82916;
				m2fy = "0.000000";
				m2pat = 2;
				scitem = -1;
				stype = 2;
				m0fy = "0.000000";
				m0pat = 0;
				m1fz = "102.809998";
				m2fx = "471.359985";
				m2ttip = 82916;
				m0fx = "7453.000000";
				m0ttip = 82915;
				title = 82902;
			}
		}
		CDboTSContGCond
		{
			cid = 4;
			prelnk = "3;";
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
			desc = 82914;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 82901;
			rwdtype = 0;
			ltime = -1;
			prelnk = "4;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "1653108;";
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 2;
			cont = 82908;
			gtype = 3;
			oklnk = 3;
			area = 82901;
			goal = 82904;
			sort = 82905;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 82902;
		}
	}
}

