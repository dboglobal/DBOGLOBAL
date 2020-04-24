CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 3;
	rq = 0;
	tid = 659;
	title = 65902;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGCond
		{
			cid = 4;
			prelnk = "100;3;";
			nolnk = 255;
			rm = 1;
			yeslnk = 5;

			CDboTSClickNPC
			{
				npcidx = "3032105;";
			}
		}
		CDboTSContUnifiedNarration
		{
			cancellnk = 100;
			cid = 5;
			idx = 6593;
			lilnk = 255;
			oklnk = 100;
			prelnk = "4;";
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
			cont = 65908;
			gtype = 2;
			oklnk = 2;
			area = 65901;
			goal = 65904;
			sort = 65905;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 65902;
		}
		CDboTSContReward
		{
			canclnk = 4;
			cid = 100;
			rwdzeny = 0;
			desc = 65914;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 65901;
			rwdtype = 0;
			ltime = -1;
			prelnk = "5;5;";
			usetbl = 1;
		}
		CDboTSContGCond
		{
			cid = 3;
			prelnk = "2;";
			nolnk = 255;
			rm = 1;
			yeslnk = 4;

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
				conv = 65909;
				ctype = 1;
				idx = 3032105;
				taid = 2;
			}
			CDboTSActSToCEvt
			{
				dt2 = 65925;
				otype2 = 7;
				widx1 = 1;
				iidx0 = 534;
				otype1 = 7;
				widx2 = 1;
				dt0 = 65921;
				esctype = 0;
				iidx1 = 534;
				it1 = 65922;
				otype0 = 7;
				tidx2 = 452;
				dt1 = 65923;
				etype = 0;
				iidx2 = 534;
				it0 = 65920;
				taid = 3;
				tidx1 = 451;
				widx0 = 1;
				apptype = 0;
				eitype = 6;
				tidx0 = 450;
				ectype = -1;
				it2 = 65924;
			}
			CDboTSActRegQInfo
			{
				cont = 65908;
				gtype = 2;
				area = 65901;
				goal = 65904;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 65905;
				stype = 2;
				taid = 1;
				title = 65902;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 65907;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 22;
			}
			CDboTSClickNPC
			{
				npcidx = "3032105;";
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;1;252;";
			type = 0;
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

			CDboTSActQItem
			{
				icnt0 = 255;
				iprob0 = "1.000000";
				iidx0 = 534;
				taid = 1;
				type = 1;
			}
		}
	}
}

