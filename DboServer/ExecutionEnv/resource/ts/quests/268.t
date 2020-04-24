CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 1;
	rq = 0;
	tid = 268;
	title = 26802;

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

			CDboTSActRegQInfo
			{
				cont = 26808;
				gtype = 1;
				area = 26801;
				goal = 26804;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 26805;
				stype = 2;
				taid = 1;
				title = 26802;
			}
			CDboTSActNPCConv
			{
				conv = 26809;
				ctype = 1;
				idx = 4751106;
				taid = 2;
			}
		}
		CDboTSContGAct
		{
			cid = 3;
			elnk = 255;
			nextlnk = 4;
			prelnk = "2;";

			CDboTSActSToCEvt
			{
				dt2 = 26825;
				otype2 = 1;
				widx1 = 1;
				iidx0 = -1;
				otype1 = 1;
				widx2 = 1;
				dt0 = 26823;
				esctype = 0;
				iidx1 = -1;
				it1 = 26821;
				otype0 = 1;
				tidx2 = 4751303;
				dt1 = 26824;
				etype = 0;
				iidx2 = -1;
				it0 = 26820;
				taid = 1;
				tidx1 = 4511403;
				widx0 = 1;
				apptype = 0;
				eitype = 6;
				tidx0 = 4751201;
				ectype = -1;
				it2 = 26822;
			}
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
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 26814;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 26801;
			rwdtype = 0;
			ltime = -1;
			prelnk = "4;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4751106;";
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 26807;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 12;
			}
			CDboTSCheckPCRace
			{
				raceflg = 2;
			}
			CDboTSClickNPC
			{
				npcidx = "4751106;";
			}
			CDboTSCheckClrQst
			{
				and = "254;";
				flink = 1;
				flinknextqid = "283;";
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
			cont = 26808;
			gtype = 1;
			oklnk = 2;
			area = 26801;
			goal = 26804;
			sort = 26805;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 26802;
		}
	}
}

