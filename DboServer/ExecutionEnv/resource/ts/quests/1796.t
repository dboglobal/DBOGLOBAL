CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 6;
	rq = 0;
	tid = 1796;
	title = 179602;

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
			cid = 2;
			elnk = 252;
			nextlnk = 3;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 179609;
				ctype = 1;
				idx = 3142217;
				taid = 2;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 0;
				eitype = 0;
				idx0 = 8441304;
				cnt0 = 12;
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
				cont = 179608;
				m1fx = "-1700.000000";
				m1fy = "0.000000";
				sort = 179605;
				m0fy = "0.000000";
				m0pat = 2;
				m1fz = "-448.000000";
				grade = 132203;
				m0fx = "-1873.000000";
				m0ttip = 179615;
				m1pat = 2;
				m1widx = 1;
				rwd = 100;
				taid = 1;
				title = 179602;
				gtype = 2;
				area = 179601;
				goal = 179604;
				m0fz = "-431.000000";
				m0widx = 1;
				m1ttip = 179615;
				scitem = -1;
				stype = 1;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 179614;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 179601;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "3142217;";
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
			cont = 179608;
			gtype = 2;
			oklnk = 2;
			area = 179601;
			goal = 179604;
			sort = 179605;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 179602;
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
			stdiag = 179607;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 50;
			}
			CDboTSCheckClrQst
			{
				flink = 1;
				flinknextqid = "1797;";
				not = 0;
			}
			CDboTSClickNPC
			{
				npcidx = "3142217;";
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
	}
}

