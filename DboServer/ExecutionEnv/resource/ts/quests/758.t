CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 3;
	rq = 0;
	tid = 758;
	title = 75802;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 3;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 75809;
				ctype = 1;
				idx = 1351207;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 75808;
				gtype = 2;
				area = 75801;
				goal = 75804;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 75805;
				stype = 2;
				taid = 1;
				title = 75802;
			}
			CDboTSActSToCEvt
			{
				dt2 = 75825;
				otype2 = 7;
				widx1 = 1;
				iidx0 = 691;
				otype1 = 7;
				widx2 = 1;
				dt0 = 75821;
				esctype = 0;
				iidx1 = 691;
				it1 = 75822;
				otype0 = 7;
				tidx2 = 499;
				dt1 = 75823;
				etype = 0;
				iidx2 = 691;
				it0 = 75820;
				taid = 3;
				tidx1 = 498;
				widx0 = 1;
				apptype = 0;
				eitype = 6;
				tidx0 = 497;
				ectype = -1;
				it2 = 75824;
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
				eitype = 6;
				esctype = 0;
				ectype = -1;
				etype = 1;
				taid = 1;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 75808;
			gtype = 2;
			oklnk = 2;
			area = 75801;
			goal = 75804;
			sort = 75805;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 75802;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 75814;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 75801;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "1351207;";
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 75807;
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
				minlvl = 25;
			}
			CDboTSClickNPC
			{
				npcidx = "1351207;";
			}
			CDboTSCheckClrQst
			{
				and = "757;";
				flink = 1;
				not = 0;
			}
		}
		CDboTSContGCond
		{
			cid = 3;
			prelnk = "2;";
			nolnk = 255;
			rm = 1;
			yeslnk = 100;

			CDboTSCheckSToCEvt
			{
				itype = 6;
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
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "101;";
			type = 1;
		}
	}
	CNtlTSGroup
	{
		gid = 254;

		CDboTSContGAct
		{
			cid = 1;
			elnk = 255;
			nextlnk = 254;
			prelnk = "0;";

			CDboTSActQItem
			{
				icnt0 = 255;
				iprob0 = "1.000000";
				iidx0 = 691;
				taid = 1;
				type = 1;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 0;
			nolnk = 255;
			rm = 0;
			yeslnk = 1;
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "1;";
			type = 0;
		}
	}
}

