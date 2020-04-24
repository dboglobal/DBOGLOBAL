CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 3;
	rq = 0;
	tid = 868;
	title = 86802;

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
			stdiag = 86807;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 30;
			}
			CDboTSClickNPC
			{
				npcidx = "1653105;";
			}
			CDboTSCheckClrQst
			{
				and = "843;";
				flink = 1;
				flinknextqid = "888;";
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
				conv = 86809;
				ctype = 1;
				idx = 1653105;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 86808;
				m1fx = "459.209991";
				m2fz = "-429.350006";
				m1fy = "0.000000";
				sort = 86805;
				m2widx = 3;
				grade = 132203;
				m1pat = 1;
				m1widx = 3;
				rwd = 100;
				taid = 1;
				gtype = 3;
				area = 86801;
				goal = 86804;
				m0fz = "-222.000000";
				m0widx = 3;
				m1ttip = 86816;
				m2fy = "0.000000";
				m2pat = 1;
				scitem = -1;
				stype = 2;
				m0fy = "0.000000";
				m0pat = 1;
				m1fz = "-221.000000";
				m2fx = "624.880005";
				m2ttip = 86817;
				m0fx = "361.880005";
				m0ttip = 86815;
				title = 86802;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 1;
				eitype = 0;
				idx0 = 8442101;
				cnt0 = 1;
				cnt2 = 1;
				ectype = -1;
				etype = 0;
				idx1 = 1652100;
				taid = 3;
				esctype = 0;
				idx2 = 1651100;
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
			desc = 86814;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 86801;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "1653105;";
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 86808;
			gtype = 3;
			oklnk = 2;
			area = 86801;
			goal = 86804;
			sort = 86805;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 86802;
		}
	}
}

