CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 2;
	rq = 0;
	tid = 412;
	title = 41202;

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
				eitype = 6;
				esctype = 0;
				ectype = -1;
				etype = 1;
				taid = 1;
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
				itype = 6;
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
				conv = 41209;
				ctype = 1;
				idx = 4152110;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 41208;
				gtype = 2;
				area = 41201;
				goal = 41204;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 41205;
				stype = 1;
				taid = 1;
				title = 41202;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 41207;
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
				minlvl = 3;
			}
			CDboTSClickNPC
			{
				npcidx = "4152110;";
			}
			CDboTSCheckClrQst
			{
				and = "405;";
				flink = 1;
				not = 0;
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;1;252;";
			type = 0;
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 41208;
			gtype = 2;
			oklnk = 2;
			area = 41201;
			goal = 41204;
			sort = 41205;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 41202;
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "101;";
			type = 1;
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
			cid = 3;
			elnk = 255;
			nextlnk = 4;
			prelnk = "2;";

			CDboTSActSToCEvt
			{
				dt2 = 41232;
				otype2 = 1;
				widx1 = 1;
				iidx0 = -1;
				otype1 = 1;
				widx2 = 1;
				dt0 = 41230;
				esctype = 0;
				iidx1 = -1;
				it1 = 41234;
				otype0 = 1;
				tidx2 = 3813101;
				dt1 = 41231;
				etype = 0;
				iidx2 = -1;
				it0 = 41233;
				taid = 1;
				tidx1 = 3812101;
				widx0 = 1;
				apptype = 0;
				eitype = 6;
				tidx0 = 3811101;
				ectype = -1;
				it2 = 41235;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 41214;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 41201;
			rwdtype = 0;
			ltime = -1;
			prelnk = "4;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4152110;";
			}
		}
	}
}

