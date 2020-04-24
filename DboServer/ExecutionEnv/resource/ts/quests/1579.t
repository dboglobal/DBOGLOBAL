CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 6;
	rq = 0;
	tid = 1579;
	title = 157902;

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
			prelnk = "2;";
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
			prelnk = "0;252;1;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 157907;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 48;
			}
			CDboTSClickNPC
			{
				npcidx = "6204201;";
			}
			CDboTSCheckClrQst
			{
				flink = 1;
				flinknextqid = "1580;";
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
				conv = 157909;
				ctype = 1;
				idx = 6204201;
				taid = 2;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 4;
				eitype = 0;
				idx0 = 5651103;
				cnt0 = 4;
				cnt2 = 4;
				ectype = -1;
				etype = 0;
				idx1 = 5652104;
				taid = 3;
				esctype = 0;
				idx2 = 5653105;
			}
			CDboTSActRegQInfo
			{
				cont = 157908;
				m1fx = "-1427.000000";
				m2fz = "910.000000";
				m1fy = "0.000000";
				sort = 157905;
				m2widx = 1;
				grade = 132203;
				m1pat = 2;
				m1widx = 1;
				rwd = 100;
				taid = 1;
				gtype = 2;
				area = 157901;
				goal = 157904;
				m0fz = "914.000000";
				m0widx = 1;
				m1ttip = 157916;
				m2fy = "0.000000";
				m2pat = 2;
				scitem = -1;
				stype = 2;
				m0fy = "0.000000";
				m0pat = 2;
				m1fz = "947.000000";
				m2fx = "-1448.000000";
				m2ttip = 157917;
				m0fx = "-1393.000000";
				m0ttip = 157915;
				title = 157902;
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
			desc = 157914;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 157901;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "6204201;";
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 157908;
			gtype = 2;
			oklnk = 2;
			area = 157901;
			goal = 157904;
			sort = 157905;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 157902;
		}
	}
}

