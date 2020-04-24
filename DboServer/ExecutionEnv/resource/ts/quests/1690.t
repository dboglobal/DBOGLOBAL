CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 6;
	rq = 0;
	tid = 1690;
	title = 169002;

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
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 3;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 169009;
				ctype = 1;
				idx = 3273103;
				taid = 2;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 2;
				eitype = 0;
				idx0 = 7114400;
				cnt0 = 1;
				cnt2 = 0;
				ectype = -1;
				etype = 0;
				idx1 = 7773100;
				taid = 3;
				esctype = 0;
				idx2 = -1;
			}
			CDboTSActRegQInfo
			{
				cont = 169008;
				m1fx = "172.000000";
				m1fy = "0.000000";
				sort = 169005;
				m0fy = "0.000000";
				m0pat = 1;
				m1fz = "-212.000000";
				grade = 132203;
				m0fx = "179.820007";
				m0ttip = 169015;
				m1pat = 2;
				m1widx = 9;
				rwd = 100;
				taid = 1;
				title = 169002;
				gtype = 3;
				area = 169001;
				goal = 169004;
				m0fz = "-217.440002";
				m0widx = 9;
				m1ttip = 169016;
				scitem = -1;
				stype = 1;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 169008;
			gtype = 3;
			oklnk = 2;
			area = 169001;
			goal = 169004;
			sort = 169005;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 169002;
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
			desc = 169014;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 169001;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "3273103;";
			}
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
			stdiag = 169007;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 49;
			}
			CDboTSCheckClrQst
			{
				and = "1689;";
				flink = 1;
				not = 0;
			}
			CDboTSClickNPC
			{
				npcidx = "3273103;";
			}
		}
	}
}

