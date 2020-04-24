CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 7;
	rq = 0;
	tid = 2639;
	title = 263902;

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
				conv = 263909;
				ctype = 1;
				idx = 4511518;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 263908;
				gtype = 2;
				area = 263901;
				goal = 263904;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 263905;
				stype = 2;
				taid = 1;
				title = 263902;
			}
			CDboTSActSToCEvt
			{
				dt2 = 263925;
				otype2 = 1;
				widx1 = 1;
				iidx0 = -1;
				otype1 = 1;
				widx2 = 1;
				dt0 = 263921;
				esctype = 0;
				iidx1 = -1;
				it1 = 263922;
				otype0 = 1;
				tidx2 = 4481202;
				dt1 = 263923;
				etype = 0;
				iidx2 = -1;
				it0 = 263920;
				taid = 3;
				tidx1 = 5061402;
				widx0 = 1;
				apptype = 0;
				eitype = 6;
				tidx0 = 3242117;
				ectype = -1;
				it2 = 263924;
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
				itype = 6;
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
			cont = 263908;
			gtype = 2;
			oklnk = 2;
			area = 263901;
			goal = 263904;
			sort = 263905;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 263902;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 263914;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 263901;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4511518;";
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
		CDboTSContStart
		{
			cid = 0;
			stdiag = 263907;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 54;
			}
			CDboTSClickNPC
			{
				npcidx = "4511518;";
			}
			CDboTSCheckClrQst
			{
				flink = 1;
				flinknextqid = "2640;";
				not = 0;
			}
		}
	}
}

